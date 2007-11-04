/*
 * PROJECT:     RosBE Options Dialog
 * LICENSE:     GPL - See LICENSE.txt in the top level directory.
 * FILE:        Tools/config/options.h
 * PURPOSE:     COnfiguring RosBE
 * COPYRIGHT:   Copyright 2007 Maarten Bosma
 *              Copyright 2007 Pierre Schweitzer
 *
 */

#include <windows.h>
#include <stdio.h>
#include <shlobj.h>
#include <wchar.h>
#include "resources.h"

#define MINGWVERSION L"\\4.1.3"

typedef LPITEMIDLIST (CALLBACK* ILCREATEFROMPATHW)(LPCWSTR path);

typedef struct _SETTINGS
{
    WCHAR logdir[MAX_PATH];
    WCHAR objdir[MAX_PATH];
    WCHAR outdir[MAX_PATH];
    WCHAR mingwpath[MAX_PATH];
    INT foreground;
    INT background;
    BOOL showtime;
    BOOL useccache;
    BOOL strip;
    BOOL writelog;
    BOOL objstate;
    BOOL outstate;
}
SETTINGS, *PSETTINGS;
