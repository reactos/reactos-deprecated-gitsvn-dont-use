#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Help.ps1
# PURPOSE:     Display help for the commands included with the ReactOS Build Environment.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

#
# Check if we are displaying help on all commands, if so, display it.
#
if ("$args" -eq "") {
    ""
    "Available Commands:"
    "    make [OPTIONS]       - Without options it does a standard build of"
    "                           ReactOS. OPTIONS are the standard ReactOS build"
    "                           options ie. ""bootcd"""
    "    makex [OPTIONS]      - Same as 'make' but automatically determines the"
    "                           number of CPU Cores in the system and uses -j"
    "                           with the appropriate number."
    "    basedir              - Switch back to the ReactOS source directory."

    if (Test-Path "$_ROSBE_BASEDIR\charch.ps1") {
        "    charch [OPTIONS]     - Change the Architecture to build for for the"
        "                           current RosBE session."
    }

    if (Test-Path "$_ROSBE_BASEDIR\chdefdir.ps1") {
        "    chdefdir [OPTIONS]   - Change the ReactOS source directory for the"
        "                           current RosBE session."
    }

    if (Test-Path "$_ROSBE_BASEDIR\chdefgcc.ps1") {
        "    chdefgcc [PATH]      - Change the MinGW/GCC directory for the"
        "                           current RosBE session."
    }

    "    clean [OPTIONS]      - Fully clean the ReactOS source directory and/or"
    "                           the RosBE build logs."

    if (Test-Path "$_ROSBE_BASEDIR\Config.ps1") {
        "    config [OPTIONS]     - Configures the way, ReactOS will be built."
    }

    "    help [COMMAND]       - Display the available commands or help on a"
    "                           specific command."

    if (Test-Path "$_ROSBE_BASEDIR\reladdr2line.ps1") {
        "    raddr2line [OPTIONS] - Translates program addresses into file names and"
        "                           line numbers to assist developers with finding"
        "                           specific bugs in ReactOS."
    }

    if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
        "    scut [OPTIONS]       - List, add, edit, remove, switch and default to"
        "                           shortcutted ReactOS source directories."
    }

    if (Test-Path "$_ROSBE_BASEDIR\sSVN.ps1") {
        "    ssvn [OPTIONS]       - Create, update or clean-up your ReactOS source"
        "                           tree or show the revision number of the local"
        "                           and online trees."
    }

    if (Test-Path "$_ROSBE_BASEDIR\options.ps1") {
        "    options              - Starts the RosBE configurator."
    }

    ""
    exit
}

#
# Now check if we are displaying help on individual commands, if so,
# display the help for that command.
#
if ("$args" -eq "make") {
    " Usage: make [OPTIONS]"
    " Without options it does a standard build of ReactOS. OPTIONS are the"
    " standard ReactOS build options ie. ""bootcd"", ""livecd"", etc."
    exit
}
if ("$args" -eq "makex") {
    " Usage: makex [OPTIONS]"
    " Same as 'make' but automatically determines the number of CPU Cores in"
    " the system and uses ""make -j x"" with the appropriate number."
    " NOTE: The number makex uses can be modified by editing Build.ps1"
    "       located in the RosBE directory, instructions for doing so are"
    "       contained within the file."
    exit
}
if ("$args" -eq "basedir") {
    " Usage: basedir"
    " Switches back to the ReactOS source directory."
    exit
}
if (Test-Path "$_ROSBE_BASEDIR\charch.ps1") {
    if ("$args" -eq "charch") {
        " Usage: charch [OPTIONS]"
        " Change the ReactOS source directory for the current RosBE session."
        " Possible Architectures are: x86, ppc, arm."
        ""
        exit
    }
}
if (Test-Path "$_ROSBE_BASEDIR\chdefdir.ps1") {
    if ("$args" -eq "chdefdir") {
        " Usage: chdefdir [OPTIONS]"
        " Change the ReactOS source directory for the current RosBE session."
        ""
        "    previous - Switch to the previous ReactOS source directory."
        exit
    }
}
if (Test-Path "$_ROSBE_BASEDIR\chdefgcc.ps1") {
    if ("$args" -eq "chdefgcc") {
        " Usage: chdefgcc [PATH]"
        " Change the MinGW/GCC directory for the current RosBE session."
        ""
        exit
    }
}
if ("$args" -eq "clean") {
    " Usage: clean [logs]"
    " Fully clean the ReactOS source directory."
    ""
    "    all  - Removes build logs and compiler output in the RosBE-Logs directory."
    "    logs - Removes all build logs in the RosBE-Logs directory."
    exit
}
if (Test-Path "$_ROSBE_BASEDIR\Config.ps1") {
    if ("$args" -eq "config") {
        " Usage: config [OPTIONS]"
        " Creates a configuration file, which tells RosBE how to build the tree."
        ""
        "    delete - Deletes the created configuration file and so sets back"
        "             to default settings."
        "    update - Deletes the old created configuration file and updates"
        "             it with a new, default one."
        exit
    }
}
if ("$args" -eq "help") {
    " Usage: help [COMMAND]"
    " Shows help for the specified command or lists all available commands."
    exit
}
if (Test-Path "$_ROSBE_BASEDIR\reladdr2line.ps1") {
    if ("$args" -eq "raddr2line") {
        " Usage: raddr2line [FILE] [ADDRESS]"
        " Translates program addresses into file names and line numbers to"
        " assist developers with finding specific bugs in ReactOS. If"
        " either of the options is not given you will be prompted for it."
        ""
        " FILE    - This can be either a fully specified path or just the"
        "           file name [you can use a partial name ie. ntdll*] of"
        "           the executable to be analyzed. When using just a file"
        "           the current directory and all sub-directories are"
        "           searched."
        " ADDRESS - The address to be translated."
        ""
        exit
    }
}
if (Test-Path "$_ROSBE_BASEDIR\scut.ps1") {
    if  ("$args" -eq "scut") {
        IEX "&'$_ROSBE_BASEDIR\Tools\scut.exe' --help"
        exit
    }
}
if (Test-Path "$_ROSBE_BASEDIR\sSVN.ps1") {
    if  ("$args" -eq "ssvn") {
        " Usage: ssvn [OPTIONS]"
        " Creates, updates or cleans up your ReactOS source tree or shows the"
        " revision number of the local and online trees."
        ""
        "    update  - Updates to HEAD revision or to a specific one when"
        "              second parameter specifies one."
        "    create  - Creates a new ReactOS source tree."
        "    cleanup - Cleans up and fixes errors in the source tree."
        "    status  - Show the current local and online revision numbers."
        exit
    }
}
if (Test-Path "$_ROSBE_BASEDIR\options.ps1") {
    if ("$args" -eq "options") {
        " Usage: options"
        " Starts the RosBE configurator."
        exit
    }
}
if ("$args" -ne $null) {
    " Unknown command specified. No help available for $args."
    exit
}
