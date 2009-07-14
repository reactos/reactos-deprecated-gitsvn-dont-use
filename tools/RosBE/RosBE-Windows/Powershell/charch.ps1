#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/charch.ps1
# PURPOSE:     Tool to change the current Arch to build ROS for in RosBE.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

$host.ui.RawUI.WindowTitle = "Change the Architecture to build for..."

if ($args.count -eq 0) {
    # Parse the command line arguments.
    $ARCH = Read-Host "Please enter a Architecture you want to build ReactOS for: "
    if ($ARCH.length -eq 0) {
        "ERROR: You must enter a Architecture."
    }
} else {
    $ARCH = $args
}

# Modify ROS_ARCH for the current environment
$ENV:ROS_ARCH = $ARCH

# Refresh all needed Params by recalling the main Path setting CMD File.
if ($ENV:ROS_ARCH -eq "i386") {
    if (Test-Path "$ENV:APPDATA\RosBE\rosbe-options.ps1") {
        IEX "& '$ENV:APPDATA\RosBE\rosbe-options.ps1'"
    }
} else {
    if (Test-Path "$ENV:APPDATA\RosBE\rosbe-options-$ENV:ROS_ARCH.ps1") {
        IEX "& '$ENV:APPDATA\RosBE\rosbe-options-$ENV:ROS_ARCH.ps1'"
    }
}

chdefgcc $ENV:ROS_ARCH target

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
