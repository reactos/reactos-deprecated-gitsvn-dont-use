#!/bin/bash
#
# Build with
# Part of Generic RosBE
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

source "$_ROSSCRIPTDIR/build-shared.sh"

CPUCOUNT=`cpucount`
buildtime mingw32-make -j $CPUCOUNT $*
