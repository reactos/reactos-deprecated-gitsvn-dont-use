#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/rosbe-gcc-env.ps1
# PURPOSE:     Set up the GCC 4.x.x build environment.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

#
# Check if we are running within the RosBE, and if not initialize GCC for the
# current directory.
#
if ($ENV:_ROSBE_MINGWPATH -eq $null) {
    $ENV:_ROSBE_MODE = "MinGW"
    $ENV:_ROSBE_MINGWPATH = "$pwd"
    $ENV:_ROSBE_ORIGINALPATH = "$PATH"
}

#
# Set up the GCC 4.x.x build environment.
#
$ENV:PATH = "$_ROSBE_MINGWPATH\bin;$_ROSBE_ORIGINALPATH"
$global:_ROSBE_GCCVERSION = $null
$global:_ROSBE_GCCTARGET = $null
$global:_ROSBE_GCCVERSION = (& gcc -v 2>&1 | select-string "gcc version") -replace ".*version ((\d|\.)+).*",'$1'
$global:_ROSBE_GCCTARGET = (& gcc -v 2>&1 | select-string  "target=") -replace ".*--target=(.+?)\b.*",'$1'
$ENV:PATH = "$_ROSBE_MINGWPATH\bin;$_ROSBE_MINGWPATH\libexec\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION;$_ROSBE_ORIGINALPATH"
$global:_ROSBE_MINGWMAKE = "$_ROSBE_MINGWPATH\bin\mingw32-make.exe"
if ($_ROSBE_MODE -eq "MinGW") {
    $ENV:C_INCLUDE_PATH = "$_ROSBE_MINGWPATH\include;$_ROSBE_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"
    $ENV:CPLUS_INCLUDE_PATH = "$_ROSBE_MINGWPATH\include;$_ROSBE_MINGWPATH\include\c++\$_ROSBE_GCCVERSION;$_ROSBE_MINGWPATH\include\c++\$_ROSBE_GCCVERSION\$_ROSBE_GCCTARGET;$_ROSBE_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"
} else {
    $ENV:HOST_CFLAGS = "-I""$_ROSBE_MINGWPATH\include"" -I""$_ROSBE_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"""
    $ENV:HOST_CPPFLAGS = "-I""$_ROSBE_MINGWPATH\include"" -I""$_ROSBE_MINGWPATH\include\c++\$_ROSBE_GCCVERSION"" -I""$_ROSBE_MINGWPATH\include\c++\$_ROSBE_GCCVERSION\$_ROSBE_GCCTARGET"" -I""$_ROSBE_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION\include"""
}
$ENV:LIBRARY_PATH = "$_ROSBE_MINGWPATH\lib;$_ROSBE_MINGWPATH\lib\gcc\$_ROSBE_GCCTARGET\$_ROSBE_GCCVERSION"

#
# Display the current version of GCC, NASM, ld and make.
#
& gcc -v 2>&1 | select-string  "gcc version"
"gcc target - $_ROSBE_GCCTARGET"
& ld -v
if (Test-Path "$_ROSBE_MINGWPATH\bin\nasm.exe") {
    & nasm -v
} else {
    if ($_ROSBE_MODE -eq "RosBE") {
        "ERROR: NASM is required to build ReactOS, none found in the current MinGW/GCC."
    }
}
if (Test-Path "$_ROSBE_MINGWPATH\bin\bison.exe") {
    & bison --version | find "GNU Bison"
} else {
    if ($_ROSBE_MODE -eq "RosBE") {
        "WARNING: Bison will soon be required to build ReactOS, none found in the current MinGW/GCC."
    }
}
if (Test-Path "$_ROSBE_MINGWPATH\bin\flex.exe") {
    $fver = (& flex --version) -replace ".*version ((\d|\.)+).*",'$1'
    "flex $fver"
} else {
    if ($_ROSBE_MODE -eq "RosBE") {
        "WARNING: Flex will soon be required to build ReactOS, none found in the current MinGW/GCC."
    }
}
& mingw32-make -v | & find "GNU Make"
