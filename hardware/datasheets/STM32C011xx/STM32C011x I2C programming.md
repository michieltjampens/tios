# How to program the STM32C011x via I2C

## Hardware requirements
- Be able to toggle nBoot0/SWCLK pin
- PB6 & PB7 connected to a I2C master

## Activation pattern

### Main flash not empty
* nBoot0(bit) = 0, nBoot1(bit) = 1, nBoot0_SEL(bit) = 1 and BOOT_LOCK(bit) = 0
* Boot0(pin) = 1, nBoot1(bit) = 1, BOOT_LOCK(bit) = 0 and nBOOT0_SEL(bit) = 0

### Main flash empty
* nBoot0(bit) = 1, nBoot0_SEL(bit) = 1, BOOT_LOCK(bit) = 0
* nBoot0(pin) = 0, nBoot0_SEL(bit) = 0, BOOT_LOCK(bit) = 0

### Set up option bits

The defaults are:
- nBoot0(bit) = 1
- nBoot1(bit) = 1
- nBoot_SEL (bit) = 1
- BOOT_LOCK ???

Given that we want to be able to program it through toggle of Boot0(pin).
We'll first have to connect a programmer to change nBOOT0_SEL to 0.

1. Connect to it via STM32CubeProgrammer with STLink
2. Go to the OB tab
3. Scroll down to the nBoot options
4. Unselect nBoot_SEL
5. Leave nBoot0, nBoot1 selected
6. Click apply

## Test

* Connect the stm according to the Hardware requirements.
* Use `sudo i2cdetect -y bus` where bus is the dev/i2c used.
* The address 64 should show up.