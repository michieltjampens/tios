# Building the Debian Rootfs

> Based on [this](https://programmer.group/imx6ull-linux-root-file-system-rootfs-build.html)

## Debian

### Preparation
1. Get the required tools: `sudo apt install binfmt-support qemu-user-static debootstrap rsync`
2. Navigate to, or create a, workdir
3. Get the base rootfs: `sudo debootstrap --arch=armhf --foreign bookworm rootfs http://ftp.debian.org/debian/`

> - **armhf** processor architecture for imx6/7/MP15
> - **--foreign** because not compiled on the target platform
> - **bookworm** is codename for the latest debian release (at time of writing)
> - **rootfs** name of the folder to create it in
> - **http://ftp.debian.org/debian/**  donwload site 


4. Copy QEMU arm static to the basic system we just downloaded: `sudo cp /usr/bin/qemu-arm-static ./rootfs/usr/bin`

5. Build it: `sudo DEBIAN_FRONTEND=noninteractive DEBCONF_NONINTERACTIVE_SEEN=true chroot rootfs debootstrap/debootstrap --second-stage`

> Takes a while but should finish with **I: Base system installed successfully.**

6. Backup and alter sources.list
- Backup `sudo cp ./rootfs/etc/apt/sources.list ./rootfs/etc/apt/sources.list.bak`
- Open it with `sudo nano ./rootfs/etc/apt/sources.list` and replace the content with the following lines
```
deb http://deb.debian.org/debian/ bookworm contrib main non-free non-free-firmware
# deb-src http://deb.debian.org/debian/ bookworm contrib main non-free non-free-firmware

deb http://deb.debian.org/debian/ bookworm-updates contrib main non-free non-free-firmware
# deb-src http://deb.debian.org/debian/ bookworm-updates contrib main non-free non-free-firmware

deb http://deb.debian.org/debian/ bookworm-proposed-updates contrib main non-free non-free-firmware
# deb-src http://deb.debian.org/debian/ bookworm-proposed-updates contrib main non-free non-free-firmware

deb http://deb.debian.org/debian/ bookworm-backports contrib main non-free non-free-firmware
# deb-src http://deb.debian.org/debian/ bookworm-backports contrib main non-free non-free-firmware

deb http://deb.debian.org/debian-security/ bookworm-security contrib main non-free non-free-firmware
# deb-src http://deb.debian.org/debian-security/ bookworm-security contrib main non-free non-free-firmware
```
7. Create a mount script with `nano mount.sh`
```bash
#!/bin/bash
mnt() {
	echo "MOUNTING"
	sudo mount -t proc /proc ${2}proc
	sudo mount -t sysfs /sys ${2}sys
	sudo mount -o bind /dev ${2}dev
	sudo mount -o bind /dev/pts ${2}dev/pts
	sudo chroot ${2}
}
umnt() {
	echo "UNMOUNTING"
	sudo umount ${2}proc
	sudo umount ${2}sys
	sudo umount ${2}dev/pts
	sudo umount ${2}dev
}

if [ "$1" == "-m" ] && [ -n "$2" ] ;
then
	mnt $1 $2
elif [ "$1" == "-u" ] && [ -n "$2" ];
then
	umnt $1 $2
else
	echo ""
	echo "Either 1'st, 2'nd or both parameters were missing"
	echo ""
	echo "1'st parameter can be one of these: -m(mount) OR -u(umount)"
	echo "2'nd parameter is the full path of rootfs directory(with trailing '/')"
	echo ""
	echo "For example: ch-mount -m /media/sdcard/"
	echo ""
	echo 1st parameter : ${1}
	echo 2nd parameter : ${2}
fi
```
Make it executable: `sudo chmod +x mount.sh`
8. Mount the rootfs: `source mount.sh -m ./rootfs/`
9. Update with `apt update` & `apt upgrade`

This is the absolute minimum, next up is configuring the rootfs.

### Customizing
#### Updating & Locale
1. Check in another terminal which locale you are using now with `locale`
1. Set the proper locale with this
```
apt update
apt install locales
dpkg-reconfigure locales
```
#### Install commonly used tools
 `apt install sudo wget screen socat ssh htop telnet i2c-tools systemd-timesyncd gpiod curl git bat`
   - **sudo** - give a user admin priveleges  
   - **git** - interact with git repo's  
   - **wget** - download via weblinks  
   - **screen** - connect to serial ports
   - **socat** - connect to tcp ports and link them
   - **ssh** - enable remote acces via ssh
   - **htop** - system resources overview
   - **telnet** - connect to telnet servers
   - **i2c-tools** - tools to interact with i2c busses
   - **systemd-timesyncd** - lightweight alternative to ntpd for clock sync
   - **gpiod** - command-line tools to interact with gpio-pins
   - **curl**
   - **git** - versioning control software
   - **bat** - Alternative version of cat, with syntax highlighting etc

#### Configute users
- Set root user password: `passwd root`
- Add a new user; `adduser username`
  - Optionally add this user to the sudo group: `adduser username sudo`
  - Optionally add this user to the dialout group to use serialports: `adduser username dialout`
  
#### Set up ethernet
**Allow hotplugging**
`echo "allow-hotplug end0" > /etc/network/interfaces.d/end0`
`echo "iface end0 inet dhcp" >> /etc/network/interfaces.d/end0`

**Hostname**
`echo "pickahostname" > /etc/hostname`
`echo "127.0.0.1 localhost" >> /etc/hosts`
`echo "127.0.0.1 pickahostname" >> /etc/hosts`

#### Set up fstab
Open fstab with `nano /etc/fstab` and add the following:
```bash
/dev/root            /                    auto       defaults              1  1
proc                 /proc                proc       defaults              0  0
devpts               /dev/pts             devpts     mode=0620,gid=5       0  0
tmpfs                /run                 tmpfs      mode=0755,nodev,nosuid,strictatime 0  0
tmpfs                /var/volatile        tmpfs      defaults              0  0

/dev/mmcblk1p1       /media/sd            auto       rw,user,exec,umask=000 0 2
/dev/mmcblk0p6       /boot                ext4       defaults,sync         0  1
/dev/mmcblk0p7       /vendor              ext4       defaults,sync         0  1
```
### Finish up
- Sign out with `exit`
- Unmount `source mount.sh -u ./rootfs/`
- Pack it up `sudo tar -zcvf debian_bullseye-rootfs.tar.gz ./rootfs/*`
- Copy it directly:
  * Using CP `sudo cp -rp ./rootfs/* /mnt/RFS/`, followed by `sync` to be certain it's copied.
  * Or rsync `sudo rsync -avPAHXx --numeric-ids ./rootfs/ /mnt/RFS/`
