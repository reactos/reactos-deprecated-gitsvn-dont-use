::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/chdefdir.cmd
:: PURPOSE:     Tool to change the current working ReactOS source directory
::              in RosBE.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::                             Colin Finck <colin@reactos.org>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion
title Change the current working ReactOS source directory...

:: Parse the command line arguments.
if "%1" == "" (
    set /p SOURCEDIR="Please enter a ReactOS source directory, or 'previous': "

    if "!SOURCEDIR!" == "" (
        echo ERROR: You must enter a ReactOS source directory, or 'previous'.
        goto :EOC
    )
) else (
    set SOURCEDIR=%1
)

if /i "%SOURCEDIR%" == "previous" (
    popd
) else (
    if not exist "%SOURCEDIR%\." (
        echo ERROR: The path specified doesn't seem to exist.
        goto :EOC
    )

    pushd %SOURCEDIR%
)

:EOC
set _ROSBE_ROSSOURCEDIR=%CD%
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
