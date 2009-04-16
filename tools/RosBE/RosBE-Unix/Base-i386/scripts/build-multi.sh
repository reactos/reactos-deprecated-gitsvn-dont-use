#!/bin/bash
#
# Detects the CPU cores in your system and builds ReactOS with this number of threads
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

source "$_ROSBE_ROSSCRIPTDIR/build-shared.sh"

CPUCOUNT=`cpucount -x1`
buildtime make -j $CPUCOUNT $*
