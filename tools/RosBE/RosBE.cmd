:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    This script provides/sets up various build environments for
::    ReactOS. Currently it provides GCC 3.4.5 and 4.1.2 build
::    environments.
@echo off

if "%1" == "" (
    echo Required parameter not specified. Exiting.
    goto :ExitRosBE
)
::if "%1" == "--RosBE3" (
::    color 0A
::    cls
::    call :RosBE3
::    goto :EndCommandParse
::)
if "%1" == "--RosBE4" (
    color 0A
    cls
    call :RosBE4
    goto :EndCommandParse
)
if not "%1" == "" (
    echo Unknown parameter specified. Exiting.
    goto :ExitRosBE
)
:EndCommandParse

::
:: Save our initial directory (should be the ReactOS
:: source directory)
::
set _ROSSOURCEDIR=%CD%

::
:: Check if our log directory exists, if it doesn't
:: create it.
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
:: Display the banner and set up the environment for
:: the GCC 3.x.x build environment.
::
:RosBE3
    ::
    :: Set the correct path for the build tools and set
    :: the MinGW make.
    ::
    set PATH=%ROSBEBASEDIR%\3.4.5\bin;%ROSBEBASEDIR%\3.4.5\libexec\gcc\mingw32\3.4.5;%PATH%
    set _MINGWMAKE=%ROSBEBASEDIR%\3.4.5\bin\mingw32-make.exe
    set _MINGWVERSION=3.4.5
    :: Set all paths (needed by Windows Vista)
    set C_INCLUDE_PATH=%ROSBEBASEDIR%\3.4.5\include;%ROSBEBASEDIR%\3.4.5\lib\gcc\mingw32\3.4.5\include
    set CPLUS_INCLUDE_PATH=%ROSBEBASEDIR%\3.4.5\include;%ROSBEBASEDIR%\3.4.5\include\c++\3.4.5;%ROSBEBASEDIR%\3.4.5\include\c++\3.4.5\mingw32;%ROSBEBASEDIR%\3.4.5\lib\gcc\mingw32\3.4.5\include
    set LIBRARY_PATH=%ROSBEBASEDIR%\3.4.5\lib;%ROSBEBASEDIR%\3.4.5\lib\gcc\mingw32\3.4.5

    echo *******************************************************************************
    echo *                                                                             *
    echo *                 ReactOS Build Environment 0.3.5b2-3.4.5                     *
    echo *                                                                             *
    echo *******************************************************************************
    echo.
goto :EOF

::
:: Display the banner and set up the environment for
:: the GCC 4.x.x build environment.
::
:RosBE4
    ::
    :: Set the correct path for the build tools and set
    :: the MinGW make.
    ::
    set PATH=%ROSBEBASEDIR%\4.1.2\bin;%ROSBEBASEDIR%\4.1.2\libexec\gcc\mingw32\4.1.2;%PATH%
    set _MINGWMAKE=%ROSBEBASEDIR%\4.1.2\bin\mingw32-make.exe
    set _MINGWVERSION=4.1.2
    :: Set all paths (needed by Windows Vista)
    set C_INCLUDE_PATH=%ROSBEBASEDIR%\4.1.2\include;%ROSBEBASEDIR%\4.1.2\lib\gcc\mingw32\4.1.2\include
    set CPLUS_INCLUDE_PATH=%ROSBEBASEDIR%\4.1.2\include;%ROSBEBASEDIR%\4.1.2\include\c++\4.1.2;%ROSBEBASEDIR%\4.1.2\include\c++\4.1.2\mingw32;%ROSBEBASEDIR%\4.1.2\lib\gcc\mingw32\4.1.2\include
    set LIBRARY_PATH=%ROSBEBASEDIR%\4.1.2\lib;%ROSBEBASEDIR%\4.1.2\lib\gcc\mingw32\4.1.2

    echo *******************************************************************************
    echo *                                                                             *
    echo *     ReactOS Build Environment 0.3.5b2-4.1.2-20070122-prerelease-patched     *
    echo *                                                                             *
    echo *******************************************************************************
    echo.
goto :EOF

:ExitRosBE
