# Measurement Data Format

`schema.csv` is a header-only convention and contains no experimental rows.

| Field | Meaning |
| --- | --- |
| trial_id | Recorded trial identifier |
| time_ms | Elapsed device time in milliseconds |
| channel | Wheel, steering module, yaw, or other measured axis |
| reference | Reference value |
| measurement | Feedback value |
| control_command | Final actuator command |
| signal_unit | Shared reference / measurement unit, such as rpm, deg, or encoder_count |
| command_unit | Actual command unit or clearly identified raw driver counts |
| mode | Operating mode or experiment phase |

Reference and measurement values must use the same unit. Convert at the logging boundary when required, and document encoder wrap handling, gear ratio, sensor scaling, and sign convention.

Keep original logs in `raw/` and derived data separately in `processed/`.
