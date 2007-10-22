::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/RosBE.cmd
:: PURPOSE:     This script provides/sets up various build environments for
::              ReactOS. Currently it provides a GCC 4.1.3 build environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Set defaults to work with and override them if edited by
:: the options utility.
::
color 0A
set _ROSBE_VERSION=1.0
set _ROSBE_BASEDIR=%~dp0
set _ROSBE_BASEDIR=%_ROSBE_BASEDIR:~0,-1%
set _ROSBE_ROSSOURCEDIR=%CD%
set _ROSBE_PREVIOUSSOURCEDIR=""
set _ROSBE_SHOWTIME=1
set _ROSBE_WRITELOG=1
set _ROSBE_USECCACHE=0
set _ROSBE_STRIP=0
set _ROSBE_MINGWPATH=%_ROSBE_BASEDIR%\4.1.3
set _ROSBE_LOGDIR=%CD%\RosBE-Logs

::
:: Check if the user has used the options utility and
:: if so, load their options.
::
if exist "%_ROSBE_BASEDIR%\rosbe-options.cmd" (
    call "%_ROSBE_BASEDIR%\rosbe-options.cmd"
)

title ReactOS Build Environment %_ROSBE_VERSION%

::
:: Make sure RosBE was initialized right.
::
if "%1" == "oldmode" (
    cls
    set _ROSBE_OLDMODE=""
    call :RosBE4
    goto :EndCommandParse
)
if not "%1" == "" (
    cls
    echo Unknown parameter specified. Exiting.
    goto :ExitRosBE
)

cls
call :RosBE4

:EndCommandParse

::
:: Load the Base Directory from the Shortcut-txt and set it as new
:: Source Directory and add PATH for Tools Folder.
::
set PATH=%_ROSBE_BASEDIR%\Tools;%PATH%
if exist "%_ROSBE_BASEDIR%\scut.cmd" (call "%_ROSBE_BASEDIR%\scut.cmd" run)

::
:: Display the current version of GCC, NASM, ld and make.
::
gcc -v 2>&1 | find "gcc version"
nasm -v
ld -v
mingw32-make -v | find "GNU Make"

::
:: Tell how to display the available commands.
::
echo.
echo For a list of all included commands, type: "help"
echo -------------------------------------------------
echo.

::
:: Load the doskey macros that serve as our commands.
::
doskey /macrofile="%_ROSBE_BASEDIR%\RosBE.mac"

::
:: Look if the Source Folder is empty. If so, ask for using "ssvn create".
::
dir /b "%_ROSBE_ROSSOURCEDIR%" 2>nul | findstr "." >nul
if errorlevel 1 (
    echo No ReactOS Source detected. Please use "ssvn create" to download it.
)

goto :ExitRosBE

::
:: Display the banner and set up the environment for the GCC 4.x.x build
:: environment.
::
:RosBE4
    ::
    :: Set the correct path for the build tools and set the MinGW make.
    ::
    set _ROSBE_GCCVERSION=4.1.3
    set PATH=%_ROSBE_MINGWPATH%\bin;%_ROSBE_MINGWPATH%\libexec\gcc\mingw32\4.1.3;%PATH%
    set _ROSBE_MINGWMAKE=%_ROSBE_MINGWPATH%\bin\mingw32-make.exe
    if defined _ROSBE_OLDMODE (
        set C_INCLUDE_PATH=%_ROSBE_MINGWPATH%\include;%_ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include
        set CPLUS_INCLUDE_PATH=%_ROSBE_MINGWPATH%\include;%_ROSBE_MINGWPATH%\include\c++\4.1.3;%_ROSBE_MINGWPATH%\include\c++\4.1.3\mingw32;%_ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include
    )
    set HOST_CFLAGS=-I"%_ROSBE_MINGWPATH%\include" -I"%_ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include"
    set HOST_CPPFLAGS=-I"%_ROSBE_MINGWPATH%\include" -I"%_ROSBE_MINGWPATH%\include\c++\4.1.3" -I"%_ROSBE_MINGWPATH%\include\c++\4.1.3\mingw32" -I"%_ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include"
    set LIBRARY_PATH=%_ROSBE_MINGWPATH%\lib;%_ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3

    echo *******************************************************************************
    echo *                                                                             *
    echo *        ReactOS Build Environment %_ROSBE_VERSION%-4.1.3-20070703 (prerelease)    *
    echo *                                                                             *
    echo *******************************************************************************
    echo.
goto :EOF

:ExitRosBE
