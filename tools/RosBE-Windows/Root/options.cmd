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
if exist "%_ROSBE_BASEDIR%\options.exe" (
    pushd %_ROSBE_BASEDIR%
    call options.exe
    popd
    if exist "%_ROSBE_BASEDIR%\rosbe-options.cmd" (
        call "%_ROSBE_BASEDIR%\rosbe-options.cmd"
    )
) else (
    echo ERROR: options.exe was not found.
    title ReactOS Build Environment %_ROSBE_VERSION%
)
