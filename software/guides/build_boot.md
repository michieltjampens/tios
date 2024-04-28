# Manually build the bootstages step by step
> Source: `https://forum.digikey.com/t/debian-getting-started-with-the-stm32mp157/12459`

This document explains how to build TF-A,OPTEE,U-Boot and package those inside a FIP.
This guide assumes the DTS files are already made.

The flashlayout for eMMC looks like this
```c
#Opt  Id	Name	    Type	    IP	    Offset	    Binary
-	  0x01	fsbl-boot	Binary		none	0x0	        arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-usb.stm32
-	  0x03	fip-boot	FIP			none	0x0	        fip/fip-stm32mp151a-tios-mx-optee.bin
P	  0x04	fsbl1		Binary		mmc1	boot1	    arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-emmc.stm32
P	  0x05	fsbl2		Binary		mmc1	boot2	    arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-emmc.stm32
P	  0x06	metadata1	FWU_MDATA	mmc1	0x00080000	arm-trusted-firmware/metadata.bin
P	  0x07	metadata2	FWU_MDATA	mmc1	0x00100000	arm-trusted-firmware/metadata.bin
P	  0x08	fip-a		FIP			mmc1	0x00180000	fip/fip-stm32mp151a-tios-mx-optee.bin
PED	  0x09	fip-b		FIP			mmc1	0x00580000	none
PED	  0x0A	u-boot-env	ENV			mmc1	0x00980000	none
P	  0x10	bootfs		System		mmc1	0x00A00000	st-image-bootfs-openstlinux-weston-stm32mp1-tios.ext4
P	  0x11	vendorfs	FileSystem	mmc1	0x04A00000	st-image-vendorfs-openstlinux-weston-stm32mp1-tios.ext4
P	  0x12	rootfs		FileSystem	mmc1	0x05A00000	st-image-core-openstlinux-weston-stm32mp1-tios.ext4
```
So the goal of this guide is to Id 1 till 8.

## 0. Prereq
* Create a working folder called `build` inside code (folder containing this file) and move into it
* Get the gcc compiler for tf-a
    * wget -c https://mirrors.edge.kernel.org/pub/tools/crosstool/files/bin/x86_64/11.3.0/x86_64-gcc-11.3.0-nolibc-arm-linux-gnueabi.tar.xz
    * Unpack `tar -xf x86_64-gcc-11.3.0-nolibc-arm-linux-gnueabi.tar.xz`
    * Export the var`export CC=`pwd`/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-`
* Get the toolchains from `https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads`
  * arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz (or newer)
  * arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-linux-gnueabihf.tar.xz
* Make sure clang (14.0.0) is installed  `sudo apt-get install clang` (needed for FIP)
* Install the ST SDK by following this guide `https://wiki.st.com/stm32mpu/wiki/STM32MP1_Developer_Package#Installing_the_SDK`

## 1. TF-A
> Source: `https://wiki.st.com/stm32mpu/wiki/How_to_configure_TF-A_BL2` and older version `https://wiki.st.com/stm32mpu/index.php?title=How_to_configure_TF-A_BL2&oldid=78136`
> Note: This step generates files that are needed for OPTEE.

### Build
* Get sources `git clone https://github.com/STMicroelectronics/arm-trusted-firmware.git`
* Copy the tios dt's `cp ../dts/tf-a/* arm-trusted-firmware/fdts/`
* If using main overwrite `stm32mp15_clksrc.h`
    * First make backup `cp arm-trusted-firmware/include/dt-bindings/clock/stm32mp15-clksrc.h arm-trusted-firmware/include/dt-bindings/clock/stm32mp15-clksrc.h.BAK`
    * Then overwrite it `cp ../dts/tf-a/include/stm32mp15-clksrc.h arm-trusted-firmware/include/dt-bindings/clock/`
* Step into `arm-trusted-firmware`
* Make sure it's clean `make CROSS_COMPILE=${CC} realclean`
* Make with `make CROSS_COMPILE=${CC} ARM_ARCH_MAJOR=7 ARCH=aarch32 PLAT=stm32mp1 STM32MP_USB_PROGRAMMER=1 STM32MP15=1 DTB_FILE_NAME=stm32mp151a-tios-mx.dtb`
  * Copy the result to deploy: `cp build/stm32mp1/release/tf-a-stm32mp151a-tios-mx.stm32 ../deploy/arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-usb.stm32`
* Make sure it's clean `make CROSS_COMPILE=${CC} realclean` for the emmc build
* Make with `make CROSS_COMPILE=${CC} ARM_ARCH_MAJOR=7 ARCH=aarch32 PLAT=stm32mp1 STM32MP_EMMC=1 STM32MP15=1 DTB_FILE_NAME=stm32mp151a-tios-mx.dtb`
  * Copy the result to deploy: `cp build/stm32mp1/release/tf-a-stm32mp151a-tios-mx.stm32 ../deploy/arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-emmc.stm32`
* Next up, metadata.bin will also be made (THIS ONLY WORKS IN THE ST VERSION OF THE REPO)
  * First create the var that references the source .json `export TF_A_METADATA_JSON=plat/st/stm32mp1/default_metadata.json`
  * Then use it to build: `tools/fwu_gen_metadata/fwumd_tool.py jsonparse $TF_A_METADATA_JSON -b ../deploy/arm-trusted-firmware/metadata.bin`

## 2. Optee
> Source: `https://wiki.st.com/stm32mpu/wiki/How_to_configure_OP-TEE`

### Build
* Get the sources `git clone https://github.com/STMicroelectronics/optee_os.git --depth=1`
* Copy the dts generated with CubeMX for OPTEE, and add the *-fw-config.dts generated for tf-a to `optee_os/core/arch/arm/dts`
* Go into the optee_os folder
* There's an issue with the conf.mk file for custom boards with 512MB ram, so we'll need to change it
    * Edit `optee_os/core/arch/arm/plat-stm32mp1/conf.mk` to make flavorlist-no_cryp-512M look like this
     ```
        flavorlist-no_cryp-512M = $(flavor_dts_file-157A_DK1) \
		                    	  $(flavor_dts_file-157D_DK1) \
			                     stm32mp151a-tios-mx.dts
     ```
* Build `make CROSS_COMPILE=${CC} PLATFORM=stm32mp1 CFG_EMBED_DTB_SOURCE_FILE=stm32mp151a-tios-mx.dts CFG_TEE_CORE_LOG_LEVEL=2 O=build all`
    * Or to get more error output `make CROSS_COMPILE=${CC} PLATFORM=stm32mp1 CFG_EMBED_DTB_SOURCE_FILE=stm32mp151a-tios-mx.dts CFG_TEE_CORE_LOG_LEVEL=4 DEBUG=1 O=build all`
* Fix the errors
* Result should be the following files
> build/core/tee-header_v2.bin
> build/core/tee-pageable_v2.bin
> build/core/tee-pager_v2.bin
* Copy those to deploy: `cp build/core/*_v2.bin ../deploy`
  
### Deploy
> Source: `https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer`

## 3. U-Boot
> **Sources** 
> `https://wiki.st.com/stm32mpu/wiki/U-Boot_overview#U-Boot_build`
> `https://wiki.st.com/stm32mpu/wiki/STM32MP15_U-Boot`
> `https://wiki.st.com/stm32mpu/wiki/How_to_configure_U-Boot_for_your_board`
> Info: U-Boot is the BL33 boot stage, will be added to the fip.

* Get the sources `git clone https://github.com/STMicroelectronics/u-boot --depth=1`
* Copy the dts generated with CubeMX for U-boot to `arch/arm/dts`
* Add the dts to the section `dtb-$(CONFIG_STM32MP15X)` inside `arch/arm/dts/Makefile`
* Apply the default config `make ARCH=arm CROSS_COMPILE=${CC} stm32mp15_defconfig`
  * Extra Info, there are two other stm32mp15 defconfig files in there, those are legacy
	
* Build it `make ARCH=arm CROSS_COMPILE=${CC} DEVICE_TREE=stm32mp151a-tios-mx -j$(nproc) all`
  * For extra debug `make DEVICE_TREE=stm32mp151a-tios-mx all LOG_LEVEL=LOG_LEVEL_INFO DEBUG=1`
* This will generate files in the u-boot root folder, the ones we need are `u-boot-nodtb.bin` and `u-boot.dtb`

> Note: To get extra debug Info
> * Add #define LOG_DEBUG to the to of these files
>   * <uboot folder>/drivers/clk/stm32/clk-stm32mp1.c
>   * <uboot folder>/board/st/stm32mp1/stm32mp1.c
> * Add CONFIG_LOG_MAX_LEVEL=7 to stm32mp1_defconfig

## 4. Build FIP
> Source: `https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/plat/stm32mp1.rst`
> Info: FIP contains OPTEE,U-Boot and the U-Boot DTB

* Open a new terminal to make sure the earlier used SDK environment isn't active anymore (because it results in a lot of errors)
* Prepare cross compiling `export CROSS_COMPILE=~/tios/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-`
```
make PLAT=stm32mp1 ARCH=aarch32 ARM_ARCH_MAJOR=7 \
    AARCH32_SP=optee \
    DTB_FILE_NAME=stm32mp151a-tios-mx.dtb \
    BL33=../deploy/u-boot-nodtb.bin \
    BL33_CFG=../deploy/u-boot.dtb \
    BL32=../deploy/tee-header_v2.bin \
    BL32_EXTRA1=../deploy/tee-pager_v2.bin \
    BL32_EXTRA2=../deploy/tee-pageable_v2.bin \
    STM32MP_EMMC=1 \
    STM32MP15=1 \
    fip
```
Should result in something like this:
> Secure Payload BL32 (Trusted OS): offset=0x100, size=0x1C, cmdline="--tos-fw"
> Secure Payload BL32 Extra1 (Trusted OS Extra1): offset=0x11C, size=0x3BB68, cmdline="--tos-fw-extra1"
> Non-Trusted Firmware BL33: offset=0x3BC84, size=0xF5D3C, cmdline="--nt-fw"
> FW_CONFIG: offset=0x1319C0, size=0x1EA, cmdline="--fw-config"
> HW_CONFIG: offset=0x131BAA, size=0x11518, cmdline="--hw-config"
>
> Built <workdir>/arm-trusted-firmware/build/stm32mp1/release/fip.bin successfully
* Copy the result to the deploy folder: `cp build/stm32mp1/release/fip.bin ../deploy`
