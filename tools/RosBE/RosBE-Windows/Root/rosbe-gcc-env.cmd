::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/rosbe-gcc-env.cmd
:: PURPOSE:     Set up toolchain-specific settings when initializing RosBE and when using "charch" or "chdefgcc"
:: COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
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

for /f "usebackq tokens=*" %%i in (`"cpp -x c -v <NUL 2>&1 | getincludes"`) do set ROSBE_HOST_CFLAGS=%%i
for /f "usebackq tokens=*" %%i in (`"cpp -x c++ -v <NUL 2>&1 | getincludes"`) do set ROSBE_HOST_CXXFLAGS=%%i
for /f "usebackq tokens=*" %%i in (`"%_ROSBE_PREFIX%cpp -x c -v <NUL 2>&1 | getincludes"`) do set ROSBE_TARGET_CFLAGS=%%i
for /f "usebackq tokens=*" %%i in (`"%_ROSBE_PREFIX%cpp -x c++ -v <NUL 2>&1 | getincludes"`) do set ROSBE_TARGET_CXXFLAGS=%%i
