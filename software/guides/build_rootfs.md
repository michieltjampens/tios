# Building the Debian Rootfs

> Based on [this](https://programmer.group/imx6ull-linux-root-file-system-rootfs-build.html)

## Debian

### Preparation
1. Get the required tools: `sudo apt install binfmt-support qemu-user-static debootstrap rsync`
2. Navigate to, or create a, workdir
3. Get the base rootfs: `sudo debootstrap --arch=armhf --foreign bookworm rootfs http://ftp.debian.org/debian/`

> - *armhf* processor architecture for imx6/7
> - *--foreign* because not compiled on the target platform
> - *bullseye* is codename for the latest debian release (at time of writing)
> - *rootfs* name of the folder to create it in
> - *http://ftp.debian.org/debian/*  donwload site 
{.is-info}


4. Copy QEMU arm static to the basic system we just downloaded: `sudo cp /usr/bin/qemu-arm-static ./rootfs/usr/bin`

5. Build it: `sudo DEBIAN_FRONTEND=noninteractive DEBCONF_NONINTERACTIVE_SEEN=true chroot rootfs debootstrap/debootstrap --second-stage`

> Takes a while but should finish with **I: Base system installed successfully.**

6. Backup and alter sources.list
- Backup `sudo cp ./rootfs/etc/apt/sources.list ./rootfs/etc/apt/sources.list.bak`
- Write it with `sudo nano ./rootfs/etc/apt/sources.list`

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
9. Update with `apt update -y && sudo apt full-upgrade -y && sudo apt autoremove -y && sudo apt clean -y && sudo apt autoclean -y`

This is the absolute minimum, next up is configuring the rootfs.

### Customizing
#### Updating & Locale
1. Check in another terminal which locale you are using now with `locale`
1. Set it in the created rootfs `export LC_ALL=<your locale>` fe. `export LC_ALL=en_IE.UTF-8`
> **If that doesn't work**
> Install the locales package with `apt install locales`
> run `locale-gen en_IE.UTF-8`
> run `update-locale LC_ALL=en_IE.UTF-8`


#### Install commonly used tools
 `apt install sudo wget screen socat ssh htop telnet i2c-tools systemd-timesyncd gpiod curl`
 *  **sudo** - give a user admin priveleges
 *  **git** - interact with git repo's
 * **wget** - download via weblinks
 *  **screen** - connect to serial ports
 *  **socat** - connect to tcp ports
 *  **ssh** - enable remote acces via ssh
 *  **htop** - system resources overview
 *  **telnet** - connect to telnet servers
 *  **i2c-tools** - tools to interact with i2c busses
 *  **systemd-timesyncd** - lightweight alternative to ntpd for clock sync
 *  **gpiod** command-line tools to interact with gpio-pins
 *  **curl**

#### Configute users
- Set root user password: `passwd root`
- Add a new user; `adduser <username>`
  - Optionally add this user to the sudo group: `adduser <username> sudo`
  - Optionally add this user to the dialout group to use serialports: `adduser <username> dialout`
  
#### Set up ethernet
**Allow hotplugging**
`echo "allow-hotplug eth0" > /etc/network/interfaces.d/eth0`
`echo "iface eth0 inet dhcp" >> /etc/network/interfaces.d/eth0`

**Hostname**
`echo "pickahostname" > /etc/hostname`
`echo "127.0.0.1 localhost" >> /etc/hosts`
`echo "127.0.0.1 pickahostname" >> /etc/hosts`

#### Set up fstab
This is hardware dependent so as a reference:
`nano /etc`
```bash
/dev/root            /                    auto       noatime               							1  1
/dev/mmcblk0p1       /boot                auto       noatime	            							0  0
proc                 /proc                proc       defaults              							0  0
devpts               /dev/pts             devpts     mode=0620,ptmxmode=0666,gid=5      0  0
tmpfs                /run                 tmpfs      mode=0755,nodev,nosuid,strictatime 0  0
tmpfs                /var/volatile        tmpfs      defaults              							0  0
#Optional for sd card
#/dev/mmcblk1p1       /media/card          auto       defaults,sync,noauto  						0  0
```

```bash
# Use 'blkid' to print the universally unique identifier for a
# device; this may be used with UUID= as a more robust way to name devices
# that works even if disks are added and removed. See fstab(5).
#
# systemd generates mount units based on this file, see systemd.mount(5).
# Please run 'systemctl daemon-reload' after making changes here.
#
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
UUID=491f6117-415d-4f53-88c9-6e0de54deac6           /               ext4    errors=remount-ro   0       1
UUID=9ff74730-6568-4a18-bebd-eaf318cce82d           /boot           vfat    umask=0077          0       1
```

### Finish up
- Sign out with `exit`
- Unmount `source mount.sh -u ./rootfs/`
- Pack it up `sudo tar -zcvf debian_bullseye-rootfs.tar.gz ./rootfs/*`
- Copy it directly:
  * With rsync `sudo rsync -avPAHXx --numeric-ids ./rootfs/ /mnt/RFS/` (goes fast)
  * Then use `sync` to actually write
  
### Good to know
* If lsblk gives a higher size dan df -H, the partition is bigger than the useable space, to fix this
    * sudo e2fsck -f /dev/<path>
    * sudo resize2fs /dev/<path>
