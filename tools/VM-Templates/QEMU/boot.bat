@echo off
cd files
qemu -L . -m 128 -hda ReactOS.vmdk -serial file:CON