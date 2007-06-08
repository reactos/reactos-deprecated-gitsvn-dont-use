:: Copyright (c) Peter Ward, Colin Finck and Daniel Reimer. All rights reserved.
::
::    Perform the build of ReactOS.
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
