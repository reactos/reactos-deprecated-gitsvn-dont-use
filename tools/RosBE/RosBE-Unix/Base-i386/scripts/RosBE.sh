#!/bin/bash
#
# Script for initializing RosBE
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2010 Colin Finck <colin@reactos.org>
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
export _ROSBE_OLDPATH="$_ROSBE_ROSSCRIPTDIR/bin:$PATH"

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

# Try to set the architecture to the one specified or fallback to "i386"
change_architecture "$3:-i386"

# Display banner
echo "*******************************************************************************"
echo "*         ReactOS Build Environment for Unix-based Operating Systems          *"
echo "*                      by Colin Finck <colin@reactos.org>                     *"
echo "*                                                                             *"
echo "*                                 Version $_ROSBE_VERSION                                 *"
echo "*******************************************************************************"
echo
echo "For a list of all included commands, type: \"help\""
echo "-------------------------------------------------"
echo

bash --rcfile "$_ROSBE_ROSSCRIPTDIR/RosBE-rc"
