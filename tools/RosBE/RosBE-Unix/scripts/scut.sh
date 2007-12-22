#!/bin/bash
#
# Script for launching scut and changing the current directory appropriately
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

case "$1" in
add|rem|edit|def|list|/?|-h|--help)
	# It's an scut parameter, which can just be passed to the program
	"$_ROSSCRIPTDIR/bin/scut" $*
	;;

*)
	# It's probably the name of a shortcut
	_ROSSOURCEDIR=`"$_ROSSCRIPTDIR/bin/scut" $*`

	if [ "$_ROSSOURCEDIR" != "Default" ] && [ -d "$_ROSSOURCEDIR" ]; then
		export _ROSSOURCEDIR
		basedir
	fi

	;;
esac
