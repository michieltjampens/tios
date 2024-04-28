# Tower of IO stuff aka TIOS

## Why?
I'm building a hexapod and want to make my own controller for it.  
Why would you design this thing?
- Learning experience
- Fun?

## What?

The hardware will be in different layers/floors/segments... That will add certain functionality.

At the moment, I'm designing the main part of the tower. Which will be based around the [Rockchip RK3308B](https://www.rock-chips.com/a/en/products/RK33_Series/2018/0614/907.html).
Before this, I've tried working with a SoM of imx6ull and stm32mp153. Although I managed to get to a working system, I just wasn't happy with the result.
The move to RK3308B has some pros and cons:  
**Pros**
- Low idle power consumption 260mW (based of long term measurement of Raxda Rock S0 running kernel 6.1 debian bookworm, wifi listening)
- Move from armhf to aarch64, mainly because I use java for [my other project](https://github.com/michieltjampens/dcafs) and new jre's aren't build anymore for armhf.
- 'Entry level' SoC, so messing things up doesn't hurt as much as with a SoM.
- Two ethernet MAC's so dual ethernet is possible, but second one shares pins with the SDIO... (wifi)
- Used by Raxda Rock pi S(0), Banana Pi BPI P2 ... so working kernel/rootfs etc should be easy to obtain
  
**Cons**
- Learning yet another environment (iMX->STM32MP1->Rockchip)
- Not starting from a SoM means the design is a lot harder, will be my first time working with BGA, DDR Ram etc.
- Documentation isn't easy to come by. Won't share them in the repo because some had 'confidantial' stamped over them.
  - Took me some time to find the datasheet for the RK3308B and that's for version 1.2 eventhough 1.3 is out. The [RK3308](https://www.rockchip.fr/RK3308%20datasheet%20V1.5.pdf) 
can be found on the official site.
  - Found the [TRM for the RK3308](https://dl.radxa.com/rockpis/docs/hw/datasheets/) but unsure if 1.1 is latest version.
  - Haven't found an official reference design/guides, so based it off the the earlier mentioned Raxda rock s0.  
- 0.65mm pitch BGA

## Status

Started the design, currently on a four layer PCB. 
- All basics are in the schematic (ram,emmc,phy,buck,usd)
- Layout of most is done (ddr was fun)

## Decisions

With starting from scratch come a lot more choices with regards to components used. 

### Picked Components
- [512MB DDR3L Ram](https://media.kingston.com/pdfs/emmc/MKF_942_iTemp-DRAM_us.pdf)
- [16GB eMMC HS200](https://www.kingston.com/en/embedded/emmc-embedded-flash), 8GB or lower isn't worth the price difference (1€ difference at the moment)
- Discrete power supplies: Buck [TPS62A0x](https://www.ti.com/product/TPS62A02v) and LDO [TLV755](https://www.ti.com/lit/ds/symlink/tlv755p.pdf)
  - Probably try a PMIC for rev 1, but keeping things 'simple' for now.
- 10/100Mbps Ethernet PHY [LAN8720A](https://www.microchip.com/en-us/product/lan8720a)
