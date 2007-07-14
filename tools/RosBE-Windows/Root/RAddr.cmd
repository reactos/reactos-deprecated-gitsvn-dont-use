::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/RAddr.cmd
:: PURPOSE:     Translates program addresses into file names and
::              line numbers.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title RAddr2Line...

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
raddr2line "%1%" "%2%"
title ReactOS Build Environment %_VER%
