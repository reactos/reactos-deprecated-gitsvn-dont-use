::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/charch.cmd
:: PURPOSE:     Tool to change the current Arch to build ROS for in RosBE.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <colin@reactos.org>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion
title Change the Architecture to build for...

:: Parse the command line arguments.
if "%1" == "" (
    set /p ARCH="Please enter an Architecture you want to build ReactOS for: "

    if "!ARCH!" == "" (
        echo ERROR: You must enter an Architecture.
        goto :EOC
    )
) else (
    set ARCH=%1
)

:: Modify ROS_ARCH for the current environment
endlocal & set ROS_ARCH=%ARCH%

:: Refresh all needed Params by recalling the main Path setting CMD File.
if exist "%APPDATA%\RosBE\rosbe-options-%ROS_ARCH%.cmd" (
    call "%APPDATA%\RosBE\rosbe-options-%ROS_ARCH%.cmd"
)

call "%_ROSBE_BASEDIR%\chdefgcc.cmd" %ROS_ARCH% target

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
