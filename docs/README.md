# CANalysis Documentation

This directory contains all documentation for the CANalysis dual CAN-to-USB interface.

## Quick Start

1. **Flash the firmware** to your PIC32MX795F512L board
2. **Connect via USB** - device appears as a COM port
3. **Use SavvyCAN** for professional CAN analysis, or
4. **Use any serial terminal** for debug commands

## Documentation Index

| Document | Description |
|----------|-------------|
| [SOFTWARE.md](SOFTWARE.md) | **Firmware documentation** - architecture, usage, Phase 4 roadmap |
| [HARDWARE_SPEC.md](HARDWARE_SPEC.md) | Hardware specifications and pin mapping |
| [REQUIREMENTS.md](REQUIREMENTS.md) | Original requirements and use cases |

## Directory Structure

```
docs/
├── README.md              # This file
├── SOFTWARE.md            # Firmware documentation (main reference)
├── HARDWARE_SPEC.md       # Hardware specifications
├── REQUIREMENTS.md        # Project requirements
├── datasheets/            # Component datasheets
│   ├── PIC32MX5XX...pdf   # MCU datasheet
│   ├── TJA1042.pdf        # CAN transceiver
│   └── ...
├── schematics/            # Board schematics and photos
│   ├── photos/            # Board photos
│   └── source/            # Original schematic images
└── reference/             # Reference documents and app notes
```

## Project Status

| Phase | Description | Status |
|-------|-------------|--------|
| Phase 1 | Hardware Bring-up | ✅ Complete |
| Phase 2 | CAN Sniffer (RX) | ✅ Complete |
| Phase 3 | CAN Injection (TX) | ✅ Complete |
| Phase 4 | MITM Bridging | 🔲 Planned |

See [SOFTWARE.md](SOFTWARE.md) for detailed Phase 4 planning.

## Key Features

- **SavvyCAN Compatible**: Full GVRET protocol support
- **Dual CAN Channels**: Independent CAN1 and CAN2
- **Bidirectional**: Sniffing and injection
- **Configurable**: Any standard baud rate
- **Debug Menu**: Built-in loopback tests

## Hardware

- **MCU**: PIC32MX795F512L @ 48MHz
- **CAN**: 2× TJA1042 transceivers
- **USB**: Full-speed CDC (Virtual COM Port)
