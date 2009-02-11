#!/bin/bash
#
# Script for cleaning the ReactOS source directory
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

pushd "$_ROSBE_ROSSOURCEDIR" >& /dev/null
source "$_ROSBE_ROSSCRIPTDIR/$_ROSBE_ARCH/clean.sh"
popd >& /dev/null
