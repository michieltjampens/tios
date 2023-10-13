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
# Create a deploy folder, if it doensn't exist
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
# Requires tools
sudo apt install python3-pyelftools build-essential swig -y
