Download the official immage [here](https://)
We have a .img file which needed to be written to the micro-SD card. 
We identify the device path for the micro-SD card using:

```sh
$ fdisk -l
```

For exampl, on my system it is /dev/mmcblk0

Then we need to umount the disk:

```sh
$ umount /dev/mmcblk0
```

Then ran the dd command with the following parameters to write the image file to the micro-SD card:

```sh
$ sudo dd bs=1M if=bone-debian-8.6-lxqt-4gb-armhf-2016-11-06-4gb.img of=/dev/mmcblk0
```

This takes about 5 minutes.

After that, with the BBB off, we have to insert the micro SD card into the slot at the base of the BBB. Now, holding down the “User Boot” button, we can power up the BeagleBone.

It takes a few seconds for the device to start up, after which we can reach the BeagleBone using SSH:

```sh
$ ssh debian@192.168.7.2
```
