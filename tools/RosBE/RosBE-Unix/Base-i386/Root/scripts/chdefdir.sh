#!/bin/bash
#
# Change the ReactOS source directory for the current session
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

if [ "$1" = "" ]; then
	echo "chdefdir: Change the ReactOS source directory for the current RosBE session"
	echo
	echo "Syntax: chdefdir <directory>"
	return 0
fi

export _ROSBE_ROSSOURCEDIR="$1"
basedir
