# Tower of IO stuff aka TIOS

## Why?
I'm building a hexapod and want to make my own controller for it.  
Why would you design this thing?
- Learning experience
- Fun?

## What?

The hardware will be in different layers/floors/segments...  

### Core 

The main part of the tower.  

#### Software
- Standard STM32MP1 bootstage of TF-A, OPTEE and U-Boot
- Kernel (6.1.28) based on ST ECO 5.0.0
- [Debian 12.x]([https://www.debian.org/](https://www.debian.org/releases/stable/amd64/release-notes/index.en.html)) rootfs
- My other project to interface with sensors etc [dcafs](https://github.com/vlizBE/dcafs)
#### Hardware
- Four layer PCB with SoM on one side and nearly everything else on the other.
- [STM32MP151 SOM by Myir](https://www.myirtech.com/list.asp?id=658)
  - 8 uarts (one used for debug port)
  - One ethernet port up to Gbit speeds
  - 4GB eMMC, 512MB ram
  - Two available SDMMC's (third one has the eMMC)
  - Pretty much all pins brought out
- **Power:**
  - [36V to 5V(0.6A max) regulator](https://www.monolithicpower.com/en/mpm3506a.html)
- **On-board Connectivity**
  - Microsd slot
  - USB Client for usb-gadget and updating
  - Connector for RJ45 adapter, LAN8720A
  - Dipswitch to choose between DFU and eMMC boot
  - Power & Reset switch
- **Other logic**
  - RTC with connected wake up pin and battery
