#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/RosBE.ps1
# PURPOSE:     This script provides/sets up various build environments for
#              ReactOS. Currently it provides a GCC 4.1.3 build environment.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

#
# Set defaults to work with and override them if edited by the options utility.
#
(Get-Host).UI.RawUI.ForegroundColor = "Green"
(Get-Host).UI.RawUI.BackgroundColor = "Black"
cls

if ($APPDATA.Length -lt 1) {
    $APPDATA = $ENV:USERPROFILE
}
$ENV:PATH = "$ENV:SystemRoot\system32;$ENV:SystemRoot"
$global:_ROSBE_VERSION = 1.3
$global:0 = $myInvocation.MyCommand.Definition
$global:_ROSBE_BASEDIR = [System.IO.Path]::GetDirectoryName($0)
$global:_ROSBE_MODE = "RosBE"
$global:_ROSBE_ROSSOURCEDIR = "$pwd"
$global:_ROSBE_ORIGINALPATH = "$ENV:PATH"
$global:_ROSBE_SHOWTIME = 1
$global:_ROSBE_WRITELOG = 1
$global:_ROSBE_USECCACHE = 0
$global:_ROSBE_STRIP = 0
$global:_ROSBE_NOSTRIP = 0
$global:_ROSBE_HOST_MINGWPATH = "$_ROSBE_BASEDIR\4.1.3"
$global:_ROSBE_TARGET_MINGWPATH = "$_ROSBE_BASEDIR\4.1.3"
$global:_ROSBE_LOGDIR = "$_ROSBE_ROSSOURCEDIR\RosBE-Logs"
$global:_ROSBE_OBJPATH = $null
$global:_ROSBE_OUTPATH = $null

#
# Display the banner and set up the environment for the GCC 4.x.x build
# environment.
#
function RosBE {
    "*******************************************************************************"
    "*                                                                             *"
    "*                        ReactOS Build Environment $_ROSBE_VERSION                        *"
    "*                                                                             *"
    "*******************************************************************************"
    ""
    ""
    (get-WmiObject Win32_OperatingSystem).caption
    #
    # Set the correct path for the build tools and set the MinGW make.
    #
    & "$_ROSBE_BASEDIR\rosbe-gcc-env.ps1"
}

#
# Load the doskey macros and delete any macros for components
# that are not actually present.
#

function LoadAliases {
    function global:BASEDIR {
        cd "$_ROSBE_ROSSOURCEDIR"
    }
    if (Test-Path "$_ROSBE_BASEDIR\chdefdir.ps1") {
        set-alias CHDEFDIR "$_ROSBE_BASEDIR\chdefdir.ps1" -scope Global
    }

    if (Test-Path "$_ROSBE_BASEDIR\chdefgcc.ps1") {
        set-alias CHDEFGCC "$_ROSBE_BASEDIR\chdefgcc.ps1" -scope Global
    }

    if (Test-Path "$_ROSBE_BASEDIR\charch.ps1") {
        set-alias CHARCH "$_ROSBE_BASEDIR\charch.ps1" -scope Global
    }

    set-alias CLEAN "$_ROSBE_BASEDIR\Clean.ps1" -scope Global

    if (Test-Path "$_ROSBE_BASEDIR\Config.ps1") {
        set-alias CONFIG "$_ROSBE_BASEDIR\Config.ps1" -scope Global
    }

    set-alias HELP "$_ROSBE_BASEDIR\Help.ps1" -scope Global
    set-alias MAKE "$_ROSBE_BASEDIR\Build.ps1" -scope Global
    set-alias MAKEX "$_ROSBE_BASEDIR\Build.ps1 multi" -scope Global

    if (Test-Path "$_ROSBE_BASEDIR\reladdr2line.ps1") {
        set-alias RADDR2LINE "$_ROSBE_BASEDIR\reladdr2line.ps1" -scope Global
    }

    if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
        set-alias SCUT "$_ROSBE_BASEDIR\scut.ps1" -scope Global
    }

    if (Test-Path "$_ROSBE_BASEDIR\sSVN.ps1") {
        set-alias SSVN "$_ROSBE_BASEDIR\sSVN.ps1" -scope Global
        set-alias SVN "$_ROSBE_BASEDIR\Tools\svn.exe" -scope Global
    }

    if (Test-Path "$_ROSBE_BASEDIR\update.ps1") {
        set-alias UPDATE "$_ROSBE_BASEDIR\update.ps1" -scope Global
    }

    if (Test-Path "$_ROSBE_BASEDIR\options.ps1") {
        set-alias OPTIONS "$_ROSBE_BASEDIR\options.ps1" -scope Global
    }
}

#
# Check if RosBE data directory exists, if not, create it.
#
if (!(Test-Path "$APPDATA\RosBE")) {
    New-Item -path "$APPDATA" -name "RosBE" -type directory
}

#
# Check if the user has used the options utility and if so, load their options.
#
if (Test-Path "$APPDATA\RosBE\rosbe-options.ps1") {
    & "$APPDATA\RosBE\rosbe-options.ps1"
}

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"

#
# Check if we are using oldmode or if any unknown parameters were specified.
#
if ($args.count -gt 0) {
    if ($args -eq "oldmode") {
        cls
        $_ROSBE_MODE = "MinGW"

    } else {
        cls
        "Unknown parameter specified. Exiting."
        exit
    }
}
RosBE

#
# Load the base directory from srclist.txt and set it as the new source
# directory.
#
if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
    & "$_ROSBE_BASEDIR\scut.ps1"
}

#
# Tell how to display the available commands.
#
""
"For a list of all included commands, type: ""help"""
"-------------------------------------------------"
""

#
# Load the doskey macros that serve as our commands.
#
LoadAliases

#
# Look if the ReactOS source directory is empty. If so, inform the user and
# mention 'ssvn create' (only if ssvn is installed).
#
if (!(Test-Path "$_ROSBE_BASEDIR\sSVN.ps1")) {
    if ((get-childitem $_ROSBE_ROSSOURCEDIR).Count -le 0) {
        "No ReactOS source detected. Please use ""ssvn create"" to download it."
    }
}
