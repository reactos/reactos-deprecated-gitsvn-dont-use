/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GNU General Public License v2. (see LICENSE.txt)
 * FILE:        Tools/buildtime.c
 * PURPOSE:     Buildtime Counter
 * COPYRIGHT:   Copyright 2007 KJK::Hyperion
 *              Copyright 2007 Peter Ward <dralnix@gmail.com>
 *              Copyright 2010 Colin Finck <colin@reactos.org>
 *
 */

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

static PWSTR SkipSelfArgument(PWSTR lpszCommandLine)
{
    PWSTR p = lpszCommandLine;
    int quote = 0;

    // Skip leading whitespace
    while(*p != 0 && iswspace(*p))
        ++ p;

    if(*p == 0)
        return p;

    // Skip argument 0
    // BUGBUG: the assumption is that argument 0 never contains escaped quotes
    do
    {
        if(*p == L'\"')
        {
            quote = !quote;
            ++ p;
            continue;
        }

        ++ p;
    }
    while(*p != 0 && (quote || !iswspace(*p)));

    // Skip trailing whitespace
    while(*p != 0 && iswspace(*p))
        ++ p;

    return p;
}

int main()
{
    DWORD ExitCode;
    PWSTR CommandLine;
    time_t StartTime, FinishTime;
    double TotalTime;
    int Hour, Minute, Second;
    PROCESS_INFORMATION ProcessInfo;
    STARTUPINFOW StartupInfo = {0};

    // Get the command line to pass on.
    CommandLine = SkipSelfArgument(GetCommandLineW());

    // If nothing is on the command-line exit
    if (CommandLine[0] == 0)
    {
        fprintf(stderr, "buildtime: required parameter not specified\n");
        return 1;
    }

    // Grab the starting timestamp.
    time(&StartTime);

    // Run the program and get its exit code.
    StartupInfo.cb = sizeof(StartupInfo);

    if(!CreateProcessW(NULL, CommandLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartupInfo, &ProcessInfo))
    {
        fprintf(stderr, "buildtime: CreateProcessW() failed!\n");
        return 1;
    }

    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    GetExitCodeProcess(ProcessInfo.hProcess, &ExitCode);

    // Grab the finishing timestamp.
    time(&FinishTime);

    // Get the elapsed time.
    TotalTime = difftime(FinishTime, StartTime);

    // Convert the elapsed time to a human readable format.
    Second = (int)TotalTime % 60;
    TotalTime = TotalTime / 60;
    Minute = (int)TotalTime % 60;
    Hour = TotalTime / 60;

    // Print the total build time.
    printf("\nTotal Build Time: %02d:%02d:%02d", Hour, Minute, Second);

    return ExitCode;
}
