## Guides
There are five guides:
* customize_dts
  * starts from the CubeMX project
  * alters the generated DTS to match the hardware
* build_yocto
   * starts from the DTS files
   * builds all boot stages, kernel and rootfs
* build_boot
   * build the boot stages and allows for customization (fe. increased debug level)
* build_rootfs
   * explains how to create a debian 12 rootfs
   
> troubleshooting.md contains most of the issues I've encountered while working on the guides/scripts