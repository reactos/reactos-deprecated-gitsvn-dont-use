#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/RosBE.ps1
# PURPOSE:     This script provides/sets up various build environments for
#              ReactOS. Currently it provides a GCC 4.1.3 build environment.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"

# Set defaults to work with and override them if edited by the options utility.
(Get-Host).UI.RawUI.ForegroundColor = "Green"
(Get-Host).UI.RawUI.BackgroundColor = "Black"
clear-host

# For NT4 compatibility
if ($ENV:APPDATA.Length -lt 1) {
    $ENV:APPDATA = $ENV:USERPROFILE
}

# Set defaults to work with and override them if edited by
# the options utility.
if ("$ENV:ROS_ARCH" -eq "") {
    $ENV:ROS_ARCH = "i386"
} else {
    $ENV:ROS_ARCH = "$($args)"
}

$global:0 = $myInvocation.MyCommand.Definition
$global:_ROSBE_BASEDIR = [System.IO.Path]::GetDirectoryName($0)
$global:_ROSBE_VERSION = "1.4.4"
$global:_ROSBE_ROSSOURCEDIR = "$pwd"
$global:_ROSBE_SHOWTIME = 1
$global:_ROSBE_WRITELOG = 1
$global:_ROSBE_USECCACHE = 0
$global:_ROSBE_STRIP = 0
$global:_ROSBE_NOSTRIP = 0
$global:_ROSBE_LOGDIR = "$pwd\RosBE-Logs"
$global:_ROSBE_HOST_MINGWPATH = "$_ROSBE_BASEDIR\i386"
$global:_ROSBE_TARGET_MINGWPATH = "$_ROSBE_BASEDIR\$ENV:ROS_ARCH"
$global:_ROSBE_ORIGINALPATH = "$_ROSBE_HOST_MINGWPATH\bin;$ENV:PATH"
$global:_ROSBE_MINGWMAKE = "$_ROSBE_HOST_MINGWPATH\bin\mingw32-make.exe"

# Get the number of CPUs in the system so we know how many jobs to execute.
# To modify the number used, see the cpucount usage for getting to know about the possible options
$_ROSBE_MAKEX_JOBS = (gwmi win32_processor).numberofcores + 1

$ENV:CCACHE_DIR = "$ENV:APPDATA\RosBE\.ccache"
$ENV:C_INCLUDE_PATH = $null
$ENV:CPLUS_INCLUDE_PATH = $null
$ENV:LIBRARY_PATH = $null

function global:Get-WebFile {
    param(
        $url = $null,
        $file = $null
    )
    $local:ErrorActionPreference = "SilentlyContinue"
    $clnt = new-object System.Net.WebClient
    $clnt.DownloadFile($url,$file)
    $local:ErrorActionPreference = "Continue"
}

# Load the doskey macros and delete any macros for components
# that are not actually present.
function LoadAliases {
    function global:BASEDIR {
        set-location "$_ROSBE_ROSSOURCEDIR"
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
    function global:MAKEX($xargs) {IEX "&'$_ROSBE_BASEDIR\Build.ps1' multi $($xargs)"}

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

    set-alias VERSION "$_ROSBE_BASEDIR\version.ps1" -scope Global

    if (Test-Path "$_ROSBE_BASEDIR\options.ps1") {
        set-alias OPTIONS "$_ROSBE_BASEDIR\options.ps1" -scope Global
    }
}

# Check if RosBE data directory exists, if not, create it.
if (!(Test-Path "$ENV:APPDATA\RosBE")) {
    New-Item -path "$ENV:APPDATA" -name "RosBE" -type directory
}

# Load the user's options if any
if (Test-Path "$ENV:APPDATA\RosBE\rosbe-options.ps1") {
    & "$ENV:APPDATA\RosBE\rosbe-options.ps1"
}

if (Test-Path "$ENV:APPDATA\RosBE\rosbe-options-$args.ps1") {
    & "$ENV:APPDATA\RosBE\rosbe-options-$args.ps1"
}

# Check if writing logs is enabled, if so check if our log directory
# exists, if it doesn't, create it.
if ($_ROSBE_WRITELOG -eq 1) {
    if (!(Test-Path "$_ROSBE_LOGDIR")) {
        New-Item -path "$_ROSBE_LOGDIR" -type directory
    }
}

# Load the doskey macros that serve as our commands.
LoadAliases

& "$_ROSBE_BASEDIR\rosbe-gcc-env.ps1"

clear-host
"*******************************************************************************"
"*                                                                             *"
"*                        ReactOS Build Environment $_ROSBE_VERSION                      *"
"*                                                                             *"
"*******************************************************************************"
""

# Load the base directory from srclist.txt and set it as the
# new source directory.
if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
    & "$_ROSBE_BASEDIR\scut.ps1"
}

# Tell how to display the available commands.
""
"For a list of all included commands, type: ""help"""
"-------------------------------------------------"
""

# Look if the ReactOS source directory is empty. If so,
# inform the user and mention 'ssvn create' (only if ssvn is installed).
if (!(Test-Path "$_ROSBE_BASEDIR\sSVN.ps1")) {
    if ((get-childitem $_ROSBE_ROSSOURCEDIR).Count -le 0) {
        "No ReactOS source detected. Please use ""ssvn create"" to download it."
    }
}
