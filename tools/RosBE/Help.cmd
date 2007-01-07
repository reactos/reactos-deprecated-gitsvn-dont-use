:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Display help for the commands included with the
::    ReactOS Build Environment.
@echo off

echo.
echo Available Commands:
echo    make [OPTIONS]  - make, without options does a standard build of
echo                      ReactOS. OPTIONS are the standard ReactOS build
echo                      options ie. bootcd.
echo    makex [OPTIONS] - Same as 'make' but automatically determines the
echo                      number of CPUs in the system and uses -j with
echo                      the appropriate number.
echo    clean           - Fully clean the ReactOS source directory.
echo    help            - Display the available commands.
echo    basedir         - Switch back to the ReactOS source directory.
