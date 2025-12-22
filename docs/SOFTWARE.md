# CANalysis Firmware Documentation

## Version 1.0 - SavvyCAN Integration

This firmware provides full GVRET protocol support for SavvyCAN integration, enabling professional-grade CAN bus sniffing and injection on both CAN channels.

## Project Status

| Phase | Description | Status |
|-------|-------------|--------|
| Phase 1 | Hardware Bring-up | ✅ Complete |
| Phase 2 | CAN Sniffer (RX) | ✅ Complete |
| Phase 3 | CAN Injection (TX) | ✅ Complete |
| Phase 4 | MITM Bridging | 🔲 Planned |

## Overview

The CANalysis firmware provides:
- **GVRET Protocol**: Full compatibility with SavvyCAN software
- **Dual CAN Channels**: Independent CAN1 and CAN2 buses
- **Bidirectional**: Both sniffing (RX) and injection (TX) support
- **USB CDC**: Virtual COM Port interface at full speed
- **Debug Menu**: Built-in loopback tests and raw CAN debugging

## Hardware Requirements

- **MCU:** PIC32MX795F512L @ 48MHz (via 8MHz crystal + PLL)
- **CAN Transceivers:** 2× TJA1042
  - CAN1: RF0 (RX), RF1 (TX)
  - CAN2: RG0 (RX), RG1 (TX)
- **USB:** CDC Virtual COM Port (full speed)
- **LEDs:**
  - LED1: USB power (hardware-connected to VBUS)
  - LED2 (RG14): Interface connected (DTR active)
  - LED3 (RG12): CAN1/USB activity
  - LED4 (RG13): CAN2 activity

## Features

### SavvyCAN Integration (GVRET Protocol)
- Auto-detection by SavvyCAN via magic byte sequence (`0xE7`)
- Binary protocol for efficient data transfer
- Microsecond-resolution timestamps
- Configurable baud rates (common speeds: 125k, 250k, 500k, 1M)
- Standard (11-bit) and Extended (29-bit) ID support
- Listen-only mode for passive sniffing

### Debug Menu (Serial Terminal)
Connect via any serial terminal (not SavvyCAN) for debug commands:

| Command | Description |
|---------|-------------|
| `d` | **Debug CAN RX** - Print raw CAN IDs at 500kbps |
| `i` | **Internal Loopback (CAN1)** - Self-test without bus |
| `l` | **Internal Loopback (CAN2)** - Self-test without bus |
| `p` | **External Loopback (CAN1→CAN2)** - Test via physical bus |
| `q` | **External Loopback (CAN2→CAN1)** - Test via physical bus |
| `s` | **Stop** - Stop current test |

## Architecture

### File Structure

```
Canalysis.X/
├── main.c              # Main loop, USB callbacks, debug menu
├── gvret.c             # GVRET protocol state machine
├── gvret.h             # GVRET protocol definitions
├── can_driver.c        # CAN hardware abstraction layer
├── can_driver.h        # CAN driver interface
├── version.h           # Firmware version definitions
├── usb_config.h        # USB stack configuration
├── usb_descriptors.c   # USB device descriptors
├── CAN.h               # CAN register definitions
└── usb/                # M-Stack USB library
    ├── usb.c
    ├── usb_cdc.c
    └── usb_hal.h
```

### Key Components

#### GVRET Protocol (`gvret.c`)
- State machine for parsing binary commands
- TX buffer management for USB responses
- CAN frame encoding/decoding
- Timestamp generation via Core Timer

#### CAN Driver (`can_driver.c`)
- Hardware abstraction for CAN1/CAN2 modules
- Baud rate calculation for any speed
- 8-message RX FIFO per channel
- Software RX queue (32 messages per bus)
- Polling-based RX to prevent USB starvation

#### Main Loop (`main.c`)
- USB service and GVRET flush
- CAN FIFO polling in GVRET mode
- Debug menu command handling
- Loopback test implementations

### Important Implementation Notes

#### PIC32 Write-Only Bits
The CAN module's `UINC` and `TXREQ` bits are **write-only** and require atomic SET register access:

```c
// WRONG - read-modify-write corrupts write-only bits
C1FIFOCON1bits.UINC = 1;

// CORRECT - atomic SET operation
C1FIFOCON1SET = _C1FIFOCON1_UINC_MASK;
```

#### Polling vs Interrupts
GVRET mode uses polling (not interrupts) for CAN RX to prevent USB starvation:
- ISR clears flags and returns immediately
- Main loop polls `RXNEMPTYIF` and processes frames
- Ensures USB gets regular service time

## Building

### Requirements
- MPLAB X IDE v6.20+
- XC32 Compiler v5.00+
- PIC32MX DFP v1.5.259

### Build Commands

```batch
cd Canalysis.X
"C:\Program Files\Microchip\MPLABX\v6.20\gnuBins\GnuWin32\bin\make.exe" -f nbproject/Makefile-default.mk CONF=default
```

Output: `Canalysis.X/dist/default/production/Canalysis.X.production.hex`

## Usage

### With SavvyCAN
1. Connect device via USB
2. Open SavvyCAN → Connection → Open Connection Window
3. Select "GVRET" and choose the COM port
4. Click "Create New Connection"
5. Enable desired CAN bus and set baud rate
6. CAN traffic will appear in the main window

### With Serial Terminal (Debug)
1. Connect device via USB
2. Open terminal (PuTTY, TeraTerm, etc.) at any baud rate
3. Press Enter to see welcome message and commands
4. Use `d` for quick CAN RX test at 500kbps

---

## Phase 4: MITM Bridging (Planned)

### Overview
Phase 4 will add firmware-level CAN bridging between CAN1 and CAN2, enabling Man-in-the-Middle (MITM) functionality without host involvement.

### Planned Features

#### Basic Bridging
- CAN1 RX → CAN2 TX (automatic forwarding)
- CAN2 RX → CAN1 TX (automatic forwarding)
- Sub-millisecond latency (firmware-level, no USB round-trip)
- Works even if USB is disconnected

#### Filter Rules
- **Block rules**: Prevent specific CAN IDs from being forwarded
- **Pass rules**: Only forward specific CAN IDs (whitelist mode)
- Configurable per-direction (CAN1→CAN2 vs CAN2→CAN1)

#### Modify Rules
- Change specific data bytes before forwarding
- Mask/set operations for bit-level modification
- ID remapping (change ID during forwarding)

#### Logging
- Still send copies of all traffic to USB for SavvyCAN monitoring
- Mark frames as "original" vs "forwarded" vs "modified"

### Proposed GVRET Extensions

New commands for bridging control:

| Command | Code | Description |
|---------|------|-------------|
| `SET_BRIDGE_MODE` | 0x20 | Enable/disable bridging |
| `ADD_BLOCK_RULE` | 0x21 | Block a CAN ID from forwarding |
| `ADD_MODIFY_RULE` | 0x22 | Modify frame before forwarding |
| `CLEAR_RULES` | 0x23 | Remove all filter/modify rules |
| `GET_BRIDGE_STATUS` | 0x24 | Query current bridge state |

### Architecture Changes

```
           ┌─────────────┐
 CAN1 ◄───►│  CAN1 RX    │◄──────────────────────────┐
           │  CAN1 TX    │───────────────────────────┼───► USB/Host
           └─────────────┘                           │
                 │                                   │
                 ▼                                   │
           ┌─────────────┐                           │
           │   Bridge    │ ◄── Filter/Modify Rules   │
           │   Logic     │                           │
           └─────────────┘                           │
                 │                                   │
                 ▼                                   │
           ┌─────────────┐                           │
 CAN2 ◄───►│  CAN2 RX    │◄──────────────────────────┘
           │  CAN2 TX    │
           └─────────────┘
```

### Use Cases
- **ECU Interception**: Sit between ECU and rest of vehicle
- **Sensor Spoofing**: Modify sensor data in real-time
- **Gateway Testing**: Simulate gateway filtering behavior
- **Security Research**: Analyze and modify ECU communications

---

## Changelog

### v1.0 - SavvyCAN Integration (December 2024)
- Full GVRET protocol implementation
- SavvyCAN compatibility verified
- Bidirectional CAN support (RX + TX)
- Configurable baud rates
- Debug menu with loopback tests
- Fixed PIC32 write-only bit handling (UINC/TXREQ)

### v0.1 - Initial Test (December 2024)
- USB CDC enumeration
- CAN1/CAN2 internal loopback tests
- External loopback tests
- Basic command interface

## License

This project is for educational and development purposes.
