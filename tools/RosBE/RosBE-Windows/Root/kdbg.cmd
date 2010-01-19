::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/kdbg.cmd
:: PURPOSE:     Displays a KDBG Session Interactively.
:: COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

call "%_ROSBE_BASEDIR%\TimeDate.cmd"

title kdbg Session...

if "%1" == "writelog" (
    if "%2" == "" (
        set LOGFILE=-l DBG-%datename%-%timename%.txt
    ) else (
        set LOGFILE=-l %2
    )
) else (
    set LOGFILE=
)

echo Now Start your VM with COM1 set to \\.\pipe\kdbg
pause

log2lines -c %LOGFILE% < \\.\pipe\kdbg

title ReactOS Build Environment %_ROSBE_VERSION%
