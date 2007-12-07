::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/reladdr2line.cmd
:: PURPOSE:     Converts a value to hex and displays it.
:: COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
::                             Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

title reladdr2line...

::
:: Receive the Parameters and decide what to do.
::
if not "%3" == "" (
    echo ERROR: Too many parameters specified.
    goto :EOC
)
if not "%1" == "" (
    set _1=%1
    call :CHECKPATH
)
if not "%2" == "" (
    set _2=%2
)
call :INTERACTIVE

::
:: First get the ImageBase of the File. If its smaller than the given
:: Parameter, everything is ok, because it was already added onto the
:: adress and can be given directly to raddr2line. If not, add it and
:: give the result to raddr2line.
::
:RADD2RLINE
    if "%_1%" == "" (
        echo ERROR: You must specify a path/file to examine.
        goto :EOC
    )
    if "%_2%" == "" (
        echo ERROR: You must specify a address to analyze.
        goto :EOC
    )
    echo %_1%
    for /f "tokens=2" %%i in ('"objdump -p %_1% 2>NUL | findstr ImageBase"') do set baseaddr=0x%%i
    if %%i lss %_2% (
        "%_ROSBE_BASEDIR%\Tools\raddr2line.exe" "%_1%" "%_2%"
    ) else (
        set /a baseaddr+=0x%_2%
        for /f %%i in ('""%_ROSBE_BASEDIR%\Tools\echoh.exe" %baseaddr%"') do set relbase=%%i
        "%_ROSBE_BASEDIR%\Tools\raddr2line.exe" "%_1%" "%relbase%"
    )
goto :EOC

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:INTERACTIVE
    if "%_1%" == "" (
        set /p _1="Please enter the path/file to be examined: "
        call :CHECKPATH
    )
    if "%_2%" == "" (
        set /p _2="Please enter the address you would like to analyze: "
    )
goto :EOF

::
:: Check if the user supplied a path, if they didn't look for
:: the specified file in the current directory and any
:: sub-directories.
::
:CHECKPATH
    "%_ROSBE_BASEDIR%\Tools\chkslash.exe" %_1%
    if errorlevel 2 (
        for /f "usebackq" %%i in (`"dir /a:-d /s /b %_1% 2>NUL | findstr "%_1%""`) do set _1=%%i
    )
goto :EOF

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%

::
:: Unload all used Vars.
::
set _1=
set _2=
set baseaddr=
set relbase=
