# Mecanum Chassis Firmware

STM32F407 / FreeRTOS source snapshot for a four-wheel mecanum chassis.

## Source guide

- [Wheel mixing and speed control](Application/chassis.c)
- [Power limiting](Application/chassis_power_control.c)
- [Dual-board command transport](modules/Dual_board/Dual_board.c)
- [CAN feedback and output](modules/CAN_receive/CAN_receive.c)
- [Task creation](Src/freertos.c)
- [Original Keil project](MDK-ARM/C_Board_Standard_Robot.uvprojx)

The source maps chassis commands to four wheel-speed references, reads speed feedback, calculates PID outputs, applies power limiting, and sends CAN commands.

The supplied startup creates the INS wrapper, chassis, board-to-board receive, and referee tasks. The command path assumes the corresponding board configuration.

See [build instructions](../../docs/build.md), [recording plan](../../docs/recording_plan.md), and [attribution](../../THIRD_PARTY_NOTICES.md).

The retained [upstream INS README](UPSTREAM_README.md) describes the credited upstream estimator. A new hardware recording and measured response are still to be added.
