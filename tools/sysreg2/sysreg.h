#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

typedef struct {
	char BootDevice[8];
} stage;

typedef struct {
	int Timeout;
	char Filename[255];
	char Name[80];
	char HardDiskImage[255];
	int ImageSize;
	stage Stage[3];
} Settings;

Settings AppSettings;

/* utils.c */
char* ReadFile (const char* filename);
ssize_t safewrite(int fd, const void *buf, size_t count);

/* options.c */
bool LoadSettings(const char* XmlConfig);

/* console.c */	
void  ProcessDebugData(const char* tty, int timeout, int stage);  
