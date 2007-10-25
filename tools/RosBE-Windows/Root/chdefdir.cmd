::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory
:: FILE:        Root/chdefdir.cmd
:: PURPOSE:     Tool to change the current working ReactOS source directory
::              in RosBE.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

title Change the current working ReactOS source directory...

::
:: Parse the command line arguments.
::
if "%1" == "" (
    call :INTERACTIVE
) else (
    set _1=%1
)
if /i "%_1%" == "previous" (
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_PREVIOUSSOURCEDIR%
) else (
    if not exist "%_1%\." (
        echo ERROR: The path specified doesn't seem to exist.
        goto :EOC
    )
    set _ROSBE_PREVIOUSSOURCEDIR=%_ROSBE_ROSSOURCEDIR%
    set _ROSBE_ROSSOURCEDIR=%_1%
)
goto :EOC

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:INTERACTIVE
set /p _1="Please enter a ReactOS source directory, or 'previous': "
goto :EOF

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
cd /d "%_ROSBE_ROSSOURCEDIR%"
