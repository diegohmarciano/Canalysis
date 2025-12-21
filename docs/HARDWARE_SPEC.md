# Hardware Specification - CANalysis

Dual-channel isolated CAN-USB adapter based on PIC32MX795F512L.

---

## 1. Microcontroller

| Parameter | Value |
|-----------|-------|
| MCU Part Number | PIC32MX795F512L |
| Package | TQFP-100 |
| Primary Crystal Frequency | 8.000 MHz |
| Secondary Crystal (RTC) | No |
| Operating Voltage | 3.3V |

---

## 2. CAN Transceivers

Both CAN channels are **galvanically isolated** via F0505XT-1WR2 DC-DC converters.

### TJA1042 #1 (CAN Channel 1)

| Parameter | Value |
|-----------|-------|
| Connected to MCU CAN Module | CAN1 |
| TXD connected to MCU pin | Pin 88 (C1TX/ETXD0/PMD10/RF1) |
| RXD connected to MCU pin | Pin 87 (C1RX/ETXD1/PMD11/RF0) |
| STB (Standby) pin | Tied to DGND (isolated ground, always active) |
| Termination on board | DIP switch: OFF=0Ω, 1-ON=120Ω, 2-ON=60Ω |
| Isolation | F0505XT-1WR2 (IC4) |

### TJA1042 #2 (CAN Channel 2)

| Parameter | Value |
|-----------|-------|
| Connected to MCU CAN Module | CAN2 |
| TXD connected to MCU pin | Pin 89 (C2TX/ETXERR/PMD9/RG1) |
| RXD connected to MCU pin | Pin 90 (C2RX/PMD8/RG0) |
| STB (Standby) pin | Tied to DGND (isolated ground, always active) |
| Termination on board | DIP switch: OFF=0Ω, 1-ON=120Ω, 2-ON=60Ω |
| Isolation | F0505XT-1WR2 (IC5) |

---

## 3. USB Interface

| Parameter | Value |
|-----------|-------|
| USB Connector Type | Type-B |
| Power Source | Bus-powered |
| VBUS Detection | Yes (pin VBUS) |
| USB ID Pin Used | No |
| ESD Protection IC | none |

---

## 4. Complete Pin Mapping

| MCU Pin | Port.Bit | Function | Connected To | Notes |
|---------|----------|----------|--------------|-------|
| 63 | RC12 | OSC1/CLKI | Primary crystal | 8.000 MHz |
| 64 | RC15 | OSC2/CLKO | Primary crystal | |
| 87 | RF0 | C1RX | TJA1042 #1 RXD | CAN1 receive |
| 88 | RF1 | C1TX | TJA1042 #1 TXD | CAN1 transmit |
| 89 | RG1 | C2TX | TJA1042 #2 TXD | CAN2 transmit |
| 90 | RG0 | C2RX | TJA1042 #2 RXD | CAN2 receive |
| 57 | RG2 | D+ | USB D+ | |
| 56 | RG3 | D- | USB D- | |
| 54 | - | VBUS | USB VBUS | 5V sense |
| 55 | - | VUSB | USB PHY power | 3.3V via AMS1117 |
| 95 | RG14 | GPIO | LED2 | Active High |
| 96 | RG12 | GPIO | LED3 | Active High |
| 97 | RG13 | GPIO | LED4 | Active High |
| 26 | RB6 | PGEC2 | ICSP Pin 5 | Programming Clock |
| 27 | RB7 | PGED2 | ICSP Pin 4 | Programming Data |
| 13 | - | MCLR | ICSP Pin 1 | Via 100Ω resistor |
| 2 | - | VDD | 3.3V rail | |
| 16 | - | VDD | 3.3V rail | |
| 37 | - | VDD | 3.3V rail | |
| 62 | - | VDD | 3.3V rail | |
| 86 | - | VDD | 3.3V rail | |
| 85 | - | VCAP/VDDCORE | Core voltage | Internal regulator |
| 30 | - | AVDD | 3.3V rail | Analog power |
| 15 | - | VSS | GND | |
| 36 | - | VSS | GND | |
| 45 | - | VSS | GND | |
| 65 | - | VSS | GND | |
| 75 | - | VSS | GND | |
| 31 | - | AVSS | GND | Analog ground |

---

## 5. Power Supply

| Parameter | Value |
|-----------|-------|
| Input Voltage Range | 5V |
| Main Regulator | AMS1117 |
| MCU Supply Voltage | 3.3V |
| CAN Transceiver Supply | 5V |
| USB VBUS available for power | Yes |

---

## 6. LEDs and User Interface

| LED/Button | MCU Pin | Color/Function | Active Level |
|------------|---------|----------------|--------------|
| LED1 | | | High / Low |
| LED2 | 95 | CAN1 Listening | High |
| LED3 | 96 | USB Connected  | High |
| LED4 | 97 | CAN2 Listening | High |

---

## 7. CAN Bus Isolation

Both CAN channels feature galvanic isolation for electrical safety and noise immunity.

| Parameter | CAN1 | CAN2 |
|-----------|------|------|
| Isolation IC | F0505XT-1WR2 (IC4) | F0505XT-1WR2 (IC5) |
| Input Voltage | VCC (5V) | VCC (5V) |
| Isolated Output | VDD1/VDD2 → TJA1042 | VDD1/VDD2 → TJA1042 |
| Isolated Ground | DGND | DGND |
| Isolation Voltage | 1500VDC (per datasheet) | 1500VDC (per datasheet) |

**Note:** The STB (standby) pins on both TJA1042 transceivers are tied to DGND (isolated ground), meaning the transceivers are always in active mode and cannot be put into standby via MCU control.

---

## 8. Additional Components

List any other components on the board:

- [x] F0505XT-1WR2 Isolated DC-DC Converters (x2) - CAN isolation
- [ ] Other: ___

---

## 9. CAN Bus Connectors

*Note: Physical connector type not relevant for firmware development.*

| Connector | Type | Pinout |
|-----------|------|--------|
| CAN1 | Quick connector | CANH, CANL |
| CAN2 | Quick connector | CANH, CANL |

---

## 10. Notes and Special Considerations

- CAN transceivers are always active (STB tied to DGND) - no standby mode available via MCU
- Both CAN channels are galvanically isolated - DGND is separate from MCU GND
- Board is bus-powered via USB (5V)
- Crystal: 8.000 MHz confirmed from board markings

---

## 11. Board Photos (Optional)

If you have photos of the board, place them in `schematics/photos/` and reference them here:

- Top view: `schematics/photos/board_top.jpg`
- Bottom view: `schematics/photos/board_bottom.jpg`

---

## 12. ICSP Header
### ICSP (In-Circuit Serial Programming) Header

| Pin | Signal | PIC32 Pin | Notes |
|-----|--------|-----------|-------|
| 1   | MCLR   | 13        | Connected via 100Ω resistor |
| 2   | VDD    | 3.3V      | Power |
| 3   | GND    | GND       | Ground |
| 4   | PGED2  | 27        | Programming Data |
| 5   | PGEC2  | 26        | Programming Clock |

- Standard 5-pin header for PIC32 ICSP.
- There is a 100ohm resistor between ICSP header and the pin 13.
- Pinout assumes traditional ICSP orientation (Pin 1 marked).



