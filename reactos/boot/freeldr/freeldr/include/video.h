/*
 *  FreeLoader
 *  Copyright (C) 1998-2003  Brian Palmer  <brianp@sginet.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __VIDEO_H
#define __VIDEO_H

#include <pshpack1.h>
typedef struct
{
  UCHAR Red;
  UCHAR Green;
  UCHAR Blue;
} PALETTE_ENTRY, *PPALETTE_ENTRY;
#include <poppack.h>

#ifdef __i386__
typedef struct _VESA_CHECK_SVGA_INFO {
    ULONG VesaMagic;
    USHORT Version;
    ULONG OEMName;
    ULONG Flags;
    ULONG ModePointer;
    USHORT VideoMemoryBlocks;
    // 1.*
    UCHAR Reserved[236];
    // 2.0
    USHORT OEMVersion;
    ULONG VendorNamePtr;
    ULONG ProductNamePtr;
    ULONG ProductRevisionPtr;
    USHORT VBEVersion;
    ULONG AccelModePtr;
    UCHAR VBEReserved[216];
    UCHAR OEMReserved[256];
} __attribute__((packed)) VESA_CHECK_SVGA_INFO, *PVESA_CHECK_SVGA_INFO;
#endif

extern	PVOID	VideoOffScreenBuffer;

USHORT		BiosIsVesaSupported(VOID);						// Implemented in i386vid.c, returns the VESA version

PVOID	VideoAllocateOffScreenBuffer(VOID);				// Returns a pointer to an off-screen buffer sufficient for the current video mode

VOID	VideoCopyOffScreenBufferToVRAM(VOID);

VOID	VideoSavePaletteState(PPALETTE_ENTRY Palette, ULONG ColorCount);
VOID	VideoRestorePaletteState(PPALETTE_ENTRY Palette, ULONG ColorCount);

VOID	VideoSetAllColorsToBlack(ULONG ColorCount);
VOID	VideoFadeIn(PPALETTE_ENTRY Palette, ULONG ColorCount);
VOID	VideoFadeOut(ULONG ColorCount);


#endif  // defined __VIDEO_H
