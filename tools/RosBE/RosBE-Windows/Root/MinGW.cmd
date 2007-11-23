::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/RosBE.cmd
:: PURPOSE:     This script provides/sets up a standard MinGW build environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

color 0A
set PATH=%SystemRoot%\system32;%SystemRoot%
set _ROSBE_BASEDIR=%~dp0
set _ROSBE_BASEDIR=%_ROSBE_BASEDIR:~0,-1%
set _ROSBE_MODE=MinGW
set _ROSBE_ORIGINALPATH=%PATH%
set _ROSBE_MINGWPATH=%_ROSBE_BASEDIR%\4.1.3

if not "%1" == "" (
    cls
    echo Unknown parameter specified. Exiting.
    goto :EOC
)

echo.
echo.
call "%_ROSBE_BASEDIR%\rosbe-gcc-env.cmd"
echo.
echo.

doskey MAKE = "%_ROSBE_MINGWMAKE%" $*

:EOC
