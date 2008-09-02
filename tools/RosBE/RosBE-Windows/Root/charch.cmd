::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/charch.cmd
:: PURPOSE:     Tool to change the current Arch to build ROS for in RosBE.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

title Change the Architecture to build for...

::
:: Parse the command line arguments.
:: ROS_ARCH: Set to x86, x64, ppc or arm.
:: ROS_PREFIX: Default is mingw32. ppc could be mingw32-ppc, arm mingw32-arm, x64 mingw32-x86_64.
::

if "%1" == "" (
    call :INTERACTIVE
) else (
    set _1=%1
)
if /i "%_1%" == "x86" (
    set ROS_ARCH=
    set ROS_PREFIX=
    goto :EOC
)
if /i "%_1%" == "arm" (
    set ROS_ARCH=arm
    set ROS_PREFIX=mingw32-arm
    goto :EOC
)
if /i "%_1%" == "ppc" (
    set ROS_ARCH=ppc
    set ROS_PREFIX=mingw32-ppc
    goto :EOC
)
if /i "%_1%" == "x64" (
    set ROS_ARCH=amd64
    set ROS_PREFIX=mingw32-x86_64
    goto :EOC
)
if not "%_1%" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :EOC
)
goto :EOC

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:INTERACTIVE
    set /p _1="Please enter a Architecture you want to build ReactOS for: "
    if "%_1%" == "" (
        echo ERROR: You must enter a Architecture.
        goto :EOC
    )
goto :EOF

:EOC
REM chdefgcc %PATH%

if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)
