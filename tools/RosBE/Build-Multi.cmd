:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Perform the build of ReactOS, but use the -j option
::    with the appropriate number for the number of CPUs
::    in the system.
@echo off

::
:: Clear variables before use.
::
set CPUCOUNT=

::
:: Get the current date and time for use in
:: in our build log's file name.
::
call "%ROSBEBASEDIR%\TimeDate.cmd"

::
:: Get the number of CPUs in the system so we know how
:: many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, Plus 1.
:: -x2       - Number of CPUs, Doubled.
for /f "usebackq" %%i in (`%ROSBEBASEDIR%\Tools\cpucount.exe -x1`) do set CPUCOUNT=%%i

::
:: Check if config.template.rbuild is newer than config.rbuild,
:: if it is then abort the build and inform the user.
::
if exist "%_ROSSOURCEDIR%\config.rbuild" (
    %ROSBEBASEDIR%\Tools\test.exe "%_ROSSOURCEDIR%\config.template.rbuild" -nt "%_ROSSOURCEDIR%\config.rbuild"
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
    %ROSBEBASEDIR%\Tools\buildtime.exe %_MINGWMAKE% -j %CPUCOUNT% 2>&1 | %ROSBEBASEDIR%\Tools\tee.exe "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
) else (
    %ROSBEBASEDIR%\Tools\buildtime.exe %_MINGWMAKE% -j %CPUCOUNT% %* 2>&1 | %ROSBEBASEDIR%\Tools\tee.exe "%_ROSBELOGDIR%\BuildLog-%_MINGWVERSION%-%DATENAME%-%TIMENAME%.txt"
)
