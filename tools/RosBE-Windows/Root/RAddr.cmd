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

::
:: Receive the Parameters and decide what to do.
::
if "%1" == "" (
    goto :MAN
)
if not "%1" == "" (
    if "%2" == "" (
        goto :AUTO1
    ) else (
        goto :AUTO2
    )
)

::
:: If Parameters were set, parse them, if not, ask the user to add them.
::
:MAN
echo Set the Path to the Executable to be examined.
SET /P EXE=
echo Set the Address you wanna analyze inside the Executable.
SET /P ADDR=
echo.
echo.
raddr2line %EXE% %ADDR%
goto :EOF

:AUTO1
echo Set the Address you wanna analyze inside the Executable.
SET /P ADDR=
raddr2line %1 %ADDR%
goto :EOF

:AUTO2
raddr2line %1 %2
goto :EOF
