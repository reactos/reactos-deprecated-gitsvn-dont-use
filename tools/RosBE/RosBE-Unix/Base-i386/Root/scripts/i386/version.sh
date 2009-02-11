# Show the i386 tool versions
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

mingw32-gcc -v 2>&1 | grep "gcc version"
nasm -v
mingw32-ld -v
