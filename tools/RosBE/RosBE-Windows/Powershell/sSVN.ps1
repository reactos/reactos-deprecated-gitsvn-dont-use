#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/sSVN.ps1
# PURPOSE:     Integrated SVN Client.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
#

function UP($arg) {
    $OFFSVN = IEX "& svn.exe info" | select-string "Revision:"
    $OFFSVN = $OFFSVN -replace "(.*)Revision: ",''
    $OFFSVN = [CONVERT]::ToInt32($OFFSVN,10)
    if ("$ENV:ROS_ARCH" -eq "amd64") {
        $ONSVN = IEX "& svn.exe info http://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos" | select-string "Revision:"
    } else {
        $ONSVN = IEX "& svn.exe info http://svn.reactos.org/reactos/trunk/reactos" | select-string "Revision:"
    }
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
                if ("$_BUILDBOT_SVNSKIPMAINTRUNK" -ne "1") {
                    IEX "& svn.exe update -r $temparg"
                }
                if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rosapps\.") {
                    Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rosapps"
                    "Updating RosApps..."
                    IEX "& svn.exe update -r $temparg"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
                if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rostests\.") {
                    Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rostests"
                    "Updating RosTests..."
                    IEX "& svn.exe update -r $temparg"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
            } else {
                if ("$_BUILDBOT_SVNSKIPMAINTRUNK" -ne "1") {
                    IEX "& svn.exe update"
                }
                if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rosapps\.") {
                    Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rosapps"
                    "Updating RosApps..."
                    IEX "& svn.exe update"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
                }
                if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rostests\.") {
                    Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rostests"
                    "Updating RosTests..."
                    IEX "& svn.exe update"
                    Set-Location "$_ROSBE_ROSSOURCEDIR"
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
                if ("$ENV:ROS_ARCH" -eq "amd64") {
                    IEX "& svn.exe -r $($args[1]) checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos ."
                } else {
                    IEX "& svn.exe -r $($args[1]) checkout http://svn.reactos.org/reactos/trunk/reactos ."
                }
            } else {
                if ("$ENV:ROS_ARCH" -eq "amd64") {
                    IEX "& svn.exe checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos ."
                } else {
                    IEX "& svn.exe checkout http://svn.reactos.org/reactos/trunk/reactos ."
                }
            }
        } else {
            "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
        }
    }
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rosapps") {
    if ("$($args[1])" -ne "") {
        if (!(Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rosapps\.")) {
            new-item -path "$_ROSBE_ROSSOURCEDIR\modules" -name rosapps -type directory
        }
        if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rosapps\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Updating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rosapps"
            IEX "& svn.exe update -r $($args[1])"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Creating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rosapps"
            $dir = get-childitem
            if ("$dir" -eq "") {
                if("$ENV:ROS_ARCH" -eq "amd64") {
                    IEX "& svn.exe checkout -r $($args[1]) http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rosapps ."
                } else {
                    IEX "& svn.exe checkout -r $($args[1]) http://svn.reactos.org/reactos/trunk/rosapps ."
                }
            } else {
                 "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    } else {
        if (!(Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rosapps\.")) {
            new-item -path "$_ROSBE_ROSSOURCEDIR\modules" -name rosapps -type directory
        }
        if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rosapps\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Updating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rosapps"
            IEX "& svn.exe update"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosApps Creating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rosapps"
            $dir = get-childitem
            if ("$dir" -eq "") {
                if ("$ENV:ROS_ARCH" -eq "amd64") {
                    IEX "& svn.exe checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rosapps ."
                } else {
                    IEX "& svn.exe checkout http://svn.reactos.org/reactos/trunk/rosapps ."
                }
            } else {
                 "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    }
    Set-Location "$_ROSBE_ROSSOURCEDIR"
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rostests") {
    if ("$($args[1])" -ne "") {
        if (!(Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rostests\.")) {
            new-item -path "$_ROSBE_ROSSOURCEDIR\modules" -name rostests -type directory
        }
        if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rostests\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Updating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rostests"
            IEX "& svn.exe update -r $($args[1])"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Creating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rostests"
            $dir = get-childitem
            if ("$dir" -eq "") {
                if("$ENV:ROS_ARCH" -eq "amd64") {
                    IEX "& svn.exe checkout -r $($args[1]) http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rostests ."
                } else {
                    IEX "& svn.exe checkout -r $($args[1]) http://svn.reactos.org/reactos/trunk/rostests ."
                }
            } else {
                 "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
        }
    } else {
        if (!(Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rostests\.")) {
            new-item -path "$_ROSBE_ROSSOURCEDIR\modules" -name rostests -type directory
        }
        if (Test-Path "$_ROSBE_ROSSOURCEDIR\modules\rostests\.svn\.") {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Updating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rostests"
            IEX "& svn.exe update"
        } else {
            $host.ui.RawUI.WindowTitle = "SVN RosTests Creating..."
            Set-Location "$_ROSBE_ROSSOURCEDIR\modules\rostests"
            $dir = get-childitem
            if ("$dir" -eq "") {
                if ("$ENV:ROS_ARCH" -eq "amd64") {
                    IEX "& svn.exe checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rostests ."
                } else {
                    IEX "& svn.exe checkout http://svn.reactos.org/reactos/trunk/rostests ."
                }
            } else {
                 "ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"
            }
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
