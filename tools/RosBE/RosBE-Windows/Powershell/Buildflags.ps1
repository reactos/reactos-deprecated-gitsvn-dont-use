#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Buildflags.cmd
# PURPOSE:     Sets the RBuild
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

$temparg = $args[1]

if ($args -eq "verbose") {
    # Be verbose.
    $ENV:ROS_RBUILDFLAGS = "-v"
} elseif ($args -eq "lowhddspace") {
    # Clean as you go. Delete generated files as soon as they are not needed anymore.
    $ENV:ROS_RBUILDFLAGS = "-c"
} elseif ($args -eq "noautodep") {
    # Disable automatic dependencies.
    $ENV:ROS_RBUILDFLAGS = "-dd"
} elseif ($args -eq "autodep") {
    # Enable automatic dependencies.
    $ENV:ROS_RBUILDFLAGS = "-da"
} elseif ($args -eq "fullautodep") {
    # Enable full dependencies.
    $ENV:ROS_RBUILDFLAGS = "-df"
} elseif ($args -eq "depforx") {
    # Check only automatic dependencies for this module.
    $ENV:ROS_RBUILDFLAGS = "-dm{$temparg}"
} elseif ($args -eq "noprecompheaders") {
    # Disable precompiled headers.
    $ENV:ROS_RBUILDFLAGS = "-hd"
} elseif ($args -eq "makegendir") {
    # Let make handle creation of install directories. Rbuild will not generate the directories.
    $ENV:ROS_RBUILDFLAGS = "-mi"
} elseif ($args -eq "proxymakefiles") {
    # Generate proxy makefiles in source tree instead of the output tree.
    $ENV:ROS_RBUILDFLAGS = "-ps"
} elseif ($args -eq "nocompunits") {
    # Disable compilation units.
    $ENV:ROS_RBUILDFLAGS = "-ud"
} elseif ($args -eq "inputxml") {
    # Input XML
    $ENV:ROS_RBUILDFLAGS = "-r"
} elseif ($args -eq "installpath") {
    # This variable controls where to install output files to when using
    # 'make install'. N.B. Don't put a path separator at the end. The variable
    # defaults to .\{ROS_CDOUTPUT}.
    $ENV:ROS_INSTALL = "$temparg"
} elseif ($args -eq "buildengine") {
    # The Build engine to be used. The variable defaults to rbuild (RBUILD_TARGET)
    $ENV:ROS_BUILDENGINE = "$temparg"
} elseif ($args -eq "reset") {
    $ENV:ROS_RBUILDFLAGS = $null
    $ENV:ROS_INSTALL = $null
    $ENV:ROS_BUILDENGINE = $null
}
