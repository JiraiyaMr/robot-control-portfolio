# Static Import Verification

The checks below cover the packaged source and project paths. Firmware compilation, linking, flashing, and physical tests were not performed.

| Snapshot | Original archive files | Retained files | Keil file references | Include directories |
| --- | --- | --- | --- | --- |
| mecanum-chassis | 1513 | 263 | 115 | 27 |
| omni-gimbal | 1472 | 244 | 98 | 29 |
| swerve | 1336 | 349 | 77 | 19 |

Each retained file is checked against the SHA-256 in source_manifest.json. Source archive hashes are additionally checked when --archives is supplied.

## Run the checks

```console
python tools/check_repository.py
```

## Scope

- Original firmware bytes and author notices are retained.
- The Keil project files and their local source references are checked.
- Generated documentation links are checked.
- Startup tasks are documented from source, not inferred from archive names.
- No recorded robot performance, video, or measured timing is included.
- The GCC Makefiles are retained upstream build files; see build.md for their source-list discrepancy.

## Check result

Status: passed. Checked documentation links: 55.
