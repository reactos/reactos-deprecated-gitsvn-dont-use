#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/rosbe-gcc-env.ps1
# PURPOSE:     Set up the GCC 4.x.x build environment.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

# Check if we're switching to the AMD64 architecture
if ("$ENV:ROS_ARCH" -eq "amd64") {
    $ENV:ROS_PREFIX = "x86_64-pc-mingw32-"
} else {
    $ENV:ROS_PREFIX = $null
}

$ENV:PATH = "$_ROSBE_TARGET_MINGWPATH\bin;$_ROSBE_ORIGINALPATH"

& "$ENV:ROS_PREFIXgcc" -v 2> gcctvers.tmp
$global:_ROSBE_TARGET_GCCVERSION = (select-string -path .\gcctvers.tmp "gcc version") -replace ".*version ((\d|\.)+).*",'$1'
$global:_ROSBE_TARGET_GCCTARGET = (select-string -path .\gcctvers.tmp "target=") -replace ".*--target=(.+?)\b.*",'$1'
gcc -v 2> gcchvers.tmp
$global:_ROSBE_HOST_GCCVERSION = (select-string -path .\gcchvers.tmp "gcc version") -replace ".*version ((\d|\.)+).*",'$1'
$global:_ROSBE_HOST_GCCTARGET = (select-string -path .\gcchvers.tmp "target=") -replace ".*--target=(.+?)\b.*",'$1'
remove-item gcchvers.tmp

$ENV:ROSBE_HOST_CFLAGS = "-I""$_ROSBE_HOST_MINGWPATH\include"" -I""$_ROSBE_HOST_MINGWPATH\lib\gcc\$_ROSBE_HOST_GCCTARGET\$_ROSBE_HOST_GCCVERSION\include"""
$ENV:ROSBE_HOST_CXXFLAGS = "-I""$_ROSBE_HOST_MINGWPATH\include"" -I""$_ROSBE_HOST_MINGWPATH\include\c++\$_ROSBE_HOST_GCCVERSION"" -I""$_ROSBE_HOST_MINGWPATH\include\c++\$_ROSBE_HOST_GCCVERSION\$_ROSBE_HOST_GCCTARGET"" -I""$_ROSBE_HOST_MINGWPATH\lib\gcc\$_ROSBE_HOST_GCCTARGET\$_ROSBE_HOST_GCCVERSION\include"""
$ENV:ROSBE_TARGET_CFLAGS = "-I""$_ROSBE_TARGET_MINGWPATH\include"" -I""$_ROSBE_TARGET_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"""
$ENV:ROSBE_TARGET_CXXFLAGS = "-I""$_ROSBE_TARGET_MINGWPATH\include"" -I""$_ROSBE_TARGET_MINGWPATH\include\c++\$_ROSBE_GCCVERSION"" -I""$_ROSBE_TARGET_MINGWPATH\include\c++\$_ROSBE_GCCVERSION\$_ROSBE_GCCTARGET"" -I""$_ROSBE_TARGET_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"""
