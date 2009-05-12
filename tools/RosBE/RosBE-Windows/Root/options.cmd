::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/options.cmd
:: PURPOSE:     Starts options.exe and restarts RosBE afterwards.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion
title Options

if "%ROS_ARCH%" == "amd64" (
    set options=%_ROSBE_BASEDIR%\Tools\options64.exe
    set cfgfile=%APPDATA%\RosBE\rosbe-options-amd64.cmd
) else (
    set options=%_ROSBE_BASEDIR%\Tools\options.exe
    set cfgfile=%APPDATA%\RosBE\rosbe-options.cmd
)

:: Run options.exe
if exist "%options%" (
    pushd "%_ROSBE_BASEDIR%"
    call "%options%"
    popd

    if exist "%cfgfile%" (
        call "%cfgfile%"
    )
) else (
    echo ERROR: options executable was not found.
)

title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
