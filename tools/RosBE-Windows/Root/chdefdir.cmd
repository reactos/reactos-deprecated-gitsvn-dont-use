::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/chdefdir.cmd
:: PURPOSE:     Tool to change the Default Work Dir in RosBE.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Change the Default Dir...

if /I "%1"=="" (
    goto :DEF
) 
if /I "%1"=="default" ( 
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_ROSSOURCEDIR%
) else (
    set _ROSBE_ROSSOURCEDIR=%1
)
goto :END

:DEF

echo Type in a Path to use RosBE in or "default" to revert to Default Dir:
SET /P XY=

if /I "%XY%"=="" (
    goto :END
) 
if /I "%XY%"=="default" ( 
    set _ROSBE_ROSSOURCEDIR=%_ROSBE_ROSSOURCEDIR%
) else (
    set _ROSBE_ROSSOURCEDIR=%XY%
)
goto :END

:END
title ReactOS Build Environment %_VER%
cd /D "%_ROSBE_ROSSOURCEDIR%"
