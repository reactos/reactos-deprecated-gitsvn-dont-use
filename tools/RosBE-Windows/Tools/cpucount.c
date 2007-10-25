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

    if (argc > 2)
    {
        fprintf(stderr, "%s: Error too many parameters specified.\n", argv[0]);
        return -1;
    }
    GetSystemInfo(&SystemInformation);
    if (argc == 1)
    {
        printf("%ld\n", SystemInformation.dwNumberOfProcessors);
    }
    else if ((!strncmp(argv[1], "/?", 2)) ||
             (!strncmp(argv[1], "-h", 2)) ||
             (!strncmp(argv[1], "--help", 6)))
    {
        printf("Usage: %s [OPTIONS]\n", argv[0]);
        printf("Running cpucount without options returns\n");
        printf("the number of processors in the system.\n");
        printf("-x1    - Number of processors in the system, plus 1.\n");
        printf("-x2    - Number of processors in the system, doubled.\n");
    }
    else if (!strncmp(argv[1], "-x1", 3))
    {
        printf("%ld\n", (SystemInformation.dwNumberOfProcessors + 1));
    }
    else if (!strncmp(argv[1], "-x2", 3))
    {
        printf("%ld\n", (SystemInformation.dwNumberOfProcessors + SystemInformation.dwNumberOfProcessors));
    }
    else
    {
        fprintf(stderr, "%s: Error unknown parameter '%s' specified.\n", argv[0], argv[1]);
        return -1;
    }

    return 0;
}
