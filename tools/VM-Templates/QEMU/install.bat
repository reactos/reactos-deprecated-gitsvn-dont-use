@echo off
cd files
qemu -L . -m 128 -hda ReactOS.vmdk -cdrom ReactOS.iso -boot d