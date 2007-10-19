::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/Build-Multi.cmd
:: PURPOSE:     Perform the build of ReactOS, but use the -j option with the appropriate
::              number for the number of CPUs in the system.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <mail@colinfinck.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

title Building...

::
:: Clear variables before use.
::
set CPUCOUNT=

::
:: Get the number of CPUs in the system so we know how many jobs to execute.
:: To modify the number used alter the options used with cpucount:
:: No Option - Number of CPUs.
:: -x1       - Number of CPUs, Plus 1.
:: -x2       - Number of CPUs, Doubled.
::
for /f "usebackq" %%i in (`cpucount -x1`) do set CPUCOUNT=%%i

::
:: Set the build command
::
set MAKE_COMMAND="%_MINGWMAKE%" -j %CPUCOUNT% %1

::
:: Execute the shared build commands
::
call "%ROSBEBASEDIR%\Build-Shared.cmd" %2 %3

title ReactOS Build Environment %_VER%
