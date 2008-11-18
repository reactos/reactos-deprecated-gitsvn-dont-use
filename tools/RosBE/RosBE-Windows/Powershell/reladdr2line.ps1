#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/reladdr2line.cmd
# PURPOSE:     Converts a value to hex and displays it.
# COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
#                             Daniel Reimer <reimer.daniel@freenet.de>
#                             Peter Ward <dralnix@gmail.com>
#
#

$host.ui.RawUI.WindowTitle = "reladdr2line..."

#
# Check if the user supplied a path, if they didn't look for
# the specified file in the current directory and any
# sub-directories.
#
function CHECKPATH {
    if ($_1.Contains("\")) {
        $_1 = get-childitem "$_1\*" -name -recurse 2>NUL | select-string "$_1"
    }
}

#
# Receive the Parameters and decide what to do.
#
$_1 = $args[0]
$_2 = $args[1]
if ($args.length -bt 2) {
    "ERROR: Too many parameters specified."
}
elseif ($args.length -lt 1) {
    if ($_1 -eq $null) {
        $_1 = Read-Host "Please enter the path/file to be examined: "
        CHECKPATH
    }
}
elseif ($args.length -lt 2) {
    if ($_2 -eq $null) {
        $_2 = Read-Host "Please enter the address you would like to analyze: "
    }
}

#
# First get the ImageBase of the File. If its smaller than the given
# Parameter, everything is ok, because it was already added onto the
# adress and can be given directly to raddr2line. If not, add it and
# give the result to raddr2line.
#
if ($_1 -eq $null) {
    "ERROR: You must specify a path/file to examine."
}
if ($_2 -eq $null) {
    "ERROR: You must specify a address to analyze."
}
$baseaddr = (objdump -p $_1 2>NUL | select-string "ImageBase")
if ($baseaddr -lt $_2) {
    IEX "& '$_ROSBE_BASEDIR\Tools\raddr2line.exe' '$_1' '$_2'"
} else {
    $baseaddr = $baseaddr + 0x$_2
    $relbase = "{0:X}" -f $baseaddr
    IEX "& '$_ROSBE_BASEDIR\Tools\raddr2line.exe' '$_1' '$relbase'"
}

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

#
# Unload all used Vars.
#
$_1 = $null
$_2 = $null
$baseaddr = $null
$relbase = $null
