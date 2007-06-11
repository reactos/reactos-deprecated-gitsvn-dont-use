Packaging the VMware Preinstallation
-------------------------------------

Replace [VERSION] with the version of ReactOS you want to package, e.g. 0.3.1.

- Copy the files in this directory into a directory called
  "ReactOS-[VERSION]-VMware".
- Copy the "ReactOS.iso" file into this directory.
- Open the "ReactOS-Installation.vmx" file in your VMware product, click on
  "Edit virtual machine settings" and on "Add".
- Add a new hard disk called "ReactOS.vmdk" with 2 GB size and in a growable
  format (deselect "Allocate all disk space now").
- Remove the created "Hard Disk 2" entry as we already have a "Hard Disk"
  entry, which now uses the created file.
- Start the virtual machine and install ReactOS on it.
- Remove all files in this directory except "ReactOS.vmx", "ReactOS.vmdk" and
  "Readme.txt"
- Create a ZIP file "ReactOS-[VERSION]-REL-vmware.zip" containing the
  "ReactOS-[VERSION]-VMware" directory.
  Do not forget to activate the option to store the relative path names.