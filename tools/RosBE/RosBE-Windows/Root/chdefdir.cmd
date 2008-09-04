::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/chdefdir.cmd
:: PURPOSE:     Tool to change the current working ReactOS source directory
::              in RosBE.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

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
    popd
) else (
    if not exist "%_1%\." (
        echo ERROR: The path specified doesn't seem to exist.
        goto :EOC
    )
    pushd %_1%
)
set _ROSBE_ROSSOURCEDIR=%CD%
goto :EOC

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:INTERACTIVE
    set /p _1="Please enter a ReactOS source directory, or 'previous': "
    if "%_1%" == "" (
        echo ERROR: You must enter a ReactOS source directory, or 'previous'.
        goto :EOC
    )

:EOC
if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

::
:: Unload all used Vars.
::
set _1=
