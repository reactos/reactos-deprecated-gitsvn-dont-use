/* Program for printing the CPU count.
   The output value will be used for the -j option of gcc.

   Developed by Colin Finck <mail@colinfinck.de>
   Released under GNU GPL v2 or any later version.
*/

#include <stdio.h>
#include <unistd.h>

int
main(int argc,
     char *argv[])
{
	printf( "%u\n", 
           sysconf(_SC_NPROCESSORS_ONLN) );

	return 0;
}

