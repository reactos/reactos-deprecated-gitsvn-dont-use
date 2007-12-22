::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/MinGW.cmd
:: PURPOSE:     This script provides/sets up a standard MinGW build environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

if not "%1" == "" (
    cls
    echo Unknown parameter specified. Exiting.
    goto :EOC
)

color 0A
set PATH=%SystemRoot%\system32;%SystemRoot%
set _ROSBE_BASEDIR=%~dp0
set _ROSBE_BASEDIR=%_ROSBE_BASEDIR:~0,-1%
set _ROSBE_MODE=MinGW
set _ROSBE_ORIGINALPATH=%PATH%
set _ROSBE_MINGWPATH=%_ROSBE_BASEDIR%\4.1.3

echo.
echo.
call "%_ROSBE_BASEDIR%\rosbe-gcc-env.cmd"
echo.
echo.

::
:: Get the number of CPUs in the system so we know how many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, plus 1.
:: -x2       - Number of CPUs, doubled.
::
for /f "usebackq" %%i in (`"%_ROSBE_BASEDIR%\Tools\cpucount.exe" -x1`) do set CPUCOUNT=%%i

doskey /macrofile="%_ROSBE_BASEDIR%\MinGW.mac"

:EOC
