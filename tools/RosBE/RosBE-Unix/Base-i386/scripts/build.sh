#!/bin/bash
#
# Builds ReactOS with one thread
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

source "$_ROSBE_ROSSCRIPTDIR/build-shared.sh"

buildtime make $*
