##!/bin/bash
# Some things to add some color to the output
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
# Go up to the code folder
cd ${SCRIPT_DIR}/..
if [ ! -d "build/deploy" ]
then
	echo -e "${RED}No deploy files yet, run other scripts first${NC}"
	exit
fi
cd build/arm-trusted-firmware
echo -e "${GREEN}Building regular fip!${NC}"
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
cp build/stm32mp1/release/fip.bin ../deploy/
echo -e "${GREEN}Building debug fip!${NC}" 
make PLAT=stm32mp1 ARCH=aarch32 ARM_ARCH_MAJOR=7 \
    AARCH32_SP=optee \
    DTB_FILE_NAME=stm32mp151a-tios-mx.dtb \
    BL33=../deploy/u-boot-nodtb.bin \
    BL33_CFG=../deploy/u-boot.dtb \
    BL32=../deploy/debug/tee-header_v2.bin \
    BL32_EXTRA1=../deploy/debug/tee-pager_v2.bin \
    BL32_EXTRA2=../deploy/debug/tee-pageable_v2.bin \
    STM32MP_EMMC=1 \
    STM32MP15=1 \
    fip
cp build/stm32mp1/release/fip.bin ../deploy/debug/fip-debug.bin
