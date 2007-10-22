::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory
:: FILE:        Root/chdefdir.cmd
:: PURPOSE:     Tool to change the current working ReactOS source directory
::              in RosBE.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Change the current working ReactOS source directory...

if not %_ROSBE_PREVIOUSSOURCEDIR% == %_ROSBE_ROSSOURCEDIR% (
    set _ROSBE_PREVIOUSSOURCEDIR=%_ROSBE_ROSSOURCEDIR%
)

::
:: Parse the command line arguments.
::
if /i "%1" == "" (
    goto :INTERACTIVE
)
if /i "%1" == "previous" (
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_PREVIOUSSOURCEDIR%
) else (
    if not exist "%1\." (
        echo The path specified doesn't seem to exist.
        goto :EOC
    )
    set _ROSBE_ROSSOURCEDIR=%1
)
goto :EOC

:INTERACTIVE
set /p XY="Please enter a ReactOS source directory, or 'previous': "

if /i "%XY%" == "" (
    echo You must either enter a directory or "previous".
    goto :EOC
)
if /i "%XY%"=="previous" (
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_PREVIOUSSOURCEDIR%
) else (
    if not exist "%XY%\." (
        echo The path specified doesn't seem to exist.
        goto :EOC
    )
    set _ROSBE_ROSSOURCEDIR=%XY%
)

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
cd /d "%_ROSBE_ROSSOURCEDIR%"
