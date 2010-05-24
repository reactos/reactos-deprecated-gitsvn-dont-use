#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/sSVN.ps1
# PURPOSE:     Integrated SVN Client.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
#

# Set branch to trunk if one is not already set
if ("$ENV:ROS_BRANCH" -eq "") {
    $_ROS_BRANCH = "trunk"
} else {
    $_ROS_BRANCH = "branches/$ENV:ROS_BRANCH"
}

# AMD64 architecture overrides branch that was previously set
if ("$ENV:ROS_ARCH" -eq "amd64") {
    $_ROS_BRANCH = "branches/ros-amd64-bringup"
}

$_ROSBE_SSVNSOURCEDIR = "$pwd"

function UP($arg) {
    $OFFSVN = IEX "& svn.exe info" | select-string "Last Changed Rev:"
    $OFFSVN = $OFFSVN -replace "(.*)Last Changed Rev: ",''
    $OFFSVN = [CONVERT]::ToInt32($OFFSVN,10)
    $ONSVN = IEX "& svn.exe info http://svn.reactos.org/reactos/$_ROS_BRANCH/reactos" | select-string "Last Changed Rev:"
    $ONSVN = $ONSVN -replace "(.*)Last Changed Rev: ",''
    $ONSVN = [CONVERT]::ToInt32($ONSVN,10)
    "Local Revision: $OFFSVN"
    "Online HEAD Revision: $ONSVN"
    ""
    if ($OFFSVN -lt $ONSVN) {
        if ("$_ROSBE_SSVN_JOB" -eq "status") {
            "Your tree is not up to date. Do you want to update it?"
            $UP = Read-Host "Please enter 'yes' or 'no': "
            if (("$UP" -eq "yes") -or ("$UP" -eq "y")) {
                $_ROSBE_SSVN_JOB = "update"
            }
        }
    }
    if ($OFFSVN -eq $ONSVN) {
        "Your tree is up to date."
        exit
    }
    if ("$_ROSBE_SSVN_JOB" -eq "update") {
        if ("$($arg[1])" -ne "") {
            $temparg = $arg[1]

            if ($temparg -eq $OFFSVN) {
                "Your Local Repository is currently $temparg"
            }
            if ($temparg -lt $OFFSVN) {
                "Downgrading to $temparg ..."
            }
            if ($temparg -gt $OFFSVN) {
                "Updating to $temparg ..."
            }
            if ("$_BUILDBOT_SVNSKIPMAINTRUNK" -ne "1") {
                IEX "& svn.exe update -r $temparg"
            } else {
                "Skipping ReactOS Trunk update."
            }
            if (Test-Path "modules\rosapps\.") {
                Set-Location "modules\rosapps"
                "Updating RosApps..."
                IEX "& svn.exe update -r $temparg"
                Set-Location "$_ROSBE_SSVNSOURCEDIR"
            }
            if (Test-Path "modules\rostests\.") {
                Set-Location "modules\rostests"
                "Updating RosTests..."
                IEX "& svn.exe update -r $temparg"
                Set-Location "$_ROSBE_SSVNSOURCEDIR"
            }
        } else {
            if ("$_BUILDBOT_SVNSKIPMAINTRUNK" -ne "1") {
                IEX "& svn.exe update"
            } else {
                "Skipping ReactOS Trunk update."
            }
            if (Test-Path "modules\rosapps\.") {
                Set-Location "modules\rosapps"
                "Updating RosApps..."
                IEX "& svn.exe update"
                Set-Location "$_ROSBE_SSVNSOURCEDIR"
            }
            if (Test-Path "modules\rostests\.") {
                Set-Location "modules\rostests"
                "Updating RosTests..."
                IEX "& svn.exe update"
                Set-Location "$_ROSBE_SSVNSOURCEDIR"
            }
        }
        "Do you want to see the changelog?"
        $CL = Read-Host "Please enter 'yes' or 'no': "
        if (("$CL" -eq "yes") -or ("$CL" -eq "y")) {
            $range = "$OFFSVN" + ":" + "$ONSVN"
            IEX "& svn.exe log -r $range"
        }
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
    IEX "& svn.exe cleanup"
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
            if ("$($args[1])" -ne "") {
                IEX "& svn.exe -r $($args[1]) checkout http://svn.reactos.org/reactos/$_ROS_BRANCH/reactos ."
            } else {
                IEX "& svn.exe checkout http://svn.reactos.org/reactos/$_ROS_BRANCH/reactos ."
            }
        } else {
            "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
        }
    }
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rosapps") {
    if ("$($args[1])" -ne "") {
        if (!(Test-Path "modules\rosapps\.")) {
            new-item -path "modules" -name rosapps -type directory
        }
        if (Test-Path "modules\rosapps\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Updating..."
            Set-Location "modules\rosapps"
            IEX "& svn.exe update -r $($args[1])"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Creating..."
            Set-Location "modules\rosapps"
            $dir = get-childitem
            if ("$dir" -eq "") {
                IEX "& svn.exe checkout -r $($args[1]) http://svn.reactos.org/reactos/$_ROS_BRANCH/rosapps ."
            } else {
                "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    } else {
        if (!(Test-Path "modules\rosapps\.")) {
            new-item -path "modules" -name rosapps -type directory
        }
        if (Test-Path "modules\rosapps\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Updating..."
            Set-Location "modules\rosapps"
            IEX "& svn.exe update"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Creating..."
            Set-Location "modules\rosapps"
            $dir = get-childitem
            if ("$dir" -eq "") {
                IEX "& svn.exe checkout http://svn.reactos.org/reactos/$_ROS_BRANCH/rosapps ."
            } else {
                "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    }
    Set-Location "$_ROSBE_SSVNSOURCEDIR"
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rostests") {
    if ("$($args[1])" -ne "") {
        if (!(Test-Path "modules\rostests\.")) {
            new-item -path "modules" -name rostests -type directory
        }
        if (Test-Path "modules\rostests\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Updating..."
            Set-Location "modules\rostests"
            IEX "& svn.exe update -r $($args[1])"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Creating..."
            Set-Location "modules\rostests"
            $dir = get-childitem
            if ("$dir" -eq "") {
                IEX "& svn.exe checkout -r $($args[1]) http://svn.reactos.org/reactos/$_ROS_BRANCH/rostests ."
            } else {
                "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    } else {
        if (!(Test-Path "modules\rostests\.")) {
            new-item -path "modules" -name rostests -type directory
        }
        if (Test-Path "modules\rostests\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Updating..."
            Set-Location "modules\rostests"
            IEX "& svn.exe update"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Creating..."
            Set-Location "modules\rostests"
            $dir = get-childitem
            if ("$dir" -eq "") {
                IEX "& svn.exe checkout http://svn.reactos.org/reactos/$_ROS_BRANCH/rostests ."
            } else {
                "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    }
    Set-Location "$_ROSBE_SSVNSOURCEDIR"
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
