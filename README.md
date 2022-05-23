# Tios
Tower of IO Stuff, hardware for logging software

## Why?

Why would you design this thing?
- There's no SBC out there (as far as I found) that matches the requirements
- Learning experience
- Fun?

So, what are those requirements?
- Low power consumption (pretty much rules out the regular RPi's and variants)
- On board eMMC
- Ethernet and Wifi
- Plenty of expansion IO's
- A lot of UART's

## What?

The hardware will be in different layers/floors/segments...
Main restriction is limiting the size of the core to 50x50mm, just because that's the size at which 4 layers are still cheap (at a certain pcb fab).

### Core

The main part of the tower (not neccessarily the base). Will contain minimum functionality required for a working system.
Which is:
- [iMX6ULL SOM by Myir](http://www.myirtech.com/list.asp?id=644)
  - Lower power consumption
  - 8 uarts
  - Built in Ethernet chip
  - 4GB eMMC
  - Pretty much all pins brought out
- microsd slot
- 5V to 3V3 regulator
- Wifi/BLE Chip 
- Battery backed RTC
- USB Client

### Base IO

This will expand the core to bring out other basic functionality that didn't fit on the core...
- Ethernet port
- 2x USB Host
- GPS (for timestamp)
- ... (wip)

### Analog

