#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/options.ps1
# PURPOSE:     Starts options.exe and restarts RosBE afterwards.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "Options"


if ("$ROS_ARCH" -eq "amd64") {
    $options="$_ROSBE_BASEDIR\Tools\options64.exe"
    $cfgfile="$ENV:APPDATA\RosBE\rosbe-options-amd64.cmd"
} else {
    $options="$_ROSBE_BASEDIR\Tools\options.exe"
    $cfgfile="$ENV:APPDATA\RosBE\rosbe-options.cmd"
}

#
# Run options.exe
#

if (Test-Path "$options") {
    Push-Location "$_ROSBE_BASEDIR"
    & "$options" | out-null
    Pop-Location
    if (Test-Path "$cfgfile") {
        & "$cfgfile"
    }
} else {
    "ERROR: options executable was not found."
}

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

$options = $null
$cfgfile = $null
