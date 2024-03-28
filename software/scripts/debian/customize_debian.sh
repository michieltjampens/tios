echo -e "${GREEN}Update apt${NC}"
apt update
apt install locales
echo -e "${GREEN}Fix locale${NC}"
dpkg-reconfigure locales
echo -e "${GREEN}Upgrade apt${NC}"
apt upgrade -y
apt dist-upgrade -y
echo -e "${GREEN}Install tools${NC}"
apt install sudo wget screen socat ssh htop telnet i2c-tools systemd-timesyncd gpiod curl -y
echo -e "${GREEN}Set Hostname to tioscore${NC}"
echo "tioscore" > /etc/hostname
echo "127.0.0.1 localhost" >> /etc/hosts
echo "127.0.0.1 tioscore" >> /etc/hosts
echo -e "${GREEN}Setting root pwd${NC}"
passwd root
echo -e "${GREEN}Adding user${NC}"
echo -e "${ORANGE}Username to use in rootfs?${NC}"
read username
adduser $username
echo -e "${GREEN}Finishing up${NC}"
exit
source mount.sh -u ./rootfs/
