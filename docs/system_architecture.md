# Control and Communication Architecture

The diagrams below follow the source. The startup table in [hardware.md](hardware.md) identifies which tasks are created in each supplied snapshot.

## Mecanum chassis

```mermaid
flowchart TB
    U["Upper-board CAN command"] --> M["Dual-board receiver and chassis_cmd topic"]
    M --> K["Four-wheel velocity mixing"]
    K --> C["Wheel-speed PID"]
    C --> P["Power limiting"]
    F["Referee power data"] --> P
    P --> D["CAN motor commands"]
    D --> W["Wheel motors"]
    W --> E["CAN speed feedback"]
    E --> C
```

Source: `Application/chassis.c`, `Application/chassis_power_control.c`, and `modules/Dual_board/Dual_board.c`.

## Swerve drive

```mermaid
flowchart TB
    R["Chassis command topic"] --> V["Wheel-speed decomposition"]
    R --> A["Steering-angle decomposition"]
    V --> C["Drive speed PID"]
    A --> S["Steering angle and speed PID"]
    C --> D["Drive motor CAN commands"]
    S --> H["Steering motor CAN commands"]
    D --> F["CAN feedback"]
    H --> F
    F --> C
    F --> S
```

Source: `Applications/chassis.c` and `Applications/helm.c`. The steering code folds target angles and the drive code changes wheel direction for corresponding orientations.

## Gimbal source path

```mermaid
flowchart TB
    I["BMI088 samples"] --> E["Quaternion EKF and INS"]
    E --> S["ins_feedback topic"]
    S --> V["Yaw rate feedback"]
    R["Gimbal command topic"] --> A["Yaw angle controller"]
    A --> C["Yaw rate controller"]
    V --> C
    C --> M["CAN yaw motor command"]
    R --> P["Pitch motor interface"]
```

Source: `Application/ins_task.c`, `Application/gimbal.c`, and `Bsp/DMPower.c`. The gimbal task's creation is commented out in the supplied snapshot. This diagram describes the available control path, not an observed closed-loop hardware run.
