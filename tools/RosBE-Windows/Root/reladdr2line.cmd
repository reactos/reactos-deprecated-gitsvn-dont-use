::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/reladdr2line.cmd
:: PURPOSE:     Converts a value to hex and displays it.
:: COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
::                             Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title relAddr2Line...

::
:: Receive the Parameters and decide what to do.
::

if "%1" == "" (
    goto :MAN
)
if not "%1" == "" (
    set _1=%1
    if "%2" == "" (
        goto :AUTO1
    ) else (
        set _2=%2
        goto :EOC
    )
)

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:MAN
set /p _1="Please enter the path to the executable to be examined: "
:AUTO1
set /p _2="Please enter the address you would like to analyze: "

:EOC
if not exist "%_1%\." (
    echo ERROR: The path specified doesn't seem to exist.
    goto :END
)
if /i "%_1%" == "" (
    echo ERROR: You must enter a valid directory.
    goto :END
)
if /i "%_2%" == "" (
    echo ERROR: You must enter a address to analyze.
    goto :END
)
::
:: First get the ImageBase of the File. If its smaller than the given
:: Parameter, everything is ok, because it was already added onto the
:: adress and can be given directly to raddr2line. If not, add it and
:: give the result to raddr2line.
::
for /f "tokens=2" %%i in ('"objdump -p %_1% 2>NUL | findstr ImageBase"') do set baseaddr=0x%%i
if %%i lss %_2% (
    raddr2line "%_1%" "%_2%" 2>NUL
) else (
    set /a baseaddr+=0x%_2%
    for /f %%i in ('"echoh %baseaddr%"') do set relbase=%%i
    raddr2line "%_1%" "%relbase%" 2>NUL
)

:END
title ReactOS Build Environment %_ROSBE_VERSION%
