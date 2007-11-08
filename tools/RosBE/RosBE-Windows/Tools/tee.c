/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/tee.c
 * PURPOSE:     Spit stdin to stdout and a file.
 * COPYRIGHT:   Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    unsigned char charbuff;
    FILE *FILE;

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
        printf("Usage: %s FILE\n", argv[0]);
        printf("Takes standard input and sends it to standard output\n");
        printf("and FILE.\n\n");
        return 0;
    }

    FILE = fopen(argv[1], "w");
    if (!FILE)
    {
        fprintf(stderr, "%s: Error cannot create/open file \"%s\".\n", argv[0], argv[1]);
        return -1;
    }
    while (!feof(stdin))
    {
        charbuff = fgetc(stdin);
        if (!feof(stdin))
        {
            fputc(charbuff, stdout);
            fputc(charbuff, FILE);
        }
    }
    if (fclose(FILE))
    {
        fprintf(stderr, "%s: Error closing file \"%s\"\n", argv[0], argv[1]);
        return -1;
    }

    return 0;
}
