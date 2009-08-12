::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Build.cmd
:: PURPOSE:     Perform a uniprocessor build
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <colin@reactos.org>
::                             Peter Ward <dralnix@gmail.com>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion

:: Set the options for a uniprocessor build
set MAKE_JOBS=1
set TITLE_COMMAND=make %*

call "%_ROSBE_BASEDIR%\Build-Shared.cmd" %*
endlocal
