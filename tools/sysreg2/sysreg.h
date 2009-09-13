/*
 * PROJECT:     ReactOS System Regression Testing Utility
 * LICENSE:     GNU GPLv2 or any later version as published by the Free Software Foundation
 * PURPOSE:     Shared header
 * COPYRIGHT:   Copyright 2008-2009 Christoph von Wittich <christoph_vw@reactos.org>
 *              Copyright 2009 Colin Finck <colin@reactos.org>
 */

#include <dirent.h>
#include <fcntl.h>
#include <libvirt.h>
#include <poll.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>

#define EXIT_CHECKPOINT_REACHED     0
#define EXIT_CONTINUE               1
#define EXIT_DONT_CONTINUE          2
#define NUM_STAGES                  3

typedef struct _stage
{
    char BootDevice[8];
    char Checkpoint[80];
}
stage;

typedef struct _Settings
{
    int Timeout;
    char Filename[255];
    char Name[80];
    char HardDiskImage[255];
    int ImageSize;
    stage Stage[3];
    unsigned int MaxCacheHits;
    unsigned int MaxRetries;
}
Settings;

typedef struct _ModuleListEntry
{
    struct _ModuleListEntry* Next;
    char* Module;
    char* Path;
}
ModuleListEntry;

/* utils.c */
char* ReadFile (const char* filename);
ssize_t safewrite(int fd, const void *buf, size_t count);
void SysregPrintf(const char* format, ...);

/* options.c */
bool LoadSettings(const char* XmlConfig);

/* console.c */	
int ProcessDebugData(const char* tty, int timeout, int stage);  

/* raddr2line.c */
void InitializeModuleList();
void CleanModuleList();
bool ResolveAddressFromFile(char* Buffer, size_t BufferSize, const char* Data);

/* virt.c */
extern const char* OutputPath;
extern Settings AppSettings;
extern ModuleListEntry* ModuleList;
