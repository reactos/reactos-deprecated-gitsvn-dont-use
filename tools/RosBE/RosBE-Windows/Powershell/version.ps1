#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/version.ps1
# PURPOSE:     Display the current version of GCC, NASM, ld and make.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

(get-WmiObject Win32_OperatingSystem).caption

(select-string -path .\gcctvers.tmp "gcc version") -replace ".*: (.+?)\b",'$1' | % { $str = $_ -replace '^.*14:',''}
$str
"gcc target - $_ROSBE_GCCTARGET"
remove-item gcctvers.tmp

if ($ENV:ROS_ARCH -ne $null) {
    & "$_ROSBE_TARGET_MINGWPATH\bin\$ENV:ROS_PREFIX-ld" -v
} else {
    & ld -v
}
if (Test-Path "$_ROSBE_HOST_MINGWPATH\bin\nasm.exe") {
    & nasm -v
} else {
    if (Test-Path "$_ROSBE_HOST_MINGWPATH\bin\yasm.exe") {
        & yasm --version | select-string "yasm 0"
    } else {
        if ($_ROSBE_MODE -eq "RosBE") {
            "ERROR: NASM or YASM is required to build ReactOS, none was found in the current MinGW/GCC."
        }
    }
}
if (Test-Path "$_ROSBE_HOST_MINGWPATH\bin\bison.exe") {
    & bison '--version' | select-string "GNU Bison"
} else {
    if ($_ROSBE_MODE -eq "RosBE") {
        "WARNING: Bison will soon be required to build ReactOS, none was found in the current MinGW/GCC."
    }
}
if (Test-Path "$_ROSBE_HOST_MINGWPATH\bin\flex.exe") {
    $fver = (& flex '--version') -replace ".*version ((\d|\.)+).*",'$1'
    "flex $fver"
} else {
    if ($_ROSBE_MODE -eq "RosBE") {
        "WARNING: Flex will soon be required to build ReactOS, none was found in the current MinGW/GCC."
    }
}
& mingw32-make -v | & find "GNU Make"
