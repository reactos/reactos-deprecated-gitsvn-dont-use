#!/bin/bash
#
# Script for changing the current directory to $_ROSSOURCEDIR
# Part of RosBE for Unix-based Operating Systems
# Copyright 2007 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

# An alias just to "cd $_ROSSOURCEDIR" won't work as bash only expands when executing the shell and then never again
cd "$_ROSSOURCEDIR"
