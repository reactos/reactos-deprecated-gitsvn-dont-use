::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/RosBE.cmd
:: PURPOSE:     This script provides/sets up various build environments for
::              ReactOS. Currently it provides GCC 4.1.3 build environments.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Set Defaults to work with and override them if edited by
:: Config Tool.
::
color 0A
set _VER=0.X.X.X
set ROSBEBASEDIR=%~dp0
for /f "usebackq" %%i in (`"echo %ROSBEBASEDIR%|%ROSBEBASEDIR%Tools\sed s/.$//g"`) do @SET ROSBEBASEDIR=%%i
set _ROSSOURCEDIR=%CD%
set ROSBE_SHOWTIME=1
set ROSBE_WRITELOG=1
set ROSBE_USECCACHE=0
set ROSBE_STRIP=0
set ROSBE_MINGWPATH=%ROSBEBASEDIR%\4.1.3
set _ROSBELOGDIR=%CD%\RosBE-Logs

::
:: Check if the user has used the options utility and
:: if so, load their options.
::
if exist %ROSBEBASEDIR%\options.cmd (
    call %ROSBEBASEDIR%\options.cmd
)

title ReactOS Build Environment %_VER%

::
:: Make sure RosBE was initialized right.
::
if "%1" == "oldmode" (
    cls
    set C_INCLUDE_PATH=%ROSBE_MINGWPATH%\include;%ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include
    set CPLUS_INCLUDE_PATH=%ROSBE_MINGWPATH%\include;%ROSBE_MINGWPATH%\include\c++\4.1.3;%ROSBE_MINGWPATH%\include\c++\4.1.3\mingw32;%ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include
    call :RosBE4
    goto :EndCommandParse
)
if not "%1" == "" (
    cls
    echo Unknown parameter specified. Exiting.
    goto :ExitRosBE
)

:EndCommandParse

::
:: Load the Base Directory from the Shortcut-XML and set it as new
:: Source Directory and add PATH for Tools Folder.
::
set PATH=%ROSBEBASEDIR%\Tools;%PATH%
for /f "usebackq" %%i in (`"grep base= "%ROSBEBASEDIR%\srclist.xml"|cutz main"`) do @SET scut=%%i
echo Used Shortcut: %scut%
echo.

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

::
:: Load the doskey macros that serve as our commands.
::
doskey /macrofile="%ROSBEBASEDIR%\RosBE.mac"

::
:: Set new source directory, if needed.
::
if not "%scut%" == "Default" (
    call %ROSBEBASEDIR%\scut %scut%
)

::
:: Look if the Source Folder is empty. If so, ask for using "svn create".
::
dir /b "%_ROSSOURCEDIR%" 2>nul|findstr "." >nul
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
    set PATH=%ROSBE_MINGWPATH%\bin;%ROSBE_MINGWPATH%\libexec\gcc\mingw32\4.1.3;%PATH%
    set _MINGWMAKE=%ROSBE_MINGWPATH%\bin\mingw32-make.exe
    set _MINGWVERSION=4.1.3
    set HOST_CFLAGS=-I"%ROSBE_MINGWPATH%\include" -I"%ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include"
    set HOST_CPPFLAGS=-I"%ROSBE_MINGWPATH%\include" -I"%ROSBE_MINGWPATH%\include\c++\4.1.3" -I"%ROSBE_MINGWPATH%\include\c++\4.1.3\mingw32" -I"%ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3\include"
    set LIBRARY_PATH=%ROSBE_MINGWPATH%\lib;%ROSBE_MINGWPATH%\lib\gcc\mingw32\4.1.3

    echo *******************************************************************************
    echo *                                                                             *
    echo *        ReactOS Build Environment %_VER%-4.1.3-20070703 (prerelease)        *
    echo *                                                                             *
    echo *******************************************************************************
    echo.
goto :EOF

:ExitRosBE
