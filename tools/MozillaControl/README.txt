The distributed (by Adam Lock, http://www.iol.ie/~locka/mozilla/mozilla.htm)
MozillaControl1712.exe contains the Mozilla control compiled with MSVC6, but
with MSVCP70.DLL included instead of the required MSVCP60.DLL. So we repackaged
the control to include the correct DLL.

To repackage: install the distributed MozillaControl1712 to its default location,
"C:\Program Files\Mozilla Control v1.7.12". Copy MSVCP60.DLL to subdir redist (of
this dir). Then run "makensis control-ros.nsi", this should result in a repackaged
MozillaControl1712-ReactOS.exe
