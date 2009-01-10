#ifndef _LIB_LUSER_LUSERINT_H
#define _LIB_LUSER_LUSERINT_H

#include <stdarg.h>

#ifndef EOF
#define EOF -1
#endif

typedef char *LFILE;

#define FILE LFILE

typedef unsigned short LWCHAR;

#define WCHAR LWCHAR

typedef unsigned long long LULONGLONG;

#define ULONGLONG LULONGLONG

typedef long long LLONGLONG;

#define LONGLONG LLONGLONG

typedef double double_t;

typedef struct _LUNICODE_STRING {
    unsigned short Length;
    unsigned short MaximumLength;
    WCHAR *Buffer;
} LUNICODE_STRING, *PLUNICODE_STRING;

#define UNICODE_STRING LUNICODE_STRING
#define PUNICODE_STRING PLUNICODE_STRING

typedef struct _LANSI_STRING {
    unsigned short Length;
    unsigned short MaximumLength;
    char *Buffer;
} LANSI_STRING, *PLANSI_STRING;

#define ANSI_STRING LANSI_STRING
#define PANSI_STRING PLANSI_STRING

int _vsprintf(char *str, const char *fmt, va_list ap);
int _vfprintf(FILE *f,   const char *fmt, va_list ap);

#define __vfprintf _x_vfprintf
#define _isnan _x_isnan
#define _isinf _x_isinf

#endif/*_LIB_LUSER_LUSERINT_H*/
