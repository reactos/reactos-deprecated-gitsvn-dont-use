#!/bin/bash
#
# Builds ReactOS with one thread
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

source "$_ROSSCRIPTDIR/build-shared.sh"

buildtime mingw32-make $*
