::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/options.cmd
:: PURPOSE:     Starts options.exe and restarts RosBE afterwards.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

title Options

::
:: Run options.exe
::
if exist "%_ROSBE_BASEDIR%\Tools\options.exe" (
    pushd "%_ROSBE_BASEDIR%"
    call "%_ROSBE_BASEDIR%\Tools\options.exe"
    popd "%_ROSBE_BASEDIR%"
    if exist "%APPDATA%\RosBE\rosbe-options.cmd" (
        call "%APPDATA%\RosBE\rosbe-options.cmd"
    )
) else (
    echo ERROR: options.exe was not found.
)

if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)
