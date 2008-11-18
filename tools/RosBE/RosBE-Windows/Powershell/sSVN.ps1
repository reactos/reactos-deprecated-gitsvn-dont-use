#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/sSVN.cmd
# PURPOSE:     Integrated SVN Client.
# COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
#
#

function UP {
    "Local $OFFSVN"
    "Online HEAD $ONSVN"
    ""
    if ($OFFSVN -lt $ONSVN) {
        "Your tree is not up to date. Do you want to update it?"
        $UP = Read-Host "Please enter 'yes' or 'no': "
        if ($UP -eq "yes") {
            IEX "&'$_ROSBE_BASEDIR\ssvn' update"
        }
    }
    if ($OFFSVN -eq $ONSVN) {
        "Your tree is up to date."
    }
}

#
# Receive the first parameter and decide what to do.
#
if ($args[0] -eq $null) {
    "No parameter specified. Try 'help [COMMAND]'."
}

#
# These two are directly parsed to svn.
#
elseif ($args[0] -eq "update") {
    $host.ui.RawUI.WindowTitle = "SVN Updating..."
    "This might take a while, so please be patient."
    ""
    if ($args[1] -ne $null) {
        $temparg = $args[1]
        IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update -r $temparg"
    } else {
        IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update"
    }
}
elseif ($args[0] -eq "cleanup") {
    $host.ui.RawUI.WindowTitle = "SVN Cleaning..."
    "This might take a while, so please be patient."
    ""
    IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' cleanup"
}

#
# Check if the folder is empty. If not, output an error.
#
elseif ($args[0] -eq "create") {
    $host.ui.RawUI.WindowTitle = "SVN Creating..."
    if (Test-Path ".svn\.") {
        "ERROR: Folder already contains a repository."
    } else {
        $dir = get-childitem
        if ($dir -eq $null) {
            IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' checkout svn://svn.reactos.org/reactos/trunk/reactos ."
        } else {
            "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
        }
    }
}

#
# Output the revision of the local and online trees and tell the user if
# its up to date or not.
#
elseif ($args[0] -eq "status") {
    $host.ui.RawUI.WindowTitle = "SVN Status"
    "This might take a while, so please be patient."
    ""
    $OFFSVN = IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' info" | select-string "Revision:"
    $ONSVN = IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' info svn://svn.reactos.org/reactos/trunk/reactos" | select-string "Revision:"
    UP
}

elseif ($args[0] -ne $null) {
    "Unknown parameter specified. Try 'help ssvn'."
}

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

#
# Unload all used Vars.
#
$OFFSVN = $null
$ONSVN = $null
$UP = $null
$dir = dir
$temparg = $null
