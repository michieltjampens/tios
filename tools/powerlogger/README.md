# Power Logger

Small board to monitor power consumption of attached devices.

## Progress
- Currently testing rev0
  - Programming and lpuart via vcp work (but had to swap rx/tx)
  - I2C and PAC1954 work
  - Script written for dcafs that reads, processes and stores voltage,current and energy 
- Changes for rev 0 -> rev 1
  - Moved LPUART1 TX to PA2 because then you have the option to use USART2 instead.
  - Fixed pinout of STMOD RX/TX swapped?
  - Added EMI filter to I2C to connector
  - Move hearbeat from PB5 to PA3
  - Added resistors on the uart lines, maybe for filter
  - General cleanup of traces/vias and the like

  
## Todo
- Flash via spi -> store and download data
- PAC Alerts
- RTC

## Features
- Up to 4 concurrent devices monitored.
- 8MB local flash storage
- Onboard programmer/debugger/usb to uart with (STLINK V3MODS)[https://www.st.com/en/development-tools/stlink-v3mods.html]
- Maximum 30V, haven't tested the current limit yet.

## Datasheets
- **DC-DC** (MPM3506A)[https://www.monolithicpower.com/en/documentview/productdocument/index/version/2/document_type/Datasheet/lang/en/sku/MPM3506AGQV-Z/document_id/2106/]
- **Flash Mem** (MX25L6433F)[https://www.macronix.com/Lists/Datasheet/Attachments/8911/MX25L6433F,%203V,%2064Mb,%20v1.3.pdf]
- **MCU** (STM32L040K)[https://www.st.com/resource/en/datasheet/stm32l010k4.pdf]
- **Monitor** (PAC1954)[https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/PAC195X-Family-Data-Sheet-DS20006539.pdf]

## dcafs folder

This contains the files needed to read/process the data from the board with (dcafs)[https://github.com/michieltjampens/dcafs].