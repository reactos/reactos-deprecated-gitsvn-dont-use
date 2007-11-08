# Shared functions for build.sh and build-multi.sh
# Part of Generic RosBE
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# Check if config.template.rbuild is newer than config.rbuild
# If it is then abort the build and inform the user.
if [ -f "$_ROSSOURCEDIR/config.rbuild" ]; then
	if [ "$_ROSSOURCEDIR/config.template.rbuild" -nt "$_ROSSOURCEDIR/config.rbuild" ]; then
		echo
		echo "*** config.template.rbuild is newer than config.rbuild ***"
		echo "*** aborting build. Please check for changes and       ***"
		echo "*** update your config.rbuild.                         ***"

		exit 1
	fi
fi

