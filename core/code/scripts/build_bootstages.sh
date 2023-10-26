##!/bin/bash
# Some things to add some color to the output
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo -e "${GREEN}Building tf-a!${NC}"
./build_tfa.sh
if [ ! $? -eq 0 ]; then
    echo -e "${RED}TF-A failed${NC}"
    exit
fi
echo -e "${GREEN}Building optee!${NC}"
./build_optee.sh
if [ ! $? -eq 0 ]; then
    echo -e "${RED}Optee failed${NC}"
    exit
fi
echo -e "${GREEN}Building u-boot!${NC}"
./build_uboot.sh
if [ ! $? -eq 0 ]; then
    echo -e "${RED}UBoot failed${NC}"
    exit
fi
echo -e "${GREEN}Building fips!${NC}"
./build_fip.sh
if [ ! $? -eq 0 ]; then
    echo -e "${RED}FIP failed${NC}"
    exit
fi
echo -e "${GREEN}Everything done!${NC}"
