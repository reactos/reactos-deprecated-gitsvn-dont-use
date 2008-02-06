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
::
if "%1" == "" (
    call :INTERACTIVE
) else (
    set _1=%1
)
if /i "%_1%" == "previous" (
    popd
)
if /i "%_1%" == "x86" (
    set _ROSBE_ARCH%=x86
    set _ROSBE_PREFIX=mingw32
    goto :EOC
)
if /i "%_1%" == "arm" (
    set _ROSBE_ARCH%=arm
    set _ROSBE_PREFIX=mingwarm
    goto :EOC
)
if /i "%_1%" == "ppc" (
    set _ROSBE_ARCH%=ppc
    set _ROSBE_PREFIX=mingwppc
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
pushd %_1%

::
:: Set to x86, ppc or arm.
::
set ROS_ARCH=%_ROSBE_ARCH%

::
:: Default is mingw32. ppc could be mingwppc, arm mingwarm.
::
set ROS_PREFIX=%_ROSBE_PREFIX%

REM chdefgcc %PATH%
REM Maybe another way to set the compiler...

if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

::
:: Unload all used Vars.
::
set _ROSBE_ARCH=
set _ROSBE_PREFIX=
