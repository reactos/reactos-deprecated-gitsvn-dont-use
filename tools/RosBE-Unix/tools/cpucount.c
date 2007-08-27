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

int
main(int argc,
     char *argv[])
{
	int cpuCount = 0;

#ifdef __APPLE__
	size_t countSize = sizeof(cpuCount);

	sysctlbyname("hw.logicalcpu", &cpuCount, &countSize, NULL, 0);
#else
	cpuCount = sysconf(_SC_NPROCESSORS_ONLN); 
#endif

	printf( "%u\n", cpuCount);

	return 0;
}
