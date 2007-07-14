!ifndef _WriteEnvStr_nsh
!define _WriteEnvStr_nsh

!include "WinMessages.nsh"
!include "LogicLib.nsh"

!ifndef WriteEnvStr_RegKey
    !define WriteEnvStr_RegKey 'HKCU "Environment"'
!endif

#
# WriteEnvStr - Writes an environment variable
# Note: Win9x systems requires reboot
#
# Example:
#  Push "HOMEDIR"           # name
#  Push "C:\New Home Dir\"  # value
#  Call WriteEnvStr
#
Function WriteEnvStr
    Exch $1 ; $1 has environment variable value
    Exch
    Exch $0 ; $0 has environment variable name
    Push $2

    Call IsNT
    Pop $2
    ${if} $2 == "1"
        WriteRegExpandStr ${WriteEnvStr_RegKey} $0 $1
        SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} \
                    0 "STR:Environment" /TIMEOUT=5000
        Pop $2
        Pop $0
        Pop $1
        Return
    ${endif}
        ; Not on NT
        StrCpy $2 $WINDIR 2 ; Copy drive of windows (c:)
        FileOpen $2 "$2\autoexec.bat" a
        FileSeek $2 0 END
        FileWrite $2 "$\r$\nSET $0=$1$\r$\n"
        FileClose $2
        SetRebootFlag true
        Pop $2
        Pop $0
        Pop $1
FunctionEnd

#
# un.DeleteEnvStr - Removes an environment variable
# Note: Win9x systems requires reboot
#
# Example:
#  Push "HOMEDIR"           # name
#  Call un.DeleteEnvStr
#
Function un.DeleteEnvStr
    Exch $0 ; $0 now has the name of the variable
    Push $1
    Push $2
    Push $3
    Push $4
    Push $5

    Call un.IsNT
    Pop $1
    ${if} $1 == "1"
        DeleteRegValue ${WriteEnvStr_RegKey} $0
        SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} \
                    0 "STR:Environment" /TIMEOUT=5000
        Pop $5
        Pop $4
        Pop $3
        Pop $2
        Pop $1
        Pop $0
        Return
    ${endif}
        ; Not on NT
        StrCpy $1 $WINDIR 2
        FileOpen $1 "$1\autoexec.bat" r
        GetTempFileName $4
        FileOpen $2 $4 w
        StrCpy $0 "SET $0="
        SetRebootFlag true

    DeleteEnvStr_dosLoop:
        FileRead $1 $3
        StrLen $5 $0
        StrCpy $5 $3 $5
        ${if} $5 == "$0"
            goto DeleteEnvStr_dosLoop
        ${endif}
        ${if} $5 == ""
            goto DeleteEnvStr_dosLoopEnd
        ${endif}
        FileWrite $2 $3
        Goto DeleteEnvStr_dosLoop

    DeleteEnvStr_dosLoopEnd:
        FileClose $2
        FileClose $1
        StrCpy $1 $WINDIR 2
        Delete "$1\autoexec.bat"
        CopyFiles /SILENT $4 "$1\autoexec.bat"
        Delete $4
        Pop $5
        Pop $4
        Pop $3
        Pop $2
        Pop $1
        Pop $0
FunctionEnd

!ifndef IsNT_KiCHiK
!define IsNT_KiCHiK

#
# [un.]IsNT - Pushes 1 if running on NT, 0 if not
#
# Example:
#   Call IsNT
#   Pop $0
#   StrCmp $0 1 +3
#     MessageBox MB_OK "Not running on NT!"
#     Goto +2
#     MessageBox MB_OK "Running on NT!"
#
!macro IsNT UN
Function ${UN}IsNT
    Push $0
    ReadRegStr $0 HKLM \
      "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
    StrCmp $0 "" 0 IsNT_yes
    ; we are not NT.
    Pop $0
    Push 0
    Return

    IsNT_yes:
        ; NT!!!
        Pop $0
        Push 1
FunctionEnd
!macroend
!insertmacro IsNT ""
!insertmacro IsNT "un."

!endif ; IsNT_KiCHiK

!endif ; _WriteEnvStr_nsh
