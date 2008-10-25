#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/MinGW.ps1
# PURPOSE:     This script provides/sets up a standard MinGW build environment.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

if ($args -ne "") {
    cls
    "Unknown parameter specified. Exiting."
    exit
}

(Get-Host).UI.RawUI.ForegroundColor = "Green"
(Get-Host).UI.RawUI.BackgroundColor = "Black"
cls
$ENV:PATH = "$ENV:SystemRoot\system32;$ENV:SystemRoot"
$global:0 = $myInvocation.MyCommand.Definition
$global:_ROSBE_BASEDIR = [System.IO.Path]::GetDirectoryName($0)
$global:_ROSBE_MODE = "MinGW"
$global:_ROSBE_ORIGINALPATH = "$ENV:PATH"
$global:_ROSBE_HOST_MINGWPATH = "$_ROSBE_BASEDIR\4.1.3"
$global:_ROSBE_TARGET_MINGWPATH = "$_ROSBE_BASEDIR\4.1.3"

""
""
& "$_ROSBE_BASEDIR\rosbe-gcc-env.ps1"
""
""

#
# Get the number of CPUs in the system so we know how many jobs to execute.
$CPUCOUNT= (gwmi win32_processor).numberofcores + 1

if (Test-Path "$_ROSBE_BASEDIR\chdefgcc.ps1") {
    set-alias CHDEFGCC "$_ROSBE_BASEDIR\chdefgcc.ps1" -scope Global
}
set-alias MAKE "$_ROSBE_BASEDIR\Build.ps1" -scope Global
set-alias MAKEX "$_ROSBE_BASEDIR\Build.ps1 multi" -scope Global
if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
    set-alias SCUT "$_ROSBE_BASEDIR\scut.ps1" -scope Global
}
