ReactOS Preinstalled on a QEMU Virtual Machine
===============================================

This package contains a preinstalled ReactOS version on a QEMU virtual
machine.
For Windows users, it also contains the QEMU application, so you can easily
start the virtual machine and run ReactOS.


Windows users
--------------
To start the preinstalled ReactOS version, just execute the "boot.bat" file.

You can improve the speed of this virtual machine by installing Kqemu.
It is available at http://www.h7.dion.ne.jp/~qemu-win (Section "Accelerators")

If you want to access files of the virtual hard disk, download and install VDK
from http://chitchat.at.infoseek.co.jp/vmware/vdk.html
This tool allows you to mount the virtual hard disk in your Windows system.


Users of Unix-based Operating Systems
--------------------------------------
If you use an Unix-based Operating System such as Linux, install the
appropriate QEMU package for your OS.
Then open a Terminal, change the current directory to the directory containing
the "ReactOS.vmdk" file and execute the following command:

   qemu -m 128 -hda ReactOS.vmdk

You can improve the speed of the virtual machine by installing the appropriate
Kqemu package for your system.

If you want to access and modify files of the virtual hard disk, you have to
convert it to the "raw" format.
To convert it, execute the following commands in the directory containing the
"ReactOS.vmdk" file:

   qemu-img convert ReactOS.vmdk -O raw ReactOS.hd

Beware that this takes 2 GB of hard disk space, since a raw hard disk image
is not growable.
Now you can easily mount the image with the following command:

   mount -oloop,offset=32256 -t vfat ReactOS.hd /mnt/vhd

In this case, I assume that you want to mount the hard disk image to /mnt/vhd.
Otherwise modify the command line accordingly.

After you unmounted the changed hard disk, you can boot it by executing the
following command:

   qemu -m 128 -hda ReactOS.hd