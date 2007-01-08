#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    LPTSTR CommandLine, FullCommandLine, CommandLineBuffer;
    time_t StartTime, FinishTime;
    float TotalTime;
    int Hour, Minute, Second;

    //
    // If nothing is on the command-line exit
    //
    if (argc == 1)
    {
        printf("Required parameter not specified. Exiting.\n");
        return 1;
    }

    //
    // Get the command line to pass on.
    //
    FullCommandLine = GetCommandLine();
    CommandLine = &FullCommandLine[strlen(argv[0]) + 1];
    CommandLineBuffer = (LPTSTR) malloc((strlen(CommandLine) + 3));
    if (!CommandLineBuffer)
    {
        printf("Unable to allocate memory. Exiting.\n");
        return 1;
    }
    strcpy(CommandLineBuffer, "\"");
    strcat(CommandLineBuffer, CommandLine);
    strcat(CommandLineBuffer, "\"");

    //
    // Grab the starting timestamp.
    //
    time(&StartTime);

    //
    // Run the program (Status is 1 on failure).
    //
    system(CommandLineBuffer);

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

    return 0;
}
