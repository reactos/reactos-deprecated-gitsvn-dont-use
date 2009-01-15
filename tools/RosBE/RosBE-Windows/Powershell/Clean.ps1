#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Clean.ps1
# PURPOSE:     Clean the ReactOS source directory.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "Cleaning..."

function remlog {
    #
    # Check if we have any logs to clean, if so, clean them.
    #
    if (Test-Path "$_ROSBE_LOGDIR") {
        "Cleaning build logs..."
        $null = (Remove-Item -path "$_ROSBE_LOGDIR\*.txt" -force)
        "Done cleaning build logs."
    } else {
        "ERROR: There are no logs to clean."
    }
}

function rembin {
    #
    # Check if we have something to clean, if so, clean it.
    #

    #
    # Apply modified obj and out paths for deletion.
    #

    if ($_ROSBE_OBJPATH -eq $null) {
        if ($ENV:ROS_ARCH -ne $null) {
            $_ROSBE_OBJCLEANPATH = "obj-$ENV:ROS_ARCH"
        } else {
            $_ROSBE_OBJCLEANPATH = "obj-i386"
        }
    } else {
        $_ROSBE_OBJCLEANPATH = "$_ROSBE_OBJPATH"
    }

    if ($_ROSBE_OUTPATH -eq $null) {
        if ($ENV:ROS_ARCH -ne $null) {
            $_ROSBE_OUTCLEANPATH = "output-$ENV:ROS_ARCH"
        } else {
            $_ROSBE_OUTCLEANPATH = "output-i386"
        }
    } else {
        $_ROSBE_OUTCLEANPATH = "$_ROSBE_OUTPATH"
    }

    if ($ENV:ROS_ARCH -ne $null) {
        $_ROSBE_MAKEFILE = "makefile-$ENV:ROS_ARCH.auto"
    } else {
        $_ROSBE_MAKEFILE = "makefile.auto"
    }

    if ($ENV:ROS_ARCH -eq $null) {
        $ENV:ROS_ARCH = "i386"
    }

    if (Test-Path "$_ROSBE_OBJCLEANPATH\.") {
        "Cleaning ReactOS $ENV:ROS_ARCH source directory..."
        if (Test-Path "$_ROSBE_OBJCLEANPATH\.") {
            $null = (Remove-Item "$_ROSBE_OBJCLEANPATH" -recurse -force)
        }
        if (Test-Path "$_ROSBE_OUTCLEANPATH\.") {
            $null = (Remove-Item "$_ROSBE_OUTCLEANPATH" -recurse -force)
        }
        if (Test-Path "$_ROSBE_MAKEFILE") {
            $null = (Remove-Item "$_ROSBE_MAKEFILE" -force)
        }
        "Done cleaning ReactOS $ENV:ROS_ARCH source directory."
    } else {
        "ERROR: There is no $ENV:ROS_ARCH compiler output to clean."
    }

    if ($ENV:ROS_ARCH -eq "i386") {
        $ENV:ROS_ARCH = $null
    }

    if (Test-Path "reactos") {
        $null = (Remove-Item "reactos" -recurse -force)
    }
}

function end {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"

    #
    # Unload all used Vars.
    #
    $_ROSBE_OBJCLEANPATH = $null
    $_ROSBE_OUTCLEANPATH = $null
    $_ROSBE_MAKEFILE = $null

    exit
}

if ("$args" -eq "") {
    rembin
    end
}
elseif ("$args" -eq "logs") {
    remlog
    end
}
elseif ("$args" -eq "all") {
    rembin
    remlog
    end
}
elseif ("$args" -ne "") {
    $cl = "$args" + "_clean"
    make $cl
    $cl = $null
    end
}
