::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/reladdr2line.cmd
:: PURPOSE:     Converts a value to hex and displays it.
:: COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
::
::
@echo off

title relAddr2Line...

set 1 = %1
set 2 = %2

::
:: Receive the Parameters and decide what to do.
::
if "%1%" == "" (
    goto :MAN
)
if not "%1%" == "" (
    if "%2%" == "" (
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
SET /P 1 =
echo Set the Address you wanna analyze inside the Executable.
SET /P 2 =
echo.
goto :EOC

:AUTO1
echo Set the Address you wanna analyze inside the Executable.
echo.
SET /P 2 =
goto :EOC

:EOC
for /f "tokens=2" %%i in ('"objdump -p %1% 2>NUL | findstr ImageBase"') do SET baseaddr=0x%%i
set /a baseaddr += 0x%2%
for /f %%i in ('"echoh %baseaddr%"') do set relbase=%%i
raddr2line "%1%" "%relbase%" 2>NUL

title ReactOS Build Environment %_VER%

::%1 Dateiname
::%2 Adresse
::%%i das was objdump rausfindet
::baseaddr = %%i + Adresse