::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/rosbe-gcc-env.cmd
:: PURPOSE:     Set up the GCC 4.x.x build environment.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

::
:: Check if we are running within the RosBE, and if not
:: initialize GCC for the current directory.
::
if not defined _ROSBE_HOST_MINGWPATH (
    set _ROSBE_MODE=MinGW
    set _ROSBE_HOST_MINGWPATH=%CD%
    set _ROSBE_ORIGINALPATH=%PATH%
)

if "%1" == "chdefgcc" (
    goto :main
)

set _ROSBE_OBJPATH=%_ROSBE_i386_OBJPATH%
set _ROSBE_OUTPATH=%_ROSBE_i386_OUTPATH%

if not "%ROS_ARCH%" == "" (
    set ROS_PREFIX=%ROS_ARCH%-pc-mingw32
    set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\%ROS_ARCH%

    if "%ROS_ARCH%" == "arm" (
        set _ROSBE_OBJPATH=%_ROSBE_ARM_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_ARM_OUTPATH%
    )
    if "%ROS_ARCH%" == "ppc" (
        set _ROSBE_OBJPATH=%_ROSBE_PPC_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_PPC_OUTPATH%
    )
    if "%ROS_ARCH%" == "amd64" (
        set _ROSBE_OBJPATH=%_ROSBE_AMD64_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_AMD64_OUTPATH%
        set ROS_PREFIX=x86_64-pc-mingw32
        set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\x86_64
    )
)

REM Check if existant arch

if not exist "%_ROSBE_TARGET_MINGWPATH%\." (   
    echo Unsupported arch specified. Fallback to Default.
    pause
    set _ROSBE_OBJPATH=%_ROSBE_i386_OBJPATH%
    set _ROSBE_OUTPATH=%_ROSBE_i386_OUTPATH%
    set ROS_ARCH=
    set ROS_PREFIX=
    set _ROSBE_TARGET_MINGWPATH=%_ROSBE_HOST_MINGWPATH%
)

REM HAXX

::
:: Set up the GCC 4.x.x build environment.
::

:main

set PATH=%_ROSBE_HOST_MINGWPATH%\bin;%_ROSBE_TARGET_MINGWPATH%\bin;%_ROSBE_ORIGINALPATH%
set _ROSBE_MINGWMAKE=%_ROSBE_HOST_MINGWPATH%\bin\mingw32-make.exe

if not .%ROS_ARCH%. == .. (
for /f "usebackq tokens=3" %%i in (`"%ROS_PREFIX%-gcc -v 2>&1 | find "gcc version""`) do set _ROSBE_GCCVERSION=%%i
for /f "usebackq tokens=2" %%i in (`"%ROS_PREFIX%-gcc -v 2>&1 | find "Target""`) do set _ROSBE_GCCTARGET=%%i
) else (
for /f "usebackq tokens=3" %%i in (`"gcc -v 2>&1 | find "gcc version""`) do set _ROSBE_GCCVERSION=%%i
for /f "usebackq tokens=2" %%i in (`"gcc -v 2>&1 | find "Target""`) do set _ROSBE_GCCTARGET=%%i
)

for /f "usebackq tokens=3" %%i in (`"gcc -v 2>&1 | find "gcc version""`) do set _ROSBE_HOST_GCCVERSION=%%i
for /f "usebackq tokens=2" %%i in (`"gcc -v 2>&1 | find "Target""`) do set _ROSBE_HOST_GCCTARGET=%%i

if "%_ROSBE_MODE%" == "MinGW" (
    set C_INCLUDE_PATH=%_ROSBE_HOST_MINGWPATH%\include;%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_GCCTARGET%\%_ROSBE_GCCVERSION%\include
    set CPLUS_INCLUDE_PATH=%_ROSBE_HOST_MINGWPATH%\include;%_ROSBE_HOST_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%;%_ROSBE_HOST_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%\%_ROSBE_GCCTARGET%;%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_GCCTARGET%\%_ROSBE_GCCVERSION%\include
    set ROSBE_HOST_CFLAGS=
    set ROSBE_HOST_CXXFLAGS=
    set ROSBE_TARGET_CFLAGS=
    set ROSBE_TARGET_CXXFLAGS=

) else (
    set C_INCLUDE_PATH=
    set CPLUS_INCLUDE_PATH=
    set ROSBE_HOST_CFLAGS=-I"%_ROSBE_HOST_MINGWPATH%\include" -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include"
    set ROSBE_HOST_CXXFLAGS=-I"%_ROSBE_HOST_MINGWPATH%\include" -I"%_ROSBE_HOST_MINGWPATH%\include\c++\%_ROSBE_HOST_GCCVERSION%" -I"%_ROSBE_HOST_MINGWPATH%\include\c++\%_ROSBE_HOST_GCCVERSION%\%_ROSBE_HOST_GCCTARGET%" -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include"
    set ROSBE_TARGET_CFLAGS=-I"%_ROSBE_TARGET_MINGWPATH%\include" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_GCCTARGET%\%_ROSBE_GCCVERSION%\include"
    set ROSBE_TARGET_CXXFLAGS=-I"%_ROSBE_TARGET_MINGWPATH%\include" -I"%_ROSBE_TARGET_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%" -I"%_ROSBE_TARGET_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%\%_ROSBE_GCCTARGET%" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_GCCTARGET%\%_ROSBE_GCCVERSION%\include"
)
if "%_ROSBE_HOST_GCCVERSION%" == "3.4.5" (
    set PATH=%_ROSBE_BASEDIR%\3.4.5\bin;%_ROSBE_BASEDIR%\3.4.5\libexec\gcc\mingw32\3.4.5;%PATH%
    set _ROSBE_MINGWMAKE=%_ROSBE_BASEDIR%\3.4.5\bin\mingw32-make.exe
    :: Set all paths (needed by Windows Vista)
    set C_INCLUDE_PATH=%_ROSBE_BASEDIR%\3.4.5\include;%_ROSBE_BASEDIR%\3.4.5\lib\gcc\mingw32\3.4.5\include
    set CPLUS_INCLUDE_PATH=%_ROSBE_BASEDIR%\3.4.5\include;%_ROSBE_BASEDIR%\3.4.5\include\c++\3.4.5;%_ROSBE_BASEDIR%\3.4.5\include\c++\3.4.5\mingw32;%_ROSBE_BASEDIR%\3.4.5\lib\gcc\mingw32\3.4.5\include
    set LIBRARY_PATH=%_ROSBE_BASEDIR%\3.4.5\lib;%_ROSBE_BASEDIR%\3.4.5\lib\gcc\mingw32\3.4.5
    set ROSBE_HOST_CFLAGS=
    set ROSBE_HOST_CXXFLAGS=
    set ROSBE_TARGET_CFLAGS=
    set ROSBE_TARGET_CXXFLAGS=
)

set LIBRARY_PATH=%_ROSBE_HOST_MINGWPATH%\lib;%_ROSBE_HOST_MINGWPATH%\lib\gcc\mingw32\%_ROSBE_GCCVERSION%

:: HAX !!!!
set HOST_CFLAGS=%ROSBE_HOST_CFLAGS%
set HOST_CPPFLAGS=%ROSBE_HOST_CXXFLAGS%
set TARGET_CFLAGS=%ROSBE_TARGET_CFLAGS%
set TARGET_CPPFLAGS=%ROSBE_TARGET_CXXFLAGS%
