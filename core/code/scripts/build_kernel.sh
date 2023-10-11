##!/bin/bash
# ------------------
# PREP
# ------------------
# Some things to add some color to the output
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
./build_prep.sh

# --------
# Kernel
# --------
# Get the sources
if [ ! -d "linux" ]
then
	echo -e "${ORANGE}Retrieving the ST linux-os git repo${NC}"
	git clone https://github.com/STMicroelectronics/linux.git -b v6.1-stm32mp-r1 --depth 1
	cd linux
	echo -e "${ORANGE}Updating the makefile to add the dts${NC}"
	git apply ${SCRIPT_DIR}/../dts/kernel/makefile.patch
	if [ ! $? -eq 0 ]; then
    	echo -e "${RED}Failed to apply patch${NC}"
    exit
    echo -e "${GREEN}Apply config${NC}"
	make ARCH=arm multi_v7_defconfig fragment*.config
fi
else
	echo -e "${GREEN}ST Linux repo present, doing a pull${NC}"
	cd linux
	git pull
fi
# Copy the dt's
echo -e "${GREEN}Adding the tios dt's to the repo, so the latest are used${NC}"
cp -v ../../dts/kernel/*.dts* arch/arm/boot/dts/
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Failed to copy dts${NC}"
    exit
fi
# Perform cleanup
#echo -e "${GREEN}Perform cleanup${NC}"
#make CROSS_COMPILE=${CC} clean

# Allow user to make changes
make ARCH=arm nconfig
# Perform build
echo -e "${GREEN}Build uImage and dtb's${NC}"
make CROSS_COMPILE=${CC} ARCH=arm uImage vmlinux dtbs LOADADDR=0xC2000040 -j$(nproc)
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit
fi
# Copy the result to deploy folder
echo -e "${GREEN}Copying result to deploy${NC}"
cp arch/arm/boot/uImage ../deploy/bootfs/
echo -e "${GREEN}Build modules${NC}"
make CROSS_COMPILE=${CC} ARCH=arm modules -j$(nproc)
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Building modules failed${NC}"
    exit
fi
echo -e "${GREEN}Building artifacts${NC}"
make CROSS_COMPILE=${CC} ARCH=arm INSTALL_MOD_PATH="../deploy/bootfs/install_artifact" modules_install -j$(nproc)
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Building artifacts failed${NC}"
    exit
fi
echo -e "${GREEN}Remove the link on install_artifact/lib/modules${NC}"
cd ../deploy/bootfs/install_artifact/lib/modules/6.1.28-dirty
rm source build
echo -e "${GREEN}Finished building kernel${NC}"
