::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory
:: FILE:        Root/chdefdir.cmd
:: PURPOSE:     Tool to change the Default Work Dir in RosBE.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Change the Default Dir...

::
:: Create a Backup Variable to revert to Default if wanted.
::
if not defined _ROSBE_ROSSOURCEDIRBCK (
    set _ROSBE_ROSSOURCEDIRBCK=%_ROSBE_ROSSOURCEDIR%
)

::
:: Parse the commands.
::
if /i "%1" == "" (
    goto :INTERACTIVE
)
if /i "%1" == "default" (
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_ROSSOURCEDIRBCK%
) else (
    set _ROSBE_ROSSOURCEDIR=%1
)
goto :END

:INTERACTIVE
echo Type in a Path to use RosBE in or "default" to revert to Default Dir:
set /p XY=

if /i "%XY%" == "" (
    goto :END
)
if /i "%XY%"=="default" (
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_ROSSOURCEDIR%
) else (
    set _ROSBE_ROSSOURCEDIR=%XY%
)
goto :END

:END
title ReactOS Build Environment %_ROSBE_VERSION%
cd /d "%_ROSBE_ROSSOURCEDIR%"
