# Functional Requirements

Complete this document to specify what features you need.

---

## 1. Primary Use Case

Select your primary use case (check one or more):

- [ ] **Dual Independent Sniffer**: Monitor two separate CAN buses simultaneously
- [ ] **Bridge/Repeater with Sniffing**: Pass traffic between two buses while logging
- [ ] **Single Bus Dual-Transceiver**: One bus with redundant transceivers
- [ ] **Man-in-the-Middle**: Intercept, modify, and forward CAN traffic
- [ ] **CAN Injector**: Inject custom messages while sniffing

---

## 2. CAN Bus Parameters

### Baud Rate Support

Check all baud rates you need to support:

- [ ] 10 kbps
- [ ] 20 kbps
- [ ] 50 kbps
- [ ] 100 kbps
- [ ] 125 kbps (common in automotive)
- [ ] 250 kbps (common in automotive)
- [ ] 500 kbps (common in automotive)
- [ ] 800 kbps
- [ ] 1 Mbps (common in automotive)
- [ ] Auto-detect baud rate

### CAN Frame Types

- [ ] Standard CAN (11-bit IDs) - Required
- [ ] Extended CAN (29-bit IDs) - Required / Not needed
- [ ] RTR frames support - Required / Not needed
- [ ] Error frame detection - Required / Nice to have / Not needed

### CAN FD

- [ ] CAN FD support needed (Note: TJA1042 does NOT support CAN FD)
- [x] Classic CAN only is sufficient

---

## 3. USB Communication Protocol

Select preferred protocol for SavvyCAN communication:

- [ ] **SLCAN/LAWICEL** (Recommended for simplicity)
  - ASCII-based serial protocol
  - Uses USB CDC (virtual COM port)
  - Easy to debug with terminal
  - Widely supported

- [ ] **GVRET** (Generalized Vehicle Reverse Engineering Tool)
  - Binary protocol, more efficient
  - Native SavvyCAN format
  - Supports advanced features

- [ ] **GS_USB / CANtact**
  - Native USB CAN protocol
  - Fastest performance
  - More complex to implement

- [ ] **Other**: _______________

---

## 4. Operating Modes

### Sniffer Mode

- [ ] Silent mode (listen only, never transmit)
- [ ] Active mode (can transmit when requested)
- [ ] Timestamp messages: Yes / No
- [ ] Timestamp resolution: ___ µs / ms

### Repeater/Bridge Mode

- [ ] Forward CAN1 → CAN2
- [ ] Forward CAN2 → CAN1
- [ ] Bidirectional forwarding
- [ ] Selective forwarding (filter by ID)
- [ ] Message modification capability
- [ ] Latency requirement: ___ µs max

### Injection Mode

- [ ] Single message injection
- [ ] Periodic message injection
- [ ] Triggered injection (on receiving specific ID)

---

## 5. Filtering

- [ ] Hardware filtering by CAN ID
- [ ] Software filtering by CAN ID
- [ ] Mask-based filtering
- [ ] Whitelist mode (only specified IDs)
- [ ] Blacklist mode (all except specified IDs)

---

## 6. LED Indicators

What should LEDs indicate?

| LED | Suggested Function | Priority |
|-----|-------------------|----------|
| LED1 | | Must have / Nice to have |
| LED2 | | Must have / Nice to have |
| LED3 | | Must have / Nice to have |

Common options:
- Power on
- USB connected/enumerated
- CAN1 activity (RX blink)
- CAN1 activity (TX blink)
- CAN2 activity (RX blink)
- CAN2 activity (TX blink)
- Error indication
- Mode indicator

---

## 7. Configuration

How should the device be configured?

- [ ] USB commands (via SavvyCAN or terminal)
- [ ] Hardware switches/jumpers
- [ ] Saved configuration in EEPROM/Flash
- [ ] Default configuration only (no user config needed)

Configurable parameters needed:
- [ ] Baud rate for each CAN channel
- [ ] Operating mode (sniffer/repeater/both)
- [ ] Filter settings
- [ ] Timestamp enable/disable
- [ ] Silent mode enable/disable

---

## 8. Performance Requirements

| Parameter | Requirement |
|-----------|-------------|
| Max messages per second (sniffing) | ___ msg/s (typical CAN bus: 5000-8000) |
| Max latency for repeater mode | ___ µs |
| Buffer size (messages) | ___ messages |
| Acceptable message loss | 0% / <1% / <5% |

---

## 9. Future Expansion (Optional)

Features you might want later (helps with architecture decisions):

- [ ] Firmware update over USB (bootloader)
- [ ] SD card logging
- [ ] Standalone operation (without PC)
- [ ] WiFi/Bluetooth connectivity
- [ ] CAN FD upgrade (different transceiver)
- [ ] LIN bus support
- [ ] K-Line / ISO9141 support

---

## 10. Priority Order

List your features in priority order (most important first):

1. ________________________________
2. ________________________________
3. ________________________________
4. ________________________________
5. ________________________________

---

## 11. Additional Notes

Any other requirements or constraints:

```


```

