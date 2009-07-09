#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/chdefdir.ps1
# PURPOSE:     Tool to change the current working ReactOS source directory
#              in RosBE.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "Change the current working ReactOS source directory..."

# Parse the command line arguments.
if ($args.count -eq 0) {
    $SOURCEDIR = Read-Host "Please enter a ReactOS source directory, or 'previous': "
    if ($SOURCEDIR.length -eq 0) {
        "ERROR: You must enter a ReactOS source directory, or 'previous'."
    }
} else {
    $SOURCEDIR = $args
}
if ($SOURCEDIR -eq "previous") {
    pop-location
} else {
    if (!(Test-Path "$SOURCEDIR\.")) {
        "ERROR: The path specified doesn't seem to exist."
    }
    push-location "$SOURCEDIR"
}

$global:_ROSBE_ROSSOURCEDIR = "$pwd"
$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
set-location "$_ROSBE_ROSSOURCEDIR"
