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
### Component changes
 * R3 from 47K5 to 44K2, won't make a different circuit wise. But decreases unique components with one.
 * C7,C8 to a 10V version
 * C18 was 47µF 6V3, replaced with dual 22µF 10V 
 * C17 changed from 0603/10V to 0805/16V because it was the only 10uF/0603
 * Replaced F1 with a 0603 fuse instead to save on bom (1,4pp -> 0,2pp)
 * J6 pattern not wide enoug 5,3mm -> 6,8mm
 * X3,X5,X6 changed holesize to 0.8