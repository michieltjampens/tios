# Hardware Changes

## Rev 3
Summary:
 * Reduced unique components by 3

### Maybe...
 * Replace HI0805P390R-10 with BLM18PG330SN1D, result:
   * Bit higher DCR 25mR vs 8mR
   * Smaller package 0603 vs 0805
   * Smaller current rating 3A VS 4a
   * Less 100MHz R 33R vs 39R
   * One less unique component
   
### Component replacements
 * R3 from 47K5 to 44K2, won't make a different circuit wise. But decreases unique components with one.
 * C7,C8 to a 10V version
 * C18 was 47µF 6V3, replaced with dual 22µF 10V 
 * C17 changed from 0603/10V to 0805/16V because it was the only 10uF/0603
 * Replaced F1 with a 0603 fuse instead to save on bom (1,4pp -> 0,2pp)
 * Replaced R8 with a part of RA14, we'll see if that is enough (R for filter)
 * Removed R7 & J4 because not needed (wasn't sure)
 * Removed TP1, was only for debug if USB->UART design was flawed
 * Replaced the common mode + esd parts for usb with single component

### Silkscreen/Pattern edits
 * X3,X5,X6 changed holesize to 0.8
 * BT1 Removed solder paste from the pattern, not sure why it was there
 * U1 make the pin 1 marking better (and put it on the correct corner)
 * J6 pattern not wide enoug 5,3mm -> 6,8mm

### Global changes
 * Increased the silkscreen around 0603 caps from 0.05mm to 0.08mm, because to small
to get a proper line printed.