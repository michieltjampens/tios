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
echo "Script directory: $SCRIPT_DIR"
# Go up to the code folder
cd ${SCRIPT_DIR}/..
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
	# Remove the tar
	rm x86_64-gcc-11.3.0-nolibc-arm-linux-gnueabi.tar.xz
else
	echo -e "${GREEN}Compiler found!${NC}"
fi
# Create the var (pwd refers to the current folder)
export CC=`pwd`/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
# Create the file to source it later
echo CC=${CC} > compiler.sh
# Create a deploy folder, if it doesn't exist
if [ ! -d "deploy" ]
then
	echo -e "${ORANGE}No deploy folder yet, creating it${NC}"
	mkdir deploy
	cd deploy
	mkdir arm-trusted-firmware
	mkdir debug
	mkdir bootfs
	cd ..
fi
# --------
# OPTEE
# --------
# Get the sources
if [ ! -d "optee_os" ]
then
	echo -e "${ORANGE}Retrieving the ST optee-os git repo${NC}"
	git clone https://github.com/STMicroelectronics/optee_os.git --depth=1	
	cd optee_os
	echo -e "${ORANGE}Updating the conf.mk to fix the ram issue${NC}"
	git apply ${SCRIPT_DIR}/../dts/optee-os/conf.patch
else
	echo -e "${GREEN}ST OPTEE repo present, doing a pull${NC}"
	cd optee_os
	git pull
fi
# Copy the dt's
echo -e "${GREEN}Adding the tios dt's to the repo, so the latest are used${NC}"
cp ../../dts/optee-os/stm32mp151a-tios-mx.dts core/arch/arm/dts/
cp ../../dts/tf-a/fdts/stm32mp151a-tios-mx-fw-config.dts core/arch/arm/dts/

# Perform cleanup
echo -e "${GREEN}Perform cleanup${NC}"
make CROSS_COMPILE=${CC} clean
# Perform build for usb boot
echo -e "${GREEN}Build it${NC}"
make CROSS_COMPILE=${CC} PLATFORM=stm32mp1 CFG_EMBED_DTB_SOURCE_FILE=stm32mp151a-tios-mx.dts CFG_TEE_CORE_LOG_LEVEL=2 O=build -j$(nproc) all
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit 1
fi
# Copy the result to deploy folder
echo -e "${GREEN}Copying result to deploy${NC}"
cp build/core/*_v2.bin ../deploy/
# Build debug version
echo -e "${GREEN}Perform cleanup${NC}"
make CROSS_COMPILE=${CC} clean
echo -e "${GREEN}Building debug lvl 4${NC}"
make CROSS_COMPILE=${CC} PLATFORM=stm32mp1 CFG_EMBED_DTB_SOURCE_FILE=stm32mp151a-tios-mx.dts CFG_TEE_CORE_LOG_LEVEL=4 DEBUG=1 O=build -j$(nproc) all
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Build failed${NC}"
    exit 1
fi
echo -e "${GREEN}Copying result to deploy/debug${NC}"
cp build/core/*_v2.bin ../deploy/debug/
echo -e "${GREEN}Finished building OPTEE${NC}"
