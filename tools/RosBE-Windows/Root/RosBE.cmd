:: Copyright (c) Peter Ward and Daniel Reimer. All rights reserved.
::
:: This script provides/sets up various build environments for
:: ReactOS. Currently it provides GCC 4.1.3 build environments.
::
@echo off

title ReactOS Build Environment 0.3.6

if "%1" == "" (
    color 0A
    cls
    call :RosBE4
    goto :EndCommandParse
)
if not "%1" == "" (
    color 0A
    cls
    echo Unknown parameter specified. Exiting.
    goto :ExitRosBE
)
:EndCommandParse

::
:: Save our initial directory (should be the ReactOS source directory)
::
set _ROSSOURCEDIR=%CD%

::
:: Check if our log directory exists, if it doesn't, create it.
::
if not exist "%_ROSSOURCEDIR%\RosBE-Logs\." (
    mkdir "%_ROSSOURCEDIR%\RosBE-Logs"
    set _ROSBELOGDIR=%_ROSSOURCEDIR%\RosBE-Logs
) else (
    set _ROSBELOGDIR=%_ROSSOURCEDIR%\RosBE-Logs
)

::
:: Display the current version of GCC, NASM, ld and make.
::
gcc -v 2>&1 | find "gcc version"
nasm -v
ld -v
mingw32-make -v | find "GNU Make"

::
:: Display the available commands
::
call "%ROSBEBASEDIR%\Help.cmd"

::
:: Load the macros that serve as our commands.
::
doskey /macrofile="%ROSBEBASEDIR%\RosBE.mac"

goto :ExitRosBE

::
:: Display the banner and set up the environment for the GCC 4.x.x build environment.
::
:RosBE4
    ::
    :: Set the correct path for the build tools and set the MinGW make.
    ::
    set PATH=%ROSBEBASEDIR%\4.1.3\bin;%ROSBEBASEDIR%\4.1.3\libexec\gcc\mingw32\4.1.3;%PATH%
    set _MINGWMAKE=%ROSBEBASEDIR%\4.1.3\bin\mingw32-make.exe
    set _MINGWVERSION=4.1.3
    :: Set all paths (needed by Windows Vista)
    set C_INCLUDE_PATH=%ROSBEBASEDIR%\4.1.3\include;%ROSBEBASEDIR%\4.1.3\lib\gcc\mingw32\4.1.3\include
    set CPLUS_INCLUDE_PATH=%ROSBEBASEDIR%\4.1.3\include;%ROSBEBASEDIR%\4.1.3\include\c++\4.1.3;%ROSBEBASEDIR%\4.1.3\include\c++\4.1.3\mingw32;%ROSBEBASEDIR%\4.1.3\lib\gcc\mingw32\4.1.3\include
    set LIBRARY_PATH=%ROSBEBASEDIR%\4.1.3\lib;%ROSBEBASEDIR%\4.1.3\lib\gcc\mingw32\4.1.3

    echo *******************************************************************************
    echo *                                                                             *
    echo *     ReactOS Build Environment 0.3.6-4.1.3-20070405-prerelease-patched       *
    echo *                                                                             *
    echo *******************************************************************************
    echo.
goto :EOF

:ExitRosBE
