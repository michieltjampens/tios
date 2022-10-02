# Hardware Changes

## Rev 3
Summary:
 * Reduced unique components by 6
   
### Component replacements
 * R3 from 47K5 to 44K2, won't make a different circuit wise. But decreases unique components with one.
 * R1 22k1 replaced with 2x44k2 in parallel, was only usage of 22k1
 * C7,C8 to a 10V version
 * C18 was 47µF 6V3, replaced with dual 22µF 10V 
 * C17 changed from 0603/10V to 0805/16V because it was the only 10uF/0603
 * Replaced F1 with a 1206 fuse instead to save on bom (1,4pp -> 0,2pp)
 * Replaced R8 with a part of RA14, we'll see if that is enough (R for filter)
 * SHT1C with SHT40, cheaper, better now available
 * Replaced the common mode + esd parts for usb with single component
 * Replace HI0805P390R-10 with BLM18SG330SN1D
 * BLM18PG330SN1D with BLM18SG330SN1D, price difference less than unique component cost
 
### Removals
 * Removed R7 & J4 because not needed (wasn't sure)
 * Removed TP1, was only for debug if USB->UART design was flawed
 * Removed C5, C6 because create more issues than possibly prevent
 * Removed pressure sensor, to expensive for usage
 
### Routing stuff
 * Increased the traces from/to sd card slot to 0,3 (was 0,25)
 * Moved the FB's for the usb client to topside
 
### Silkscreen/Pattern edits
 * X3,X5,X6 changed holesize to 0.8
 * BT1 Removed solder paste from the pattern center pad, not sure why it was there
 * U1 make the pin 1 marking better (and put it on the correct corner)
 * J6 pattern not wide enoug 5,3mm -> 6,8mm
 * SHTC pattern didn't have mask for the gnd plane underneath
 
### Global changes
 * Increased the silkscreen around 0603 caps from 0.05mm to 0.08mm, because to small
to get a proper line printed.