::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
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
if exist %ROSBEBASEDIR%\options.cmd (
    call %ROSBEBASEDIR%\options.cmd
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
:: Clear variables before use.
::
set CPUCOUNT=

::
:: Get the number of CPUs in the system so we know how many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, Plus 1.
:: -x2       - Number of CPUs, Doubled.
::
for /f "usebackq" %%i in (`cpucount -x1`) do set CPUCOUNT=%%i

::
:: Check if strip or ccache are being used and set the appropriate options.
::
if defined ROSBE_STRIP (
    if %ROSBE_STRIP% == 1 (
        set ROS_LEAN_AND_MEAN=yes
    ) else (
        set ROS_LEAN_AND_MEAN=no
    )
)
if defined ROSBE_USECCACHE (
    if %ROSBE_USECCACHE% == 1 (
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
call "%ROSBEBASEDIR%\TimeDate.cmd"

::
:: Check if writing logs is enabled, if so check if our log directory
:: exists, if it doesn't, create it, finally if creating the
:: directory fails then fall back on the current directory.
::
if %ROSBE_WRITELOG% == 1 (
    if not exist "_ROSBELOGDIR\." (
        mkdir "%_ROSBELOGDIR%" 1> NUL 2> NUL
        if errorlevel 1 (
            echo *** Error - Writing logs requested, but log directory doesn't ***
            echo *** exist and can't be created. Logs will be created in the   ***
            echo *** current directory as a fallback.                          ***
            set _ROSBELOGDIR=%CD%
        )
    )
)

::
:: Check if we are using -j or not.
::
if "%1" == "multi" (
    title Started: %TIMERAW%, Building...
    goto :BUILDMULTI
) else (
    title Started: %TIMERAW%, Building...
    goto :BUILD
)

:BUILD
if %ROSBE_SHOWTIME% == 1 (
    if %ROSBE_WRITELOG% == 1 (
        buildtime "%_MINGWMAKE%" %* 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        buildtime "%_MINGWMAKE%" %*
    )
) else (
    if %ROSBE_WRITELOG% == 1 (
        "%_MINGWMAKE%" %* 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_MINGWMAKE%" %*
    )
)
goto :EOB

:BUILDMULTI
if %ROSBE_SHOWTIME% == 1 (
    if %ROSBE_WRITELOG% == 1 (
        buildtime "%_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        buildtime "%_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9
    )
) else (
    if %ROSBE_WRITELOG% == 1 (
        "%_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9 2>&1 | tee "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
    ) else (
        "%_MINGWMAKE%" -j %CPUCOUNT% %2 %3 %4 %5 %6 %7 %8 %9
    )
)

:EOB
::
:: Highlight the fact that building has ended.
::
flash

title ReactOS Build Environment %_VER%
