# Firmware Snapshots

| Snapshot | Main project | Control focus |
| --- | --- | --- |
| [Mecanum chassis](mecanum-chassis/README.md) | C_Board_Standard_Robot.uvprojx | Four-wheel drive and power limiting |
| [Swerve](swerve/README.md) | WUST_Frame_Two.uvprojx | Drive and steering |
| [Gimbal / INS](omni-gimbal/README.md) | C_Board_Standard_Robot.uvprojx | Gimbal source, inertial sensing, and host communication |

Retained firmware files preserve the supplied archive bytes. The import removes generated build outputs and unrelated vendor material while keeping local project dependencies.

See [build notes](../docs/build.md), [task configuration](../docs/hardware.md), and [source manifest](../docs/source_manifest.json).
