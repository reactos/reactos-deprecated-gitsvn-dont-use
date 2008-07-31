all: testline.exe

testline.exe: testline.o
	gcc -g -o testline.exe testline.o libdbghelp.a

testline.o: testline.c
	gcc -g -I. -c testline.c