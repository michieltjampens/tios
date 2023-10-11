##!/bin/bash
# ------------------
# PREP
# ------------------
# Some things to add some color to the output
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
# Go up to the code folder
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
./build_prep.sh
# --------
# U-BOOT
# --------
# Get the sources
if [ ! -d "u-boot" ]
then
	echo -e "${ORANGE}Retrieving the U-Boot git repo${NC}"
	#git clone -b v2023.10 https://github.com/u-boot/u-boot --depth=1
	#Stuck on the missing label hsem... 
	git clone https://github.com/STMicroelectronics/u-boot --depth=1
	echo -e "${ORANGE}Applying the patch${NC}"	
	cd u-boot
	git apply ${SCRIPT_DIR}/../dts/u-boot/makefile.patch
else
	echo -e "${GREEN}ST OPTEE repo present, doing a pull${NC}"
	cd u-boot
	git pull
fi
# Copy the dt's
echo -e "${GREEN}Adding the tios dt's to the repo, so the latest are used${NC}"
cp ../../dts/u-boot/* arch/arm/dts/

# Perform cleanup
echo -e "${GREEN}Perform cleanup${NC}"
make ARCH=arm CROSS_COMPILE=${CC} distclean
# Perform build for default version
echo -e "${GREEN}Prepare the config${NC}"
make ARCH=arm CROSS_COMPILE=${CC} stm32mp15_defconfig
echo -e "${GREEN}Build${NC}"
make ARCH=arm CROSS_COMPILE=${CC} DEVICE_TREE=stm32mp151a-tios-mx -j$(nproc) all
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit
fi
echo -e "${GREEN}Copying result to deploy${NC}"
cp u-boot-dtb.bin ../deploy/
cp u-boot-nodtb.bin ../deploy/
cp u-boot.dtb ../deploy/
echo -e "${GREEN}Perform cleanup${NC}"
make ARCH=arm CROSS_COMPILE=${CC} distclean
echo -e "${GREEN}Prepare the config${NC}"
make ARCH=arm CROSS_COMPILE=${CC} stm32mp15_defconfig
echo -e "${GREEN}Build with debug enabled${NC}"
make ARCH=arm CROSS_COMPILE=${CC} DEVICE_TREE=stm32mp151a-tios-mx -j$(nproc) LOG_LEVEL=LOG_LEVEL_INFO DEBUG=1 all
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit
fi
echo -e "${GREEN}Copying result to deploy/debug${NC}"
cp u-boot-dtb.bin ../deploy/debug/
cp u-boot-nodtb.bin ../deploy/debug/
cp u-boot.dtb ../deploy/debug/
echo -e "${GREEN}Finished with u-boot${NC}"
