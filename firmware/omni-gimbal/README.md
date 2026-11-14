# Gimbal and Inertial-Sensing Firmware

STM32F407 / FreeRTOS snapshot containing gimbal control code, BMI088 inertial acquisition, quaternion EKF integration, and host communication.

## Source guide

- [Gimbal control](Application/gimbal.c)
- [INS integration](Application/ins_task.c)
- [Quaternion EKF component](Components/Algorithm/QuaternionEKF.c)
- [Pitch motor interface](Bsp/DMPower.c)
- [PC communication](modules/PC_task/PC_task.c)
- [USB task](modules/usb_task/usb_task.c)
- [Task creation](Src/freertos.c)
- [Original Keil project](MDK-ARM/C_Board_Standard_Robot.uvprojx)

**Supplied startup:** INS, PC, and USB tasks are created. Gimbal-task creation is commented out. The available yaw and pitch control paths should therefore be distinguished from this snapshot's enabled tasks.

See [build instructions](../../docs/build.md), [recording plan](../../docs/recording_plan.md), and [attribution](../../THIRD_PARTY_NOTICES.md).

The [upstream INS README](UPSTREAM_README.md) and EKF author headers are preserved. A closed-loop hardware recording and measured tracking data are still to be added.
