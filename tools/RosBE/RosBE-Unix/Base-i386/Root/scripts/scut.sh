#!/bin/bash
#
# Script for launching scut and changing the current directory appropriately
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

case "$1" in
add|rem|edit|def|list|/?|-h|--help)
	# It's an scut parameter, which can just be passed to the program
	"$_ROSBE_ROSSCRIPTDIR/bin/scut" $*
	;;

*)
	# It's probably the name of a shortcut
	_ROSBE_ROSSOURCEDIR=`"$_ROSBE_ROSSCRIPTDIR/bin/scut" $*`

	if [ "$_ROSBE_ROSSOURCEDIR" != "Default" ] && [ -d "$_ROSBE_ROSSOURCEDIR" ]; then
		export _ROSBE_ROSSOURCEDIR
		basedir
	fi

	;;
esac
