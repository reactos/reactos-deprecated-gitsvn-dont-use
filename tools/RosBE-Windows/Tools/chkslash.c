/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/chkslash.c
 * PURPOSE:     Checks if a string has a backslash '\' in it or not.
 * COPYRIGHT:   Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int hasslash = 0;
    unsigned int i = 0;

    if (argc > 2)
    {
        fprintf(stderr, "%s: Error too many parameters specified.\n", argv[0]);
        return -1;
    }
    if ((argc == 1) ||
        (!strncmp(argv[1], "/?", 2)) ||
        (!strncmp(argv[1], "-h", 2)) ||
        (!strncmp(argv[1], "--help", 6)))
    {
        printf("Usage: %s STRING\n", argv[0]);
        printf("Checks if STRING has a backslash or not. Returns\n");
        printf("1 if STRING has a backslash and 2 if not.\n\n");
        return 0;
    }
    for (i = 0; i < strlen(argv[1]); i++)
    {
        if (argv[1][i] == '\\')
        {
            hasslash = 1;
        }
    }

    if (hasslash)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
