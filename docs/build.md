# Building the Firmware

## Original Keil projects

| Snapshot | Project file | Compiler recorded in project | Device pack recorded in project |
| --- | --- | --- | --- |
| Mecanum chassis | firmware/mecanum-chassis/MDK-ARM/C_Board_Standard_Robot.uvprojx | ARMCC 5.06 update 5, build 528 | Keil.STM32F4xx_DFP.2.17.1 |
| Gimbal / INS | firmware/omni-gimbal/MDK-ARM/C_Board_Standard_Robot.uvprojx | ARMCC 5.06 update 5, build 528 | Keil.STM32F4xx_DFP.2.12.0 |
| Swerve | firmware/swerve/MDK-ARM/WUST_Frame_Two.uvprojx | ARMCC 5.06 update 5, build 528 | Keil.STM32F4xx_DFP.2.17.1 |

Open the appropriate project in Keil, select its configured target, and use the recorded toolchain and device support. Required local source files and referenced ARM math libraries are included. Device packs, the compiler, and debugger configuration are environment dependencies.

Before a hardware run, match the task configuration, controller parameters, motor identifiers, calibration, and board role to the intended robot. Preserve the supplied snapshot while documenting any later experimental revision separately.

## Makefiles

The mecanum and gimbal snapshots also contain original GCC Makefiles. Their source lists do not include all application and communication files referenced by the current Keil projects. They are retained as source history and should be reconciled before using them as a complete build route for these snapshots. Required GCC math archives from the originals are retained.

## Verification scope

The packaging checks file integrity and local Keil source references. Firmware compilation, linking, flashing, and physical behavior have not been revalidated during this import. Original source encodings and line endings are retained; some include spelling relies on the case-insensitive Windows build environment.

A new, successful build should record the project, compiler, pack version, configuration, resulting commit, and build log.
