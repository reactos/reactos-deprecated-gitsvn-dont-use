#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/options.cmd
# PURPOSE:     Starts options.exe and restarts RosBE afterwards.
# COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "Options"

#
# Run options.exe
#
if (Test-Path "$_ROSBE_BASEDIR\Tools\options.exe") {
    & "$_ROSBE_BASEDIR\Tools\options.exe" | out-null
    if (Test-Path "$ENV:APPDATA\RosBE\rosbe-options.cmd") {
        & "$ENV:APPDATA\RosBE\rosbe-options.cmd"
    }
} else {
    "ERROR: options.exe was not found."
}

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}
