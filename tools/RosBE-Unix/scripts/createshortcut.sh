#!/bin/bash
#
# Script for creating shortcuts
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Constants
DEFAULT_SHORTCUT_DIR="$HOME/Desktop"

# Functions
greenmsg()
{
	echo -e "\e[32m$1\e[0m"
}

boldmsg()
{
	echo -e "\e[1m$1\e[0m"
}

# Get the absolute path to the script directory
cd `dirname $0`
SCRIPTDIR="$PWD"

# Read the RosBE version
# The file "RosBE-Version" has been created by the RosBE-Builder.sh script
ROSBE_VERSION=`cat "$SCRIPTDIR/RosBE-Version"`

# Select the source directory
boldmsg "ReactOS Source Directory"

echo "Enter the directory where the ReactOS sources are located."
echo "This directory will become the current directory, when you start the Build Environment."

sourcedir=""
while [ "$sourcedir" = "" ]; do
	read -p "Directory: " sourcedir

	if ! [ -d "$sourcedir" ]; then
		echo "The entered path is no directory. Please enter the right path to the ReactOS sources."
		sourcedir=""
	fi

	echo
done

# Select the shortcut directory
boldmsg "Shortcut Directory"

echo "In which directory do you want to create the shortcut?"
echo "Enter the path to the directory here or simply press ENTER to install it into the Desktop directory of the current user."

shortcutdir=""
while [ "$shortcutdir" = "" ]; do
	read -p "[$DEFAULT_SHORTCUT_DIR] " shortcutdir

	if [ "$shortcutdir" = "" ]; then
		shortcutdir="$DEFAULT_SHORTCUT_DIR"
	elif ! [ -d "$shortcutdir" ]; then
		echo "The entered path is no directory. Please enter a valid path."
		shortcutdir=""
	fi

	echo
done

# Create the shortcut
boldmsg "Create Shortcut"

echo -n "Creating shortcut... "
shortcut="$shortcutdir/ReactOS Build Environment.desktop"

echo "[Desktop Entry]" > "$shortcut"
echo "Type=Application" >> "$shortcut"
echo "Version=$ROSBE_VERSION" >> "$shortcut"
echo "Name=ReactOS Build Environment" >> "$shortcut"
echo "Icon=$SCRIPTDIR/RosBE.png" >> "$shortcut"
echo "Exec=bash \"$SCRIPTDIR/RosBE.sh\" \"$sourcedir\" 34" >> "$shortcut"		# 34 = Blue color
echo "Terminal=true" >> "$shortcut"

greenmsg "OK"
