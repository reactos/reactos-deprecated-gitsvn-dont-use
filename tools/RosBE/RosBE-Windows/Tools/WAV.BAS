'==============================================================================
'
' WAV.EXE - Command line .WAV file player
' Copyright (c) 1998 by PowerBASIC, Inc.  All Rights Reserved.
'
'==============================================================================

$INCLUDE "WIN32API.INC"

FUNCTION PbMain() AS LONG

  DIM wav AS ASCIIZ * 256

  wav = COMMAND$

  STDOUT "WAV.EXE v1.0 - Command line .WAV file player"
  STDOUT "Copyright (c) 1998 by PowerBASIC, Inc."
  STDOUT ""

  IF LEN(wav) = 0 THEN
    STDOUT "Usage:  WAV filename.wav

    SndPlaySound BYVAL %NULL, %SND_SYNC  'stop anything currently playing

    FUNCTION = 1
    EXIT FUNCTION
  END IF

  IF INSTR(wav, ".") = 0 THEN
    wav = wav + ".WAV"
  END IF

  IF LEN(DIR$(wav)) THEN
    STDOUT "Playing " & wav
    SndPlaySound wav, %SND_SYNC
  ELSE
    STDOUT "File not found: " & wav
    FUNCTION = 1
  END IF

END FUNCTION

