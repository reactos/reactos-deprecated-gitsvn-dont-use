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
    if (Test-Path "obj-i386") {
        "Cleaning ReactOS source directory..."
        #
        # Remove directories/makefile.auto created by the build.
        #
        if (Test-Path "obj-i386") {
            $null = (Remove-Item "obj-i386" -recurse -force)
        }
        if (Test-Path "output-i386") {
            $null = (Remove-Item "output-i386" -recurse -force)
        }
        if (Test-Path "reactos") {
            $null = (Remove-Item "reactos" -recurse -force)
        }
        if (Test-Path "makefile.auto") {
            $null = (Remove-Item "makefile.auto" -force)
        }
        "Done cleaning ReactOS source directory."
    } else {
        "ERROR: There is no compiler output to clean."
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
if ("$args" -eq "logs") {
    remlog
    end
}
if ("$args" -eq "all") {
    rembin
    remlog
    end
}
if ("$args" -ne "") {
    "Unknown parameter specified. Try ''help [COMMAND]''."
    end
}
