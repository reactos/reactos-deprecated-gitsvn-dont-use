::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Build.cmd
:: PURPOSE:     Perform the build of ReactOS.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <colin@reactos.org>
::                             Peter Ward <dralnix@gmail.com>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion

:: Check if config.template.rbuild is newer than config.rbuild, if it is then
:: abort the build and inform the user.
if exist .\config.rbuild (
    "%_ROSBE_BASEDIR%\Tools\chknewer.exe" .\config.template.rbuild .\config.rbuild
    if !errorlevel! == 1 (
        echo.
        echo *** config.template.rbuild is newer than config.rbuild ***
        echo *** aborting build. Please check for changes and       ***
        echo *** update your config.rbuild.                         ***
        echo.
        goto :EOC
    )
)

call "%_ROSBE_BASEDIR%\TranslateOptions.cmd"

if "%_ROSBE_USECCACHE%" == "1" (
    set _ROSBE_CCACHE=ccache 
) else (
    set _ROSBE_CCACHE=
)

set HOST_CC=%_ROSBE_CCACHE%gcc
set HOST_CPP=%_ROSBE_CCACHE%g++
set TARGET_CC=%_ROSBE_CCACHE%%ROS_PREFIX%gcc
set TARGET_CPP=%_ROSBE_CCACHE%%ROS_PREFIX%g++

:: Get the current date and time for use in in our build log's file name.
call "%_ROSBE_BASEDIR%\TimeDate.cmd"

:: Check whether we were called as "makex" or "make"
if "%1" == "multi" (
    shift
    set MAKE_JOBS=%_ROSBE_MAKEX_JOBS%
    set TITLE_COMMAND=makex %2
) else (
    set MAKE_JOBS=1
    set TITLE_COMMAND=make %1
)

title '%TITLE_COMMAND%' build started: %TIMERAW%   (%ROS_ARCH%)

:: Do the actual building
if %_ROSBE_SHOWTIME% == 1 (
    set BUILDTIME_COMMAND="%_ROSBE_BASEDIR%\Tools\buildtime.exe"
) else (
    set BUILDTIME_COMMAND=
)

if %_ROSBE_WRITELOG% == 1 (
    %BUILDTIME_COMMAND% "%_ROSBE_MINGWMAKE%" -j %MAKE_JOBS% %1 2>&1 | "%_ROSBE_BASEDIR%\Tools\tee.exe" "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_TARGET_GCCVERSION%-%datename%-%timename%.txt"
) else (
    %BUILDTIME_COMMAND% "%_ROSBE_MINGWMAKE%" -j %MAKE_JOBS% %1
)

:EOC
:: Highlight the fact that building has ended.
"%_ROSBE_BASEDIR%\Tools\flash.exe"
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
