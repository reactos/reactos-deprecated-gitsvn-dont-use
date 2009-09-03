#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/sSVN.ps1
# PURPOSE:     Integrated SVN Client.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

function UP($arg) {
    $OFFSVN = IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' info" | select-string "Revision:"
    $OFFSVN = $OFFSVN -replace "(.*)Revision: ",''
    $OFFSVN = [CONVERT]::ToInt32($OFFSVN,10)
    $ONSVN = IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' info svn://svn.reactos.org/reactos/trunk/reactos" | select-string "Revision:"
    $ONSVN = $ONSVN -replace "(.*)Revision: ",''
    $ONSVN = [CONVERT]::ToInt32($ONSVN,10)
    "Local Revision: $OFFSVN"
    "Online HEAD Revision: $ONSVN"
    ""
    if (($OFFSVN -lt $ONSVN) -or ("$($arg[1])" -ne "")) {
        if ("$_ROSBE_SSVN_JOB" -eq "status") {
            "Your tree is not up to date. Do you want to update it?"
            $UP = Read-Host "Please enter 'yes' or 'no': "
            if (("$UP" -eq "yes") -or ("$UP" -eq "y")) {
                $_ROSBE_SSVN_JOB = "update"
            }
        }
        if ("$_ROSBE_SSVN_JOB" -eq "update") {
            if ("$($arg[1])" -ne "") {
                $temparg = $arg[1]
                IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update -r $temparg"
                if (Test-Path "modules\rosapps\.") {
                    Set-Location modules\rosapps
                    IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update -r $temparg"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
                if (Test-Path "modules\rostests\.") {
                    Set-Location modules\rostests
                    IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update -r $temparg"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
            } else {
                IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update"
                if (Test-Path "modules\rosapps\.") {
                    Set-Location modules\rosapps
                    IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
                if (Test-Path "modules\rostests\.") {
                    Set-Location modules\rostests
                    IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' update"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
            }
            "Do you want to see the changelog?"
            $CL = Read-Host "Please enter 'yes' or 'no': "
            if (("$CL" -eq "yes") -or ("$CL" -eq "y")) {
                $range = "$OFFSVN" + ":" + "$ONSVN"
                IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' log -r $range"
            }
        }
    }
    if ($OFFSVN -eq $ONSVN) {
        "Your tree is up to date."
    }
}

# Receive the first parameter and decide what to do.
if ("$($args[0])" -eq "") {
    "No parameter specified. Try 'help [COMMAND]'."
}

# These two are directly parsed to svn.
elseif ("$($args[0])" -eq "update") {
    $host.ui.RawUI.WindowTitle = "SVN Updating..."
    "This might take a while, so please be patient."
    ""
    $_ROSBE_SSVN_JOB = "update"
    UP($args)
}
elseif ("$($args[0])" -eq "cleanup") {
    $host.ui.RawUI.WindowTitle = "SVN Cleaning..."
    "This might take a while, so please be patient."
    ""
    IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' cleanup"
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "create") {
    $host.ui.RawUI.WindowTitle = "SVN Creating..."
    if (Test-Path ".svn\.") {
        "ERROR: Folder already contains a repository."
    } else {
        $null = (Remove-Item "$_ROSBE_LOGDIR" -recurse -force)
        $dir = get-childitem
        if ("$dir" -eq "") {
            IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' checkout svn://svn.reactos.org/reactos/trunk/reactos ."
        } else {
            "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
        }
    }
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rosapps") {
    $host.ui.RawUI.WindowTitle = "SVN RosApps Creating..."
    if (Test-Path "modules\rosapps\.svn\.") {
        "ERROR: Folder already contains a RosApps repository."
    } else {
        if (!(Test-Path "modules\rosapps\.")) {
            new-item -path "$_ROSBE_ROSSOURCEDIR\modules" -name rosapps -type directory
        }
        Set-Location modules\rosapps
        $dir = get-childitem
        if ("$dir" -eq "") {
            IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' checkout svn://svn.reactos.org/reactos/trunk/rosapps ."
        } else {
            "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
        }
    }
    Set-Location "$_ROSBE_ROSSOURCEDIR"
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rostests") {
    $host.ui.RawUI.WindowTitle = "SVN RosTests Creating..."
    if (Test-Path "modules\rostests\.svn\.") {
        "ERROR: Folder already contains a RosTests repository."
    } else {
        if (!(Test-Path "modules\rostests\.")) {
            new-item -path "$_ROSBE_ROSSOURCEDIR\modules" -name rostests -type directory
        }
        Set-Location modules\rostests
        $dir = get-childitem
        if ("$dir" -eq "") {
            IEX "&'$_ROSBE_BASEDIR\Tools\svn.exe' checkout svn://svn.reactos.org/reactos/trunk/rostests ."
        } else {
            "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
        }
    }
    Set-Location "$_ROSBE_ROSSOURCEDIR"
}

# Output the revision of the local and online trees and tell the user if
# its up to date or not.
elseif ("$($args[0])" -eq "status") {
    $host.ui.RawUI.WindowTitle = "SVN Status"
    $_ROSBE_SSVN_JOB = "status"
    UP($args)
}

elseif ("$($args[0])" -ne "") {
    "Unknown parameter specified. Try 'help ssvn'."
}

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
