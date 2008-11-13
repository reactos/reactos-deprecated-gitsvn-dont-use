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
            del ".\config.rbuild"
            "Main Configuration File was found and deleted."
        } else {
            "Main Configuration File was not found in ReactOS Source Tree."
        }
        if (Test-Path "$ENV:APPDATA\RosBE\config.rbuild") {
            del "$ENV:APPDATA\RosBE\config.rbuild"
            "Working Configuration File was found and deleted."
        } else {
            "Working Configuration File was not found in ReactOS Source Tree."
        }
    }
}
elseif ($args[0] -eq "update") {
    "old config.rbuild will be deleted and will be updated with a recent,"
    "default one. You will need to reconfigure it to your wishes later."
    "Continue?"
    $YESNO = Read-Host "(yes), (no)"
    if ($YESNO -eq "yes") {
        del "$_ROSBE_BASEDIR\*.rbuild"
        del ".\config.rbuild"
        copy ".\config.template.rbuild" "$ENV:APPDATA\RosBE\config.rbuild"
       "Successfully Updated."
    }
}
elseif ($args[0] -ne $null) {
    "Unknown parameter specified. Try 'help [COMMAND]'."
}

#
# Check if config.rbuild already exists. If not, get a working copy.
#
if (!(Test-Path "$ENV:APPDATA\RosBE\config.rbuild")) {
    copy ".\config.template.rbuild" "$ENV:APPDATA\RosBE\config.rbuild"
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

if ($YESNO -eq "no") {settitle}

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
        if ($YESNO -eq "yes") {del "$ENV:APPDATA\RosBE\*.rbuild" | del ".\config.rbuild" | copy ".\config.template.rbuild" "$ENV:APPDATA\RosBE\config.rbuild"}
    }
}

#
# Start with reading settings from config.rbuild and let the user edit them.
#
"Sub-Architecture to build for."
"Default is: none"
""
$SARCH = type "$ENV:APPDATA\RosBE\config.rbuild" | find "SARCH" | find "property name"
"$SARCH = $SARCH:~7,-1"
"Right now: $SARCH"
$SARCH_CH = Read-Host "(), (xbox)"
cls

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
$OARCH = type "$ENV:APPDATA\RosBE\config.rbuild" | find "OARCH" | find "property name"
"$OARCH = $OARCH:~7,-1"
"Right now: $OARCH"
$OARCH_CH = Read-Host
if ($OARCH_CH -eq $null) {
    $OARCH_CH = "pentium"
}
cls

"What level do you want ReactOS to be optimized at."
"This setting does not work if GDB is set."
"0 = off"
"1 = Normal compiling. Recommended. It is the default setting in"
"official release builds and debug builds."
"warning : 2,3,4,5 is not tested on ReactOS. Change at own risk."
""
$OPTIMIZE = type "$ENV:APPDATA\RosBE\config.rbuild" | find "OPTIMIZE" | find "property name"
"$OPTIMIZE = $OPTIMIZE:~7,-1"
"Right now: $OPTIMIZE"
OPTIMIZE_CH = Read-Host "(0), (1), (2), (3), (4), (5)"
if ($OPTIMIZE_CH -eq $null) {
    $OPTIMIZE_CH = 1
}
cls

"Whether to compile in the integrated kernel debugger."
"Default is: 1"
""
$KDBG = type "$ENV:APPDATA\RosBE\config.rbuild" | find "KDBG" | find "property name"
"$KDBG = $KDBG:~7,-1"
"Right now: $KDBG"
$KDBG_CH = Read-Host "(0), (1)"
if ($KDBG_CH -eq $null) {
    $KDBG_CH = 1
}
cls

"Whether to compile for debugging. No compiler optimizations will be"
"performed."
"Default is: 1"
""
$DBG = type "$ENV:APPDATA\RosBE\config.rbuild" | find "DBG" | find "property name" | find /V "KDBG"
"$DBG = $DBG:~7,-1"
"Right now: $DBG"
$DBG_CH = Read-Host "(0), (1)"
if ($KDBG_CH -eq $null) {
    $DBG_CH = 1
}
cls

"Whether to compile for debugging with GDB. If you don't use GDB,"
"don't enable this."
"Default is: 0"
""
$GDB = type "$ENV:APPDATA\RosBE\config.rbuild" | find "GDB" | find "property name"
"$GDB = $GDB:~7,-1"
"Right now: $GDB"
$GDB_CH = Read-Host "(0), (1)"
if ($GDB_CH -eq $null) {
    $GDB_CH = 0
}
cls

"Whether to compile apps/libs with features covered software patents"
"or not. If you live in a country where software patents are"
"valid/apply, don't enable this (except they/you purchased a license"
"from the patent owner)."
"Default is: 0"
""
$NSWPAT = type "$ENV:APPDATA\RosBE\config.rbuild" | find "NSWPAT" | find "property name"
"$NSWPAT = $NSWPAT:~7,-1"
"Right now: $NSWPAT"
$NSWPAT_CH = Read-Host "(0), (1)"
if ($NSWPAT_CH -eq $null) {
    $NSWPAT_CH = 0
}
cls

"Whether to compile with the KD protocol. This will disable support for"
"KDBG as well as rossym and symbol lookups, and allow WinDBG to connect"
"to ReactOS. This is currently not fully working, and requires kdcom"
"from Windows 2003 or TinyKRNL. Booting into debug mode with this flag"
"enabled will result in a failure to enter GUI mode. Do not enable"
"unless you know what you're doing."
"Default is: 0"
""
$WINKD = type "$ENV:APPDATA\RosBE\config.rbuild" | find "_WINKD_" | find "property name"
"$WINKD = $WINKD:~7,-1"
"Right now: $WINKD"
$WINKD_CH = Read-Host "(0), (1)"
if ($WINKD_CH -eq $null) {
    $WINKD_CH = 0
}
cls

#
# Generate a config.rbuild, copy it to the Source Tree and delete temp files.
#
'<?xml version="1.0"?>' | out-file "$ENV:TEMP\config.tmp"
'<!DOCTYPE group SYSTEM "tools/rbuild/project.dtd">' | out-file -append "$ENV:TEMP\config.tmp"
'<group>' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="SARCH" value="' + $SARCH_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="OARCH" value="' + $OARCH_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="OPTIMIZE" value="' + $OPTIMIZE_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="KDBG" value="' + $KDBG_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="DBG" value="' + $DBG_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="GDB" value="' + $GDB_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="NSWPAT" value="' + $NSWPAT_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'<property name="_WINKD_" value="' + $WINKD_CH + '" />' | out-file -append "$ENV:TEMP\config.tmp"
'</group^>' | out-file -append "'$ENV:TEMP\config.tmp"

copy "$ENV:TEMP\config.tmp" "$ENV:APPDATA\RosBE\config.rbuild"
del $ENV:TEMP\config.tmp
copy "$ENV:APPDATA\RosBE\config.rbuild" ".\config.rbuild"

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
$SARCH = $null
$OARCH = $null
$OPTIMIZE = $null
$KDBG = $null
$DBG = $null
$GDB = $null
$NSWPAT = $null
$WINKD = $null

settitle
