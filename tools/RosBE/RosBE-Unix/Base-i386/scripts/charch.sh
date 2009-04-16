#!/bin/bash
#
# Change the current target build tools to an architecture to build for
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

if [ "$1" = "" ]; then
	echo "charch: Changes the architecture to build for."
	echo "The appropriate build tools for this architecture need to be installed by a"
	echo "RosBE-Unix package."
	echo
	echo "Syntax: charch <architecture>"
	return 0
fi

# Change the architecture
source "$_ROSBE_ROSSCRIPTDIR/rosbelibrary.sh"
change_architecture $1
source "$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/setvars.sh"
set_path_variable

# Display tool versions
source "$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/version.sh"
