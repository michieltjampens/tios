##!/bin/bash
# Some things to add some color to the output
RED='\033[0;31m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo -e "${GREEN}Building tf-a!${NC}"
./build_tfa.sh
echo -e "${GREEN}Building optee!${NC}"
./build_optee.sh
echo -e "${GREEN}Building u-boot!${NC}"
./build_uboot.sh
echo -e "${GREEN}Building fips!${NC}"
./build_fip.sh
echo -e "${GREEN}Everything done!${NC}"
