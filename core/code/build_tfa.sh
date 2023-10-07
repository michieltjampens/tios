##!/bin/bash
# ------------------
# PREP
# ------------------
# Some things to add some color to the output
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
# Create build directory if it doesn't exists
if [ ! -d "build" ]
then
	mkdir build
	echo -e "${ORANGE}Created build directory${NC}"
fi
# Go into it
cd build
# Get the compiler if not there yet
if [ ! -d "gcc-11.3.0-nolibc" ]
then
	echo -e "${ORANGE}No gcc compiler yet, downloading and unpacking v11.3.0${NC}"
	wget -c https://mirrors.edge.kernel.org/pub/tools/crosstool/files/bin/x86_64/11.3.0/x86_64-gcc-11.3.0-nolibc-arm-linux-gnueabi.tar.xz
	# Unpack it
	tar -xf x86_64-gcc-11.3.0-nolibc-arm-linux-gnueabi.tar.xz
else
	echo -e "${GREEN}Compiler found!${NC}"
fi
# Create the var (pwd refers to the current folder)
export CC=`pwd`/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
# Create a deploy folder, if it doensn't exist
if [ ! -d "deploy" ]
then
	echo -e "${ORANGE}No deploy folder yet, creating it${NC}"
	mkdir deploy
	cd deploy
	mkdir arm-trusted-firmware
	cd ..
fi

# --------
# TF-A
# --------
# Get the sources
if [ ! -d "arm-trusted-firmware" ]
then
	echo -e "${ORANGE}Retrieving the arm-trusted-firmware git repo branch 2.9.0${NC}"
	git clone -b v2.9.0 https://github.com/ARM-software/arm-trusted-firmware --depth=1
	# Copy the dt's
	echo "Added the tios dt's to the repo"
	cp ../dts/tf-a/fdts/* arm-trusted-firmware/fdts/
	echo "Adding the old stm32mp15_clksrc.h to the bindings"
	# Fix the changed stm32mp15_clksrc.h by first backing up the old one
	echo "Backing up stm32mp15_clksrc.h just to be sure"
	cp arm-trusted-firmware/include/dt-bindings/clock/stm32mp15-clksrc.h arm-trusted-firmware/include/dt-bindings/clock/stm32mp15-clksrc.h.BAK
	echo "Overwriting the existing one"
	cp ../dts/tf-a/include/stm32mp15-clksrc.h arm-trusted-firmware/include/dt-bindings/clock/
	cd arm-trusted-firmware
else
	echo -e "${GREEN}Arm-trusted repo present, doing a pull"
	cd arm-trusted-firmware 
	git pull
fi
# Perform cleanup
echo -e "${GREEN}Perform cleanup${NC}"
make CROSS_COMPILE=${CC} realclean
# Perform build for usb boot
echo -e "${GREEN}Build the usb booting one${NC}"
make CROSS_COMPILE=${CC} ARM_ARCH_MAJOR=7 ARCH=aarch32 PLAT=stm32mp1 STM32MP_USB_PROGRAMMER=1 STM32MP15=1 DTB_FILE_NAME=stm32mp151a-tios-mx.dtb
# Copy the result to deploy folder
echo -e "${GREEN}Copying result to deploy${NC}"
cp build/stm32mp1/release/tf-a-stm32mp151a-tios-mx.stm32 ../deploy/arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-usb.stm32
# cleanup
echo -e "${GREEN}Perform cleanup${NC}"
make CROSS_COMPILE=${CC} realclean
# Perform build for mmc boot
echo -e "${GREEN}Build the emmc booting one${NC}"
make CROSS_COMPILE=${CC} ARM_ARCH_MAJOR=7 ARCH=aarch32 PLAT=stm32mp1 STM32MP_EMMC=1 STM32MP15=1 DTB_FILE_NAME=stm32mp151a-tios-mx.dtb
# Copy the result to deploy folder
echo -e "${GREEN}Copying result to deploy${NC}"
cp build/stm32mp1/release/tf-a-stm32mp151a-tios-mx.stm32 ../deploy/arm-trusted-firmware/tf-a-stm32mp151a-tios-mx-emmc.stm32
# Create metadata.bin??
echo -e "${RED}Still need to create metadata, TODO${NC}"
echo -e "${GREEN}TF-A finished!${NC}"
