== Introduction ==

RosTE is a Windows based test environment for both ReactOS testers and developers. 
It aims to provide an official test environment in a similar fashion to RosBE. 
It also aims to provide an environment which is quick and easy to setup, allowing 
non-techs or newbies interested in the project to get up an running quickly, with 
easy access to important features such as live debugging or quick mounting of the 
VM image.

RosTE encompasses the following key components:

- ReactOS QEmu Manager 1.3
This is a GUI front end to RosTE giving a high degree of versatility over the 
virtual machine, with many other features added to ensure management of your 
test environment is simple.

- QEmu 0.9.0
This is the latest QEmu virtual machine for Windows. It provides a reliable 
and proven test bed for operating system development

- KQEmu 1.3.0pre-11
This is the latest offering of QEmu accelerator, a driver allowing the emulator 
to run much faster, improving the performance of ReactOS significantly

- VDK  3.2
This is a virtual disk driver allowing quick mounting of your virtual machine 
image, allowing it to appear as a regular drive on your Windows operating system.



== Installation ==

Instalation is simple, just double click the installer, and follow the simple 
instructions. Upon completion, the QEmu manager will appear.



== Setup ==

Before your newly installed test environment is ready for use, you must configure 
some vital settings.

1. Creating a new VM image (recommended)
If you don’t plan on using an existing image, you must setup a new image for use 
with RosTE.  Click on the ‘tools’ tab, set the ‘size’ of the image to your preferred 
value (512 is recommended), ensure the format is ‘vmdk’ and click ‘create image’
In the dialog, choose a save location (remember where you save this) and click ‘save’

2. Setting paths
- Paths tab
	If you choose to leave the install path as default (C:\Program Files\RosTE) 
	then you needn’t worry about setting the QEmu and VDK paths, however if you 
	changed this, then you will need to re-associate your paths with your chosen 
	install location.

	Set the QEmu and VDK paths by clicking on the open dialog buttons and locating 
	the paths of the corresponding folders (<your path>/RosTE/QEmu and <your path>/RosTE/VDK)
	
- Harddisk tab
	Check the ‘HDA’ setting and locate the path of your virtual machine image (ReactOS.vmdk)

- CD-ROM tab
	Ensure the CD-ROM setting is checked.
	If you plan on booting ReactOS from the iso provided by RosBE, click on ‘Image File’ 
	and enter the path of your default ReactOS source directory. This is where RosBE 
	will save the generated iso. Otherwise, point this image file to the file located on 
	your harddisk.
	This setting can be changed to ‘Host CD-ROM’ once you have an installed system, 
	allowing you to use your computers CD-ROM as the virtual machine CD-ROM

- Tools tab
	You must be an administrator to use the VDK feature.
	Set this to the path of your virtual machine image (ReactOS.vmdk)

3. Setup debugging
To setup live debugging, click the ‘Debug’ tab and ensure ‘Redirect to screen’ is 
checked. This will setup a telnet session and stream the debug output to a window 
provided by RosTE. Note, you must choose ‘Debug’ at the first boot option to receive 
debug information
