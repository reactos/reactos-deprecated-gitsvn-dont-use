#!/bin/bash
#
# Script for initializing RosBE
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Save the ReactOS source directory
if [ "$1" = "" ]; then
	_ROSBE_ROSSOURCEDIR="$PWD"
else
	_ROSBE_ROSSOURCEDIR="$1"
fi

export _ROSBE_ROSSOURCEDIR

# Get the absolute path to the script directory
cd `dirname $0`
export _ROSBE_ROSSCRIPTDIR="$PWD"

# Save the current PATH variable
export _ROSBE_OLDPATH="$PATH"

# Make sure that some important variables are clean
export HOST=
export CFLAGS=
export CXXFLAGS=
export LDFLAGS=

# Export the host compiler search pathes to make them available for other build tools
export ROSBE_HOST_CFLAGS=`cpp -x c -v < /dev/null 2>&1 | $_ROSBE_ROSSCRIPTDIR/bin/getincludes`
export ROSBE_HOST_CXXFLAGS=`cpp -x c++ -v < /dev/null 2>&1 | $_ROSBE_ROSSCRIPTDIR/bin/getincludes`

# Read the RosBE version
# The file "RosBE-Version" has been created by the RosBE-Builder.sh script
export _ROSBE_VERSION=`cat "$_ROSBE_ROSSCRIPTDIR/RosBE-Version"`

# Set the text color
if [ "$2" != "" ]; then
	echo -e "\e[$2m"
fi

# Set the architecture to build for
source "$_ROSBE_ROSSCRIPTDIR/rosbelibrary.sh"

if [ "$3" = "" ]; then
	# No architecture specified, fall back to i386 without any message
	export _ROSBE_ARCH="i386"
else
	# Try to set the architecture to the one specified
	change_architecture $3
fi

source "$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/setvars.sh"

# Display banner
echo "*******************************************************************************"
echo "*         ReactOS Build Environment for Unix-based Operating Systems          *"
echo "*                      by Colin Finck <mail@colinfinck.de>                    *"
echo "*                                                                             *"
echo "*                               Version $_ROSBE_VERSION                                 *"
echo "*******************************************************************************"
echo
echo "For a list of all included commands, type: \"help\""
echo "-------------------------------------------------"
echo

bash --rcfile "$_ROSBE_ROSSCRIPTDIR/RosBE-rc"
