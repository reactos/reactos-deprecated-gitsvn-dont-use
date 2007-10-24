/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/chknewer.c
 * PURPOSE:     Check if a file is newer than another.
 * COPYRIGHT:   Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

time_t
StatFile(
    char* FileName
);

int main(int argc, char* argv[])
{
    FILE *FILE1, *FILE2;
    time_t file1time = 0, file2time = 0;

    if (argc > 3)
    {
        printf("%s: Error too many parameters specified.\n", argv[0]);
        return -1;
    }
    if ((argc == 1) ||
        (!strncmp(argv[1], "/?", 2)) ||
        (!strncmp(argv[1], "-h", 2)) ||
        (!strncmp(argv[1], "--help", 6)))
    {
        printf("Usage: %s FILE1 FILE2\n", argv[0]);
        printf("Checks if FILE1 is newer than FILE2 by checking\n");
        printf("the last data modification time's. Returns 1 if\n");
        printf("FILE1 is newer and 2 if FILE2 is.\n\n");
        return 0;
    }

    FILE1 = fopen(argv[1], "r");
    if (!FILE1)
    {
        printf("%s: Error file \"%s\" doesn't seem to exist.\n", argv[0], argv[1]);
        return -1;
    }
    else
    {
        if (fclose(FILE1))
        {
            printf("%s: Error closing file \"%s\"\n", argv[0], argv[1]);
        }
        file1time = StatFile(argv[1]); 
        if (!file1time)
        {
            printf("%s: Error unable to aquire stats for file: %s\n", argv[0], argv[1]);
            return -1;
        }
    }

    FILE2 = fopen(argv[2], "r");
    if (!FILE2)
    {
        printf("%s: Error file \"%s\" doesn't seem to exist.\n", argv[0], argv[2]);
        return -1;
    }
    else
    {
        if (fclose(FILE2))
        {
            printf("%s: Error closing file \"%s\"\n", argv[0], argv[2]);
        }
        file2time = StatFile(argv[2]); 
        if (!file2time)
        {
            printf("%s: Error unable to aquire stats for file: %s\n", argv[0], argv[2]);
            return -1;
        }
    }

    if (file1time > file2time)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

time_t StatFile(char* FileName)
{
    struct stat filestat;

    if (!stat(FileName, &filestat))
    {
        return mktime(localtime(&filestat.st_mtime));
    }
    else
    {
        return 0;
    }
}
