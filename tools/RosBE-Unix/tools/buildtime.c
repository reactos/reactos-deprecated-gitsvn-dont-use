/* Program for computing the build time.

   Developed by Colin Finck <mail@colinfinck.de>
   Derived from "buildtime.c" of RosBE for Windows
   Released under GNU GPL v2 or any later version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int
main(int argc,
     char* argv[])
{
	char* CommandLine;
	double TotalTime;
	int i;
	int CommandLineLength = 0;
	int Hour;
	int Minute;
	int Ret;
	int Second;
	time_t StartTime;
	time_t FinishTime;

	// Do we have command line arguments?
	if( argc <= 1 )
	{
		fprintf( stderr, "buildtime: No command line arguments!\n" );
		return 1;
	}

	// First compute the memory size to allocate
	for( i = 1; i < argc; i++ )
		// Every argument is enclosed between quotes and followed by a space.
		// The last argument is followed by a terminating null character instead of a space.
		CommandLineLength += 3 + strlen( argv[i] );

	// Now allocate the needed memory
	CommandLine = malloc( CommandLineLength + 1 );
	if( !CommandLine )
	{
		fprintf( stderr, "buildtime: Unable to allocate memory!\n" );
		return 1;
	}

	memset( CommandLine, 0, CommandLineLength + 1 );

	// Put the command line into the variable
	for( i = 1; i < argc; i++ )
	{
		strcat( CommandLine, "\"" );
		strcat( CommandLine, argv[i] );
		strcat( CommandLine, "\" " );
	}

	CommandLine[CommandLineLength] = 0;

	// Get the timestamps and execute the command line
	time( &StartTime );
	Ret = system( CommandLine );
	time( &FinishTime );

	// Compute the needed time and print it
	TotalTime = difftime( FinishTime, StartTime );

	Second = (int)TotalTime % 60;
	TotalTime = TotalTime / 60;
	Minute = (int)TotalTime % 60;
	Hour = TotalTime / 60;

	printf( "\nTotal Build Time: %02d:%02d:%02d\n", Hour, Minute, Second );

	// Final actions
	free( CommandLine );
	return Ret;
}
