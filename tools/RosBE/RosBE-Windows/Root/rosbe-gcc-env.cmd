::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/rosbe-gcc-env.cmd
:: PURPOSE:     Set up toolchain-specific settings when initializing RosBE and when using "charch" or "chdefgcc"
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::                             Colin Finck <colin@reactos.org>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

:: Check if we're switching to the AMD64 or ARM architecture.
if "%ROS_ARCH%" == "amd64" (
    set ROS_PREFIX=x86_64-w64-mingw32
) else if "%ROS_ARCH%" == "arm" (
    set ROS_PREFIX=arm-mingw32ce
) else (
    set ROS_PREFIX=
)

if "%ROS_PREFIX%" == "" (
    set _ROSBE_PREFIX=
) else (
    set _ROSBE_PREFIX=%ROS_PREFIX%-
)

set PATH=%_ROSBE_HOST_MINGWPATH%\bin;%_ROSBE_TARGET_MINGWPATH%\bin;%_ROSBE_ORIGINALPATH%

for /f "usebackq tokens=3" %%i in (`"%_ROSBE_PREFIX%gcc -v 2>&1 | find "gcc version""`) do set _ROSBE_TARGET_GCCVERSION=%%i
for /f "usebackq tokens=2" %%i in (`"%_ROSBE_PREFIX%gcc -v 2>&1 | find "Target""`) do set _ROSBE_TARGET_GCCTARGET=%%i
for /f "usebackq tokens=3" %%i in (`"gcc -v 2>&1 | find "gcc version""`) do set _ROSBE_HOST_GCCVERSION=%%i
for /f "usebackq tokens=2" %%i in (`"gcc -v 2>&1 | find "Target""`) do set _ROSBE_HOST_GCCTARGET=%%i

set ROSBE_HOST_CFLAGS=-I"%_ROSBE_HOST_MINGWPATH%\%_ROSBE_HOST_GCCTARGET%\include" -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include"
set ROSBE_HOST_CXXFLAGS=-I"%_ROSBE_HOST_MINGWPATH%\%_ROSBE_HOST_GCCTARGET%\include" -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include\c++"  -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include"
set ROSBE_TARGET_CFLAGS=-I"%_ROSBE_TARGET_MINGWPATH%\%_ROSBE_TARGET_GCCTARGET%\include" -I"%_ROSBE_TARGET_MINGWPATH%\include" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_TARGET_GCCTARGET%\%_ROSBE_TARGET_GCCVERSION%\include"
set ROSBE_TARGET_CXXFLAGS=-I"%_ROSBE_TARGET_MINGWPATH%\%_ROSBE_TARGET_GCCTARGET%\include" -I"%_ROSBE_TARGET_MINGWPATH%\include" -I"%_ROSBE_TARGET_MINGWPATH%\include\c++\%_ROSBE_TARGET_GCCVERSION%" -I"%_ROSBE_TARGET_MINGWPATH%\include\c++\%_ROSBE_TARGET_GCCVERSION%\%_ROSBE_TARGET_GCCTARGET%" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_TARGET_GCCTARGET%\%_ROSBE_TARGET_GCCVERSION%\include" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_TARGET_GCCTARGET%\%_ROSBE_TARGET_GCCVERSION%\include\c++" -I"%_ROSBE_TARGET_MINGWPATH%\%_ROSBE_TARGET_GCCTARGET%\include\c++\%_ROSBE_TARGET_GCCVERSION%" -I"%_ROSBE_TARGET_MINGWPATH%\%_ROSBE_TARGET_GCCTARGET%\include\c++\%_ROSBE_TARGET_GCCVERSION%\%_ROSBE_TARGET_GCCTARGET%"
