Packaging the QEMU Preinstallation
-----------------------------------

Replace [VERSION] with the version of ReactOS you want to package, e.g. 0.3.1.

- Copy the files in this directory into a directory called
  "ReactOS-[VERSION]-QEMU".
- Put the QEMU program files in the "files" subdirectory, including its subdirectories.
- Use the following command to create the virtual hard disk:

     qemu-img create -f vmdk ReactOS.vmdk 2G

- Copy the "ReactOS.iso" file into the "files" subdirectory and run "install.bat" to
  install it.
- Remove the "ReactOS.iso", "install.bat" and this "SVN-Readme.txt" file
- Create a ZIP file "ReactOS-[VERSION]-REL-qemu.zip" containing this
  directory.
  Do not forget to activate the option to store the relative path names.