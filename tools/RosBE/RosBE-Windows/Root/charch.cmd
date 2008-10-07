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
:: ROSBE_ARCH: Default is i386, can be set to amd64, ppc or arm.
::

if "%1" == "" (
    call :INTERACTIVE
) else (
    set _1=%1
)
if /i "%_1%" == "i386" (
    set _ROSBE_ARCH=
    goto :EOA
)
if /i "%_1%" == "arm" (
    set _ROSBE_ARCH=1
    goto :EOA
)
if /i "%_1%" == "ppc" (
    set _ROSBE_ARCH=2
    goto :EOA
)
if /i "%_1%" == "amd64" (
    set _ROSBE_ARCH=3
    goto :EOA
)
if not "%_1%" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :EOC
)

:EOA

::
:: Refresh all needed Params by recalling the main Path setting CMD File.
::
call "%_ROSBE_BASEDIR%\rosbe-gcc-env.cmd"
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
if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

::
:: Unload all used Vars.
::
set _1=
