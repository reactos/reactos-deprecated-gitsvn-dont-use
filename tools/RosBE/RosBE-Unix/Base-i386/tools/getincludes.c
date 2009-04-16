/*
 * PROJECT:     ReactOS Build Environment Tools
 * LICENSE:     GNU GPLv2 or any later version as published by the Free Software Foundation
 * PURPOSE:     Parse the search directory output of 'cpp -v' to compiler flags
 * COPYRIGHT:   Copyright 2009 Colin Finck <colin@reactos.org>
 */

#include <stdio.h>
#include <string.h>

int main()
{
    char Line[1024];

    /* Skip all lines up to the second #include */
    for(;;)
    {
        if(!fgets(Line, sizeof(Line), stdin))
            return 1;

        if(strstr(Line, "#include <...> "))
            break;
    }
 
    /* The pathes should now follow prefixed by a single space */
    for(;;)
    {
        if(!fgets(Line, sizeof(Line), stdin) || *Line != ' ')
            break;

        /* This is an include directory, remove the newline character and output it */
        Line[strlen(Line) - 1] = 0;
        printf("-I%s ", &Line[1]);
    }

    printf("\n");
    return 0;
}
