#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/kdbg.ps1
# PURPOSE:     Displays a KDBG Session Interactively.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
#

$DATENAME = get-date -f dyMMyyyy
$TIMENAME = get-date -f HHmm

$host.ui.RawUI.WindowTitle = "kdbg Session..."

if ("$($args[0])" -eq "writelog") {
    if ("$($args[1])" -eq "") {
        $LOGFILE = "-l DBG-$datename-$timename.txt"
    } else {
        $LOGFILE = "-l ""$($args[1])"""
    }
} else {
    $LOGFILE = $null
}

"Now Start your VM with COM1 set to \\.\pipe\kdbg"
read-host 'Press any key to continue . . .'

log2lines -c $LOGFILE < \\.\pipe\kdbg

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
