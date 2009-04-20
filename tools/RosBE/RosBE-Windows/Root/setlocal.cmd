::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/setlocal.cmd
:: PURPOSE:     Bypass for the stupid setlocal in CMD Batch
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)


if not "%_ROSBE_ARCH%" == "" (
    set ROS_ARCH=%_ROSBE_ARCH%
    set ROS_PREFIX=%_ROSBE_ARCH%-pc-mingw32
    set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\%_ROSBE_ARCH%

    REM HAXX

    if "%_ROSBE_ARCH%" == "arm" (
        set _ROSBE_OBJPATH=%_ROSBE_ARM_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_ARM_OUTPATH%
    )
    if "%_ROSBE_ARCH%" == "ppc" (
        set _ROSBE_OBJPATH=%_ROSBE_PPC_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_PPC_OUTPATH%
    )
    if "%_ROSBE_ARCH%" == "amd64" (
        set _ROSBE_OBJPATH=%_ROSBE_AMD64_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_AMD64_OUTPATH%
        set ROS_PREFIX=x86_64-pc-mingw32
        set _ROSBE_TARGET_MINGWPATH=%_ROSBE_BASEDIR%\x86_64
    )

    REM Check if existant arch

    if not exist "%_ROSBE_TARGET_MINGWPATH%\." (   
        echo Unsupported arch specified. Fallback to Default.
        pause
        set _ROSBE_OBJPATH=%_ROSBE_i386_OBJPATH%
        set _ROSBE_OUTPATH=%_ROSBE_i386_OUTPATH%
        set ROS_ARCH=
        set ROS_PREFIX=
        set _ROSBE_TARGET_MINGWPATH=
    )
)
