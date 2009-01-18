#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Build.ps1
# PURPOSE:     Perform the build of ReactOS.
# COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
#
#

#
# Check if config.template.rbuild is newer than config.rbuild, if it is then
# abort the build and inform the user.
#
if (Test-Path ".\config.rbuild") {
    if ((gi .\config.template.rbuild).LastWriteTime -gt (gi .\config.rbuild).LastWriteTime) {
        ""
        "*** config.template.rbuild is newer than config.rbuild ***"
        "*** aborting build. Please check for changes and       ***"
        "*** update your config.rbuild.                         ***"
        ""
        exit
    }
}

#
# Check if strip, no Debug Symbols or ccache are being used and set the appropriate options.
#
if ($_ROSBE_NOSTRIP -ne $null) {
    if ($_ROSBE_NOSTRIP -eq 1) {
        $ENV:ROS_BUILDNOSTRIP = "yes"
    } else {
        $ENV:ROS_BUILDNOSTRIP = "no"
    }
}
if ($_ROSBE_STRIP -ne $null) {
    if ($_ROSBE_STRIP -eq 1) {
        $ENV:ROS_LEAN_AND_MEAN = "yes"
    } else {
        $ENV:ROS_LEAN_AND_MEAN = "no"
    }
}
# Small Security Check to prevent useless apps.
if ($ENV:ROS_LEAN_AND_MEAN -eq "yes") {
    if ($ENV:ROS_BUILDNOSTRIP -eq "yes") {
        clear-host
        "Selecting Stripping and removing Debug Symbols together will most likely cause useless apps. Please deselect one of them."
        exit
    }
}

if ($_ROSBE_USECCACHE -ne $null) {
    if ($_ROSBE_USECCACHE -eq 1) {
        $ENV:CCACHE_DIR = "$APPDATA\RosBE\.ccache"
        $_ROSBE_CCACHE = "ccache "
    }
    $ENV:HOST_CC = "$_ROSBE_CCACHE" + "gcc"
    $ENV:HOST_CPP = "$_ROSBE_CCACHE" + "g++"

    #
    #Target defaults to host(i386)
    #

    $ENV:TARGET_CC = "$_ROSBE_CCACHE" + "gcc"
    $ENV:TARGET_CPP = "$_ROSBE_CCACHE" + "g++"
    if ($ENV:ROS_ARCH -ne $null) {
        $ENV:TARGET_CC = "$_ROSBE_CCACHE" + "$ROS_ARCH-pc-mingw32-gcc"
        $ENV:TARGET_CPP = "$_ROSBE_CCACHE" + "$ROS_ARCH-pc-mingw32-g++"
    }
}

#
# Check if the user has chosen to use a different object or output path and set
# it accordingly.
#
if ($_ROSBE_OBJPATH -ne $null) {
    if (Test-Path "$_ROSBE_OBJPATH") {
        "WARNING: The Object-Path specified doesn't seem to exist. Creating..."
    } 
    $ENV:ROS_INTERMEDIATE = "$_ROSBE_OBJPATH"
}
if ($_ROSBE_OUTPATH -ne $null) {
    if (Test-Path "$_ROSBE_OUTPATH") {
        "WARNING: The Output-Path specified doesn't seem to exist. Creating..."
    }
    $ENV:ROS_OUTPUT = "$_ROSBE_OUTPATH"
    $ENV:ROS_TEMPORARY = "$_ROSBE_OUTPATH"
}

#
# Get the current date and time for use in in our build log's file name.
#
$TIMERAW = get-date -f t
$DATENAME = get-date -f dyMMyyyy
$TIMENAME = get-date -f HHmm

#
# Check if writing logs is enabled, if so check if our log directory exists, if
# it doesn't, create it.
#
if ($_ROSBE_WRITELOG -eq 1) {
    if (!(Test-Path "$_ROSBE_LOGDIR")) {
        $null = (New-Item -path "$_ROSBE_ROSSOURCEDIR" -name "RosBE-Logs" -type directory)
    }
}

function BUILD {
    $ErrorActionPreference = "SilentlyContinue"
    if ($_ROSBE_SHOWTIME -eq 1) {
        [System.Diagnostics.Stopwatch] $sw;
        $sw = New-Object System.Diagnostics.StopWatch
        if ($_ROSBE_WRITELOG -eq 1) {
            if (!(Test-Path "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt")) {
                $null = (New-Item -path "$_ROSBE_LOGDIR" -name "BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt" -type file)
            }
            $sw.Start()
            IEX "&'$_ROSBE_MINGWMAKE' $($args)" 2>&1 | tee-object -filepath "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt"
            $sw.Stop()
            write-host "Total Build Time:" $sw.Elapsed.ToString()
        } else {
            $sw.Start()
            IEX "&'$_ROSBE_MINGWMAKE' $($args)"
            $sw.Stop()
            write-host "Total Build Time:" $sw.Elapsed.ToString()
        }
    } else {
        if ($_ROSBE_WRITELOG -eq 1) {
            if (!(Test-Path "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt")) {
                $null = (New-Item -path "$_ROSBE_LOGDIR" -name "BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt" -type file)
            }
            IEX "&'$_ROSBE_MINGWMAKE' $($args)" 2>&1 | tee-object -filepath "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt"
        } else {
            IEX "&'$_ROSBE_MINGWMAKE' $($args)"
        }
    }
}

function BUILDMULTI {
    #
    # Get the number of CPUs in the system so we know how many jobs to execute.
    $CPUCOUNT = (gwmi win32_processor).numberofcores + 1

    if ($_ROSBE_SHOWTIME -eq 1) {
        [System.Diagnostics.Stopwatch] $sw;
        $sw = New-Object System.Diagnostics.StopWatch
        if ($_ROSBE_WRITELOG -eq 1) {
            if (!(Test-Path "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt")) {
                $null = (New-Item -path "$_ROSBE_LOGDIR" -name "BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt" -type file)
            }
            $sw.Start()
            IEX "&'$_ROSBE_MINGWMAKE' -j $CPUCOUNT $($args)" 2>&1 | tee-object -filepath "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt"
            $sw.Stop()
            write-host "Total Build Time:" $sw.Elapsed.ToString()
        } else {
            $sw.Start()
            IEX "&'$_ROSBE_MINGWMAKE' -j $CPUCOUNT $($args)"
            $sw.Stop()
            write-host "Total Build Time:" $sw.Elapsed.ToString()
        }
    } else {
        if ($_ROSBE_WRITELOG -eq 1) {
            if (!(Test-Path "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt")) {
                $null = (New-Item -path "$_ROSBE_LOGDIR" -name "BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt" -type file)
            }
            IEX "&'$_ROSBE_MINGWMAKE' -j $CPUCOUNT $($args)" 2>&1 | tee-object -filepath "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_GCCVERSION-$DATENAME-$TIMENAME.txt"
        } else {
            IEX "&'$_ROSBE_MINGWMAKE' -j $CPUCOUNT $($args)"
        }
    }
}

#
# Check if we are using -j or not.
#
if ($args[0] -eq "multi") {
    $args.setvalue($null, 0)
    $host.ui.RawUI.WindowTitle = "''makex $($args)'' parallel build started: $TIMERAW   $ENV:ROS_ARCH"
    BUILDMULTI $args
} else {
    $host.ui.RawUI.WindowTitle = "''make $($args)'' build started: $TIMERAW   $ENV:ROS_ARCH"
    BUILD $args
}

#
# Highlight the fact that building has ended.
#
"$_ROSBE_BASEDIR\Tools\flash.exe"

if ($_ROSBE_VERSION -ne $null) {
    $host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
}

#
# Unload all used Vars.
#
$ENV:ROS_BUILDNOSTRIP = $null
$ENV:ROS_LEAN_AND_MEAN = $null
$ENV:CCACHE_DIR = $null
$ENV:HOST_CC = $null
$ENV:HOST_CPP = $null
$ENV:TARGET_CC = $null
$ENV:TARGET_CPP = $null
$ENV:ROS_INTERMEDIATE = $null
$ENV:ROS_OUTPUT = $null
$ENV:ROS_TEMPORARY = $null
$TIMERAW = $null
$DATENAME = $null
$TIMENAME = $null
$CPUCOUNT = $null
$sw = $null
$_ROSBE_CCACHE = $null
