# Content
## Folders
* **code** Anything related to the core that can be considered code/scripts etc
* **datasheets** contains the relevant datasheets
## Files
All the files are Diptrace schematics or layout.

### Core_v1.*
- No longer worked on, but also not 'production ready'. Might pick this back up at some time.
- Based on Myir SOM for the IMX6ULL (512MB ram/4GB eMMC)
- Board contains: RTC (+bat), µSDC slot, Temp/Hum sensor, pressure sensor, ESP32, DC-DC

### Core_v2_poc.*
- No longer worked on, minimal first board to check 'proof of concept'.
- Made for using the STM32MP13 Myir SOM
- Board contains: DC-DC, µSDCard slot, µUSB and all minimum required connections (boot,rst,recovery...)
- Brings out nearly all gpio's

### Core_v2_tiny
- Work in progress
- Based on the poc, this will be a more feature complete version with the restriction of max 50mmx50mm. This restriction
is mainly because then the prices of PCB go up.
- Board contains: 24V DC-DC with filtering etc, RTC (+bat), µSDC slot, µUSB client, ethernet, 
- Limited amount of GPIO made available on 2mm pitch pinheaders, about 55 of 90 (but will increase)

### Planned
- Core_v2_med, still keep it small but don't care as much about the 50x50 
- Core_v2_sbc, add the RJ45 and switch to 2.56mm pitch pinheaders