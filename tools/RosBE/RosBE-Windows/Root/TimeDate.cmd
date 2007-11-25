::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/TimeDate.cmd
:: PURPOSE:     Get the current date and time for use in our build log's file name.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Clear variables before use.
::
set DATERAW=
set TIMERAW=
set DATENAME=
set TIMENAME=

::
:: Get the raw date and time.
::
for /f "usebackq tokens=1" %%u in (`"%_ROSBE_BASEDIR%\Tools\getdate.exe"`) do set DATERAW=%%u
for /f "usebackq tokens=1" %%u in (`time /t`) do set TIMERAW=%%u

::
:: Check if we need to pad the time with a zero.
::
if "%TIMERAW:~1,1%" == ":" (
    set TIMERAW=0%TIMERAW%
)

::
:: Format the date and time for use in a file name.
::
set DATENAME=%DATERAW:~6, 4%%DATERAW:~3, 2%%DATERAW:~0, 2%
set TIMENAME=%TIMERAW:~0, 2%%TIMERAW:~3, 2%
