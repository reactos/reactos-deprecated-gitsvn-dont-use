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

    if(argc != 1)
    {
        if(!strncmp(argv[1], "-x1", 3))
        {
            SystemInformation.dwNumberOfProcessors++;
        }
        else if(!strncmp(argv[1], "-x2", 3))
        {
            SystemInformation.dwNumberOfProcessors += SystemInformation.dwNumberOfProcessors;
        }
        else
        {
            printf("Usage: %s [OPTIONS]\n", argv[0]);
            printf("Running cpucount without options returns the number of\n");
            printf("processors in the system.\n");
            printf("-x1    - Number of processors in the system, plus 1.\n");
            printf("-x2    - Number of processors in the system, doubled.\n");
            return 0;
        }
    }

    printf("%u\n", SystemInformation.dwNumberOfProcessors);
    return 0;
}
