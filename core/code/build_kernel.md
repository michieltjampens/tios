# Build the kernel
> Source: README_HOW_TO.txt found in the kernel folder of the ST Dev package

## 0. Prereq's

* Have git installed and configured
* Required tools
    * libcurses`sudo apt-get install libncurses5-dev libncursesw5-dev libyaml-dev`
    * mkimage: `sudo apt-get install u-boot-tools`
    * yaml (check dts) `sudo apt-get install libyaml-dev`
    
## 1. Get the sources

* Clone the git `git clone https://github.com/STMicroelectronics/linux.git -b v6.1-stm32mp-r1 --depth 1`
    * -b v6.1-stm32mp-r1 -> only get that branch
    * --depth 1 -> do't get the history (makes for a smaller download/install)
* Copy the generated `stm32mp151a-tios-mx.dts` into `arch/arm/boot/dts`
* Add it to the list of generated dtb's 
    * Open the file `arch/arm/boot/dts/Makefile`
    * Look for dtb-$(CONFIG_ARCH_STM32) += \
    * Append a \ to the last entry of that listing
    * Add `stm32mp151a-tios-mx.dts`to the list
    
## 2. Building
For easier build management it's recommended to make a dedicated folder for that 
* First go into the git folder `cd <directory to kernel source code>`
* Create the var that holds the build folder `export OUTPUT_BUILD_DIR=$PWD/../build`
* Create the folder for the var `mkdir -p ${OUTPUT_BUILD_DIR}`
* Prepare the .config inside there `make ARCH=arm O="${OUTPUT_BUILD_DIR}" multi_v7_defconfig fragment*.config`

Now it's possible to make changes to the config
* To open a gui `make ARCH=arm nconfig O="${OUTPUT_BUILD_DIR}"`

Finally build it
* Kernel `make ARCH=arm uImage vmlinux dtbs LOADADDR=0xC2000040 O="${OUTPUT_BUILD_DIR}" -j$(nproc)`
    * ARCH=arm -> Because compiling for arm proc
    * uImage vmlinux dtbs -> build uImage, vmlinux and dtbs
    * LOADADDR=0xC2000040
    * O="${OUTPUT_BUILD_DIR}" -> use earlier defined build folder
    * -j$(nproc) -> Use threads according to processor
* Modules `make ARCH=arm modules O="${OUTPUT_BUILD_DIR}"  -j$(nproc)`
    * ARCH=arm -> Because compiling for arm proc
    * modules -> build the modules
    * LOADADDR=0xC2000040    
    * O="${OUTPUT_BUILD_DIR}" -> use earlier defined build folder
    * -j$(nproc) -> Use threads according to processor
* Artifacts
    * Build them `make ARCH=arm INSTALL_MOD_PATH="${OUTPUT_BUILD_DIR}/install_artifact" modules_install O="${OUTPUT_BUILD_DIR}" -j$(nproc)`
    * mkdir -p ${OUTPUT_BUILD_DIR}/install_artifact/boot/
    * cp ${OUTPUT_BUILD_DIR}/arch/arm/boot/uImage ${OUTPUT_BUILD_DIR}/install_artifact/boot/
    * cp ${OUTPUT_BUILD_DIR}/arch/arm/boot/dts/st*.dtb ${OUTPUT_BUILD_DIR}/install_artifact/boot/
