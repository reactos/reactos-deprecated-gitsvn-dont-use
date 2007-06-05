:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Perform the build of ReactOS, but use the -j option
::    with the appropriate number for the number of CPUs
::    in the system.
@echo off

::
:: Clear variables before use.
::
set CPUCOUNT=

::
:: Get the number of CPUs in the system so we know how
:: many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, Plus 1.
:: -x2       - Number of CPUs, Doubled.
for /f "usebackq" %%i in (`"%ROSBEBASEDIR%\Tools\cpucount.exe" -x1`) do set CPUCOUNT=%%i

::
:: Set the build command
::
set MAKE_COMMAND="%_MINGWMAKE%" -j %CPUCOUNT% %*

::
:: Execute the shared build commands
::
call "%ROSBEBASEDIR%\Build-Shared.cmd"
