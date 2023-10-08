# Build using Yocto
> Source: `https://wiki.st.com/stm32mpu/wiki/How_to_create_your_own_machine`

## 0. Prereq
* DTS files have been generated with CubeMX and altered or downloaded from this git at `tios/core/code/dts`.
* Required tools: `sudo apt install gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev python3-subunit mesa-common-dev zstd liblz4-tool file locales repo`
  * If repo isn't found, add `contrib` to sources.
* Locale `en_US.UTF-8 UTF-8` exists, if not `sudo locale-gen en_US.UTF-8`

## 1 . Get the destribution pacakges
> Source :`https://wiki.st.com/stm32mpu/wiki/STM32MP1_Distribution_Package`
* Create working dir, and move into it
* Get the repo : `repo init -u https://github.com/STMicroelectronics/oe-manifest.git -b refs/tags/openstlinux-6.1-yocto-mickledore-mp1-v23.06.21`
* Init the repo: `repo sync` (this takes a while)

## 2. Create custom machine
* Copy the content of `tios/Core/code/dts` to `layers/meta-st/meta-st-stm32mp-addons`
* Go into `layers/meta-st/meta-st-stm32mp-addons/conf/machine/` inside the working dir

## A. Use this repo
* Copy `tios/Core/yocto/stm32mp1-tios.conf` into `layers/meta-st/meta-st-stm32mp-addons/conf/machine/`

## B. Start from scratch
* Copy the file `stm32mp15-mx.conf` as `stm32mp1-tios.conf`
* Open the resulting file in a text editor
  * Uncomment line 90 to enable emmc boot
  * Alter line 114 to `CUBEMX_DTB = "stm32mp151a-tios-mx"` (base name of dts files)
  * Alter line 115 to `CUBEMX_PROJECT = "tios"` (name of folder in first step)
  * Alter line 118 to `CUBEMX_SOC_PACKAGE = "A"` (means no encrypt)
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
 
## 4. Stuff to do
* Fix `/etc/fstab` because for some reason this is filled in according to the PC that build it