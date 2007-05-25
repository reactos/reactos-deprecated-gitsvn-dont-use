:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Perform the build of ReactOS.
@echo off

::
:: Get the current date and time for use in
:: in our build log's file name.
::
call "%ROSBEBASEDIR%\TimeDate.cmd"

::
:: Check if config.template.rbuild is newer than config.rbuild,
:: if it is then abort the build and inform the user.
::
if exist "%_ROSSOURCEDIR%\config.rbuild" (
    "%ROSBEBASEDIR%\Tools\test.exe" "%_ROSSOURCEDIR%\config.template.rbuild" -nt "%_ROSSOURCEDIR%\config.rbuild"
    if not errorlevel 1 (
        echo.
        echo *** config.template.rbuild is newer than config.rbuild ***
        echo *** aborting build. Please check for changes and       ***
        echo *** update your config.rbuild.                         ***
        echo.
        goto :EOF
    )
)

::
:: Now use mingw32-make to build ReactOS, passing along options, if any.
::
if "%1" == "" (
    "%ROSBEBASEDIR%\Tools\buildtime.exe" "%_MINGWMAKE%" 2>&1 | "%ROSBEBASEDIR%\Tools\tee.exe" "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
) else (
    "%ROSBEBASEDIR%\Tools\buildtime.exe" "%_MINGWMAKE%" %* 2>&1 | "%ROSBEBASEDIR%\Tools\tee.exe" "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
)

::
:: highlight the fact that building has ended
::
call "%ROSBEBASEDIR%\Tools\flash.exe"