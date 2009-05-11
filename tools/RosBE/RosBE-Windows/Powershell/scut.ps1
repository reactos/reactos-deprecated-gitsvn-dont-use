#
# PROJECT:     RosBE - ReactOS Build Environment for Windows.
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/scut.ps1
# PURPOSE:     Manages named shortcuts to ReactOS source directories.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

function paramcall($arg) {
    IEX "&'$_ROSBE_BASEDIR\Tools\scut.exe' $arg"
}

function parse($arg) {
    $OUT = paramcall($arg)
    # It's a shortcut name, so change the directory

    if ($OUT -ne "Default") {
        if (Test-Path "$OUT\.") {
           set-location $OUT
            $_ROSBE_ROSSOURCEDIR = $OUT
        }
    }
}

# Check if the parameter is an scut parameter, which can just be forwarded to the EXE file
if ($args[0] -eq "add")    {paramcall($args)}
elseif ($args[0] -eq "rem")    {paramcall($args)}
elseif ($args[0] -eq "edit")   {paramcall($args)}
elseif ($args[0] -eq "def")    {paramcall($args)}
elseif ($args[0] -eq "list")   {paramcall($args)}
elseif ($args[0] -eq "/?")     {paramcall($args)}
elseif ($args[0] -eq "-h")     {paramcall($args)}
elseif ($args[0] -eq "--help") {paramcall($args)}
else {parse($args)}

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"

#
# Unload all used Vars.
#
$OUT = $null
