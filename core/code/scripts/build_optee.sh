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
    exit
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
    exit
fi
echo -e "${GREEN}Copying result to deploy/debug${NC}"
cp build/core/*_v2.bin ../deploy/debug/
echo -e "${GREEN}Finished building OPTEE${NC}"
