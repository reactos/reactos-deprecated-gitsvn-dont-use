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

    if exist "%_ROSBE_BASEDIR%\chdefgcc.cmd" (
        echo    chdefgcc [PATH]      - Change the MinGW/GCC directory for the
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
        echo                           line numbers to assist developers with finding
        echo                           specific bugs in ReactOS.
    )

    if exist "%_ROSBE_BASEDIR%\scut.cmd" (
        echo    scut [OPTIONS]       - List, add, edit, remove, switch and default to
        echo                           shortcutted ReactOS source directories.
    )

    if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
        echo    ssvn [OPTIONS]       - Create, update or clean-up your ReactOS source
        echo                           tree or show the revision number of the local
        echo                           and online trees.
    )

    if exist "%_ROSBE_BASEDIR%\options.cmd" (
        echo    options              - Starts the RosBE configurator.
    )

    echo.
    goto :EOC
)

::
:: Now check if we are displaying help on individual commands, if so,
:: display the help for that command.
::
if /i "%1" == "make" (
    echo Usage: make [OPTIONS]
    echo Without options it does a standard build of ReactOS. OPTIONS are the
    echo standard ReactOS build options ie. "bootcd", "livecd", etc.
    goto :EOC
)
if /i "%1" == "makex" (
    echo Usage: makex [OPTIONS]
    echo Same as 'make' but automatically determines the number of CPU Cores in
    echo the system and uses "make -j x" with the appropriate number.
    echo NOTE: The number makex uses can be modified by editing Build.cmd
    echo       located in the RosBE directory, instructions for doing so are
    echo       contained within the file.
    goto :EOC
)
if /i "%1" == "basedir" (
    echo Usage: basedir
    echo Switches back to the ReactOS source directory.
    goto :EOC
)
if exist "%_ROSBE_BASEDIR%\chdefdir.cmd" (
    if /i "%1" == "chdefdir" (
        echo Usage: chdefdir [OPTIONS]
        echo Change the ReactOS source directory for the current RosBE session.
        echo.
        echo    previous - Switch to the previous ReactOS source directory.
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\chdefgcc.cmd" (
    if /i "%1" == "chdefgcc" (
        echo Usage: chdefgcc [PATH]
        echo Change the MinGW/GCC directory for the current RosBE session.
        echo.
        goto :EOC
    )
)
if /i "%1" == "clean" (
    echo Usage: clean [logs]
    echo Fully clean the ReactOS source directory.
    echo.
    echo    logs - Removes all build logs in the RosBE-Logs directory.
    goto :EOC
)
if exist "%_ROSBE_BASEDIR%\Config.cmd" (
    if /i "%1" == "config" (
        echo Usage: config [OPTIONS]
        echo Creates a configuration file, which tells RosBE how to build the tree.
        echo.
        echo    delete - Deletes the created configuration file and so sets back
        echo             to default settings.
        echo    update - Deletes the old created configuration file and updates
        echo             it with a new, default one.
        goto :EOC
    )
)
if /i "%1" == "help" (
    echo Usage: help [COMMAND]
    echo Shows help for the specified command or lists all available commands.
    goto :EOC
)
if exist "%_ROSBE_BASEDIR%\reladdr2line.cmd" (
    if /i "%1" == "raddr2line" (
        echo Usage: raddr2line [FILE] [ADDRESS]
        echo Translates program addresses into file names and line numbers to
        echo assist developers with finding specific bugs in ReactOS. If
        echo either of the options is not given you will be prompted for it.
        echo.
        echo FILE    - This can be either a fully specified path or just the
        echo           file name [you can use partial name ie. ntdll*] of
        echo           the executable to be analyzed. When using just a file
        echo           the current directory and all sub-directories are
        echo           searched.
        echo ADDRESS - The address to be translated.
        echo.
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\scut.cmd" (
    if /i "%1" == "scut" (
        "%_ROSBE_BASEDIR%\Tools\scut.exe" --help
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
    if /i "%1" == "ssvn" (
        echo Usage: ssvn [OPTIONS]
        echo Creates, updates or cleans up your ReactOS source tree or shows the
        echo revision number of the local and online trees.
        echo.
        echo    update  - Updates to HEAD revision or to a specific one when
        echo              second parameter specifies one.
        echo    create  - Creates a new ReactOS source tree.
        echo    cleanup - Cleans up and fixes errors in the source tree.
        echo    status  - Show the current local and online revision numbers.
        goto :EOC
    )
)
if exist "%_ROSBE_BASEDIR%\options.cmd" (
    if /i "%1" == "options" (
        echo Usage: options
        echo Starts the RosBE configurator.
        goto :EOC
    )
)
if not "%1" == "" (
    echo Unknown command specified. No help available for %1.
    goto :EOC
)

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
