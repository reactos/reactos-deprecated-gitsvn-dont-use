#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Clean.ps1
# PURPOSE:     Clean the ReactOS source directory.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
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
    # Check if we have any binaries to clean, if so, clean them.

    # Check if the user set any custom filenames or pathes, otherwise locally set the appropriate variables.

    if ("$ENV:ROS_AUTOMAKE" -eq "") {
        $ROS_AUTOMAKE = "makefile-$ENV:ROS_ARCH.auto"
    } else {
        $ROS_AUTOMAKE = $ENV:ROS_AUTOMAKE
    }
    if ("$ENV:ROS_INTERMEDIATE" -eq "") {
        $ROS_INTERMEDIATE = "obj-$ENV:ROS_ARCH"
    } else {
        $ROS_INTERMEDIATE = $ENV:ROS_INTERMEDIATE
    }
    if ("$ENV:ROS_OUTPUT" -eq "") {
        $ROS_OUTPUT = "output-$ENV:ROS_ARCH"
    } else {
        $ROS_OUTPUT = $ENV:ROS_OUTPUT
    }
    if ("$ENV:ROS_CDOUTPUT" -eq "") {
        $ROS_CDOUTPUT = "reactos"
    } else {
        $ROS_CDOUTPUT = $ENV:ROS_CDOUTPUT
    }

    if ((Test-Path "$ROS_INTERMEDIATE\.") -and (Test-Path "$ROS_OUTPUT\.")) {
        "Cleaning ReactOS $ENV:ROS_ARCH source directory..."

            $null = (Remove-Item "$ROS_AUTOMAKE" -force)
            $null = (Remove-Item "$ROS_INTERMEDIATE" -recurse -force)
            $null = (Remove-Item "$ROS_OUTPUT" -recurse -force)
            $null = (Remove-Item "$ROS_CDOUTPUT" -recurse -force)

        "Done cleaning ReactOS $ENV:ROS_ARCH source directory."
    } else {
        "ERROR: This directory contains no $ENV:ROS_ARCH compiler output to clean."
    }
}

function end {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
    exit
}

if ("$args" -eq "") {
    rembin
}
elseif ("$args" -eq "logs") {
    remlog
}
elseif ("$args" -eq "all") {
    rembin
    remlog
}
elseif ("$args" -ne "") {
    $argindex = 0
    while ( "$($args[$argindex])" -ne "") {
        $cl = "$($args[$argindex])" + "_clean"
        make $cl
        $argindex += 1
    }
    remove-variable cl
}
end
