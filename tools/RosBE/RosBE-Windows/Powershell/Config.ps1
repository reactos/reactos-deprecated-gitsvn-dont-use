#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Config.cmd
# PURPOSE:     A Basic Config.rbuild Creator for ReactOS.
# COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
#
#

$host.ui.RawUI.WindowTitle = "ReactOS Build Configurator"

function settitle {
    if ($_ROSBE_VERSION -ne $null) {
        $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
    }
    exit
}

#
# Receive the first Parameter and decide what to do.
#
if ($args[0] -eq "delete") {
    "config.rbuild will be permanently deleted. All your settings will be gone."
    "Continue?"
    $YESNO = Read-Host "(yes), (no)"
    if ($YESNO -eq "yes") {
        if (Test-Path ".\config.rbuild") {
            remove-item ".\config.rbuild"
            "Main Configuration File was found and deleted."
        } else {
            "Main Configuration File was not found in ReactOS Source Tree."
        }
        if (Test-Path "$ENV:APPDATA\RosBE\config.rbuild") {
            remove-item "$ENV:APPDATA\RosBE\config.rbuild"
            "Working Configuration File was found and deleted."
        } else {
            "Working Configuration File was not found in ReactOS Source Tree."
        }
    }
    settitle
}
elseif ($args[0] -eq "update") {
    "old config.rbuild will be deleted and will be updated with a recent,"
    "default one. You will need to reconfigure it to your wishes later."
    "Continue?"
    $YESNO = Read-Host "(yes), (no)"
    if ($YESNO -eq "yes") {
        remove-item "$_ROSBE_BASEDIR\*.rbuild"
        remove-item ".\config.rbuild"
        copy-item ".\config.template.rbuild" "$ENV:APPDATA\RosBE\config.rbuild"
       "Successfully Updated."
    }
    settitle
}
elseif ($args[0] -ne $null) {
    "Unknown parameter specified. Try 'help [COMMAND]'."
    settitle
}

#
# Check if config.rbuild already exists. If not, get a working copy.
#
if (!(Test-Path "$ENV:APPDATA\RosBE\config.rbuild")) {
    copy-item ".\config.template.rbuild" "$ENV:APPDATA\RosBE\config.rbuild"
}

#
# Help prevent non-useful bug reports/questions.
#
""
"*** Configurations other than release/debug are not useful for ***"
"*** posting bug reports, and generally not very useful for     ***"
"*** IRC/Forum discussion. Please refrain from doing so unless  ***"
"*** you are sure about what you are doing.                     ***"
""

$YESNO = Read-Host "(yes), (no)"

if ($YESNO -ne "yes") {settitle}

#
# Check if config.template.rbuild is newer than config.rbuild, if it is then
# inform the user and offer an update.
#
if (Test-Path ".\config.rbuild") {
    if ((gi .\config.template.rbuild).LastWriteTime -gt (gi .\config.rbuild).LastWriteTime) {
        ""
        "*** config.template.rbuild is newer than working config.rbuild ***"
        "*** The Editor cannot continue with this file. Do you wanna    ***"
        "*** update to the most recent one? You need to reset all your  ***"
        "*** previously made settings.                                  ***"
        ""
        $YESNO = Read-Host "(yes), (no)"
        if ($YESNO -eq "yes") {remove-item "$ENV:APPDATA\RosBE\*.rbuild" | remove-item ".\config.rbuild" | copy-item ".\config.template.rbuild" "$ENV:APPDATA\RosBE\config.rbuild"}
        else {settitle}
    }
}

#
# Prepare XML Parser.
#
[xml] $XML = get-content "$ENV:APPDATA\RosBE\config.rbuild"

#
# Start with reading settings from config.rbuild and let the user edit them.
#
"Sub-Architecture to build for."
"Default is: none"
""
$SARCH = $xml.group.property | ? { $_.Name -eq "SARCH" } | % { $_.Value}
"Right now: $SARCH"
$SARCH_CH = Read-Host "(), (xbox)"
clear-host

"Which CPU ReactOS should be optimized for."
""
"Examples:"
"Intel: i486, i586 / pentium, pentium-mmx, i686 / pentiumpro, pentium2, pentium3"
"        pentium3m, pentium-m, pentium4 / pentium4m, prescott, nocona"
" AMD:   k6, k6-2 / k6-3, athlon / athlon-tbird, athlon-4 / athlon-xp / athlon-mp"
"        k8 / opteron / athlon64 / athlon-fx"
" IDT:   winchip-c6, winchip2"
" VIA:   c3, c3-2"
" Default is: pentium"
""
$OARCH = $xml.group.property | ? { $_.Name -eq "OARCH" } | % { $_.Value}
"Right now: $OARCH"
$OARCH_CH = Read-Host
if ($OARCH_CH -eq $null) {
    $OARCH_CH = $OARCH
}
clear-host

"What level do you want ReactOS to be optimized at."
"This setting does not work if GDB is set."
"0 = off"
"1 = Normal compiling. Recommended. It is the default setting in"
"official release builds and debug builds."
"warning : 2,3,4,5 is not tested on ReactOS. Change at own risk."
""
$OPTIMIZE = $xml.group.property | ? { $_.Name -eq "OPTIMIZE" } | % { $_.Value}
"Right now: $OPTIMIZE"
$OPTIMIZE_CH = Read-Host "(0), (1), (2), (3), (4), (5)"
if ($OPTIMIZE_CH -eq $null) {
    $OPTIMIZE_CH = $OPTIMIZE
}
clear-host

"Whether to compile in the integrated kernel debugger."
"Default is: 1"
""
$KDBG = $xml.group.property | ? { $_.Name -eq "KDBG" } | % { $_.Value}
"Right now: $KDBG"
$KDBG_CH = Read-Host "(0), (1)"
if ($KDBG_CH -eq $null) {
    $KDBG_CH = $KDBG
}
clear-host

"Whether to compile for debugging. No compiler optimizations will be"
"performed."
"Default is: 1"
""
$DBG = $xml.group.property | ? { $_.Name -eq "DBG" } | % { $_.Value}
"Right now: $DBG"
$DBG_CH = Read-Host "(0), (1)"
if ($KDBG_CH -eq $null) {
    $DBG_CH = $DBG
}
clear-host

"Whether to compile for debugging with GDB. If you don't use GDB,"
"don't enable this."
"Default is: 0"
""
$GDB = $xml.group.property | ? { $_.Name -eq "GDB" } | % { $_.Value}
"Right now: $GDB"
$GDB_CH = Read-Host "(0), (1)"
if ($GDB_CH -eq $null) {
    $GDB_CH = $GDB
}
clear-host

"Whether to compile apps/libs with features covered software patents"
"or not. If you live in a country where software patents are"
"valid/apply, don't enable this (except they/you purchased a license"
"from the patent owner)."
"Default is: 0"
""
$NSWPAT = $xml.group.property | ? { $_.Name -eq "NSWPAT" } | % { $_.Value}
"Right now: $NSWPAT"
$NSWPAT_CH = Read-Host "(0), (1)"
if ($NSWPAT_CH -eq $null) {
    $NSWPAT_CH = $NSWPAT
}
clear-host

"Whether to compile with the KD protocol. This will disable support for"
"KDBG as well as rossym and symbol lookups, and allow WinDBG to connect"
"to ReactOS. This is currently not fully working, and requires kdcom"
"from Windows 2003 or TinyKRNL. Booting into debug mode with this flag"
"enabled will result in a failure to enter GUI mode. Do not enable"
"unless you know what you're doing."
"Default is: 0"
""
$WINKD = $xml.group.property | ? { $_.Name -eq "_WINKD_" } | % { $_.Value}
"Right now: $WINKD"
$WINKD_CH = Read-Host "(0), (1)"
if ($WINKD_CH -eq $null) {
    $WINKD_CH = $WINKD
}
clear-host

"Whether to compile support for ELF files. Do not enable unless you know what"
"you're doing."
"Default is: 0"
""
$ELF = $xml.group.property | ? { $_.Name -eq "_ELF_" } | % { $_.Value}
"Right now: $ELF"
$ELF_CH = Read-Host "(0), (1)"
if ($ELF_CH -eq $null) {
    $ELF_CH = $ELF
}
clear-host

#
# Generate a config.rbuild, copy it to the Source Tree and delete temp files.
#
$xml.group.property | ? { $_.Name -eq "SARCH" } | % { $_.Value = "$SARCH_CH"}
$xml.group.property | ? { $_.Name -eq "OARCH" } | % { $_.Value = "$OARCH_CH"}
$xml.group.property | ? { $_.Name -eq "OPTIMIZE" } | % { $_.Value = "$OPTIMIZE_CH"}
$xml.group.property | ? { $_.Name -eq "KDBG" } | % { $_.Value = "$KDBG_CH"}
$xml.group.property | ? { $_.Name -eq "DBG" } | % { $_.Value = "$DBG_CH"}
$xml.group.property | ? { $_.Name -eq "GDB" } | % { $_.Value = "$GDB_CH"}
$xml.group.property | ? { $_.Name -eq "NSWPAT" } | % { $_.Value = "$NSWPAT_CH"}
$xml.group.property | ? { $_.Name -eq "_WINKD_" } | % { $_.Value = "$WINKD_CH"}
$xml.group.property | ? { $_.Name -eq "_ELF_" } | % { $_.Value = "$ELF_CH"}
$xml.save("$ENV:APPDATA\RosBE\config.rbuild")
copy-item "$ENV:APPDATA\RosBE\config.rbuild" ".\config.rbuild"

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

#
# Unload all used Vars.
#
$YESNO = $null
$SARCH_CH = $null
$OARCH_CH = $null
$OPTIMIZE_CH = $null
$KDBG_CH = $null
$DBG_CH = $null
$GDB_CH = $null
$NSWPAT_CH = $null
$WINKD_CH = $null
$ELF_CH = $null
$SARCH = $null
$OARCH = $null
$OPTIMIZE = $null
$KDBG = $null
$DBG = $null
$GDB = $null
$NSWPAT = $null
$WINKD = $null
$ELF = $null
$XML = $null

settitle
