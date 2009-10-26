::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/chdefgcc.cmd
:: PURPOSE:     Tool to change the current gcc in RosBE.
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
title Change the current MinGW/GCC Host/Target directory...

:: Parse the command line arguments. Ask the user if certain parameters are missing.
if "%1" == "" (
    set /p TOOLPATH="Please enter a MinGW/GCC directory (don't use quotes): "

    if "!TOOLPATH!" == "" (
        echo ERROR: You must enter a MinGW/GCC directory.
        goto :EOC
    )
) else (
    for /f "usebackq tokens=*" %%i in (`""%_ROSBE_BASEDIR%\Tools\rquote.exe" %1"`) do set TOOLPATH=%%i
)

if "%2" == "" (
    set /p TOOLMODE="Please specify, if this will be the Target or Host GCC: "

    if "!TOOLMODE!" == "" (
        echo ERROR: You must enter "target" or "host".
        goto :EOC
    )
) else (
    set TOOLMODE=%2
)

:: Verify the entered values
if exist "%_ROSBE_BASEDIR%\%TOOLPATH%\." (
    set TOOLPATH=%_ROSBE_BASEDIR%\%TOOLPATH%
) else if not exist "%TOOLPATH%\." (
    echo ERROR: The path specified doesn't seem to exist.
    goto :EOC
)

if not exist "%TOOLPATH%\bin\*gcc.exe" (
    echo ERROR: No MinGW/GCC found in the specified path.
    goto :EOC
)

:: Set the values
if /i "%TOOLMODE%" == "target" (
    echo Target Location: %TOOLPATH%
    endlocal & set _ROSBE_TARGET_MINGWPATH=%TOOLPATH%
) else if /i "%TOOLMODE%" == "host" (
    echo Host Location: %TOOLPATH%
    endlocal & set _ROSBE_HOST_MINGWPATH=%TOOLPATH%
) else (
    echo ERROR: You specified wrong parameters.
    endlocal
    goto :EOC
)

call "%_ROSBE_BASEDIR%\rosbe-gcc-env.cmd"
call "%_ROSBE_BASEDIR%\version.cmd"

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
