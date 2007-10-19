::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/Build-Shared.cmd
:: PURPOSE:     Shared actions for "Build.cmd" and "Build-Multi.cmd".
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <mail@colinfinck.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Take over the 2nd parameter of Build.cmd, which enables/disables stripping.
::

if "%1" == "strip" (
    set ROS_LEAN_AND_MEAN=yes
    goto :Build
)
if "%1" == "ccache" (
    set ROS_LEAN_AND_MEAN=no
    set HOST_CC=ccache gcc
    set HOST_CPP=ccache g++
    set TARGET_CC=ccache gcc
    set TARGET_CPP=ccache g++
    goto :Build
)
if "%2" == "strip" (
    set ROS_LEAN_AND_MEAN=yes
    goto :Build
)
if "%2" == "ccache" (
    set HOST_CC=ccache gcc
    set HOST_CPP=ccache g++
    set TARGET_CC=ccache gcc
    set TARGET_CPP=ccache g++
    goto :Build
)
if "%1"  == "" (
    set ROS_LEAN_AND_MEAN=no
    goto :Build
)

:Build

::
:: Check if our log directory exists, if it doesn't, create it. Also check
:: if the default Path was editied by Config Tool. If so, update the
:: path.
::
if %logdiff% == 0 (
if not exist "RosBE-Logs\." (
        if not exist "Makefile" (
            echo Folder is empty. No Build can be performed without a Source Code.
            goto :EOB
        ) else (
            mkdir "RosBE-Logs"
            set _ROSBELOGDIR=RosBE-Logs
        )
    ) else (
        set _ROSBELOGDIR=RosBE-Logs
    )
)
if %logdiff% == 1 (
   if not exist "%ROSBE_LOGPATH%\." (
        echo LogFolder does not exist.
   ) else (
        set _ROSBELOGDIR=%ROSBE_LOGPATH%
   )
   )
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
:: Get the current date and time for use in in our build log's file name.
::
call "%ROSBEBASEDIR%\TimeDate.cmd"

::
:: Now use mingw32-make to build ReactOS, passing along options, if any.
::
title Started: %TIMERAW%, Building...

if %ROSBE_SHOWTIME% == 1 (
    if %ROSBE_WRITELOG% == 1 (
        buildtime "%MAKE_COMMAND%" 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    )
    if %ROSBE_WRITELOG% == 0 (
        buildtime "%MAKE_COMMAND%" 2>&1
    )
)
if %ROSBE_SHOWTIME% == 0 (
    if %ROSBE_WRITELOG% == 1 (
        "%MAKE_COMMAND%" 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    )
    if %ROSBE_WRITELOG% == 0 (
        "%MAKE_COMMAND%" 2>&1
    )
)

::
:: Highlight the fact that building has ended.
::
flash

:EOB
title ReactOS Build Environment %_VER%
