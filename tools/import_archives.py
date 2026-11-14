"""Import three supplied robot archives without changing firmware source bytes."""
from pathlib import Path, PurePosixPath
from zipfile import ZipFile
from collections import Counter
import argparse
import hashlib
import json
import posixpath
import xml.etree.ElementTree as ET

ARCHIVES = {
    "mecanum-chassis": "\u9ea6\u8f6e\u6b65\u5175 - \u5e95\u76d8.zip",
    "omni-gimbal": "\u5168\u5411\u8f6e\u6b65\u5175 - \u4e91\u53f0.zip",
    "swerve": "\u8235\u8f6e.zip",
}
VENDOR_PREFIXES = (
    "Drivers/STM32F4xx_HAL_Driver/Inc/",
    "Drivers/STM32F4xx_HAL_Driver/Src/",
    "Drivers/CMSIS/Include/",
    "Drivers/CMSIS/Device/ST/STM32F4xx/Include/",
    "Drivers/CMSIS/DSP/Include/",
    "Middlewares/Third_Party/FreeRTOS/Source/",
    "Middlewares/ST/STM32_USB_Device_Library/",
)
ARTIFACT_SUFFIXES = {
    ".o", ".d", ".crf", ".axf", ".elf", ".hex", ".bin", ".map", ".lnp",
    ".htm", ".html", ".uvoptx", ".uvguix", ".iex", ".dbgconf", ".scvd",
    ".jdebug", ".user", ".bat", ".pdf",
}

def sha(data):
    return hashlib.sha256(data).hexdigest()

def source_root(names):
    first = names[0].split("/")[0] + "/"
    return first + "WUST_Frame/" if any(n.startswith(first + "WUST_Frame/") for n in names) else first

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("archive_directory", type=Path)
    parser.add_argument("--repository", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    repo = args.repository.resolve()
    previous_manifest_path = repo / "docs" / "source_manifest.json"
    previous_records = []
    if previous_manifest_path.exists():
        previous = json.loads(previous_manifest_path.read_text(encoding="utf-8"))
        previous_records = [r for p in previous["projects"] for r in p["files"]]
    projects = []
    for slug, filename in ARCHIVES.items():
        archive = args.archive_directory / filename
        project = repo / "firmware" / slug
        with ZipFile(archive) as z:
            entries = [i for i in z.infolist() if not i.is_dir()]
            names = [i.filename for i in entries]
            prefix = source_root(names)
            by_relative = {i.filename[len(prefix):]: i for i in entries}
            required = set()
            project_files = [n for n in by_relative if n.endswith(".uvprojx")]
            for rel in project_files:
                tree = ET.fromstring(z.read(by_relative[rel]))
                for node in tree.findall(".//Files/File/FilePath"):
                    normalized = node.text.replace("\\", "/")
                    required.add(posixpath.normpath(posixpath.join(posixpath.dirname(rel), normalized)))
            if "Makefile" in by_relative:
                required.add("Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a")
            records = []
            skipped = Counter()
            for rel, info in by_relative.items():
                parts = PurePosixPath(rel).parts
                if not parts or ".." in parts or PurePosixPath(rel).is_absolute():
                    raise ValueError("Unsafe archive member")
                suffix = PurePosixPath(rel).suffix.lower()
                reason = None
                if parts[0].startswith("."):
                    reason = "editor_or_generator_metadata"
                elif parts[0] == "build" or suffix in ARTIFACT_SUFFIXES:
                    reason = "build_or_local_artifact"
                elif parts[0] == "MDK-ARM" and rel not in required and suffix != ".uvprojx":
                    reason = "unused_mdk_file"
                elif suffix in {".lib", ".a"} and rel not in required:
                    reason = "unused_prebuilt_library"
                elif rel.startswith("Drivers/CMSIS/Device/ST/STM32F4xx/Include/") and PurePosixPath(rel).name not in {"stm32f407xx.h", "stm32f4xx.h", "system_stm32f4xx.h"}:
                    reason = "unused_mcu_header"
                elif parts[0] == "Drivers" and not (
                    rel in required or rel.startswith(VENDOR_PREFIXES)
                    or PurePosixPath(rel).name.lower().startswith(("license", "copying", "notice"))
                ):
                    reason = "unused_vendor_example_or_platform"
                elif parts[0] == "Middlewares" and not rel.startswith(VENDOR_PREFIXES):
                    reason = "unused_middleware"
                if reason:
                    skipped[reason] += 1
                    continue
                target_rel = "UPSTREAM_README.md" if rel == "README.md" else rel
                target = project.joinpath(*PurePosixPath(target_rel).parts)
                if not target.resolve().is_relative_to(project.resolve()):
                    raise ValueError("Output path leaves project")
                payload = z.read(info)
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(payload)
                records.append({
                    "path": target.relative_to(repo).as_posix(),
                    "archive_member": info.filename,
                    "bytes": len(payload),
                    "sha256": sha(payload),
                })
            projects.append({
                "slug": slug,
                "archive_name": filename,
                "archive_sha256": sha(archive.read_bytes()),
                "source_root": prefix,
                "archive_file_count": len(entries),
                "retained_file_count": len(records),
                "retained_bytes": sum(r["bytes"] for r in records),
                "omitted": dict(skipped),
                "keil_projects": project_files,
                "files": records,
            })
    manifest = {"format_version": 1, "source_policy": "Retained firmware files preserve the original bytes.", "projects": projects}
    retained_paths = {r["path"] for p in projects for r in p["files"]}
    for old in previous_records:
        if old["path"] in retained_paths:
            continue
        stale = (repo / old["path"]).resolve()
        if not stale.is_relative_to((repo / "firmware").resolve()):
            raise ValueError("Stale imported path leaves firmware directory")
        if stale.exists():
            if sha(stale.read_bytes()) != old["sha256"]:
                raise ValueError("A previously imported file was edited: " + old["path"])
            stale.unlink()
    path = repo / "docs" / "source_manifest.json"
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    for p in projects:
        print(json.dumps({k: v for k, v in p.items() if k != "files"}, ensure_ascii=False))

if __name__ == "__main__":
    main()
