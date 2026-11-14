# Experiment Protocol

Choose conditions appropriate to the actual platform and document its operating range.

## Experiment record

Record the trial ID, date, hardware configuration, firmware revision, controller parameters, supply conditions, load, initial state, reference signal, control period, and logging period.

## Step response

1. Start from the stated initial condition.
2. Record before changing the reference.
3. Apply the same reference step for each repeated trial.
4. Continue recording long enough to characterize the response.
5. Preferably repeat each condition five times.
6. Test both motion directions when relevant.

## Continuous tracking

Use a reproducible reference and state its amplitude, frequency, duration, and initial phase.

## Optional disturbance response

State the disturbance method and timing. Quantitative comparisons require sufficiently controlled disturbance conditions. Unmeasured manual disturbance demonstrates recovery behavior.

## Data checks and metrics

Use device timestamps. Check sample intervals, missing samples, clock resets, and invalid values. Preserve original data and document preprocessing.

| Metric | Definition |
| --- | --- |
| Tracking RMSE | Root mean squared reference-minus-measurement error over a stated interval |
| Steady-state error | Mean absolute error over a stated settled interval |
| Overshoot | Peak exceedance normalized by the reference step amplitude |
| Settling time | Time to enter and remain within a stated error band |

Define evaluation intervals and error bands. Report mean and standard deviation for repeated trials where available.
