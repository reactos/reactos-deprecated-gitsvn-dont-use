::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/RAddr.cmd
:: PURPOSE:     Analyze Adresses in Executables.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title RAddr2Line...

if "%1" == "" (
    goto :MAN
)
if not "%1" == "" (
    if "%2" == "" (
    echo No Address Set.
    goto :EOF
    ) else (
    goto :AUTO
)

:MAN
echo Set the Path to the Executable to be examined.
SET /P EXE=
echo Set the Address you wanna analyze inside the Executable.
SET /P ADDR=
echo.
echo.
"%ROSBEBASEDIR%\Tools\raddr2line.exe" %EXE% %ADDR%

:AUTO
"%ROSBEBASEDIR%\Tools\raddr2line.exe" %1 %2