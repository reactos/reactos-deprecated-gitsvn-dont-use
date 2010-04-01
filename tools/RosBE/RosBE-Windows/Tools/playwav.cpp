/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows.
 * LICENSE:     GNU General Public License v2. (see LICENSE.txt)
 * FILE:        Tools/playwav.c
 * PURPOSE:     WAVE Player
 * COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
 *
 */

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

int _tmain(int argc, TCHAR* argv[])
{
    PlaySound(argv[1],NULL,SND_FILENAME|SND_SYNC);
    return 0;
}
