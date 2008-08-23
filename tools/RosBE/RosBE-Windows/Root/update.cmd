::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/update.cmd
:: PURPOSE:     RosBE Updater.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

:: The Update Server
set url=www.foo.bar

:: PS1 Files
if exist "%_ROSBE_BASEDIR%\"%_ROSBE_BASEDIR%\Build.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Build.ps1
)
if exist "%_ROSBE_BASEDIR%\"%_ROSBE_BASEDIR%\Clean.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Clean.ps1
)
if exist "%_ROSBE_BASEDIR%\"%_ROSBE_BASEDIR%\Help.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Help.ps1
)
if exist "%_ROSBE_BASEDIR%\MinGW.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/MinGW.ps1
)
if exist "%_ROSBE_BASEDIR%\RosBE.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/RosBE.ps1
)
if exist "%_ROSBE_BASEDIR%\rosbe-gcc-env.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/rosbe-gcc-env.ps1
)

:: Options Files
if exist "%_ROSBE_BASEDIR%\options.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/options.cmd
)

:: SVN Files
if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/sSVN.cmd
)

:: SCut Files
if exist "%_ROSBE_BASEDIR%\scut.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/scut.cmd
)

:: RelAddr2Line Files
if exist "%_ROSBE_BASEDIR%\reladdr2line.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/reladdr2line.cmd
)

:: Other Tools Files
if exist "%_ROSBE_BASEDIR%\Config.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Config.cmd
)
if exist "%_ROSBE_BASEDIR%\chdefdir.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/chdefdir.cmd
)

"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Build.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/ChangeLog.txt
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/chdefgcc.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Clean.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/Help.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/LICENSE.TXT
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/MinGW.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/mingw.ico
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/MinGW.mac
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/README.pdf
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/RosBE.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/rosbe.ico
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/RosBE.mac
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/rosbe-gcc-env.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/TimeDate.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %url%/update.cmd
