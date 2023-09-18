# A. Step by Step
## 0. Prereq
* Get the toolchains from `https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads`
  * arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz (or newer)
  * arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-linux-gnueabihf.tar.xz
* Make sure clang (14.0.0) is installed  `sudo apt-get install clang`
 
### Initialize cross-compilation via SDK

`source ~/tios/SDK/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi`
Check with `set | grep CROSS`, this should return
CROSS_COMPILE=arm-ostl-linux-gnueabi-

## 1. TF-A
> Source: `https://wiki.st.com/stm32mpu/wiki/How_to_configure_TF-A_BL2` and older version `https://wiki.st.com/stm32mpu/index.php?title=How_to_configure_TF-A_BL2&oldid=78136`

(NOTE: If this is the first time, start with this. Because it generates files used by OPTEE)
### Build
* Get sources `git clone https://github.com/STMicroelectronics/arm-trusted-firmware.git`
* Copy the dts's generated with CubeMX into the fdts folder
* Reset the flags `unset LDFLAGS` & `unset CFLAGS`
* Make usb boot with `make ARM_ARCH_MAJOR=7 ARCH=aarch32 PLAT=stm32mp1 STM32MP_USB_PROGRAMMER=1 STM32MP15=1 DTB_FILE_NAME=stm32mp151a-tios-mx.dtb`
  * Copy the result to deploy: `cp build/stm32mp1/release/tf-a-stm32mp151a-tios-mx.stm32 ../deploy/f-a-stm32mp151a-tios-usb.stm32`
* Make with `make ARM_ARCH_MAJOR=7 ARCH=aarch32 PLAT=stm32mp1 STM32MP_EMMC=1 STM32MP15=1 DTB_FILE_NAME=stm32mp151a-tios-mx.dtb`
  * Copy the result to deploy: `cp build/stm32mp1/release/tf-a-stm32mp151a-tios-mx.stm32 ../deploy/f-a-stm32mp151a-tios-emmc.stm32`

## 2. Optee
> Source: `https://wiki.st.com/stm32mpu/wiki/How_to_configure_OP-TEE`

### Build
* Get the sources `git clone https://github.com/STMicroelectronics/optee_os.git`
* Copy the dts generated with CubeMX for OPTEE, and add the *-fw-config.dts generated for tf-a to `optee_os/arch/arm/dts`
* Go into the optee_os folder
* Reset the flags `unset LDFLAGS` & `unset CFLAGS`
* Build `make PLATFORM=stm32mp1 CFG_EMBED_DTB_SOURCE_FILE=stm32mp151a-tios-mx.dts CFG_TEE_CORE_LOG_LEVEL=2 O=build all`
    * Or to get more error output `make PLATFORM=stm32mp1 CFG_EMBED_DTB_SOURCE_FILE=stm32mp151a-tios-mx.dts CFG_TEE_CORE_LOG_LEVEL=4 DEBUG=1 O=build all`
* Fix the errors
* Result should be the following files
> <optee-os>/build/core/tee-header_v2.bin
> <optee-os>/build/core/tee-pageable_v2.bin
> <optee-os>/build/core/tee-pager_v2.bin
* Copy those to deploy: `cp build/core/*_v2.bin ../deploy`
  
### Deploy
> Source: `https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer`

## 3. U-Boot
> **Sources** 
> `https://wiki.st.com/stm32mpu/wiki/U-Boot_overview#U-Boot_build`
> `https://wiki.st.com/stm32mpu/wiki/STM32MP15_U-Boot`
> `https://wiki.st.com/stm32mpu/wiki/How_to_configure_U-Boot_for_your_board`

* Get the sources `git clone https://github.com/STMicroelectronics/u-boot`
* Copy the dts generated with CubeMX for U-boot to `arch/arm/dts`
* Make sure these are mentioned in `arch/arm/dts/Makefile` in the section `dtb-$(CONFIG_STM32MP15X)`
* Apply the default config `make stm32mp15_defconfig`
* Build it `make DEVICE_TREE=stm32mp151a-tios-mx all`
* This will generate files in the u-boot root folder, the ones we need are `u-boot-nodtb.bin` and `u-boot.dtb`

## 4. Build FIP
> Source: `https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/plat/stm32mp1.rst`
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

# B. Using Yocto
> Source: `https://wiki.st.com/stm32mpu/wiki/How_to_create_your_own_machine`

## 0. Prereq
* DTS files have been generated with CubeMX
  * Altered and ddr calibration added
* Required tools: `sudo apt install gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev python3-subunit mesa-common-dev zstd liblz4-tool file locales repo`
  * If repo isn't found, add `contrib` to sources.
* Locale `en_US.UTF-8 UTF-8` exists, if not `sudo locale-gen en_US.UTF-8`

## 1 . Get the destribution pacakges
> Source :`https://wiki.st.com/stm32mpu/wiki/STM32MP1_Distribution_Package`
* Create working dir, and move into it
* Get the repo : `repo init -u https://github.com/STMicroelectronics/oe-manifest.git -b refs/tags/openstlinux-6.1-yocto-mickledore-mp1-v23.06.21`
* Init the repo: `repo sync` (this takes a while)

## 2. Create custom machine
* Paste the folder that contains the dts subfolders inside `layers/meta-st/meta-st-stm32mp-addons`
* Go into `layers/meta-st/meta-st-stm32mp-addons/conf/machine/` inside the working dir
* Copy the file `stm32mp15-mx.conf` as `stm32mp1-tios.conf`
* Open the resulting file in a text editor
  * Uncomment line 90 to enable emmc boot
  * Alter line 114 to `CUBEMX_DTB = "stm32mp151a-tios-mx"` (base name of dts files)
  * Alter line 115 to `CUBEMX_PROJECT = "tios"` (name of folder in first step)
  * Alter line 118 to `CUBEMX_SOC_PACKAGE = "A"` (mean no encrypt)
  * Alter line 121 to `CUBEMX_BOARD_DDR_SIZE = "512"` (ram size of board)
  * Uncomment line 124 
  * Save and quit
* Accept EULA to support third party content using a symbolic link
  * Go back up `cd..`
  * Enter eula `cd eula`
  * Create link: `ln -s ST_EULA_SLA stm32mp1-tios`
 
## 3. Compile the image with yocto
* Go back up to the working dir folder
* Run `MACHINE=stm32mp1-tios DISTRO=openstlinux-weston source layers/meta-st/scripts/envsetup.sh`
  * Ignore the warning (if any)
  * Accept the eula
* Run `bitbake st-image-core` 
* Wait...
  * Virtual kernel error? Check if you altered the conf correctly and copied the dts folder to the right place
  * Missing a tool? Did you do the prereq?
  * In case build fail for an error in the machine.conf, pay attention to do a -c cleanall prior to relaunch the build after correction 
* Check inside `build-openstlinuxweston-stm32mp1-tios/tmp-glibc/deploy/images/stm32mp1-tios/`
  * 
