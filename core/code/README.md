# Content
## Folders
* **Yocto** contains the machine/conf
* **dts** contains the final DTS
* **cubemx** contains the CubeMX project files
  
### Guides
There are three guides:
* build_dts.md
  * starts from the CubeMX project
  * alters the generated DTS to match the hardware
* build_yocto.md
   * starts from the DTS files
   * builds all boot stages, kernel and rootfs
* build_boot.md
   * build the boot stages and allows for customization (fe. increased debug level)
