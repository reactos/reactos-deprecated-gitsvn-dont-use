# Shared functions for build.sh and build-multi.sh
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Check if config.template.rbuild is newer than config.rbuild
# If it is then abort the build and inform the user.
if [ -f "$_ROSBE_ROSSOURCEDIR/config.rbuild" ]; then
	if [ "$_ROSBE_ROSSOURCEDIR/config.template.rbuild" -nt "$_ROSBE_ROSSOURCEDIR/config.rbuild" ]; then
		echo
		echo "*** config.template.rbuild is newer than config.rbuild ***"
		echo "*** aborting build. Please check for changes and       ***"
		echo "*** update your config.rbuild.                         ***"

		exit 1
	fi
fi
