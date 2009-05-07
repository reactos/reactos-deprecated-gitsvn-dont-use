#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/rosbe-gcc-env.ps1
# PURPOSE:     Set up the GCC 4.x.x build environment.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

#
# Check if we are running within the RosBE, and if not initialize GCC for the
# current directory.
#
if ($_ROSBE_HOST_MINGWPATH -eq $null) {
    $_ROSBE_MODE = "MinGW"
    $_ROSBE_HOST_MINGWPATH = "$pwd"
    $_ROSBE_ORIGINALPATH = "$ENV:PATH"
}


#
# Set the Arch Variables
#

$ENV:ROS_ARCH = $null
$ENV:ROS_PREFIX = $null
$_ROSBE_OBJPATH = "$_ROSBE_i386_OBJPATH"
$_ROSBE_OUTPATH = "$_ROSBE_i386_OUTPATH"


if ($_ROSBE_ARCH -ne $null) {
    $ENV:ROS_ARCH = $_ROSBE_ARCH
    $ENV:ROS_PREFIX = "$_ROSBE_ARCH-pc-mingw32"
    $_ROSBE_TARGET_MINGWPATH = "$_ROSBE_BASEDIR\$_ROSBE_ARCH"

    # HAXX
    if ($_ROSBE_ARCH -eq "arm") {
        $_ROSBE_OBJPATH = "$_ROSBE_ARM_OBJPATH"
        $_ROSBE_OUTPATH = "$_ROSBE_ARM_OUTPATH"
    }
    if ($_ROSBE_ARCH -eq "ppc") {
        $_ROSBE_OBJPATH = "$_ROSBE_PPC_OBJPATH"
        $_ROSBE_OUTPATH = "$_ROSBE_PPC_OUTPATH"
    }
    if ($_ROSBE_ARCH -eq "amd64") {
        $_ROSBE_OBJPATH = "$_ROSBE_AMD64_OBJPATH"
        $_ROSBE_OUTPATH = "$_ROSBE_AMD64_OUTPATH"
        $ENV:ROS_PREFIX = "x86_64-pc-mingw32"
        $_ROSBE_TARGET_MINGWPATH = "$_ROSBE_BASEDIR\x86_64"
    }

    # Check if existant arch

    if (Test-Path "$_ROSBE_TARGET_MINGWPATH\.") {   
    "Unsupported arch specified. Fallback to Default."
    pause
    $_ROSBE_OBJPATH = "$_ROSBE_i386_OBJPATH"
    $_ROSBE_OUTPATH = "$_ROSBE_i386_OUTPATH"
    $ENV:ROS_ARCH = $null
    $ENV:ROS_PREFIX = $null
    $_ROSBE_TARGET_MINGWPATH = $null
    }
}

#
# Set up the GCC 4.x.x build environment.
#
$ENV:PATH = "$_ROSBE_HOST_MINGWPATH\bin;$_ROSBE_TARGET_MINGWPATH\bin;$_ROSBE_ORIGINALPATH"
$global:_ROSBE_MINGWMAKE = "$_ROSBE_HOST_MINGWPATH\bin\mingw32-make.exe"

if ($ENV:ROS_ARCH -ne $null) {
    & "$ENV:ROS_PREFIX-gcc" -v 2> gcctvers.tmp
    $global:_ROSBE_GCCVERSION = (select-string -path .\gcctvers.tmp "gcc version") -replace ".*version ((\d|\.)+).*",'$1'
    $global:_ROSBE_GCCTARGET = (select-string -path .\gcctvers.tmp "target=") -replace ".*--target=(.+?)\b.*",'$1'
} else {
    gcc -v 2> gcctvers.tmp
    $global:_ROSBE_GCCVERSION = (select-string -path .\gcctvers.tmp "gcc version") -replace ".*version ((\d|\.)+).*",'$1'
    $global:_ROSBE_GCCTARGET = (select-string -path .\gcctvers.tmp "target=") -replace ".*--target=(.+?)\b.*",'$1'
}
gcc -v 2> gcchvers.tmp
$global:_ROSBE_HOST_GCCVERSION = (select-string -path .\gcchvers.tmp "gcc version") -replace ".*version ((\d|\.)+).*",'$1'
$global:_ROSBE_HOST_GCCTARGET = (select-string -path .\gcchvers.tmp "target=") -replace ".*--target=(.+?)\b.*",'$1'
remove-item gcchvers.tmp

if ($_ROSBE_MODE -eq "MinGW") {
    $ENV:C_INCLUDE_PATH = "$_ROSBE_HOST_MINGWPATH\include;$_ROSBE_HOST_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"
    $ENV:CPLUS_INCLUDE_PATH = "$_ROSBE_HOST_MINGWPATH\include;$_ROSBE_HOST_MINGWPATH\include\c++\$_ROSBE_GCCVERSION;$_ROSBE_HOST_MINGWPATH\include\c++\$_ROSBE_GCCVERSION\$_ROSBE_GCCTARGET;$_ROSBE_HOST_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"
    $ENV:ROSBE_HOST_CFLAGS = $null
    $ENV:ROSBE_HOST_CXXFLAGS = $null
    $ENV:ROSBE_TARGET_CFLAGS = $null
    $ENV:ROSBE_TARGET_CXXFLAGS = $null

} else {
    $ENV:C_INCLUDE_PATH = $null
    $ENV:CPLUS_INCLUDE_PATH = $null
    $ENV:ROSBE_HOST_CFLAGS = "-I""$_ROSBE_HOST_MINGWPATH\include"" -I""$_ROSBE_HOST_MINGWPATH\lib\gcc\$_ROSBE_HOST_GCCTARGET\$_ROSBE_HOST_GCCVERSION\include"""
    $ENV:ROSBE_HOST_CXXFLAGS = "-I""$_ROSBE_HOST_MINGWPATH\include"" -I""$_ROSBE_HOST_MINGWPATH\include\c++\$_ROSBE_HOST_GCCVERSION"" -I""$_ROSBE_HOST_MINGWPATH\include\c++\$_ROSBE_HOST_GCCVERSION\$_ROSBE_HOST_GCCTARGET"" -I""$_ROSBE_HOST_MINGWPATH\lib\gcc\$_ROSBE_HOST_GCCTARGET\$_ROSBE_HOST_GCCVERSION\include"""
    $ENV:ROSBE_TARGET_CFLAGS = "-I""$_ROSBE_TARGET_MINGWPATH\include"" -I""$_ROSBE_TARGET_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"""
    $ENV:ROSBE_TARGET_CXXFLAGS = "-I""$_ROSBE_TARGET_MINGWPATH\include"" -I""$_ROSBE_TARGET_MINGWPATH\include\c++\$_ROSBE_GCCVERSION"" -I""$_ROSBE_TARGET_MINGWPATH\include\c++\$_ROSBE_GCCVERSION\$_ROSBE_GCCTARGET"" -I""$_ROSBE_TARGET_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"""
}
if ($_ROSBE_HOST_GCCVERSION -eq "3.4.5") {
    $ENV:PATH = "$_ROSBE_BASEDIR\3.4.5\bin;$_ROSBE_BASEDIR\3.4.5\libexec\gcc\mingw32\3.4.5;$ENV:PATH"
    $_ROSBE_MINGWMAKE = "$_ROSBE_BASEDIR\3.4.5\bin\mingw32-make.exe"
    # Set all paths (needed by Windows Vista)
    $ENV:C_INCLUDE_PATH = "$_ROSBE_BASEDIR\3.4.5\include;$_ROSBE_BASEDIR\3.4.5\lib\gcc\mingw32\3.4.5\include"
    $ENV:CPLUS_INCLUDE_PATH = "$_ROSBE_BASEDIR\3.4.5\include;$_ROSBE_BASEDIR\3.4.5\include\c++\3.4.5;$_ROSBE_BASEDIR\3.4.5\include\c++\3.4.5\mingw32;$_ROSBE_BASEDIR\3.4.5\lib\gcc\mingw32\3.4.5\include"
    $ENV:LIBRARY_PATH = "$_ROSBE_BASEDIR\3.4.5\lib;$_ROSBE_BASEDIR\3.4.5\lib\gcc\mingw32\3.4.5"
    $ENV:ROSBE_HOST_CFLAGS = $null
    $ENV:ROSBE_HOST_CXXFLAGS = $null
    $ENV:ROSBE_TARGET_CFLAGS = $null
    $ENV:ROSBE_TARGET_CXXFLAGS = $null
}

$ENV:LIBRARY_PATH = "$_ROSBE_HOST_MINGWPATH\lib;$_ROSBE_HOST_MINGWPATH\lib\gcc\mingw32\$_ROSBE_GCCVERSION"
