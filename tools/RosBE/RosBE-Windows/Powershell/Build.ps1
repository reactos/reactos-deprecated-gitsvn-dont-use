#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Build.ps1
# PURPOSE:     Perform the build of ReactOS.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

# Check if config.template.rbuild is newer than config.rbuild, if it is then
# abort the build and inform the user.
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

if ($_ROSBE_USECCACHE -eq 1) {
    $_ROSBE_CCACHE = "ccache "
} else {
    $_ROSBE_CCACHE = $null
}
    $ENV:HOST_CC = "$_ROSBE_CCACHE" + "gcc"
    $ENV:HOST_CPP = "$_ROSBE_CCACHE" + "g++"
    $ENV:TARGET_CC = "$_ROSBE_CCACHE" + "$_ROSBE_PREFIX" + "gcc"
    $ENV:TARGET_CPP = "$_ROSBE_CCACHE" + "$_ROSBE_PREFIX" + "g++"

# Get the current date and time for use in in our build log's file name.
$TIMERAW = get-date -f t
$DATENAME = get-date -f dyMMyyyy
$TIMENAME = get-date -f HHmm

# Check whether we were called as "makex" or "make"
if ("$($args[0])" -eq "multi") {
    $args.setvalue($null, 0)
    $MAKE_JOBS = "$_ROSBE_MAKEX_JOBS"
    $TITLE_COMMAND = "makex $($args)"
} else {
    $MAKE_JOBS = 1
    $TITLE_COMMAND = "make $($args)"
}

$host.ui.RawUI.WindowTitle = "'$TITLE_COMMAND' build started: $TIMERAW   ($ENV:ROS_ARCH)"

# Do the actual building
if ($_ROSBE_SHOWTIME -eq 1) {
    [System.Diagnostics.Stopwatch] $sw;
    $sw = New-Object System.Diagnostics.StopWatch
    $sw.Start()
}

if ($_ROSBE_WRITELOG -eq 1) {
    if (!(Test-Path "$_ROSBE_LOGDIR")) {
        New-Item -path "$_ROSBE_LOGDIR" -type directory
    }
    $file = "$_ROSBE_LOGDIR\BuildLog-$_ROSBE_TARGET_GCCVERSION-$DATENAME-$TIMENAME.txt"
    &{IEX "&'make.exe' -j $MAKE_JOBS $($args)"} $($args) 2>&1 | tee-object $file
} else {
    &{IEX "&'make.exe' -j $MAKE_JOBS $($args)"} $($args)
}
if ($_ROSBE_SHOWTIME -eq 1) {
    $sw.Stop()
    write-host "Total Build Time:" $sw.Elapsed.ToString()
}

# Highlight the fact that building has ended.
FlashWindow (ps -id $pid).MainWIndowHandle $true
$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
