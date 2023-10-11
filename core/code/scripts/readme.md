# Content

This folder contains bash scripts that automate the whole build process.  
Do note that you'll probably have to chmod +x them to use.

* build_all.sh
  * run build_tfa.sh
  * run build_optee.sh
  * run build_uboot.sh
  * run build_fip.sh
* build_tfa.sh
  * Create a 'build' folder inside 'code'
  * Get the compiler
  * Get the repo or pull
  * Copy the dts and replace stm32mp15_clksrc.h
  * Build usb, emmc boot (emmc both optee and sp_min)
  * Copy the results to deploy
* build_optee.sh
  * Create a 'build' folder inside 'code'
  * Get the compiler
  * Get the repo or pull
  * Copy the dts and replace conf.mk
  * Build regular and debug (level=4) version
  * Copy the results to deploy
* build_uboot.sh
  * Create a 'build' folder inside 'code'
  * Get the compiler
  * Get the repo or pull
  * Copy the dts and alter makefile in the dts folder
  * Build regular and debug (level=4) version
  * Copy the results to deploy
* build_debian.sh
  * Create a 'build' folder inside 'code'
  * Get the compiler
  * Get the debian bookworm sources
  * Alter sources.list, fstab and hostname
  * Mount as chroot
