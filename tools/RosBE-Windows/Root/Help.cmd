::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/Help.cmd
:: PURPOSE:     Display help for the commands included with the ReactOS Build Environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

title ReactOS Build Environment 0.3.7

if "%1" == "" (
    echo.
    echo Available Commands:
    echo    make [OPTIONS]  - make, without options does a standard build of
    echo                      ReactOS. OPTIONS are the standard ReactOS build
    echo                      options ie. "bootcd" and "strip" as second option
    echo                      sets striping to enabled.
    echo    makex [OPTIONS] - Same as 'make' but automatically determines the
    echo                      number of CPUs in the system and uses -j with the
    echo                      appropriate number.
    echo    clean [logs]    - Fully clean the ReactOS source directory or the
    echo                      RosBE build logs.
    echo    help [COMMAND]  - Display the available commands or help on a
    echo                      specific command.
    echo    svn [OPTIONS]   - Create, Update or Clean-up your ReactOS Source
    echo                      Tree or show the Status.
    echo    config [OPTIONS]- Configures the way, ReactOS will be built.
    echo.
    echo    basedir         - Switch back to the ReactOS source directory.
    goto :EOF
)
if "%1" == "make" (
    echo Usage: make [OPTIONS1] [OPTIONS2]
    echo make, without options does a standard build of ReactOS. OPTIONS are
    echo the standard ReactOS build options ie. "bootcd", "livecd", etc. "strip"
    echo as second option sets stripping to enabled.
    goto :EOF
)
if "%1" == "makex" (
    echo Usage: makex [OPTIONS] [OPTIONS2]
    echo Same as 'make' but automatically determines the number of CPUs in the
    echo system and uses -j with the appropriate number.
    echo NOTE: The number makex uses can be modified by editing Build-Multi.cmd
    echo       located in the RosBE directory, instructions for doing so are
    echo       contained within the file.
    goto :EOF
)
if "%1" == "clean" (
    echo Usage: clean [logs]
    echo Fully clean the ReactOS source directory.
    echo.
    echo    logs - Removes all build logs in the RosBE-Logs directory.
    goto :EOF
)
if "%1" == "help" (
    echo Usage: help [COMMAND]
    echo Shows help for the specified command or lists all available commands.
    goto :EOF
)
if "%1" == "svn" (
    echo Usage: svn [OPTIONS]
    echo Creates, Updates or cleans up your ReactOS Source Tree.
    echo.
    echo    update  - Updates to HEAD Revision or to a specific one when
    echo              second parameter specifies one.
    echo    create  - Creates a new ReactOS Tree.
    echo    cleanup - Cleans up and fixes errors in Tree.
    echo    status  - Shows Actual offline Revision Number and online HEAD
    echo              Revision Number.
    goto :EOF
)
if "%1" == "config" (
    echo Usage: config [OPTIONS]
    echo Creates a Configuration File, which tells RosBE how to build the Tree.
    echo.
    echo    delete - Deletes the configuration File and so sets back to default
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
