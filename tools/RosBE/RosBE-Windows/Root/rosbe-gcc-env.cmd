::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/rosbe-gcc-env.cmd
:: PURPOSE:     Set up the GCC 4.x.x build environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
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

::
:: Set the Arch Variables
::

set ROS_ARCH=
set ROS_PREFIX=

if "%1" == "chdefgcc" (
    goto :main
)

if .%_ROSBE_ARCH%. == .1. (
    set ROS_ARCH=arm
    set ROS_PREFIX=arm-pc-mingw32
    REM set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\4.4.0
)
if .%_ROSBE_ARCH%. == .2. (
    set ROS_ARCH=ppc
    set ROS_PREFIX=ppc-pc-mingw32
    REM set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\4.4.0
)
if .%_ROSBE_ARCH%. == .3. (
    set ROS_ARCH=amd64
    set ROS_PREFIX=x86_64-pc-mingw32
    set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\4.4.0
)

:main

::
:: Set up the GCC 4.x.x build environment.
::
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
) else (
    set HOST_CFLAGS=-I"%_ROSBE_HOST_MINGWPATH%\include" -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include"
    set HOST_CPPFLAGS=-I"%_ROSBE_HOST_MINGWPATH%\include" -I"%_ROSBE_HOST_MINGWPATH%\include\c++\%_ROSBE_HOST_GCCVERSION%" -I"%_ROSBE_HOST_MINGWPATH%\include\c++\%_ROSBE_HOST_GCCVERSION%\%_ROSBE_HOST_GCCTARGET%" -I"%_ROSBE_HOST_MINGWPATH%\lib\gcc\%_ROSBE_HOST_GCCTARGET%\%_ROSBE_HOST_GCCVERSION%\include"
    set TARGET_CFLAGS=-I"%_ROSBE_TARGET_MINGWPATH%\include" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_GCCTARGET%\%_ROSBE_GCCVERSION%\include"
    set TARGET_CPPFLAGS=-I"%_ROSBE_TARGET_MINGWPATH%\include" -I"%_ROSBE_TARGET_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%" -I"%_ROSBE_TARGET_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%\%_ROSBE_GCCTARGET%" -I"%_ROSBE_TARGET_MINGWPATH%\lib\gcc\%_ROSBE_GCCTARGET%\%_ROSBE_GCCVERSION%\include"
)

::
:: Display the current version of GCC, NASM, ld and make.
::
if not .%ROS_ARCH%. == .. (
    "%_ROSBE_TARGET_MINGWPATH%\bin\%ROS_PREFIX%-gcc" -v 2>&1 | find "gcc version"
    if .%ROS_PREFIX%. == .. (
        echo gcc target^: %_ROSBE_GCCTARGET%
    ) else (
        echo gcc target^: %ROS_PREFIX:~0,-11%
    )
    "%_ROSBE_TARGET_MINGWPATH%\bin\%ROS_PREFIX%-ld" -v
) else (
    gcc -v 2>&1 | find "gcc version"
    if .%ROS_PREFIX%. == .. (
        echo gcc target^: %_ROSBE_GCCTARGET%
    ) else (
        echo gcc target^: %ROS_PREFIX:~0,-11%
    )
    ld -v
)

if exist "%_ROSBE_HOST_MINGWPATH%\bin\nasm.exe" (
    nasm -v
) else (
    if exist "%_ROSBE_HOST_MINGWPATH%\bin\yasm.exe" (
        yasm --version | find "yasm 0"
    ) else (
        if "%_ROSBE_MODE%" == "RosBE" (
            echo ERROR: NASM or YASM is required to build ReactOS, none found in the current MinGW/GCC.
        )
    )
)
if exist "%_ROSBE_HOST_MINGWPATH%\bin\bison.exe" (
    bison --version | find "GNU Bison"
) else (
    if "%_ROSBE_MODE%" == "RosBE" (
        echo WARNING: Bison will soon be required to build ReactOS, none found in the current MinGW/GCC.
    )
)
if exist "%_ROSBE_HOST_MINGWPATH%\bin\flex.exe" (
    flex --version
) else (
    if "%_ROSBE_MODE%" == "RosBE" (
        echo WARNING: Flex will soon be required to build ReactOS, none found in the current MinGW/GCC.
    )
)
mingw32-make -v | find "GNU Make"
