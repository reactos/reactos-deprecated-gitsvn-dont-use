::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/Build.cmd
:: PURPOSE:     Perform the build of ReactOS.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <mail@colinfinck.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Check if config.template.rbuild is newer than config.rbuild, if it is then
:: abort the build and inform the user.
::
setlocal enabledelayedexpansion
if exist .\config.rbuild (
    "%_ROSBE_BASEDIR%\Tools\chknewer.exe" .\config.template.rbuild .\config.rbuild
    if !errorlevel! == 1 (
        echo.
        echo *** config.template.rbuild is newer than config.rbuild ***
        echo *** aborting build. Please check for changes and       ***
        echo *** update your config.rbuild.                         ***
        echo.
        endlocal
        goto :EOC
    )
)
endlocal

::
:: Check if strip or ccache are being used and set the appropriate options.
::
if defined _ROSBE_STRIP (
    if %_ROSBE_STRIP% == 1 (
        set ROS_LEAN_AND_MEAN=yes
    ) else (
        set ROS_LEAN_AND_MEAN=no
    )
)
if defined _ROSBE_USECCACHE (
    if %_ROSBE_USECCACHE% == 1 (
        set CCACHE_DIR=%APPDATA%\RosBE\.ccache
        set HOST_CC=ccache gcc
        set HOST_CPP=ccache g++
        set TARGET_CC=ccache gcc
        set TARGET_CPP=ccache g++
    ) else (
        set HOST_CC=gcc
        set HOST_CPP=g++
        set TARGET_CC=gcc
        set TARGET_CPP=g++
    )
)

::
:: Check if the user has chosen to use a different object or output path
:: and set it accordingly.
::
if defined _ROSBE_OBJPATH (
    if not exist "%_ROSBE_OBJPATH%\." (
        echo ERROR: The path specified doesn't seem to exist.
        goto :EOC
    ) else (
        set ROS_INTERMEDIATE=%_ROSBE_OBJPATH%
    )
)
if defined _ROSBE_OUTPATH (
    if not exist "%_ROSBE_OUTPATH%\." (
        echo ERROR: The path specified doesn't seem to exist.
        goto :EOC
    ) else (
        set ROS_OUTPUT=%_ROSBE_OUTPATH%
        set ROS_TEMPORARY=%_ROSBE_OUTPATH%
    )
)

::
:: Get the current date and time for use in in our build log's file name.
::
call "%_ROSBE_BASEDIR%\TimeDate.cmd"

::
:: Check if writing logs is enabled, if so check if our log directory
:: exists, if it doesn't, create it.
::
if %_ROSBE_WRITELOG% == 1 (
    if not exist "%_ROSBE_LOGDIR%\." (
        mkdir "%_ROSBE_LOGDIR%" 1> NUL 2> NUL
    )
)

::
:: Check if we are using -j or not.
::
if "%1" == "multi" (
    title 'makex' parallel build started: %TIMERAW%
    call :BUILDMULTI %*
) else (
    title 'make' build started: %TIMERAW%
    call :BUILD %*
)
goto :EOC

:BUILD
if %_ROSBE_SHOWTIME% == 1 (
    if %_ROSBE_WRITELOG% == 1 (
        "%_ROSBE_BASEDIR%\Tools\buildtime.exe" "%_ROSBE_MINGWMAKE%" %* 2>&1 | "%_ROSBE_BASEDIR%\Tools\tee.exe" "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_ROSBE_BASEDIR%\Tools\buildtime.exe" "%_ROSBE_MINGWMAKE%" %*
    )
) else (
    if %_ROSBE_WRITELOG% == 1 (
        "%_ROSBE_MINGWMAKE%" %* 2>&1 | "%_ROSBE_BASEDIR%\Tools\tee.exe" "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_ROSBE_MINGWMAKE%" %*
    )
)
goto :EOF

:BUILDMULTI
::
:: Get the number of CPUs in the system so we know how many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, plus 1.
:: -x2       - Number of CPUs, doubled.
::
for /f "usebackq" %%i in (`"%_ROSBE_BASEDIR%\Tools\cpucount.exe" -x1`) do set CPUCOUNT=%%i

if %_ROSBE_SHOWTIME% == 1 (
    if %_ROSBE_WRITELOG% == 1 (
        "%_ROSBE_BASEDIR%\Tools\buildtime.exe" "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9 2>&1 | "%_ROSBE_BASEDIR%\Tools\tee.exe" "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_ROSBE_BASEDIR%\Tools\buildtime.exe" "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9
    )
) else (
    if %_ROSBE_WRITELOG% == 1 (
        "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9 2>&1 | "%_ROSBE_BASEDIR%\Tools\tee.exe" "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9
    )
)
goto :EOF

:EOC
::
:: Highlight the fact that building has ended.
::
"%_ROSBE_BASEDIR%\Tools\flash.exe"

title ReactOS Build Environment %_ROSBE_VERSION%

::
:: Unload all used Vars.
::
set ROS_LEAN_AND_MEAN=
set HOST_CC=
set HOST_CPP=
set TARGET_CC=
set TARGET_CPP=
set ROS_INTERMEDIATE=
set ROS_OUTPUT=
set ROS_TEMPORARY=
set CPUCOUNT=
set CCACHE_DIR=
