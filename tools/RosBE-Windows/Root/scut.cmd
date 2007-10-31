::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows.
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/scut.cmd
:: PURPOSE:     Manages named shortcuts to ReactOS source directories.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
@echo off

if /i "%1" == "" (
    for /f "usebackq tokens=*" %%i in (`"%_ROSBE_BASEDIR%\Tools\scut.exe"`) do (
        if /i not "%%i" == "Default" (
            cd /d %%i
            set _ROSBE_ROSSOURCEDIR=%%i
        )
    )
    goto :EOC
)
if /i "%1" == "add" (
    call "%_ROSBE_BASEDIR%\Tools\scut.exe" %*
    goto :EOC
)
if /i "%1" == "rem" (
    call "%_ROSBE_BASEDIR%\Tools\scut.exe" %*
    goto :EOC
)
if /i "%1" == "edit" (
    call "%_ROSBE_BASEDIR%\Tools\scut.exe" %*
    goto :EOC
)
if /i "%1" == "def" (
    call "%_ROSBE_BASEDIR%\Tools\scut.exe" %*
    goto :EOC
)
if not "%1" == "" (
    for /f "usebackq tokens=*" %%i in (`""%_ROSBE_BASEDIR%\Tools Test\scut.exe" %*"`) do (
        if /i not "%%i" == "Default" (
            cd /d %%i
            set _ROSBE_ROSSOURCEDIR=%%i
        )
    )
    goto :EOC
)

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
