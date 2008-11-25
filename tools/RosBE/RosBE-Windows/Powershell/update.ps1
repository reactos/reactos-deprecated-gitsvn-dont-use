#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/update.ps1
# PURPOSE:     RosBE Updater.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

#
# Set Title
#
$host.ui.RawUI.WindowTitle = "Updating..."

function EOC {
    set-location "$_ROSBE_OPATH"
    if ($_ROSBE_VERSION -ne $null) {
        $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
    }
    exit
}

function UPDCHECK {
    if (Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt") {
        break
    }
    if (!(Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt")) {
        IEX "'$_ROSBE_BASEDIR\Tools\wget.exe' -N --ignore-length --no-verbose $_ROSBE_URL/$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt"
    }
    if (Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt") {
        get-Content "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt"
        ""
        "Install?"
        $YESNO = Read-Host "(yes), (no)"
        if ($YESNO -eq "yes") {
            if (!(Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.7z")) {
                IEX "&'$_ROSBE_BASEDIR\Tools\wget.exe' -N --ignore-length --no-verbose $_ROSBE_URL/$_ROSBE_VERSION-$_ROSBE_STATCOUNT.7z"
            }
            if (Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.7z") {
                remove-item "$_ROSBE_VERSION-$_ROSBE_STATCOUNT\*.*" -force
                IEX "&'$_ROSBE_BASEDIR\Tools\7z.exe' x '$_ROSBE_VERSION-$_ROSBE_STATCOUNT.7z'"
                set-location "$_ROSBE_VERSION-$_ROSBE_STATCOUNT"
                "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.ps1"
                break
            } else {
                "ERROR: This Update does not seem to exist or the Internet connection is not working correctly."
                break
            }
        } elseif ($YESNO -eq "no") {
            "Do you want to be asked again to install this update?"
            $YESNO = Read-Host "(yes), (no)"
            if ($YESNO -eq "yes") {
                remove-item "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt" -force
            }
            break
        }
    } else {
        if ($_ROSBE_MULTIUPD -ne "1") {
            "ERROR: This Update does not seem to exist or the Internet connection is not working correctly."
            break
        }
    }
}

function STATUS {
    set-location tmp
    if (!(Test-path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt")) {
        IEX "&'$_ROSBE_BASEDIR\Tools\wget.exe' -N --ignore-length --no-verbose $_ROSBE_URL/$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt"
        if (Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt") {
             $_ROSBE_UPDATES = "$_ROSBE_UPDATES $_ROSBE_STATCOUNT"
        }
    }
    set-loaction ..
}

function UPDINFO {
    set-location tmp
    if (!(Test-path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt")) {
        IEX "&'$_ROSBE_BASEDIR\Tools\wget.exe' -N --ignore-length --no-verbose $_ROSBE_URL/$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt"
        if (Test-Path "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt") {
            get-content "$_ROSBE_VERSION-$_ROSBE_STATCOUNT.txt"
        } else {
            "ERROR: This Update does not seem to exist or the Internet connection is not working correctly."
        }
    }
    set-location ..
    remove-item "tmp\*.*" -force
}

function UPDFIN {
    remove-item "tmp\*.*" -force
    if ($_ROSBE_UPDATES -ne $null) {
        "Following Updates available: $_ROSBE_UPDATES"
    } else {
    "RosBE is up to Date."
    }
    EOC
}

#
# The Update Server.
#
$_ROSBE_URL = "http://mitglied.lycos.de/reimerdaniel/rosbe"

#
# Save the recent dir to cd back there at the end.
#
$_ROSBE_OPATH = "$pwd"

if (Test-Path "$_ROSBE_BASEDIR\Tools\7z.exe") {
    set-location "$_ROSBE_BASEDIR\Tools"
    IEX "& wget.exe -N --ignore-length --no-verbose $_ROSBE_URL/7z.exe"
    set-location $_ROSBE_OPATH
}

set-location $_ROSBE_BASEDIR

#
# First check for a new Updater
#
$_ROSBE_UPDDATE = get-content update.ps1
IEX "$'Tools\wget.exe' -N --ignore-length --no-verbose $_ROSBE_URL/update.ps1"
$_ROSBE_UPDDATE = get-content update.ps1
if ($_ROSBE_UPDDATE -ne $_ROSBE_UPDDATE2) {
    # clear-host
    "Updater got updated and needs to be restarted."
    EOC
}

#
# Get to the Updates Subfolder.
#
if (!(Test-Path "$ENV:APPDATA\RosBE\Updates")) {New-Item -path "$ENV:APPDATA\RosBE" -name "Updates" -type directory}
set-location "$ENV:APPDATA\RosBE\Updates"

#
# Parse the args.
#
if ("$args" -eq "") {
    $_ROSBE_MULTIUPD = 1
    $_ROSBE_STATCOUNT = 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    $_ROSBE_STATCOUNT += 1
    UPDCHECK
    EOC
}
if ("$args" -eq "reset") {
    remove-item "$ENV:APPDATA\RosBE\Updates\*.*" -force
    remove-item "$ENV:APPDATA\RosBE\Updates\tmp\*.*" -force
    EOC
}
if ("$args[0]" -eq "nr") {
    $_ROSBE_STATCOUNT = $args[1]
    UPDCHECK
    EOC
}
if ("$args[0]" -eq "info") {
    $_ROSBE_STATCOUNT = $args[1]
    UPDINFO
    EOC
}
if ("$args" -eq "status") {
    if (!(test-path "tmp")) {New-Item -name "Updates" -type directory}
    copy-item *.txt .\tmp\.
    $_ROSBE_STATCOUNT = 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    $_ROSBE_STATCOUNT += 1
    STATUS
    UPDFIN
}
if ("$args" -ne "") {
    "Unknown parameter specified. Try 'help update'."
    EOC
}

$_ROSBE_UPDFINISH = $null
$_ROSBE_OPATH = $null
$_ROSBE_UPDDATE = $null
$_ROSBE_UPDATES = $null
$_ROSBE_UPDDATE2 = $null
$_ROSBE_MULTIUPD = $null
$_ROSBE_STATCOUNT = $null