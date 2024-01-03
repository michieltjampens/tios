# Content
## Folders
* **code** Anything related to the core that can be considered code/scripts etc
* **datasheets** contains the relevant datasheets
* **corev2_rev0** Files for rev0 of core v2, including pdf of the schematic and png of the layout
  
## Files
All the files are [Diptrace](https://diptrace.com/) schematics or layout.

### Core_v1.*
- No longer worked on, but also not 'production ready'. Might pick this back up at some time.
- Based on Myir SOM for the IMX6ULL (512MB ram/4GB eMMC)
- Board contains: RTC (+bat), µSDC slot, Temp/Hum sensor, pressure sensor, ESP32, DC-DC

### Core_v2
- Rev 1 being tested
  - DONE
    - Console port (pinout changed from rev0)
    - Reset button and boot selection
    - SD card (sdmmc1 -> sdmmc3), not for booting yet
	- Heartbeat,fault led
	- Ethernet
  - TODO
    - RTC (i2c6, i2cdetevt really slow))
    - Power button 
    - USB Gadget
	- USB1 host (pcb in prod)
- 55x52mm
- Board contains: 24V DC-DC with filtering etc, RTC (+bat), µSDC slot, µUSB otg, ethernet.
- 1mm pitch JST for I2C6, ethernet, USB1, debug/console
