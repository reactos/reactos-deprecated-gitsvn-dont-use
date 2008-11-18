#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/chdefdir.cmd
# PURPOSE:     Tool to change the current working ReactOS source directory
#              in RosBE.
# COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
#                             Peter Ward <dralnix@gmail.com>
#
#

$host.ui.RawUI.WindowTitle = "Change the current working ReactOS source directory..."

#
# Parse the command line arguments.
#
if ($args.count -eq 0) {
    #
    # If Parameters were set, parse them, if not, ask the user to add them.
    #
    $_1 = Read-Host "Please enter a ReactOS source directory, or 'previous': "
    if ($_1.length -eq 0) {
        "ERROR: You must enter a ReactOS source directory, or 'previous'."
    }
} else {
    $_1 = $args
}
if ($_1 -eq "previous") {
    pop-location
} else {
    if (!(Test-Path "$_1\.")) {
        "ERROR: The path specified doesn't seem to exist."
        
    }
    push-location "$_1"
}
$global:_ROSBE_ROSSOURCEDIR = "$pwd"

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

#
# Unload all used Vars.
#
$_1 = $null
