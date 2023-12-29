# Core v2 rev Changes

## Rev 1

* Added extra via's to the GND pin of the USB chokes
* Moved some refdes labels
* Changed via's on the 5V pin
* Connected X1.39, X4.19, X5.15, X6.16 to gnd
* Decreased clearance for copper pour to 0,2 from 0,33 (Diptrace default). 
* Moved/added via's to minimise inductance
* Added teardrops to the small via's and thin traces
* Trace to the input of the dcdc wasn't altered...
* Extra EMI/ESD filtering
* Moved via's further away from pads to have a minimum distance (if possible), to decrease chance of bad connection due to inaccurate drill

## Rev 2

* Added led to 3V3.
* Added silk to show the + on the input power connector.
* Cleaned up refdes positions.
* Rotated S2 so text isn't upside down anymore and added silk to know how to mount.
* Replaced D5 with same diode as D3/D4
* Connected the ID pin on USB 2 to PA10 through series resistor and a pull down
* Changed C9 to 10nF
* Added the resistors for the rj45 leds
* Changed R22 to 12k1 to match appnote (was 12k)
* Added 10uF bulkcaps to ethernet power inputs