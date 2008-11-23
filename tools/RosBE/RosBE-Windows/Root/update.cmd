::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/update.cmd
:: PURPOSE:     RosBE Updater.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

setlocal
setlocal enableextensions
setlocal enabledelayedexpansion

if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)
set _ROSBE_UPDFINISH=0
set _ROSBE_OPATH=0
set _ROSBE_UPDDATE=0
set _ROSBE_UPDDATE2=0
set _ROSBE_UPDATES=0
set _ROSBE_OPATH=%~dp0
set _ROSBE_OPATH=%_ROSBE_OPATH:~0,-1%

::
:: Set Title
::
title Updating...

cd /d %_ROSBE_BASEDIR%

::
:: The Update Server.
::
set _ROSBE_URL=http://mitglied.lycos.de/reimerdaniel/rosbe

::
::First check for a new Updater
::
for %%F in (update.cmd) do set _ROSBE_UPDDATE=%%~tF
"Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/update.cmd 1> NUL 2> NUL
for %%F in (update.cmd) do set _ROSBE_UPDDATE2=%%~tF
if !_ROSBE_UPDDATE! NEQ !_ROSBE_UPDDATE2! (
    cls
    echo Updater got updated and needs to be restarted.
    goto :EOC
)
for %%F in (updcheckproc.cmd) do set _ROSBE_UPDDATE=%%~tF
"Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/update.cmd 1> NUL 2> NUL
for %%F in (updcheckproc.cmd) do set _ROSBE_UPDDATE2=%%~tF
if !_ROSBE_UPDDATE! NEQ !_ROSBE_UPDDATE2! (
    cls
    echo Updater got updated and needs to be restarted.
    goto :EOC
)

if not exist "Updates" mkdir Updates 1> NUL 2> NUL
cd Updates

::
:: Parse the args.
::
if "%1" == "" (
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 1 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 2 next
	if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 3 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 4 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 5 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 6 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 7 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 8 next
    if "!_ROSBE_UPDFINISH!" == "1" (goto :EOC)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 9 next
	goto :EOC
)
if /i "%1" == "reset" (
    del /F /Q "%_ROSBE_BASEDIR%\Updates\*.*" 1> NUL 2> NUL
    goto :EOC
)
if /i "%1" == "nr" (
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" %2
	goto :EOC
)
if /i "%1" == "status" (
    mkdir tmp 1> NUL 2> NUL
    copy *.txt .\tmp\. 1> NUL 2> NUL
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 1 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 2 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 3 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 4 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 5 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 6 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 7 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 8 status
    if "!_ROSBE_UPDFINISH!" == "1" (goto :UPDFIN)
    call "%_ROSBE_BASEDIR%\updcheckproc.cmd" 9 status
)

:UPDFIN
del /F /Q tmp\*.*
echo Following Updates available: %_ROSBE_UPDATES%

:EOC

cd /d %_ROSBE_OPATH%
if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

set _ROSBE_UPDFINISH=
set _ROSBE_OPATH=
set _ROSBE_UPDDATE=
set _ROSBE_UPDATES=
set _ROSBE_UPDDATE2=