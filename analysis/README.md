# Analysis Workflow

Add MATLAB or Python scripts that reproduce figures and metrics from actual robot logs.

1. Read original CSV data.
2. Validate columns, units, timestamps, and sampling intervals.
3. Document invalid samples and preprocessing.
4. Plot reference and measurement, tracking error, and actuator command.
5. Calculate metrics over explicitly defined intervals.
6. Summarize repeated trials where available.
7. Save plots to `figures/` and derived tables to `data/processed/`.

After implementing and checking the scripts, document dependencies and exact execution commands.

No analysis program or calculated result is included in this initial setup.
