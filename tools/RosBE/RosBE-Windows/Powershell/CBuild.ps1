#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/CBuild.ps1
# PURPOSE:     Perform the CMake build of ReactOS.
# COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
#

if ($_ROSBE_WRITELOG -eq 1) {
    if (!(Test-Path "$_ROSBE_LOGDIR")) {
        New-Item -path "$_ROSBE_LOGDIR" -type directory
    }
    $file1 = "$_ROSBE_LOGDIR\BuildToolLog-$ENV:ROS_ARCH-$DATENAME-$TIMENAME.txt"
    $file2 = "$_ROSBE_LOGDIR\BuildROSLog-$ENV:ROS_ARCH-$DATENAME-$TIMENAME.txt"
}

# Get the current date and time for use in in our build log's file name.
$TIMERAW = get-date -f t
$DATENAME = get-date -f dMMyyyy
$TIMENAME = get-date -f Hms

# Check whether we were called as "makex" or "make"
if ("$($args[0])" -eq "multi") {
    $args.setvalue($null, 0)
    $MAKE_JOBS = "$_ROSBE_MAKEX_JOBS"
    $TITLE_COMMAND = "makex $($args)"
} else {
    $MAKE_JOBS = 1
    $TITLE_COMMAND = "make $($args)"
}

$host.ui.RawUI.WindowTitle = "'$TITLE_COMMAND' cmake build started: $TIMERAW   ($ENV:ROS_ARCH)"

# Do the actual building
if ($_ROSBE_SHOWTIME -eq 1) {
    [System.Diagnostics.Stopwatch] $sw;
    $sw = New-Object System.Diagnostics.StopWatch
    $sw.Start()
}

if (!(Test-Path "build")) {
        New-Item -path "build" -type directory
    }

cd build
&{IEX "&'cmake.exe' -G ""MinGW Makefiles"" ..\"}

if ($_ROSBE_WRITELOG -eq 1) {
    &{IEX "&'mingw32-make.exe' -j $MAKE_JOBS $($args)"} $($args) 2>&1 | tee-object $file1
} else {
    &{IEX "&'mingw32-make.exe' -j $MAKE_JOBS $($args)"} $($args)
}

cd..
""

if (!(Test-Path "build-ros")) {
        New-Item -path "build-ros" -type directory
    }

cd build-ros
&{IEX "&'cmake.exe' -G ""MinGW Makefiles"" -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw32.cmake ..\"}

if ($_ROSBE_WRITELOG -eq 1) {
    &{IEX "&'mingw32-make.exe' -j $MAKE_JOBS $($args)"} $($args) 2>&1 | tee-object $file2
} else {
    &{IEX "&'mingw32-make.exe' -j $MAKE_JOBS $($args)"} $($args)
}

cd..

if ($_ROSBE_SHOWTIME -eq 1) {
    $sw.Stop()
    write-host "Total Build Time:" $sw.Elapsed.ToString()
}

# Highlight the fact that building has ended.
FlashWindow (ps -id $pid).MainWIndowHandle $true

$sound = new-Object System.Media.SoundPlayer;

if ($LASTEXITCODE -ne 0) {
    $sound.SoundLocation="$_ROSBE_BASEDIR\samples\error.wav";
} else {
    $sound.SoundLocation="$_ROSBE_BASEDIR\samples\notification.wav";
}

$sound.Play();

$host.ui.RawUI.WindowTitle = "ReactOS Build Environment $_ROSBE_VERSION"
