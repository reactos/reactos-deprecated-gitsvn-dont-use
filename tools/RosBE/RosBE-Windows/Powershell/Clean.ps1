#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Clean.ps1
# PURPOSE:     Clean the ReactOS source directory.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "Cleaning..."

function remlog {
    # Check if we have any logs to clean, if so, clean them.
    if (Test-Path "$_ROSBE_LOGDIR") {
        "Cleaning build logs..."
        $null = (Remove-Item -path "$_ROSBE_LOGDIR\*.txt" -force)
        "Done cleaning build logs."
    } else {
        "ERROR: There are no logs to clean."
    }
}

function rembin {
    # Check if we have something to clean, if so, clean it.

    # Apply modified obj and out paths for deletion.

    if ($_ROSBE_OBJPATH -eq $null) {
        $OBJCLEANPATH = "$_ROSBE_ROSSOURCEDIR\obj-$ENV:ROS_ARCH"
    } else {
        $OBJCLEANPATH = "$_ROSBE_OBJPATH"
    }

    if ($_ROSBE_OUTPATH -eq $null) {
        $OUTCLEANPATH = "$_ROSBE_ROSSOURCEDIR\output-$ENV:ROS_ARCH"
    } else {
        $OUTCLEANPATH = "$_ROSBE_OUTPATH"
    }

    if ($ENV:ROS_ARCH -eq "i386") {
        $MAKEFILE = "$_ROSBE_ROSSOURCEDIR\makefile-$ENV:ROS_ARCH.auto"
    } else {
        $MAKEFILE = "$_ROSBE_ROSSOURCEDIR\makefile.auto"
    }

    if (Test-Path "$MAKEFILE") {
        $null = (Remove-Item "$MAKEFILE" -force)
    }

    if (Test-Path "$OBJCLEANPATH\.") {
        "Cleaning ReactOS $ENV:ROS_ARCH source directory..."
        if (Test-Path "$OBJCLEANPATH\.") {
            $null = (Remove-Item "$OBJCLEANPATH" -recurse -force)
        }
        if (Test-Path "$OUTCLEANPATH\.") {
            $null = (Remove-Item "$OUTCLEANPATH" -recurse -force)
        }
        "Done cleaning ReactOS $ENV:ROS_ARCH source directory."
    } else {
        "ERROR: There is no $ENV:ROS_ARCH compiler output to clean."
    }

    if (Test-Path "$_ROSBE_ROSSOURCEDIR\reactos") {
        $null = (Remove-Item "$_ROSBE_ROSSOURCEDIR\reactos" -recurse -force)
    }
}

function end {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
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
    remove-variable cl
    end
}
