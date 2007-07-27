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

title Building...

::
:: Take over the 2nd parameter of Build.cmd, which enables/disables stripping
::
if "%1" == "strip" (
    set ROS_LEAN_AND_MEAN = yes
    goto :Build
)
if "%1"  == "" (
    set ROS_LEAN_AND_MEAN = no
    goto :Build
)

:Build

::
:: Check if our log directory exists, if it doesn't, create it.
::
if %logdiff% == 0 (
if not exist "%CD%\RosBE-Logs\." (
        if not exist "%CD%\.svn\." (
            echo Folder is empty. No Build can be performed.
            goto :EOB
        ) else (
            mkdir "%CD%\RosBE-Logs"
            set _ROSBELOGDIR=%CD%\RosBE-Logs
        )
    ) else (
        set _ROSBELOGDIR=%CD%\RosBE-Logs
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
if exist "%CD%\config.rbuild" (
    test "%CD%\config.template.rbuild" -nt "%CD%\config.rbuild"
    if not errorlevel 1 (
        echo.
        echo *** config.template.rbuild is newer than config.rbuild ***
        echo *** aborting build. Please check for changes and       ***
        echo *** update your config.rbuild.                         ***
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
if %ROSBE_SHOWTIME% == 1 (
    if %ROSBE_WRITELOG% == 1 (
        call buildtime "%MAKE_COMMAND%" 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    )
    if %ROSBE_WRITELOG% == 0 (
        call buildtime "%MAKE_COMMAND%" 2>&1
    )
)
if %ROSBE_SHOWTIME% == 0 (
    if %ROSBE_WRITELOG% == 1 (
        call "%MAKE_COMMAND%" 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    )
    if %ROSBE_WRITELOG% == 0 (
        call "%MAKE_COMMAND%" 2>&1
    )
)

::
:: Highlight the fact that building has ended.
::
call flash

goto :EOB

:EOB
title ReactOS Build Environment %_VER%
