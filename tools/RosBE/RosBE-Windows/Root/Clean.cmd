::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Clean.cmd
:: PURPOSE:     Clean the ReactOS source directory.
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
title Cleaning...

if "%1" == "" (
    call :BIN
) else if /i "%1" == "logs" (
    call :LOG
) else if /i "%1" == "all" (
    call :BIN
    call :LOG
) else (
    call :MODULE %*
)
goto :EOC

:MODULE
    if "%1" == "" goto :EOF
    call "%_ROSBE_BASEDIR%\Make.cmd" %1_clean
    shift /1
    echo.
    GOTO :MODULE %*

:: Check if we have any logs to clean, if so, clean them.
:LOG
if exist "%_ROSBE_LOGDIR%\*.txt" (
    echo Cleaning build logs...
    del /f "%_ROSBE_LOGDIR%\*.txt" 1> NUL 2> NUL
    echo Done cleaning build logs.
) else (
    echo ERROR: There are no logs to clean.
)
goto :EOF


:: Check if we have any binaries to clean, if so, clean them.
:BIN

:: Apply modified obj and out paths for deletion.
if "%_ROSBE_OBJPATH%" == "" (
    set OBJCLEANPATH=obj-%ROS_ARCH%
) else (
    set OBJCLEANPATH=%_ROSBE_OBJPATH%
)

if "%_ROSBE_OUTPATH%" == "" (
    set OUTCLEANPATH=output-%ROS_ARCH%
) else (
    set OUTCLEANPATH=%_ROSBE_OUTPATH%
)

if /i "%ROS_ARCH%" == "i386" (
    set MAKEFILE=makefile.auto
) else (
    set MAKEFILE=makefile-%ROS_ARCH%.auto
)

if exist "%MAKEFILE%" (
    del "%MAKEFILE%" 1> NUL 2> NUL
)

if exist "%OBJCLEANPATH%\." (
    echo Cleaning ReactOS %ROS_ARCH% source directory...

    if exist "%OBJCLEANPATH%\." (
        rd /s /q "%OBJCLEANPATH%" 1> NUL 2> NUL
    )

    if exist "%OUTCLEANPATH%\." (
        rd /s /q "%OUTCLEANPATH%" 1> NUL 2> NUL
    )

    echo Done cleaning ReactOS %ROS_ARCH% source directory.
) else (
    echo ERROR: There is no %ROS_ARCH% compiler output to clean.
)

if exist "reactos\." (
    rd /s /q "reactos" 1> NUL 2> NUL
)

goto :EOF

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
