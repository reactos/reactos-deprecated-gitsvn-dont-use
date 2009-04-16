#!/bin/bash
#
# Show the tool versions
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

source "$_ROSBE_ROSSCRIPTDIR/rosbelibrary.sh"

# Display RosBE version
echo "This is the ReactOS Build Environment for Unix-based Operating Systems"
echo "Version $_ROSBE_VERSION"
echo

# Environment Information
boldmsg "Environment Information"
echo "Selected Target Architecture: $_ROSBE_ARCH"
echo

# Display tool versions
boldmsg "Build Tools"
source "$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/version.sh"
