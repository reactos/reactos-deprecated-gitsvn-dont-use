/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        Tools/buildtime.c
 * PURPOSE:     Buildtime Counter
 * COPYRIGHT:   Copyright 2007 KJK::Hyperion
 *              Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */


#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <tchar.h>

static LPTSTR SkipSelfArgument(LPTSTR lpszCommandLine)
{
	LPTSTR p = lpszCommandLine;
	int quote = 0;

	// Skip leading whitespace
	while(*p != 0 && _istspace(*p))
		++ p;

	if(*p == 0)
		return p;

	// Skip argument 0
	// BUGBUG: the assumption is that argument 0 never contains escaped quotes
	do
	{
		if(*p == TEXT('\"'))
		{
			quote = !quote;
			++ p;
			continue;
		}

		++ p;
	}
	while(*p != 0 && (quote || !_istspace(*p)));

	// Skip trailing whitespace
	while(*p != 0 && _istspace(*p))
		++ p;

	return p;
}

int main()
{
    LPTSTR CommandLine, FullCommandLine, CommandLineBuffer;
    time_t StartTime, FinishTime;
    double TotalTime;
    int Hour, Minute, Second;
	int Status;

    //
    // Get the command line to pass on.
    //
    FullCommandLine = GetCommandLine();
    CommandLine = SkipSelfArgument(FullCommandLine);

    //
    // If nothing is on the command-line exit
    //
    if (CommandLine[0] == 0)
    {
		fprintf(stderr, "buildtime: required parameter not specified\n");
        return 1;
    }

    CommandLineBuffer = malloc((strlen(CommandLine) + 2 + 1));
    if (!CommandLineBuffer)
    {
		fprintf(stderr, "buildtime: unable to allocate memory\n");
        return 1;
    }

	CommandLineBuffer[0] = 0;
    //strcat(CommandLineBuffer, "\"");
    strcat(CommandLineBuffer, CommandLine);
    //strcat(CommandLineBuffer, "\"");

    //
    // Grab the starting timestamp.
    //
    time(&StartTime);

    //
    // Run the program (Status is 1 on failure).
    //
    Status = system(CommandLineBuffer);

    //
    // Grab the finishing timestamp.
    //
    time(&FinishTime);

    //
    // Get the elapsed time.
    //
    TotalTime = difftime(FinishTime, StartTime);

    //
    // Convert the elapsed time to a human readable format.
    //
    Second = (int)TotalTime % 60;
    TotalTime = TotalTime / 60;
    Minute = (int)TotalTime % 60;
    Hour = TotalTime / 60;

    //
    // Print the total build time.
    //
    printf("\nTotal Build Time: %02d:%02d:%02d", Hour, Minute, Second);

    //
    // Free the memory we allocated for the command line.
    //
    free(CommandLineBuffer);

    return Status;
}
