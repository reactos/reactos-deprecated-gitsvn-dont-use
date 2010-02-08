#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/rosbe-gcc-env.ps1
# PURPOSE:     Set up the GCC 4.x.x build environment.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
#

# Check if we're switching to the AMD64 or AMR architecture.
if ("$ENV:ROS_ARCH" -eq "amd64") {
    $ENV:ROS_PREFIX = "x86_64-w64-mingw32"
} elseif ("$ENV:ROS_ARCH" -eq "arm") {
    $ENV:ROS_PREFIX = "arm-mingw32ce"
} else {
    $ENV:ROS_PREFIX = $null
}

if ("$ENV:ROS_PREFIX") {
    $global:_ROSBE_PREFIX = "$ENV:ROS_PREFIX" + "-"
} else {
    $global:_ROSBE_PREFIX = $null
}
 
$ENV:PATH = "$_ROSBE_HOST_MINGWPATH\bin;$_ROSBE_TARGET_MINGWPATH\bin;$_ROSBE_ORIGINALPATH"

$ENV:ROSBE_HOST_CFLAGS = "" | cpp.exe -x c -v 2>&1 | getincludes
$ENV:ROSBE_HOST_CXXFLAGS = "" | cpp.exe -x c++ -v 2>&1 | getincludes
$TARGETGCC = "$_ROSBE_PREFIX" + "cpp.exe"
$ENV:ROSBE_TARGET_CFLAGS = "" | &$TARGETGCC -x c -v 2>&1 | getincludes
$ENV:ROSBE_TARGET_CXXFLAGS = "" | &$TARGETGCC -x c++ -v 2>&1 | getincludes
