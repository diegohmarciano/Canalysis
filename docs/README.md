# CANalysis Documentation

This directory contains all hardware documentation, datasheets, and project specifications for the CANalysis dual CAN sniffer project.

## Directory Structure

```
docs/
├── README.md                    # This file
├── HARDWARE_SPEC.md             # Hardware specifications and pin mapping
├── REQUIREMENTS.md              # Functional requirements and use cases
├── datasheets/                  # Component datasheets
│   ├── README.md                # Index of datasheets
│   ├── PIC32MX795F512L.pdf      # MCU datasheet
│   ├── TJA1042.pdf              # CAN transceiver datasheet
│   └── ...
├── schematics/                  # Board schematics
│   ├── README.md                # Schematic notes
│   ├── main_schematic.pdf       # Full schematic (or image)
│   ├── main_schematic.png       # Image version if applicable
│   └── source/                  # Original CAD files (KiCad, Altium, etc.)
└── reference/                   # Reference designs and application notes
    └── README.md
```

## Quick Start

1. Fill out `HARDWARE_SPEC.md` with your board's pin mapping and hardware details
2. Fill out `REQUIREMENTS.md` with your desired features and use cases
3. Add datasheets to the `datasheets/` folder
4. Add schematic files to the `schematics/` folder

Once complete, the AI assistant can use this information to generate appropriate firmware.

