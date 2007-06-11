@echo off
qemu -L . -m 128 -hda ReactOS.vmdk -cdrom ReactOS.iso -boot d