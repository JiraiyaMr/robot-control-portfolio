# Source Map

| Function | Mecanum / gimbal snapshots | Swerve snapshot |
| --- | --- | --- |
| Task creation | Src/freertos.c | Core/Src/freertos.c |
| Robot mode and command handling | Robots/robots.c | Robots/robots.c |
| Drive control | Application/chassis.c | Applications/chassis.c |
| Steering | Application/helm.c | Applications/helm.c |
| Gimbal | Application/gimbal.c | Applications/gimbal.c |
| PID implementation | Controller/pid.c | Controller/pid.c |
| Message transport | Communication/pub_sub.c | Communication/pub_sub.c |
| Board-to-board commands | modules/Dual_board/Dual_board.c | modules/Dual_board/Dual_board.c |
| Motor CAN feedback | modules/CAN_receive/CAN_receive.c | modules/CAN_receive/CAN_receive.c |
| INS wrapper | Application/ins_task.c | modules/ins/ins.c |
| EKF component | Components/Algorithm/QuaternionEKF.c | Components/Algorithm/QuaternionEKF.c |

Paths in the table are relative to each snapshot directory. Source availability does not establish that a task is created or that every mode has been exercised on hardware.

The referee, supercapacitor, UI, and competition-specific modules remain in their original snapshot so project references and context are preserved.
