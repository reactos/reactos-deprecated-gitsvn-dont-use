#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/options.ps1
# PURPOSE:     Starts options.exe and restarts RosBE afterwards.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

$host.ui.RawUI.WindowTitle = "Options"

if ("$ENV:ROS_ARCH" -ne "i386") {
    $param = "$ENV:ROS_ARCH"
    $cfgfile ="$ENV:APPDATA\RosBE\rosbe-options-$ENV:ROS_ARCH.ps1"
} else {
    $param = $null
    $cfgfile="$ENV:APPDATA\RosBE\rosbe-options.ps1"
}

# Run options.exe

if (Test-Path "$_ROSBE_BASEDIR\Tools\options.exe") {
    Push-Location "$_ROSBE_BASEDIR"
    &options.exe $param | out-null
    Pop-Location
    if (Test-Path "$cfgfile") {
        & "$cfgfile"
    }
} else {
    "ERROR: options executable was not found."
}

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
