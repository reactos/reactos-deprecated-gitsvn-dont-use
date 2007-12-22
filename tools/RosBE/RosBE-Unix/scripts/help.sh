#!/bin/bash
#
# Script for displaying the help
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

echo
echo "Available Commands:"
echo " make [OPTIONS]  - make, without options does a standard build of"
echo "                   ReactOS. OPTIONS are the standard ReactOS build"
echo "                   options, i.e. bootcd."
echo " makex [OPTIONS] - Same as 'make' but automatically determines the"
echo "                   number of CPUs in the system and uses -j with"
echo "                   the appropriate number."
echo " basedir         - Switch back to the ReactOS source directory."
echo " clean           - Fully clean the ReactOS source directory."
echo " help            - Display the available commands."
echo " scut [OPTIONS]  - List, add, edit, remove, switch and default to"
echo "                   shortcutted ReactOS source directories."
echo " version         - Show information about the tool versions."

echo

