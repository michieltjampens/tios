# Tower of IO stuff aka TIOS

## Why?
I'm building a hexapod and want to make my own controller for it.
Why would you design this thing?
- Learning experience
- Fun?

Other info on supporting [wiki](https://wiki.iostuff.dev/)

## What?

The hardware will be in different layers/floors/segments...  
Main restriction is limiting the size of the core to 50x50mm, just because that's the size at which 4 layers are still cheap (at a certain pcb fab).  
But no EMI testing to pass, so 2 layers for now...



### Core

The main part of the tower (not neccessarily the base). 

#### Software
- Custom U-boot based on Myir u-boot
- Recent IMX-linux kernel (5.16.x)
- Debian 11.x rootfs
- [dcafs](https://github.com/vlizBE/dcafs)
#### Hardware
- Two layer PCB with a SoM on one side and everything else on the other.
- [iMX6ULL SOM by Myir](http://www.myirtech.com/list.asp?id=644)
  - Lower power consumption
  - 8 uarts
  - Built in Ethernet chip
  - 4GB eMMC
  - Pretty much all pins brought out
- **Power:**
  - [6V to 3V3(3A max) regulator](https://www.monolithicpower.com/en/mpm3833c.html)
  - Peripherials behind user controllable load switch and fused
- **On board Connectivity**
  - microsd slot
  - [ESP32-C3 for Wifi/BLE (isp, some IO available)](https://docs.ai-thinker.com/en/esp32c3)
  - USB Client for usb-gadget and updating
  - [Debug uart via USB](https://www.silabs.com/interface/usb-bridges/usbxpress)
  - 2.00mm pinheaders, all also have atleast one 3V3 and gnd pin
    - X1: both fec, Vbat, + 18x SoM IO
    - X2 : RTC Int + 12x SoM IO 
    - X3 : 13x SoM IO
    - X4 : USB Host pins and 'clean' 5V
    - X5 : 10x SoM IO 
    - X6: 4x ESP32-C3 IO, 4 pins for tower passthrough 
  - Recovery mode switch
- **Sensors**
  - [Battery backed RTC](https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-ic-bus/tiny-real-time-clock-calendar-with-alarm-function-battery-switch-over-time-stamp-input-and-ic-bus:PCF85263A)
    - With INT's connected to ESP32-C3 and one on connector
  - [Hum/temp I2C sensor](https://sensirion.com/products/catalog/SHTC1/) 
  - [pressure/temp I2C sensor](https://invensense.tdk.com/products/smartpressure/icp-20100/)
    - With interrupt connected to SoM IO
### Base IO

Will be the first extension, will probably:
- one or two ethernet connectors
- one or two USB host connectors
- 12V to 5V DCDC
