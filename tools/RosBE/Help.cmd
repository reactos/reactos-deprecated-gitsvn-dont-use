:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Display help for the commands included with the
::    ReactOS Build Environment.
@echo off

if "%1" == "" (
    echo.
    echo Available Commands:
    echo    make [OPTIONS]  - make, without options does a standard build of
    echo                      ReactOS. OPTIONS are the standard ReactOS build
    echo                      options ie. bootcd.
    echo    makex [OPTIONS] - Same as 'make' but automatically determines the
    echo                      number of CPUs in the system and uses -j with
    echo                      the appropriate number.
    echo    clean [logs]    - Fully clean the ReactOS source directory or the
    echo                      RosBE build logs.
    echo    help [COMMAND]  - Display the available commands or help on a
    echo                      specific command.
    echo    basedir         - Switch back to the ReactOS source directory.
    goto :EOF
)
if "%1" == "make" (
    echo Usage: make [OPTIONS]
    echo make, without options does a standard build of ReactOS. OPTIONS
    echo are the standard ReactOS build options ie. bootcd.
    goto :EOF
)
if "%1" == "makex" (
    echo Usage: makex [OPTIONS]
    echo Same as 'make' but automatically determines the number of CPUs
    echo in the system and uses -j with the appropriate number.
    echo NOTE: The number makex uses can be modified by editing
    echo       Build-Multi.cmd located in the RosBE directory,
    echo       instructions for doing so are contained within the file.
    goto :EOF
)
if "%1" == "clean" (
    echo Usage: clean [logs]
    echo Fully clean the ReactOS source directory.
    echo.
    echo   logs - Removes all build logs in the RosBE-Logs directory.
    goto :EOF
)
if "%1" == "help" (
    echo Usage: help [COMMAND]
    echo Shows help for the specified command or lists all available commands.
    goto :EOF
)
if "%1" == "basedir" (
    echo Usage: basedir
    echo Switches back to the ReactOS source directory.
    goto :EOF
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :EOF
)
