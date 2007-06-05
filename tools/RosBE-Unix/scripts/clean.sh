#!/bin/bash
#
# Script for cleaning the ReactOS source directory
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

pushd "$_ROSSOURCEDIR" >& /dev/null

if [ -d "obj-i386" ]; then
	echo "Cleaning ReactOS source directory..."

	if [ -f "makefile.auto" ]; then
		make clean >& /dev/null
	fi

	rm -rf "obj-i386"
	rm -rf "output-i386"
	rm -rf "reactos."
	rm -f "makefile.auto"

	echo "Done cleaning ReactOS source directory."
else
	echo "There is nothing to clean."
fi

popd >& /dev/null
