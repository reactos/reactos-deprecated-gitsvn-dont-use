#!/bin/bash
#
# Script for changing the current directory to $_ROSBE_ROSSOURCEDIR
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007-2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# An alias just to "cd $_ROSBE_ROSSOURCEDIR" won't work as bash only expands when executing the shell and then never again
cd "$_ROSBE_ROSSOURCEDIR"
