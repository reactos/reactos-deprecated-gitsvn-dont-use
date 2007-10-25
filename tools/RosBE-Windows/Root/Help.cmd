::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/Help.cmd
:: PURPOSE:     Display help for the commands included with the ReactOS Build Environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Check if we are displaying help on all commands, if so, display it.
::
if "%1" == "" (
    echo.
    echo Available Commands:
    echo    make [OPTIONS]       - Without options it does a standard build of
    echo                           ReactOS. OPTIONS are the standard ReactOS build
    echo                           options ie. "bootcd"
    echo    makex [OPTIONS]      - Same as 'make' but automatically determines the
    echo                           number of CPU Cores in the system and uses -j
    echo                           with the appropriate number.
    echo    basedir              - Switch back to the ReactOS source directory.

    if exist "%_ROSBE_BASEDIR%\chdefdir.cmd" (
        echo    chdefdir [OPTIONS]   - Change the ReactOS source directory for the
        echo                           current RosBE session.
    )

    echo    clean [logs]         - Fully clean the ReactOS source directory and/or
    echo                           the RosBE build logs.

    if exist "%_ROSBE_BASEDIR%\Config.cmd" (
        echo    config [OPTIONS]     - Configures the way, ReactOS will be built.
        echo.
    )

    echo    help [COMMAND]       - Display the available commands or help on a
    echo                           specific command.

    if exist "%_ROSBE_BASEDIR%\reladdr2line.cmd" (
        echo    raddr2line [OPTIONS] - Translates program addresses into file names and
        echo                           line numbers to assist Developers to find
        echo                           specific Bugs in ReactOS.
    )

    if exist "%_ROSBE_BASEDIR%\scut.cmd" (
        echo    scut [OPTIONS]       - Define, Edit, Remove, Switch and Default to
        echo                           Shortcutted Source-Directories.
    )

    if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
        echo    ssvn [OPTIONS]       - Create, Update or Clean-up your ReactOS Source
        echo                           Tree or show the Revision Number of the Offline
        echo                           Tree and Online HEAD Revision.
    )

    if exist "%_ROSBE_BASEDIR%\options.cmd" (
        echo    options              - Starts options.exe and restarts RosBE to make
        echo                           the changes effective at the end.
    )

    echo.
    goto :EOC
)

::
:: Now check if we are displaying help on individual commands, if so,
:: display the help for that command.
::
if "%1" == "make" (
    echo Usage: make [OPTIONS]
    echo Without options it does a standard build of ReactOS. OPTIONS are the
    echo standard ReactOS build options ie. "bootcd", "livecd", etc.
    goto :EOC
)
if "%1" == "makex" (
    echo Usage: makex [OPTIONS]
    echo Same as 'make' but automatically determines the number of CPU Cores in
    echo the system and uses "make -j x" with the appropriate number.
    echo NOTE: The number makex uses can be modified by editing Build.cmd
    echo       located in the RosBE directory, instructions for doing so are
    echo       contained within the file.
    goto :EOC
)
if "%1" == "basedir" (
    echo Usage: basedir
    echo Switches back to the ReactOS source directory.
    goto :EOC
)
if exist "%_ROSBE_BASEDIR%\chdefdir.cmd" (
    if "%1" == "chdefdir" (
        echo Usage: chdefdir [OPTIONS]
        echo Change the ReactOS source directory for the current RosBE session.
        echo.
        echo    previous - Switch to the previous ReactOS source directory.
        goto :EOC
    )
)
if "%1" == "clean" (
    echo Usage: clean [logs]
    echo Fully clean the ReactOS source directory.
    echo.
    echo    logs - Removes all build logs in the RosBE-Logs directory.
    goto :EOC
)
if exist "%_ROSBE_BASEDIR%\Config.cmd" (
    if "%1" == "config" (
        echo Usage: config [OPTIONS]
        echo Creates a Configuration File, which tells RosBE how to build the Tree.
        echo.
        echo    delete - Deletes the created configuration File and so sets back
        echo             to default settings.
        echo    update - Deletes the old created configuration File and updats
        echo             with a new, default one.
        goto :EOC
    )
)
if "%1" == "help" (
    echo Usage: help [COMMAND]
    echo Shows help for the specified command or lists all available commands.
    goto :EOC
)
if exist "%_ROSBE_BASEDIR%\reladdr2line.cmd" (
    if "%1" == "raddr2line" (
        echo Usage: raddr2line [OPTIONS]
        echo Translates program addresses into file names and line numbers to
        echo assist Developers to find specific Bugs in ReactOS.
        echo.
        echo USAGE: - The first parameter has to be the Executable to be analyzed.
        echo        - The second one has to be the address to be translated.
        echo.
        echo If you don't set the needed Parameters, you will be asked for them.
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\scut.cmd" (
    if "%1" == "scut" (
        echo Usage: scut [OPTIONS]
        echo Defines, Edits, Removes, Switches and Defaults to Shortcutted Source
        echo Directories. scut started without a parameter or with the name of a
        echo Shortcut as parameter sets this Shortcut active.
        echo.
        echo    add     - Adds a Shortcut.
        echo              Optional: "Shortcut" "Directory" as second and third parameter
        echo    rem     - Removes a shortcut.
        echo              Optional: "Shortcut" as second parameter
        echo    edit    - Edits a shortcut.
        echo              Optional: "Shortcut" "Directory" as second and third parameter
        echo    def     - Chooses Default Shortcut which is enabled on
        echo              every start of RosBE. "Default" is the Source
        echo              Directory as you know it from previous Versions
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
    if "%1" == "ssvn" (
        echo Usage: ssvn [OPTIONS]
        echo Creates, Updates or cleans up your ReactOS Source Tree or shows the
        echo Revision Number of the Offline Tree and Online HEAD Revision.
        echo.
        echo    update  - Updates to HEAD Revision or to a specific one when
        echo              second parameter specifies one.
        echo    create  - Creates a new ReactOS Source Tree.
        echo    cleanup - Cleans up and fixes errors in the Source Tree.
        echo    status  - Shows Actual offline Revision Number and online HEAD
        echo              Revision Number.
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\options.cmd" (
    if "%1" == "options" (
        echo Usage: options
        echo Starts RosBE Configurator and restarts RosBE afterwards to make the
        echo changes effective at the end.
        goto :EOC
    )
)
if not "%1" == "" (
    echo Unknown command specified. No help available for %1.
    goto :EOC
)

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
