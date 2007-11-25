::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/RosBE.cmd
:: PURPOSE:     This script provides/sets up various build environments for
::              ReactOS. Currently it provides a GCC 4.1.3 build environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

if not "%1" == "" (
    cls
    echo Unknown parameter specified. Exiting.
    goto :EOC
)

::
:: Set defaults to work with and override them if edited by
:: the options utility.
::
color 0A
if "%APPDATA%"== "" set APPDATA=%USERPROFILE%
set PATH=%SystemRoot%\system32;%SystemRoot%
set _ROSBE_VERSION=1.1
set _ROSBE_BASEDIR=%~dp0
set _ROSBE_BASEDIR=%_ROSBE_BASEDIR:~0,-1%
set _ROSBE_MODE=RosBE
set _ROSBE_ROSSOURCEDIR=%CD%
set _ROSBE_ORIGINALPATH=%PATH%
set _ROSBE_SHOWTIME=1
set _ROSBE_WRITELOG=1
set _ROSBE_USECCACHE=0
set _ROSBE_STRIP=0
set _ROSBE_MINGWPATH=%_ROSBE_BASEDIR%\4.1.3
set _ROSBE_LOGDIR=%CD%\RosBE-Logs
set _ROSBE_OBJPATH=
set _ROSBE_OUTPATH=

::
:: Check if RosBE data directory exists, if not, create it.
::
if not exist "%APPDATA%\RosBE\." (
    mkdir "%APPDATA%\RosBE"
)

::
:: Check if the user has used the options utility and
:: if so, load their options.
::
if exist "%APPDATA%\RosBE\rosbe-options.cmd" (
    call "%APPDATA%\RosBE\rosbe-options.cmd"
)

title ReactOS Build Environment %_ROSBE_VERSION%

cls
call :RosBE4

:EndCommandParse

::
:: Load the base directory from srclist.txt and set it as the
:: new source directory.
::
if exist "%_ROSBE_BASEDIR%\scut.cmd" (
    call "%_ROSBE_BASEDIR%\scut.cmd"
)

::
:: Tell how to display the available commands.
::
echo.
echo For a list of all included commands, type: "help"
echo -------------------------------------------------
echo.

::
:: Load the doskey macros that serve as our commands.
::
call :LOADDOSKEYMACROS

::
:: Look if the ReactOS source directory is empty. If so,
:: inform the user and mention 'ssvn create' (only if ssvn is installed).
::
setlocal enabledelayedexpansion
if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
    dir /b "%_ROSBE_ROSSOURCEDIR%" 2>nul | findstr "." >nul
    if !errorlevel! == 1 (
        echo No ReactOS source detected. Please use "ssvn create" to download it.
    )
)
endlocal

goto :EOC

::
:: Display the banner and set up the environment for the GCC 4.x.x build
:: environment.
::
:RosBE4
    echo *******************************************************************************
    echo *                                                                             *
    echo *                        ReactOS Build Environment %_ROSBE_VERSION%                        *
    echo *                                                                             *
    echo *******************************************************************************
    echo.
    echo.
    ver
    ::
    :: Set the correct path for the build tools and set the MinGW make.
    ::
    call "%_ROSBE_BASEDIR%\rosbe-gcc-env.cmd"
goto :EOF

::
:: Load the doskey macros and delete any macros for components
:: that are not actually present.
::
:LOADDOSKEYMACROS
    doskey /macrofile="%_ROSBE_BASEDIR%\RosBE.mac"

    if not exist "%_ROSBE_BASEDIR%\chdefdir.cmd" ( doskey CHDEFDIR= )
    if not exist "%_ROSBE_BASEDIR%\chdefgcc.cmd" ( doskey CHDEFGCC= )
    if not exist "%_ROSBE_BASEDIR%\Config.cmd" ( doskey CONFIG= )
    if not exist "%_ROSBE_BASEDIR%\reladdr2line.cmd" ( doskey RADDR2LINE= )
    if not exist "%_ROSBE_BASEDIR%\scut.cmd" ( doskey SCUT= )
    if not exist "%_ROSBE_BASEDIR%\sSVN.cmd" ( doskey SSVN= )
    if not exist "%_ROSBE_BASEDIR%\sSVN.cmd" ( doskey SVN= )
    if not exist "%_ROSBE_BASEDIR%\options.cmd" ( doskey OPTIONS= )
goto :EOF

:EOC
