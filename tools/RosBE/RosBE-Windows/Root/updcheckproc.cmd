::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/update.cmd
:: PURPOSE:     RosBE Updater.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

setlocal
setlocal enableextensions
setlocal enabledelayedexpansion

if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

if "%2" == "status" (
    cd tmp
    if not exist "%_ROSBE_VERSION%-%1.txt" (
        "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%1.txt 1> NUL 2> NUL
        if exist "%_ROSBE_VERSION%-%1.txt" (
		    echo %1
            set _ROSBE_UPDATES=%_ROSBE_UPDATES%, %1 
        ) else (
            set _ROSBE_UPDFINISH=1
        )
    )
	goto :EOF
)
if not exist "%_ROSBE_VERSION%-%1.txt" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%1.txt 1> NUL 2> NUL
)
if exist "%_ROSBE_VERSION%-%1.txt" (
    type "%_ROSBE_VERSION%-%1.txt"
    echo.
    echo Install?
    set /p YESNO="(yes), (no)"
    if /i "!YESNO!"=="yes" (
        if not exist "%_ROSBE_VERSION%-%1.7z" (
            "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%1.7z 1> NUL 2> NUL
        )
        if exist "%_ROSBE_VERSION%-%1.7z" (
            "%_ROSBE_BASEDIR%\Tools\7z.exe" x "%_ROSBE_VERSION%-%1.7z"
            cd "%_ROSBE_VERSION%-%1"
            call "%_ROSBE_VERSION%-%1.cmd"
        ) else (
            echo ERROR: This Update does not seem to exist or the Internet connection is not working correctly.
            goto :EOF
        )
    ) else if /i "!YESNO!"=="no" (
        del "%_ROSBE_VERSION%-%1.txt" 1> NUL 2> NUL
        goto :EOF
    )
) else (
    if "%2" == "next" (
	    echo set %2
        set _ROSBE_UPDFINISH=1
    ) else (
        echo ERROR: This Update does not seem to exist or the Internet connection is not working correctly.
        goto :EOF
    )
)
