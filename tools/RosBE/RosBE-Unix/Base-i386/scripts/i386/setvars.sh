# Set architecture-specific environment variables
# Part of RosBE for Unix-based Operating Systems
# Copyright 2009-2010 Colin Finck <colin@reactos.org>
#
# Released under GNU GPL v2 or any later version.

export ROSBE_TARGET_CFLAGS=`mingw32-cpp -x c -v < /dev/null 2>&1 | $_ROSBE_ROSSCRIPTDIR/bin/getincludes`
export ROSBE_TARGET_CXXFLAGS=`mingw32-cpp -x c++ -v < /dev/null 2>&1 | $_ROSBE_ROSSCRIPTDIR/bin/getincludes`
