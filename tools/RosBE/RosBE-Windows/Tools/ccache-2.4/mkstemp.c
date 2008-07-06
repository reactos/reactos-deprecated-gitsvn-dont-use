#ifdef _WIN32
/* Copyright (C) 1998, 1999, 2001, 2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* Ported for ccache-win32 by Colin Finck <colin@reactos.org>
   Source: http://sources.redhat.com/cgi-bin/cvsweb.cgi/libc/sysdeps/posix/tempname.c?rev=1.39&cvsroot=glibc
*/

#include "ccache.h"

/* These are the characters used in temporary filenames.  */
static const char letters[] =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

/* Generate a unique temporary file name from TEMPLATE.
   The last six characters of TEMPLATE must be "XXXXXX";
   they are replaced with a string that makes the filename unique.
   Then open the file and return a fd. */
int
mkstemp (char *template)
{
    char *XXXXXX;
    time_t value;
    unsigned int count;
    int fd;

    /* A lower bound on the number of temporary files to attempt to
     generate.  The maximum total number of temporary file names that
     can exist for a given template is 62**6.  It should never be
     necessary to try all these combinations.  Instead if a reasonable
     number of names is tried (we define reasonable as 62**3) fail to
     give the system administrator the chance to remove the problems.  */
    unsigned int attempts = (62 * 62 * 62);

    /* This is where the Xs start.  */
    XXXXXX = &template[ strlen(template) - 6 ];

    /* Get some more or less random data.  */
    value = time(NULL) ^ _getpid();

    for (count = 0; count < attempts; value += 7777, ++count)
    {
        time_t v = value;

        /* Fill in the random bits.  */
        XXXXXX[0] = letters[v % 62];
        v /= 62;
        XXXXXX[1] = letters[v % 62];
        v /= 62;
        XXXXXX[2] = letters[v % 62];
        v /= 62;
        XXXXXX[3] = letters[v % 62];
        v /= 62;
        XXXXXX[4] = letters[v % 62];
        v /= 62;
        XXXXXX[5] = letters[v % 62];

        fd = _open (template, O_RDWR | O_BINARY | O_CREAT | O_EXCL, 0600);

        if (fd >= 0)
            return fd;
        else if (errno != EEXIST)
            return -1;
    }

    /* We got out of the loop because we ran out of combinations to try.  */
    return -1;
}
#endif
