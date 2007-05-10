:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Get the current date and time for use in
::    in our build log's file name.
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
for /f "usebackq tokens=1" %%u in (`"%ROSBEBASEDIR%\Tools\getdate.exe"`) do set DATERAW=%%u
for /f "usebackq tokens=1" %%u in (`time /t`) do set TIMERAW=%%u

::
:: Check if we have an hour under 10, if so pad
:: it with a zero.
::
if "%TIMERAW:~1,1%" == ":" (
    set TIMERAW=0%t%
)

::
:: Format the date and time for use in a file name.
::
set DATENAME=%DATERAW:~6, 4%%DATERAW:~3, 2%%DATERAW:~0, 2%
set TIMENAME=%TIMERAW:~0, 2%%TIMERAW:~3, 2%
