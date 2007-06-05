#!/bin/bash
#
# Build with
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Display RosBE version
ROSBE_VERSION=`cat "$_ROSSCRIPTDIR/RosBE-Version"`

echo "This is the ReactOS Build Environment for Unix-based Operating Systems"
echo "Version $ROSBE_VERSION, containing the following components:"
echo

# Display tool versions
mingw32-gcc -v 2>&1 | grep "gcc version"
nasm -v
mingw32-ld -v
mingw32-make -v | grep "GNU Make"
