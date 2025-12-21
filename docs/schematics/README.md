# Schematics

Place your board schematics and design files here.

## File Organization

```
schematics/
├── README.md              # This file
├── main_schematic.pdf     # Full schematic (printable PDF)
├── main_schematic.png     # Image version for quick reference
├── photos/                # Board photos
│   ├── board_top.jpg
│   └── board_bottom.jpg
└── source/                # Original CAD files
    ├── project.kicad_pro  # (or .brd, .sch, etc.)
    └── ...
```

## Schematic Checklist

Please ensure your schematic clearly shows:

### MCU Section
- [ ] PIC32MX795F512L with all power pins
- [ ] Decoupling capacitors
- [ ] Crystal connections (primary and secondary if used)
- [ ] Reset circuit
- [ ] ICSP/programming header (if present)

### CAN Section
- [ ] TJA1042 #1 complete circuit
- [ ] TJA1042 #2 complete circuit
- [ ] CAN bus connectors
- [ ] Termination resistors (if present)
- [ ] TVS/ESD protection (if present)

### USB Section
- [ ] USB connector pinout
- [ ] ESD protection (if present)
- [ ] VBUS detection circuit (if present)
- [ ] USB ID connection (if present)

### Power Section
- [ ] Input power source
- [ ] Voltage regulator(s)
- [ ] Power indicators

## Accepted Formats

| Format | Extension | Notes |
|--------|-----------|-------|
| PDF | `.pdf` | Preferred for viewing |
| PNG/JPG | `.png`, `.jpg` | For quick reference |
| KiCad | `.kicad_pro`, `.kicad_sch` | Place in `source/` |
| Altium | `.PrjPcb`, `.SchDoc` | Place in `source/` |
| Eagle | `.brd`, `.sch` | Place in `source/` |
| EasyEDA | Export as PDF/PNG | |

## If You Don't Have a Schematic

If this is a commercial board or you don't have schematics:

1. Take clear photos of both sides of the board
2. Trace the connections manually and fill out `HARDWARE_SPEC.md`
3. Note any visible component markings

## Board Origin

- [ ] Custom designed board
- [ ] Commercial product (specify: _____________)
- [ ] Development kit (specify: _____________)
- [ ] Clone/derivative of (specify: _____________)

