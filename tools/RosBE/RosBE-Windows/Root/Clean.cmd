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
    call :DEL
    goto :EOC
) else if /i "%1" == "logs" (
    call :LOG
    goto :EOC
) else if /i "%1" == "all" (
    call :DEL
    call :LOG
    goto :EOC
) else (
    "%_ROSBE_BASEDIR%\Build.cmd" %1_clean
    goto :EOC
)


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


:: Check if we have something to clean, if so, clean it.
:DEL

:: Apply modified obj and out paths for deletion.
if "%_ROSBE_OBJPATH%" == "" (
    set OBJCLEANPATH=%_ROSBE_ROSSOURCEDIR%\obj-%ROS_ARCH%
) else (
    set OBJCLEANPATH=%_ROSBE_OBJPATH%
)

if "%_ROSBE_OUTPATH%" == "" (
    set OUTCLEANPATH=%_ROSBE_ROSSOURCEDIR%\output-%ROS_ARCH%
) else (
    set OUTCLEANPATH=%_ROSBE_OUTPATH%
)

if "%ROS_ARCH%" == "i386" (
    set MAKEFILE=%_ROSBE_ROSSOURCEDIR%\makefile.auto
) else (
    set MAKEFILE=%_ROSBE_ROSSOURCEDIR%\makefile-%ROS_ARCH%.auto
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

if exist "%_ROSBE_ROSSOURCEDIR%\reactos\." (
    rd /s /q "%_ROSBE_ROSSOURCEDIR%\reactos" 1> NUL 2> NUL
)

goto :EOF


:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
