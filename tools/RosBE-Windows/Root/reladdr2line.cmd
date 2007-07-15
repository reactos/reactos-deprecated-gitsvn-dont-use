::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/reladdr2line.cmd
:: PURPOSE:     Converts a value to hex and displays it.
:: COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
::                             Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title relAddr2Line...

set _1=%1
set _2=%2

::
:: Receive the Parameters and decide what to do.
::
if "%_1%" == "" (
    goto :MAN
)
if not "%_1%" == "" (
    if "%_2%" == "" (
        goto :AUTO1
    ) else (
        goto :EOC
    )
)

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:MAN
echo Set the Path to the Executable to be examined.
SET /P _1=
echo Set the Address you wanna analyze inside the Executable.
SET /P _2=
echo.
goto :EOC

:AUTO1
echo Set the Address you wanna analyze inside the Executable.
echo.
SET /P _2=
goto :EOC

:EOC
for /f "tokens=2" %%i in ('"objdump -p %_1% 2>NUL | findstr ImageBase"') do SET baseaddr=0x%%i
if %%i LSS %_2% (
    raddr2line "%_1%" "%_2%" 2>NUL
) else (
    set /a baseaddr += 0x%_2%
    for /f %%i in ('"echoh %baseaddr%"') do set relbase=%%i
    raddr2line "%_1%" "%relbase%" 2>NUL
)
title ReactOS Build Environment %_VER%
