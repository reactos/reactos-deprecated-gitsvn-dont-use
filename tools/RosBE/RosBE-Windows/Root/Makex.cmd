::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Build-Multi.cmd
:: PURPOSE:     Perform a multiprocessor build
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

:: Set the options for a multiprocessor build
set MAKE_JOBS=%_ROSBE_MAKEX_JOBS%
set TITLE_COMMAND=makex %*

call "%_ROSBE_BASEDIR%\Build-Shared.cmd" %*
endlocal
