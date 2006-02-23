// switch_default.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;

	switch(a) {
	default:
		printf("Foo\n");
		break;
	}

	printf("Bar\n");

	return 0;
}

