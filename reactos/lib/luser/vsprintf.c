/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <luserint.h>
/*#include <precomp.h>*/
/*#include <tchar.h>*/

/*
 * @implemented
 */
int
_vsprintf(char *str, const char *fmt, va_list ap)
{
    int len = _vfprintf(&str, fmt, ap);
    str[len] = 0;
    return len;
}
