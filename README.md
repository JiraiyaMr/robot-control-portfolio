# Robot Control Projects

The robot projects are now maintained as three standalone repositories, each containing both chassis and gimbal application code, dependencies, configuration notes, and attribution.

| Robot project | Repository |
| --- | --- |
| 云台-麦克纳姆轮底盘机器人 | [gimbal-mecanum-robot](https://github.com/JiraiyaMr/gimbal-mecanum-robot) |
| 云台-全向轮底盘机器人 | [gimbal-omniwheel-robot](https://github.com/JiraiyaMr/gimbal-omniwheel-robot) |
| 云台-舵轮底盘机器人 | [gimbal-swerve-robot](https://github.com/JiraiyaMr/gimbal-swerve-robot) |

This original collection retains the source history and original imported directories. The new repositories distinguish the robot platform from the tasks enabled in each archived board configuration. Original source headers and third-party notices are retained.

## Original collection documentation


Firmware snapshots from RoboMaster development covering a mecanum chassis, swerve drive, and a gimbal / inertial-sensing stack.

**Platform:** STM32F407IGHx, FreeRTOS, CMSIS-RTOS, CAN, UART and SPI. The supplied clock configurations specify 168 MHz.

## Projects

| Project | Source-level features | Start reading |
| --- | --- | --- |
| [Mecanum chassis](firmware/mecanum-chassis/README.md) | Four-wheel velocity mixing, wheel-speed PID, CAN feedback, referee-based power limiting | [Chassis control](firmware/mecanum-chassis/Application/chassis.c) |
| [Swerve drive](firmware/swerve/README.md) | Four-module velocity and steering decomposition, steering-angle handling, cascaded steering control | [Steering control](firmware/swerve/Applications/helm.c) |
| [Gimbal and INS](firmware/omni-gimbal/README.md) | Yaw angle / angular-rate control, pitch motor interface, BMI088 acquisition, quaternion EKF integration, USB / host communication | [Gimbal control](firmware/omni-gimbal/Application/gimbal.c) |

The task configuration supplied with each snapshot is documented in [hardware and startup configuration](docs/hardware.md). The gimbal snapshot currently starts the INS, PC, and USB tasks; the gimbal control task is present in source but its creation is commented out.

## Architecture and source guide

- [Control and communication diagrams](docs/system_architecture.md)
- [Build instructions and dependencies](docs/build.md)
- [Source map](docs/source_map.md)
- [Original authors and third-party notices](THIRD_PARTY_NOTICES.md)
- [Personal contribution record](docs/contributions.md)

## Demonstration and experiments

| Evidence | Current contents |
| --- | --- |
| Firmware | Three imported source snapshots |
| Static verification | [Import checks and project-file checks](docs/verification_report.md) |
| Demonstration video | To be recorded on the selected robot |
| Measurement logs | To be captured; no measured performance is claimed here |
| Experiment design | [Protocol](docs/experiment_protocol.md) and [source-specific recording plan](docs/recording_plan.md) |

Use the [results page](docs/results.md) to connect each future video, figure, and metric to its original log and firmware configuration.

## Check this repository

With Python 3:

```console
python tools/check_repository.py
```

This checks imported file hashes and Keil project references. Building firmware uses the original Keil project files and requires the matching embedded toolchain and board configuration.

## Source provenance

Imported firmware files retain their original bytes and author notices. Generated build outputs, editor settings, unrelated MCU headers, unused CMSIS examples, and unused precompiled libraries were omitted. Required source dependencies and referenced math libraries were retained.

[Source manifest](docs/source_manifest.json) records the source archives and every retained file. Application-specific ownership will be specified in the contribution record; third-party algorithms and framework files retain their original attribution.
