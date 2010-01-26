::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Clean.cmd
:: PURPOSE:     Clean the ReactOS source directory.
:: COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
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

:: Check if the user set any custom filenames or pathes, otherwise locally set the appropriate variables.
if "%ROS_AUTOMAKE%"     == "" (set ROS_AUTOMAKE=makefile-%ROS_ARCH%.auto)
if "%ROS_INTERMEDIATE%" == "" (set ROS_INTERMEDIATE=obj-%ROS_ARCH%)
if "%ROS_OUTPUT%"       == "" (set ROS_OUTPUT=output-%ROS_ARCH%)
if "%ROS_CDOUTPUT%"     == "" (set ROS_CDOUTPUT=reactos)

:: Do some basic sanity checks to verify that we are working in a ReactOS source tree.
:: Consider that we also want to clean half-complete builds, so don't depend on too many existing files.
set IS_SOURCE_DIRECTORY=1
call :BIN_CHECKEXISTENCE "%ROS_INTERMEDIATE%\." "%ROS_OUTPUT%\."

if "%IS_SOURCE_DIRECTORY%" == "1" (
    echo Cleaning ReactOS %ROS_ARCH% source directory...
    
    del "%ROS_AUTOMAKE%" 1>NUL 2>NUL
    rd /s /q "%ROS_INTERMEDIATE%" 1>NUL 2>NUL
    rd /s /q "%ROS_OUTPUT%" 1>NUL 2>NUL
    rd /s /q "%ROS_CDOUTPUT%" 1>NUL 2>NUL
    
    echo Done cleaning ReactOS %ROS_ARCH% source directory.
) else (
    echo ERROR: This does not look like a ReactOS %ROS_ARCH% source directory to clean.
)
goto :EOF

:: Check if all given files exist. If one does not, set IS_SOURCE_DIRECTORY to 0.
:BIN_CHECKEXISTENCE
if "%1" == "" (goto :EOF)
if not exist "%1" (
    set IS_SOURCE_DIRECTORY=0
    goto :EOF
)
shift /1
goto :BIN_CHECKEXISTENCE %*

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
