#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/RosBE.ps1
# PURPOSE:     This script provides/sets up various build environments for
#              ReactOS. Currently it provides a GCC 4.1.3 build environment.
# COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
#                             Peter Ward <dralnix@gmail.com>
#
#

#
# Set defaults to work with and override them if edited by
# the options utility.
#
(Get-Host).UI.RawUI.ForegroundColor = "Green"
(Get-Host).UI.RawUI.BackgroundColor = "Black"
cls
if ($APPDATA.Length -lt 1) {$APPDATA = $ENV:USERPROFILE}
$global:PATH = "$ENV:SystemRoot\system32;$ENV:SystemRoot"
$global:_ROSBE_VERSION = 1.2
$global:_ROSBE_BASEDIR = "C:\Programme\RosBE" #NOCH FALSCH
$global:_ROSBE_MODE = "RosBE"
$global:_ROSBE_ROSSOURCEDIR = "$pwd"
$global:_ROSBE_ORIGINALPATH = "$PATH"
$global:_ROSBE_SHOWTIME = 1
$global:_ROSBE_WRITELOG = 1
$global:_ROSBE_USECCACHE = 0
$global:_ROSBE_STRIP = 0
$global:_ROSBE_MINGWPATH = "$_ROSBE_BASEDIR\4.1.3"
$global:_ROSBE_LOGDIR = "$_ROSBE_ROSSOURCEDIR\RosBE-Logs"
$global:_ROSBE_OBJPATH = $null
$global:_ROSBE_OUTPATH = $null

#
# Display the banner and set up the environment for the GCC 4.x.x build
# environment.
#
function RosBE4
{
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

function LOADDOSKEYMACROS
{
if (Test-Path "$_ROSBE_BASEDIR\chdefdir.ps1") {
set-alias CHDEFDIR "& $_ROSBE_BASEDIR\chdefdir.ps1"
}

if (Test-Path "$_ROSBE_BASEDIR\chdefgcc.ps1") {
set-alias CHDEFGCC "& $_ROSBE_BASEDIR\chdefgcc.ps1"
}

if (Test-Path "$_ROSBE_BASEDIR\charch.ps1") {
set-alias CHARCH "& $_ROSBE_BASEDIR\charch.ps1"
}

set-alias CLEAN "& $_ROSBE_BASEDIR\Clean.ps1"

if (Test-Path "$_ROSBE_BASEDIR\Config.ps1") {
set-alias CONFIG "& $_ROSBE_BASEDIR\Config.ps1"
}

set-alias HELP "& $_ROSBE_BASEDIR\Help.ps1"
set-alias MAKE "& $_ROSBE_BASEDIR\Build.ps1"
set-alias MAKEX "& $_ROSBE_BASEDIR\Build.ps1 multi"

if (Test-Path "$_ROSBE_BASEDIR\reladdr2line.ps1") {
set-alias RADDR2LINE "& $_ROSBE_BASEDIR\reladdr2line.ps1"
}

if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
set-alias SCUT "& $_ROSBE_BASEDIR\scut.ps1"
}

if (Test-Path "$_ROSBE_BASEDIR\sSVN.ps1") {
set-alias SSVN "& $_ROSBE_BASEDIR\sSVN.ps1"
set-alias SVN "& $_ROSBE_BASEDIR\Tools\svn.exe"
}

if (Test-Path "$_ROSBE_BASEDIR\options.ps1") {
set-alias OPTIONS "& $_ROSBE_BASEDIR\options.ps1"
}
}

#
# Check if RosBE data directory exists, if not, create it.
#
if (!(Test-Path "$APPDATA\RosBE")) {mkdir $APPDATA\RosBE}

#
# Check if the user has used the options utility and
# if so, load their options.
#
if (Test-Path "$APPDATA\RosBE\rosbe-options.ps1") {
& "$APPDATA\RosBE\rosbe-options.ps1"
}

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"

#
# Check if we are using oldmode or if any unknown parameters
# were specified.
#
if ($args.count -gt 0) {
if ($args[0] = "oldmode") {
#    cls
    $_ROSBE_MODE = "MinGW"

} else {
#    cls
    "Unknown parameter specified. Exiting."
exit
}
}
RosBE4
#cls

#
# Load the base directory from srclist.txt and set it as the
# new source directory.
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
LOADDOSKEYMACROS

#
# Look if the ReactOS source directory is empty. If so,
# inform the user and mention 'ssvn create' (only if ssvn is installed).
#
if (!(Test-Path "$_ROSBE_BASEDIR\sSVN.ps1")) {
#    dir /b $_ROSBE_ROSSOURCEDIR 2>nul | findstr "." >nul
    if ($errorlevel = 1) {
        "No ReactOS source detected. Please use ""ssvn create"" to download it."
    }
}
