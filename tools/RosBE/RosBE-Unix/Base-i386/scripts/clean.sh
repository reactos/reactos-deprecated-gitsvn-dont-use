#!/bin/bash
#
# Script for cleaning the ReactOS source directory
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2010 Colin Finck <colin@reactos.org>
#
# Released under GNU GPL v2 or any later version.

# Check if the user set any custom filenames or pathes, otherwise locally set the appropriate variables.
ROS_AUTOMAKE="${ROS_AUTOMAKE:-makefile-$ROS_ARCH.auto}"
ROS_INTERMEDIATE="${ROS_INTERMEDIATE:-obj-$ROS_ARCH}"
ROS_OUTPUT="${ROS_OUTPUT:-output-$ROS_ARCH}"
ROS_CDOUTPUT="${ROS_CDOUTPUT:-reactos.}"

# Do some basic sanity checks to verify that we are working in a ReactOS source tree.
# Consider that we also want to clean half-complete builds, so don't depend on too many existing files.
if [ -d "$ROS_INTERMEDIATE" ] && [ -d "$ROS_OUTPUT" ]; then
	echo "Cleaning ReactOS $ROS_ARCH source directory..."

	rm -f "$ROS_AUTOMAKE"
	rm -rf "$ROS_INTERMEDIATE"
	rm -rf "$ROS_OUTPUT"
	rm -rf "$ROS_CDOUTPUT"

	echo "Done cleaning ReactOS $ROS_ARCH source directory."
else
	echo "ERROR: There is no $ROS_ARCH compiler output to clean."
fi
