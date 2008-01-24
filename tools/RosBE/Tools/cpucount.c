/*
 * PROJECT:     ReactOS Build Environment Tools
 * LICENSE:     GNU General Public License v2. (see LICENSE.txt)
 * FILE:        Tools/cpucount.c
 * PURPOSE:     CPU Core Counter
 * COPYRIGHT:   Copyright 2007 Christoph von Wittich <Christoph_vW@reactos.org>
 *              Copyright 2007 Colin Finck <mail@colinfinck.de>
 *              Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */

#if defined(WIN32)
#   include <windows.h>
#elif defined(__APPLE__)
#   include <sys/sysctl.h>
#else
#   include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int cpuCount = 0;

#if defined(WIN32)
    if(argc > 3)
#else
    if(argc > 2)
#endif
    {
        fprintf(stderr, "%s: Error too many parameters specified.\n", argv[0]);
        return -1;
    }

#if defined(WIN32)
    if (argc != 1 && !strncmp(argv[argc - 1], "-a", 2))
    {
        DWORD_PTR ProcessAffinityMask, SystemAffinityMask;

        if (GetProcessAffinityMask(GetCurrentProcess(), &ProcessAffinityMask, &SystemAffinityMask))
        {
            while (ProcessAffinityMask != 0)
            {
                if (ProcessAffinityMask & 0x1)
                    cpuCount++;

                ProcessAffinityMask >>= 1;
            }
        }

        argc--; /* don't look at this argument again */
    }
    else
    {
        SYSTEM_INFO SystemInformation;

        GetSystemInfo(&SystemInformation);
        cpuCount = SystemInformation.dwNumberOfProcessors;
    }
#elif defined(__APPLE__)
    size_t countSize = sizeof(cpuCount);

    sysctlbyname("hw.logicalcpu", &cpuCount, &countSize, NULL, 0);
#else
    cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    if (argc != 1)
    {
        if(!strncmp(argv[1], "-x1", 3))
        {
            cpuCount++;
        }
        else if(!strncmp(argv[1], "-x2", 3))
        {
            cpuCount += cpuCount;
        }
        else
        {
#if defined(WIN32)
            printf("Usage: %s [-x1|-x2] [-a]\n", argv[0]);
#else
            printf("Usage: %s [-x1|-x2]\n", argv[0]);
#endif
            printf("Running cpucount without options returns the number of\n");
            printf("processors in the system.\n");
            printf("-x1    - Number of processors in the system, plus 1.\n");
            printf("-x2    - Number of processors in the system, doubled.\n");
#if defined(WIN32)
            printf("-a     - Use the affinity mask to determine number of processors\n");
#endif
            return 0;
        }
    }

    printf("%u\n", cpuCount);
    return 0;
}
