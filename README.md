# Tower of IO stuff aka TIOS

## Why?
I'm building a hexapod and want to make my own controller for it.  
Why would you design this thing?
- Learning experience
- Fun?

## What?

The hardware will be in different layers/floors/segments...  
Main restriction is limiting the size of the core to 50x50mm, because that was the max size at which 4 layers was still 'cheap'.  
But no EMI testing to pass yet and trying to keep prototyping costs low, so 2 layers for now...

### Core 

The main part of the tower.  

#### Software
- Standard STM32MP1 boostage of TF-A, OPTEE and U-Boot
- Recent kernel based on ST ECO 5.0.0
- [Debian 12.x]([https://www.debian.org/](https://www.debian.org/releases/stable/amd64/release-notes/index.en.html)) rootfs
- My other project to interface with sensors etc [dcafs](https://github.com/vlizBE/dcafs)
#### Hardware
- Two layer PCB with SoM on one side and nearly everything else on the other.
- [STM32MP151 SOM by Myir](https://www.myirtech.com/list.asp?id=658)
  - 8 uarts (one used for debug port)
  - One ethernet port up to Gbit speeds
  - 4GB eMMC, 512MB ram
  - Two available SDMMC's (third one has the eMMC)
  - Pretty much all pins brought out
- **Power:**
  - [36V to 5V(0.6A max) regulator](https://www.monolithicpower.com/en/mpm3506a.html)
- **On-board Connectivity**
  - microsd slot
  - USB Client for usb-gadget and updating
  - [Debug uart via USB](https://www.silabs.com/interface/usb-bridges/usbxpress)
  - Recovery mode switch
    
### Base IO

Will be the base 'floor', providing:
- one ethernet RJ45
- one USB type A host connector
- 26V to 5V@2A DC-DC 
- Filtered Power in passthrough to upper layers via one of the reserved pins
