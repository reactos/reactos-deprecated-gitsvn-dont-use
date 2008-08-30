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

char* ReadFile (const char *filename);
void  ProcessDebugData(const char* tty, int timeout);  
ssize_t safewrite(int fd, const void *buf, size_t count);
