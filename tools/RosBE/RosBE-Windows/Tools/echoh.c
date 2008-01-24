/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GNU General Public License v2. (see LICENSE.txt)
 * FILE:        Tools/echoh.c
 * PURPOSE:     Converts a value to hex and displays it
 * COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc == 2)
        printf("%x", atoi(argv[1]));

    return 0;
}
