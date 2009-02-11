# Clean i386 source directories
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

if [ -d "obj-i386" ]; then
	echo "Cleaning ReactOS i386 source directory..."

	rm -rf "obj-i386"
	rm -rf "output-i386"
	rm -rf "reactos."
	rm -f "makefile.auto"

	echo "Done cleaning ReactOS i386 source directory."
else
	echo "ERROR: There is no i386 compiler output to clean."
fi
