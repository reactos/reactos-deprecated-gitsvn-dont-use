#
# PROJECT:     RosBE - ReactOS Build Environment for Windows
# LICENSE:     GNU General Public License v2. (see LICENSE.txt)
# FILE:        Root/Remake.ps1
# PURPOSE:     Fully re-make one or several modules.
# COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
#

$argindex = 0
while ( "$($args[$argindex])" -ne "") {
    $cl = "$($args[$argindex])" + "_clean" + " " + "$($args[$argindex])"
    make $cl
    $argindex += 1
}
remove-variable cl
