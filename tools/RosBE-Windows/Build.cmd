:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Perform the build of ReactOS.
@echo off

::
:: Set the build command
::
set MAKE_COMMAND="%_MINGWMAKE%" %*

::
:: Execute the shared build commands
::
call "%ROSBEBASEDIR%\Build-Shared.cmd"
