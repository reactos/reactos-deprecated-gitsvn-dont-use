# Set architecture-specific environment variables
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009 Colin Finck <mail@colinfinck.de>
#
# Released under GNU GPL v2 or any later version.

export ROSBE_TARGET_CFLAGS="-I$_ROSBE_ROSSCRIPTDIR/i386/lib/gcc/mingw32/4.1.3/include -I$_ROSBE_ROSSCRIPTDIR/i386/mingw32/include"
export ROSBE_TARGET_CXXFLAGS="-I$_ROSBE_ROSSCRIPTDIR/i386/include/c++/4.1.3 -I$_ROSBE_ROSSCRIPTDIR/i386/include/c++/4.1.3/mingw32 -I$_ROSBE_ROSSCRIPTDIR/i386/include/c++/4.1.3/backward -I$_ROSBE_ROSSCRIPTDIR/i386/lib/gcc/mingw32/4.1.3/include -I$_ROSBE_ROSSCRIPTDIR/i386/mingw32/include"
