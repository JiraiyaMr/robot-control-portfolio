# Hardware and Startup Configuration

These facts come from the supplied source and project configuration. Physical wiring, actuator models, and calibration should be checked against the robot used for the demonstration.

## Common configuration

| Item | Evidence in the snapshots |
| --- | --- |
| MCU | STM32F407IGHx in the Keil projects |
| Clock | 168 MHz in the CubeMX configurations |
| OS | FreeRTOS with CMSIS-RTOS interfaces |
| RTOS tick | 1000 Hz in FreeRTOSConfig.h |
| Motor transport | CAN transmit commands and interrupt-based feedback |
| Command routing | Local publish / subscribe messages and dual-board CAN modules |
| Inertial sensing | BMI088 code and quaternion EKF integration; activation varies by snapshot |

## Tasks started by the supplied configuration

| Snapshot | Tasks created in freertos.c | Tasks present but not created there |
| --- | --- | --- |
| Mecanum chassis | StartINSTask, chassis_task, can_receive_task, referee_usart_task | gimbal_task, helm_task, robot_task, supercap_task and other commented alternatives |
| Swerve | StartDefaultTask, chassis_task, helm_task, can_receive_task | gimbal_task, robot_task, ins_task, supercap_task |
| Gimbal / INS | StartINSTask, pc_task, usb_task | gimbal_task, chassis_task, robot_task, dual-board tasks and other commented alternatives |

Mecanum and gimbal StartINSTask initialize USB and the INS pipeline. Swerve StartDefaultTask is an idle loop.

The wheel and steering task bodies use `osDelay(2)`; the two active INS wrappers use `osDelay(1)`. At the configured tick rate these are 2 ms and 1 ms requested delays, respectively, in addition to computation and scheduling time. They are not measurements of exact control frequency.

## Information to record on the real robot

- Board revision, motor and driver models, wiring, and bus connections.
- Wheel radius, gear ratios, steering zero offsets, and applicable sign conventions.
- Supply voltage, payload, and controller configuration.
- Measured loop period and log period.
- Configuration of the other board when the dual-board command path is used.
