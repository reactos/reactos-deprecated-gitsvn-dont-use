#!/bin/bash
#
# Show the tool versions
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Display RosBE version
echo "This is the ReactOS Build Environment for Unix-based Operating Systems"
echo "Version $_ROSBE_VERSION, currently running with these components:"
echo

# Display tool versions
source "$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/version.sh"
