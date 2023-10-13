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
sudo apt install binfmt-support qemu-user-static debootstrap rsync -y
# --------
# Debian
# --------
# Get the sources
if [ ! -d "rootfs" ]
then
	echo -e "${GREEN}Retrieving the bare rootfs${NC}"
	sudo debootstrap --arch=armhf --foreign bookworm rootfs http://ftp.debian.org/debian/
	echo -e "${GREEN}Copying QEMU${NC}"
	sudo cp /usr/bin/qemu-arm-static ./rootfs/usr/bin	
	echo -e "${GREEN}Building it...${NC}"
	sudo DEBIAN_FRONTEND=noninteractive DEBCONF_NONINTERACTIVE_SEEN=true chroot rootfs debootstrap/debootstrap --second-stage
	echo -e "${GREEN}Backing up sources.list${NC}"
	sudo cp ./rootfs/etc/apt/sources.list ./rootfs/etc/apt/sources.list.bak
	echo -e "${GREEN}Overwrite sources.list with bookworm sources${NC}"
	sudo cp -v ../scripts/debian/sources.list ./rootfs/etc/apt/sources.list
	echo -e "${GREEN}Overwrite /etc/fstab${NC}"
	sudo cp -v ../scripts/debian/fstab ./rootfs/etc/
	echo -e "${GREEN}Add mounting script${NC}"
	sudo cp ../scripts/debian/mount.sh .
	sudo chmod +x mount.sh
	sudo cp ../scripts/debian/customize_debian.sh ./rootfs/opt/
else
	echo -e "${GREEN}Rootfs already present${NC}"	
fi
echo -e "${GREEN}Mounting rootfs, ready for user customs${NC}"
echo -e "${GREEN}Default setup script found at /opt/customize_debian.sh${NC}"
source mount.sh -m ./rootfs/
# Stops here...
