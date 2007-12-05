/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/rquote.c
 * PURPOSE:     Removes all quotes from a string.
 * COPYRIGHT:   Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int i = 0;

    if (argc > 2)
    {
        fprintf(stderr, "%s: Error too many parameters specified.\n", argv[0]);
        return -1;
    }
    if ((argc == 1) ||
        (!strncmp(argv[1], "/?", 2)) ||
        (!_strnicmp(argv[1], "-h", 2)) ||
        (!_strnicmp(argv[1], "--help", 6)))
    {
        printf("Usage: %s STRING\n", argv[0]);
        printf("Removes all quotes from STRING.\n\n");
        return 0;
    }
    for (i = 0; i < strlen(argv[1]); i++)
    {
        if ((argv[1][i] == '\"') || (argv[1][i] == '\''))
            continue;
        else
            fputc(argv[1][i], stdout);
    }

    return 0;
}
