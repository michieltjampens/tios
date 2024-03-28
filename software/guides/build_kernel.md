# Build the kernel
> Source: README_HOW_TO.txt found in the kernel folder of the ST Dev package

## 0. Prereq's

* Have git installed and configured
* Required tools
    * libcurses`sudo apt-get install libncurses5-dev libncursesw5-dev libyaml-dev`
    * mkimage: `sudo apt-get install u-boot-tools`
    * yaml (check dts) `sudo apt-get install libyaml-dev`
    
## 1. Get the sources
* ST Github
    * Clone the git `git clone https://github.com/STMicroelectronics/linux.git -b v6.1-stm32mp-r1 --depth 1`
        * -b v6.1-stm32mp-r1 -> only get that branch
        * --depth 1 -> don't get the history (makes for a smaller download/install)
* Mainline github
    * Clone the git `git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git -b linux-6.1.y --depth 1`
            * -b linux-6.1.y -> get that branch
            * --depth 1 -> don't get the history (makes for a smaller download/install)
* Go into the created linux folder
* Copy the generated `stm32mp151a-tios-mx.dts` into `arch/arm/boot/dts`
    * cp -v ../../dts/kernel/*.dts* arch/arm/boot/dts/
* Apply the patch to alter the makefile
    * git apply ../../dts/kernel/makefile.patch
    
## 2. Building

* Prepare the .config `make ARCH=arm multi_v7_defconfig fragment*.config`

Now it's possible to make changes to the config
* To open a gui `make ARCH=arm nconfig`

Finally build it
* Kernel 
    * Build `make ARCH=arm CROSS_COMPILE=${CC} uImage vmlinux dtbs LOADADDR=0xC2000040 -j$(nproc)` 
        * ARCH=arm -> Because compiling for arm proc
        * CROSS_COMPILE=${CC} -> Use the cross compiler
        * uImage vmlinux dtbs -> build uImage, vmlinux and dtbs
        * LOADADDR=0xC2000040 -> a uImage needs this
        * -j$(nproc) -> Use threads according to processor
    * Copy resulting uImage
        * cp arch/arm/boot/uImage ../deploy/bootfs/
* Modules 
    * Build `make ARCH=arm CROSS_COMPILE=${CC} modules -j$(nproc)` 
        * ARCH=arm -> Because compiling for arm proc
        * modules -> build the modules
        * -j$(nproc) -> Use threads according to processor
* Artifacts
    * Build `make ARCH=arm INSTALL_MOD_PATH="../deploy/" modules_install -j$(nproc)`
    * cp arch/arm/boot/dts/st*.dtb ../deploy/bootfs/
* Go into the install_artifact folder
* Remove the link on install_artifact/lib/modules/<kernel version>/
    * rm lib/modules/<kernel version>/source lib/modules/<kernel version>/build
* Optionally, strip kernel modules (to reduce the size of each kernel modules)
    * find . -name "*.ko" | xargs $STRIP --strip-debug --remove-section=.comment --remove-section=.note --preserve-dates

## 3. Copy to board

* Boot into U-boot
* Use `ums mmc 1` to be able to mount the partitions
* Mount rootfs and bootfs
* Copy the kernel & dtb
    * sudo cp -r boot/* /media/$USER/bootfs/
* Copy kernel modules
    * sudo cp -rf lib/modules/* /media/$USER/rootfs/lib/modules/
    * sync
* Unmount the partitions
    * sudo umount /media/$USER/rootfs
    * sudo umount /media/$USER/bootfs
* In the U-Boot console, cancel ums with CTRL+C
* Reset the board with `reset`
* Hope that the kernel starts
* Generate the list of modules dependencies with `depmod -a`
* Then `sync` and `reboot`

