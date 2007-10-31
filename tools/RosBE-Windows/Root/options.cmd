::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/options.cmd
:: PURPOSE:     Starts options.exe and restarts RosBE afterwards.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Options

::
:: Run options.exe
::
if exist "%_ROSBE_BASEDIR%\Tools\options.exe" (
    call "%_ROSBE_BASEDIR%\Tools\options.exe"
    if exist "%APPDATA%\RosBE\rosbe-options.cmd" (
        call "%APPDATA%\RosBE\rosbe-options.cmd"
    )
) else (
    echo ERROR: options.exe was not found.
)

title ReactOS Build Environment %_ROSBE_VERSION%
