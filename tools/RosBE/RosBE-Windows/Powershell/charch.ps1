#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/charch.ps1
# PURPOSE:     Tool to change the current Arch to build ROS for in RosBE.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#

$host.ui.RawUI.WindowTitle = "Change the Architecture to build for..."

#
# Parse the command line arguments.
# ROSBE_ARCH: Default is i386, can be set to amd64, ppc or arm.
#

#
# Refresh all needed Params by recalling the main Path setting CMD File.
#
function SYSPARAM {
    IEX "& '$_ROSBE_BASEDIR\rosbe-gcc-env.ps1'"
}
if ($args.count -eq 0) {
    #
    # If Parameters were set, parse them, if not, ask the user to add them.
    #
    $_1 = Read-Host "Please enter a Architecture you want to build ReactOS for: "
    if ($_1.length -eq 0) {
        "ERROR: You must enter a Architecture."
    }
} else {
    $_1 = $args
}
if ($_1 -eq "i386") {
    $_ROSBE_ARCH = $null
} else {
    $_ROSBE_ARCH = $_1
}
SYSPARAM

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

#
# Unload all used Vars.
#
$_1 = $null
