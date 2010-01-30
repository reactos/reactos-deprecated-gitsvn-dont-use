/* Program for computing the build time.

   Developed by Colin Finck <colin@reactos.org>
   Derived from "buildtime.c" of RosBE for Windows
   Released under GNU GPL v2 or any later version.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
	pid_t ProcessId;

	/* Do we have command line arguments? */
	if(argc <= 1)
	{
		fprintf(stderr, "buildtime: No command line arguments!\n");
		return 1;
	}

	ProcessId = fork();
	if(ProcessId < 0)
	{
		/* Error */
		fprintf(stderr, "buildtime: fork() failed!\n");
		return 1;
	}
	else if(ProcessId == 0)
	{
		/* Child process */
		execvp(argv[1], &argv[1]);
		
		/* If this function returned, an error occured */
		fprintf(stderr, "execvp() failed!\n");
		return 1;
	}
	else
	{
		/* Parent process */
		double TotalTime;
		int Hour;
		int Minute;
		int Ret;
		int Second;
		time_t StartTime;
		time_t FinishTime;
		
		time(&StartTime);

		if(wait(&Ret) != ProcessId)
		{
			fprintf(stderr, "buildtime: wait() failed!\n");
			return 1;
		}
		
		time(&FinishTime);

		/* Compute the needed time and print it */
		TotalTime = difftime(FinishTime, StartTime);

		Second = (int)TotalTime % 60;
		TotalTime = TotalTime / 60;
		Minute = (int)TotalTime % 60;
		Hour = TotalTime / 60;

		printf("\nTotal Build Time: %02d:%02d:%02d\n", Hour, Minute, Second);
		return WEXITSTATUS(Ret);
	}
}
