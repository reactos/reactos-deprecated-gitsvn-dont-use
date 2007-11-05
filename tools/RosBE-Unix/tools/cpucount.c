/* Program for printing the CPU count.
   The output value will be used for the -j option of gcc.

   Developed by Colin Finck <mail@colinfinck.de>
   Released under GNU GPL v2 or any later version.
*/

#include <stdio.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

int main(int argc, char *argv[])
{
	int cpuCount = 0;

	if(argc > 2)
	{
		fprintf(stderr, "%s: Error too many parameters specified.\n", argv[0]);
		return -1;
	}

#ifdef __APPLE__
	size_t countSize = sizeof(cpuCount);

	sysctlbyname("hw.logicalcpu", &cpuCount, &countSize, NULL, 0);
#else
	cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif

	if(argc != 1)
	{
		if(!strncmp(argv[1], "-x1", 3))
		{
			cpuCount++;
		}
		else if(!strncmp(argv[1], "-x2", 3))
		{
			cpuCount += cpuCount;
		}
		else
		{
			printf("Usage: %s [OPTIONS]\n", argv[0]);
			printf("Running cpucount without options returns the number of\n");
			printf("processors in the system.\n");
			printf("-x1    - Number of processors in the system, plus 1.\n");
			printf("-x2    - Number of processors in the system, doubled.\n");
			return 0;
		}
	}

	printf("%u\n", cpuCount);
	return 0;
}
