# Tower of IO stuff aka TIOS

## Why?
I'm building a hexapod and want to make my own controller for it.  
Why would you design this thing?
- Learning experience
- Fun?

## What?

The hardware will be in different layers/floors/segments...  

## Core 

The main part of the tower. I'm at the third iteration/attempt of this but still not happy...
Latest/Last one will probably be based around the RK3308B, which means:
- Learning yet another environment (iMX->STM32MP1->rockchip)
- Much lower idle power consumption 250mW instead of 500mW
- Move from armhf to aarch64 because newer java jre aren't released for armhf
- Unsure about the SoM versus custom design. Biggest difference being the 'hard to route' things like DDR ram.

### Stuff decided so far
- [512MB DDR3L Ram](https://media.kingston.com/pdfs/emmc/MKF_942_iTemp-DRAM_us.pdf)
- [16GB eMMC HS200](https://www.kingston.com/en/embedded/emmc-embedded-flash), 8GB or lower isn't worth the price difference
- Discrete power supplies: Buck [TPS62A0x](https://www.ti.com/product/TPS62A02v) and LDO [TLV755](https://www.ti.com/lit/ds/symlink/tlv755p.pdf)
  - Probably try a PMIC for rev 1, but keeping things 'simple' for now.
 
## Previous attempts

### First attempt
This was with a IMX6ULL SoM made by Myir, but that had odd power rail issues and at some point I gave up.
Design files are still on here, but that's about it.

### Second attempt

With a STM32MP151 SoM, again by Myir (because those are easy to obtain). The Hardware works, but
getting to a working linux was a pain. The support from ST (nor Myir) is plainly bad for custom designs (by hobbyists).
The board in the end uses way to much power and felt way to slow (compared to others) and it uses an already old
ARM Cortex-A7, so not much future proofing there.

#### Software
- Standard STM32MP1 bootstage of TF-A, OPTEE and U-Boot
- Kernel (6.1.28) based on ST ECO 5.0.0
- [Debian 12.x]([https://www.debian.org/](https://www.debian.org/releases/stable/amd64/release-notes/index.en.html)) rootfs
- My other project to interface with sensors etc [dcafs](https://github.com/vlizBE/dcafs)
#### Hardware
- Four layer PCB with SoM on one side and nearly everything else on the other.
- [STM32MP151 SOM by Myir](https://www.myirtech.com/list.asp?id=658)
  - 8 uarts (one used for debug port)
  - One ethernet port (using 10/100 instead of Gbit)
  - 4GB eMMC, 512MB ram
  - Two available SDIO's (third one has the eMMC)
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
