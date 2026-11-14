# Swerve Drive Firmware

STM32F407 / FreeRTOS source snapshot for a four-module steerable drive.

## Source guide

- [Wheel-speed and direction decomposition](Applications/chassis.c)
- [Steering targets and cascaded control](Applications/helm.c)
- [CAN feedback and output](modules/CAN_receive/CAN_receive.c)
- [Dual-board command transport](modules/Dual_board/Dual_board.c)
- [Task creation](Core/Src/freertos.c)
- [Original Keil project](MDK-ARM/WUST_Frame_Two.uvprojx)

The source calculates wheel-speed magnitudes and motion directions, derives steering targets, handles encoder wrapping, and applies angle / speed control to the steering motors.

The supplied startup creates the default, chassis, steering, and board-to-board receive tasks. The INS and gimbal task creation calls are commented out.

See [build instructions](../../docs/build.md), [recording plan](../../docs/recording_plan.md), and [attribution](../../THIRD_PARTY_NOTICES.md). Steering calibration offsets and physical module orientation should be recorded for the robot used in a demonstration.
