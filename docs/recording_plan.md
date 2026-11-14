# Recording Plan for the Supplied Firmware

Select one working subsystem and document its actual experimental configuration.

## Mecanum wheel-speed experiment

Useful source locations in `firmware/mecanum-chassis/`:
- `Application/chassis.c`: `chassis_t.chassis_move_t[i].speed_set`, `speed`, and `give_current`.
- `Application/chassis_power_control.c`: command modification by the power limiter.
- `Src/freertos.c`: active chassis and dual-board receive tasks.

Record wheel identifier, reference speed, feedback speed, command after power limiting, device timestamp, and operating mode. Feedback is read from `speed_rpm`; confirm gear ratio and the reference scaling before interpreting wheel-ground speed.

A wheel-speed step experiment is the most direct initial demonstration for this snapshot.

## Swerve steering experiment

Useful source locations in `firmware/swerve/`:
- `Applications/helm.c`: target encoder angle, measured encoder angle, speed target, speed feedback, and command.
- `Applications/chassis.c`: target wheel speeds and motion direction.
- `Core/Src/freertos.c`: chassis and steering tasks are created.

Record module identifier and calibration offset. Encoder counts and radians occur at different stages of the calculation; convert and label units at the logging boundary.

## Gimbal demonstration

The supplied `firmware/omni-gimbal/` startup activates INS and host communication but does not create the gimbal task. Identify the working hardware revision and its task / command configuration before collecting a closed-loop demonstration.

Potential variables in the available control path include target yaw, measured yaw, yaw rate, and yaw command. Angle degrees, gyroscope units, and the pitch interface's radian conversion must be documented separately.

The existing USB / PC code is a communication starting point. The archive does not contain a verified timestamped tracking CSV or performance results.

## Capture procedure

1. Record the exact firmware revision and configuration.
2. Sample coherent reference, feedback, and command values with a device timestamp.
3. Send logs outside the time-critical control calculation.
4. Check sample intervals and losses.
5. Preserve unmodified logs and link each video to its trial ID.
6. Run the analysis and report actual measured results.
