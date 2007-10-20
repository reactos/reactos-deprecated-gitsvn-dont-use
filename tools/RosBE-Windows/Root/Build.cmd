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

title Building...

::
:: Check if the user has used the options utility and
:: if so, load their options.
::
if exist %_ROSBE_BASEDIR%\options.cmd (
    call %_ROSBE_BASEDIR%\options.cmd
)

::
:: Check if config.template.rbuild is newer than config.rbuild, if it is then
:: abort the build and inform the user.
::
if exist "config.rbuild" (
    test "config.template.rbuild" -nt "config.rbuild"
    if not errorlevel 1 (
        echo.
        echo *** config.template.rbuild is newer than config.rbuild ***
        echo *** aborting build. Please update your config.rbuild   *** 
        echo *** by typeing "config update".                        ***
        echo.
        goto :EOB
    )
)

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
:: Get the current date and time for use in in our build log's file name.
::
call "%_ROSBE_BASEDIR%\TimeDate.cmd"

::
:: Check if writing logs is enabled, if so check if our log directory
:: exists, if it doesn't, create it, finally if creating the
:: directory fails then fall back on the current directory.
::
if %_ROSBE_WRITELOG% == 1 (
    if not exist "_ROSBE_LOGDIR\." (
        mkdir "%_ROSBE_LOGDIR%" 1> NUL 2> NUL
        if errorlevel 1 (
            echo *** Error - Writing logs requested, but log directory doesn't ***
            echo *** exist and can't be created. Logs will be created in the   ***
            echo *** current directory as a fallback.                          ***
            set _ROSBE_LOGDIR=%CD%
        )
    )
)

::
:: Check if we are using -j or not.
::
title Started: %TIMERAW%, Building...
if "%1" == "multi" (
    goto :BUILDMULTI
) else (
    goto :BUILD
)

:BUILD
if %_ROSBE_SHOWTIME% == 1 (
    if %_ROSBE_WRITELOG% == 1 (
        buildtime "%_ROSBE_MINGWMAKE%" %* 2>&1 | tee "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        buildtime "%_ROSBE_MINGWMAKE%" %*
    )
) else (
    if %_ROSBE_WRITELOG% == 1 (
        "%_ROSBE_MINGWMAKE%" %* 2>&1 | tee "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_ROSBE_MINGWMAKE%" %*
    )
)
goto :EOB

:BUILDMULTI
::
:: Get the number of CPUs in the system so we know how many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, Plus 1.
:: -x2       - Number of CPUs, Doubled.
::
set CPUCOUNT=
for /f "usebackq" %%i in (`cpucount -x1`) do set CPUCOUNT=%%i

if %_ROSBE_SHOWTIME% == 1 (
    if %_ROSBE_WRITELOG% == 1 (
        buildtime "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9 2>&1 | tee "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        buildtime "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9
    )
) else (
    if %_ROSBE_WRITELOG% == 1 (
        "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9 2>&1 | tee "%_ROSBE_LOGDIR%\BuildLog-%_ROSBE_GCCVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_ROSBE_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9
    )
)

:EOB
::
:: Highlight the fact that building has ended.
::
flash

title ReactOS Build Environment %_ROSBE_VERSION%
