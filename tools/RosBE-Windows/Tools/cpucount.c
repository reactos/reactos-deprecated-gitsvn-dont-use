/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/cpucount.c
 * PURPOSE:     CPU Core Counter
 * COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
 *              Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */

#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    SYSTEM_INFO SystemInformation;

    if (argc == 1)
    {
        GetSystemInfo(&SystemInformation);
        printf("%ld\n", SystemInformation.dwNumberOfProcessors);
        return 0;
    }
    else if (!strcmp(argv[1], "-x1"))
    {
        GetSystemInfo(&SystemInformation);
        printf("%ld\n", (SystemInformation.dwNumberOfProcessors + 1));
        return 0;
    }
    else if (!strcmp(argv[1], "-x2"))
    {
        GetSystemInfo(&SystemInformation);
        printf("%ld\n", (SystemInformation.dwNumberOfProcessors + SystemInformation.dwNumberOfProcessors));
        return 0;
    }
    else
    {
        printf("Unknown parameter specified. Exiting.\n");
        return 1;
    }
}
