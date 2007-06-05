#!/bin/bash
#
# Script for initializing RosBE
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Get the absolute path to the script directory
cd `dirname $0`
_ROSSCRIPTDIR="$PWD"
export _ROSSCRIPTDIR

# Save the ReactOS source directory
_ROSSOURCEDIR="$1"
export _ROSSOURCEDIR

# Read the RosBE version
# The file "RosBE-Version" has been created by the RosBE-Builder.sh script
ROSBE_VERSION=`cat "$_ROSSCRIPTDIR/RosBE-Version"`

# Set the text color
if [ "$2" != "" ]; then
	echo -e "\e[$2m"
fi

# Display banner
echo "*******************************************************************************"
echo "*         ReactOS Build Environment for Unix-based Operating Systems          *"
echo "*                      by Colin Finck <mail@colinfinck.de>                    *"
echo "*                                                                             *"
echo "*                                Version $ROSBE_VERSION                                *"
echo "*******************************************************************************"

bash --rcfile "$_ROSSCRIPTDIR/RosBE-rc"
