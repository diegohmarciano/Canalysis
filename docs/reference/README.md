# Reference Materials

Place reference designs, application notes, and related documentation here.

## Useful References for This Project

### CAN Protocol
- [ ] `CAN_specification_2.0.pdf` - CAN 2.0 specification
- [ ] `ISO_11898.pdf` - ISO CAN standard (if available)

### USB Implementation
- [ ] USB CDC specification
- [ ] USB Device Framework

### SavvyCAN / SLCAN
- [ ] SLCAN/LAWICEL protocol documentation
- [ ] GVRET protocol documentation

### Similar Projects (for reference)
- [CANtact](https://github.com/linklayer/cantact-fw) - STM32 CAN adapter
- [CANable](https://github.com/normaldotcom/canable-fw) - Another STM32 CAN adapter  
- [SLCAN Arduino](https://github.com/kahiroka/slcan) - Arduino SLCAN implementation
- [USBtin](https://www.fischl.de/usbtin/) - USB CAN adapter

## Protocol Quick Reference

### SLCAN/LAWICEL Commands (for reference)

| Command | Description | Example |
|---------|-------------|---------|
| `O` | Open CAN channel | `O\r` |
| `C` | Close CAN channel | `C\r` |
| `Sn` | Set baud rate (n=0-8) | `S6\r` (500kbps) |
| `tiiildd..` | Transmit standard frame | `t1230123\r` |
| `Tiiiiiiiildd..` | Transmit extended frame | `T123456780123\r` |
| `V` | Get version | `V\r` → `V1234` |
| `N` | Get serial number | `N\r` → `NXXXX` |

Baud rate codes:
- S0 = 10 kbps
- S1 = 20 kbps
- S2 = 50 kbps
- S3 = 100 kbps
- S4 = 125 kbps
- S5 = 250 kbps
- S6 = 500 kbps
- S7 = 800 kbps
- S8 = 1 Mbps

### Received Frame Format

Standard frame: `tiiildd..\r`
- `t` = standard frame marker
- `iii` = 3 hex digits for 11-bit ID
- `l` = data length (0-8)
- `dd..` = data bytes (2 hex chars each)

Extended frame: `Tiiiiiiiildd..\r`
- `T` = extended frame marker
- `iiiiiiii` = 8 hex digits for 29-bit ID

## Notes

```
Add any notes about reference materials here.

```

