::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/Build.cmd
:: PURPOSE:     Perform the build of ReactOS.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Colin Finck <mail@colinfinck.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

title Building...

::
:: Set the build command
::
set MAKE_COMMAND="%_MINGWMAKE%" %*

::
:: Execute the shared build commands
::
call "%ROSBEBASEDIR%\Build-Shared.cmd"
