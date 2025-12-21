# CANalysis Firmware Documentation

## Version 0.1 - Initial Test

This is the initial working firmware for the CANalysis dual CAN-to-USB interface.

## Overview

The CANalysis firmware provides a USB CDC (Virtual COM Port) interface for CAN bus testing and development. It supports both CAN1 and CAN2 modules on the PIC32MX795F512L microcontroller.

## Hardware Requirements

- **MCU:** PIC32MX795F512L running at 48MHz (USB clock via 8MHz crystal with UPLL)
- **CAN Transceivers:** 2x TJA1042 (CAN1 on RF0/RF1, CAN2 on RG0/RG1)
- **USB:** USB CDC Virtual COM Port at full speed
- **LEDs:**
  - LED1 (RC14): USB power indicator (directly connected to USB VCC)
  - LED2 (RD0): USB configured/active indicator
  - LED3 (RD1): CAN1 activity indicator
  - LED4 (RD2): CAN2 activity indicator

## Features

### USB CDC Interface
- Virtual COM Port for host communication
- 64-byte endpoint buffers
- Automatic enumeration as "CANalysis USB-CAN Bridge"

### CAN Bus Support
- Dual CAN channels (CAN1 and CAN2)
- 125 kbps baud rate (configurable)
- Standard ID (11-bit) messages
- 8-byte data frames
- Interrupt-driven reception

## Commands

Connect to the virtual COM port and use the following commands:

| Command | Description |
|---------|-------------|
| `i` | **Internal Loopback (CAN1)** - CAN1 transmits and receives its own messages internally. No bus connection required. |
| `l` | **Internal Loopback (CAN2)** - CAN2 transmits and receives its own messages internally. No bus connection required. |
| `p` | **External Loopback (CAN1→CAN2)** - CAN1 transmits, CAN2 receives via the physical bus. Requires CAN1 and CAN2 to be wired together with 120Ω termination. |
| `q` | **External Loopback (CAN2→CAN1)** - CAN2 transmits, CAN1 receives via the physical bus. Requires CAN1 and CAN2 to be wired together with 120Ω termination. |
| `s` | **Stop** - Stops the current test and displays summary statistics. |

## Test Output Example

```
=== CAN1 INTERNAL LOOPBACK TEST ===
(CAN1 TX -> CAN1 RX, no bus needed)

  CAN1: Full module reset...
  CAN1: Enabling module...
  CAN1: Entering config mode...
  CAN1: Configuring 125kbps...
  CAN1: Setting FIFO base...
  CAN1: OK (internal loopback)

Starting test... Press 's' to stop.

CAN1 TX: SID=0x123 Data=[00 CA FE BA BE FF 55 AA]
CAN1 RX: SID=0x123 Data=[00 CA FE BA BE FF 55 AA]
  OK - Data verified!

CAN1 TX: SID=0x123 Data=[01 CA FE BA BE FE 55 AA]
CAN1 RX: SID=0x123 Data=[01 CA FE BA BE FE 55 AA]
  OK - Data verified!

--- CAN Bus loopback test ---
    Packets: Sent = 2, Received = 2, Lost = 0 (0% loss)
    Timeouts = 0

Ready.
```

## Technical Details

### Memory Layout
- CAN message buffers use `__attribute__((coherent, aligned(2048)))` for DMA compatibility
- Separate 512-byte buffers for CAN1 and CAN2 FIFOs
- Physical addresses are converted using `KVA_TO_PA()` macro

### CAN Configuration
- **Baud Rate:** 125 kbps @ 48MHz peripheral clock
- **Bit Timing:** BRP=11, SJW=3, PRSEG=2, SEG1PH=5, SEG2PH=5
- **Sample Point:** ~75%
- **Filter:** Accept all messages (mask = 0)

### Interrupt Handling
- CAN1 and CAN2 use separate ISR vectors
- Priority level 4 (IPL4SOFT)
- ISRs handle both internal loopback (FIFO1) and external RX (FIFO0) modes
- Proper RBIF clearing prevents infinite interrupt loops

## Building

### Requirements
- MPLAB X IDE v6.20 or later
- XC32 Compiler v5.00 or later
- PIC32MX DFP v1.5.259

### Build Commands

From the project directory:
```batch
cd CANalysis.X
"C:\Program Files\Microchip\MPLABX\v6.20\gnuBins\GnuWin32\bin\make.exe" -f nbproject/Makefile-default.mk CONF=default
```

The output hex file is located at:
```
CANalysis.X/dist/default/production/CANalysis.X.production.hex
```

## File Structure

```
CANalysis.X/
├── main.c              # Main application code
├── version.h           # Firmware version definitions
├── usb_config.h        # USB stack configuration
├── usb_descriptors.c   # USB device descriptors
├── CAN.h              # CAN register definitions
└── usb/               # M-Stack USB library
    ├── usb.c
    ├── usb_cdc.c
    └── usb_hal.h
```

## Known Limitations

- Fixed 125 kbps baud rate (not runtime configurable in v0.1)
- Standard ID only (no extended ID support in v0.1)
- Single message FIFO depth (no buffering for high-traffic scenarios)

## Future Enhancements

- Configurable baud rates
- Extended ID (29-bit) support
- CAN FD support (requires different transceiver)
- Continuous sniffing mode
- Message injection mode
- Timestamp logging

## License

This project is for educational and development purposes.

## Changelog

### v0.1 - Initial Test (December 2025)
- USB CDC enumeration working
- CAN1 and CAN2 internal loopback tests
- External loopback tests (CAN1↔CAN2)
- Proper interrupt handling for all test modes
- Clean command interface

