#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/sSVN.ps1
# PURPOSE:     Integrated SVN Client.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
#

$ROS_SVNURL = "http://svn.reactos.org/reactos"

if ("$ENV:ROS_ARCH" -eq "amd64") {
    $ROS_SVNURL = "$ROS_SVNURL/branches/ros-amd64-bringup"
} else {
    if ("$ENV:ROS_BRANCH" -eq "") {
        $ROS_SVNURL = "$ROS_SVNURL/trunk"
    } else {
        $ROS_SVNURL = "$ROS_SVNURL/branches/$ENV:ROS_BRANCH"
    }
}
"$ROS_SVNURL"
get-webfile $ROS_SVNURL/reactos "$ENV:TEMP\tmp"
if ("$_ROSBE_DWERRLVL" -eq "1") {
    get-webfile $ROS_SVNURL "$ENV:TEMP\tmp2"
    if ("$_ROSBE_DWERRLVL" -eq "1") {
        throw {"ERROR: The selected branch does not exist or the Internet Connection is down."}
        exit
    } else {
        $rsubfolder = ""
        $disapptest = 1
    }
} else {
    $rsubfolder = "/reactos"
    $disapptest = 0
}

$_ROSBE_SSVNSOURCEDIR = "$pwd"

function UP($arg) {
    $OFFSVN = IEX "& svn.exe info" | select-string "Last Changed Rev:"
    $OFFSVN = $OFFSVN -replace "(.*)Last Changed Rev: ",''
    $OFFSVN = [CONVERT]::ToInt32($OFFSVN,10)
    $ONSVN = IEX "& svn.exe info $ROS_SVNURL$rsubfolder" | select-string "Last Changed Rev:"
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
        throw {"ERROR: Folder already contains a repository."}
    } else {
        $null = (Remove-Item "$_ROSBE_LOGDIR" -recurse -force)
        $dir = get-childitem
        if ("$dir" -eq "") {
            if ("$($args[1])" -ne "") {
                IEX "& svn.exe -r $($args[1]) checkout $ROS_SVNURL$rsubfolder ."
            } else {
                IEX "& svn.exe checkout $ROS_SVNURL$rsubfolder ."
            }
        } else {
            throw {"ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"}
        }
    }
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rosapps") {
    if ("$disapptest" -eq "0") {
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
                    IEX "& svn.exe checkout -r $($args[1]) $ROS_SVNURL/rosapps ."
                } else {
                    throw {"ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"}
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
                    IEX "& svn.exe checkout $ROS_SVNURL/rosapps ."
                } else {
                    throw {"ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"}
                }
            }
        }
        Set-Location "$_ROSBE_SSVNSOURCEDIR"
    } else {
        "Rosapps and Rostests disabled."
    }
}

# Check if the folder is empty. If not, output an error.
elseif ("$($args[0])" -eq "rostests") {
    if ("$disapptest" -eq "0") {
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
                    IEX "& svn.exe checkout -r $($args[1]) $ROS_SVNURL/rostests ."
                } else {
                    throw {"ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"}
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
                    IEX "& svn.exe checkout $ROS_SVNURL/rostests ."
                } else {
                    throw {"ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED"}
                }
            }
        }
        Set-Location "$_ROSBE_SSVNSOURCEDIR"
    } else {
        "Rosapps and Rostests disabled."
    }
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
