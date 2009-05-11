#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/reladdr2line.ps1
# PURPOSE:     Converts a value to hex and displays it.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "reladdr2line..."

# Receive all parameters.
$FILEPATH = $args[0]
$ADDRESS = $args[1]
if ($args.length -lt 1) {
    if ($FILEPATH -eq $null) {
        $FILEPATH = Read-Host "Please enter the path/file to be examined: "
        if ($FILEPATH.Contains("\")) {
            $FILEPATH = get-childitem "$FILEPATH\*" -name -recurse 2>NUL | select-string "$FILEPATH"
        }
    }
}
elseif ($args.length -lt 2) {
    if ($ADDRESS -eq $null) {
        $ADDRESS = Read-Host "Please enter the address you would like to analyze: "
    }
}

# Check if parameters were really given
if ($FILEPATH -eq $null) {
    "ERROR: You must specify a path/file to examine."
}
if ($ADDRESS -eq $null) {
    "ERROR: You must specify a address to analyze."
}

$baseaddr = (objdump -p $FILEPATH | select-string "ImageBase").tostring().split()
$baseaddr = "0x" + ($baseaddr.get($baseaddr.length - 1))

if ($baseaddr -lt $ADDRESS) {
    IEX "& '$_ROSBE_BASEDIR\Tools\raddr2line.exe' '$FILEPATH' '$ADDRESS'"
} else {
    $baseaddr = ($baseaddr | % {[Convert]::ToInt32($_,16)}) + ($ADDRESS | % {[Convert]::ToInt32($_,16)})
    $relbase = "0x" + ("{0:X}" -f $baseaddr)
    IEX "& '$_ROSBE_BASEDIR\Tools\raddr2line.exe' '$FILEPATH' '$relbase'"
}

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"

#
# Unload all used Vars.
#
$FILEPATH = $null
$ADDRESS = $null
$baseaddr = $null
$relbase = $null
