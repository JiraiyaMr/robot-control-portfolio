"""Verify imported source integrity and local project references without compiling firmware."""
from pathlib import Path
import argparse
import hashlib
import json
import re
import xml.etree.ElementTree as ET

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--archives", type=Path)
    parser.add_argument("--write-report", action="store_true")
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    manifest = json.loads((root / "docs/source_manifest.json").read_text(encoding="utf-8"))
    failures = []
    summary = []
    for project in manifest["projects"]:
        for item in project["files"]:
            p = root / item["path"]
            if not p.is_file():
                failures.append("Missing imported file: " + item["path"])
            elif hashlib.sha256(p.read_bytes()).hexdigest() != item["sha256"]:
                failures.append("Modified imported file: " + item["path"])
        archive_verified = False
        if args.archives:
            archive = args.archives / project["archive_name"]
            if not archive.is_file():
                failures.append("Missing source archive: " + project["archive_name"])
            elif hashlib.sha256(archive.read_bytes()).hexdigest() != project["archive_sha256"]:
                failures.append("Source archive differs: " + project["archive_name"])
            else:
                archive_verified = True
        ref_count = 0
        include_count = 0
        for relative in project["keil_projects"]:
            uvproj = root / "firmware" / project["slug"] / relative
            doc = ET.parse(uvproj)
            for node in doc.findall(".//Files/File/FilePath"):
                ref_count += 1
                target = (uvproj.parent / node.text.replace("\\", "/")).resolve()
                if not target.is_relative_to((root / "firmware" / project["slug"]).resolve()):
                    failures.append("Project reference leaves snapshot: " + node.text)
                elif not target.is_file():
                    failures.append("Missing Keil reference: " + project["slug"] + "/" + node.text)
            for node in doc.findall(".//Cads/VariousControls/IncludePath"):
                for token in (node.text or "").split(";"):
                    if not token.strip() or "$" in token:
                        continue
                    include_count += 1
                    target = uvproj.parent / token.replace("\\", "/")
                    if not target.is_dir():
                        failures.append("Missing include directory: " + project["slug"] + "/" + token)
        summary.append({
            "project": project["slug"],
            "original_files": project["archive_file_count"],
            "retained_files": project["retained_file_count"],
            "retained_bytes": project["retained_bytes"],
            "keil_references": ref_count,
            "include_directories": include_count,
            "source_archive_verified": archive_verified,
        })
    # Check links in generated documentation, excluding immutable upstream docs.
    report_path = root / "docs/verification_report.md"
    if args.write_report:
        lines = [
            "# Static Import Verification", "",
            "The checks below cover the packaged source and project paths. Firmware compilation, linking, flashing, and physical tests were not performed.", "",
            "| Snapshot | Original archive files | Retained files | Keil file references | Include directories |",
            "| --- | --- | --- | --- | --- |",
        ]
        for item in summary:
            lines.append("| {project} | {original_files} | {retained_files} | {keil_references} | {include_directories} |".format(**item))
        lines += ["", "Each retained file is checked against the SHA-256 in source_manifest.json. Source archive hashes are additionally checked when --archives is supplied.", "",
                  "## Run the checks", "", "```console", "python tools/check_repository.py", "```", "",
                  "## Scope", "", "- Original firmware bytes and author notices are retained.",
                  "- The Keil project files and their local source references are checked.",
                  "- Generated documentation links are checked.",
                  "- Startup tasks are documented from source, not inferred from archive names.",
                  "- No recorded robot performance, video, or measured timing is included.",
                  "- The GCC Makefiles are retained upstream build files; see build.md for their source-list discrepancy.", ""]
        report_path.write_text("\n".join(lines), encoding="utf-8")
    generated_md = [root / "README.md", root / "THIRD_PARTY_NOTICES.md", root / "firmware/README.md"]
    generated_md += list((root / "docs").glob("*.md"))
    generated_md += [root / "firmware" / p["slug"] / "README.md" for p in manifest["projects"]]
    checked_links = 0
    for p in generated_md:
        if not p.is_file():
            failures.append("Missing generated document: " + p.relative_to(root).as_posix())
            continue
        text = p.read_text(encoding="utf-8")
        for target in re.findall(r"\]\(([^)]+)\)", text):
            if target.startswith(("https://", "http://", "#", "mailto:")):
                continue
            checked_links += 1
            if not (p.parent / target.split("#", 1)[0]).exists():
                failures.append("Broken link: " + p.relative_to(root).as_posix() + " -> " + target)
    result = {"status": "passed" if not failures else "failed", "projects": summary, "documentation_links": checked_links, "failures": failures}
    if args.write_report:
        with report_path.open("a", encoding="utf-8", newline="\n") as stream:
            stream.write("\n## Check result\n\nStatus: " + result["status"] + ". Checked documentation links: " + str(checked_links) + ".\n")
            for failure in failures:
                stream.write("\n- " + failure + "\n")
    print(json.dumps(result, ensure_ascii=False, indent=2))
    raise SystemExit(1 if failures else 0)

if __name__ == "__main__":
    main()
