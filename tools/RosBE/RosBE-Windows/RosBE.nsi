!define PRODUCT_NAME "ReactOS Build Environment for Windows"
!define PRODUCT_VERSION "1.1"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\RosBE.cmd"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKCU"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

;;
;; Basic installer options
;;
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "RosBE-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES64\RosBE"
InstallDirRegKey HKCU "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

;;
;; Add version/product information metadata to the installation file.
;;
VIAddVersionKey /LANG=1033 "FileVersion" "1.1.0.0"
VIAddVersionKey /LANG=1033 "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=1033 "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=1033 "Comments" "This installer was written by Peter Ward and Daniel Reimer using Nullsoft Scriptable Install System (http://nsis.sourceforge.net/)"
VIAddVersionKey /LANG=1033 "CompanyName" "ReactOS Team"
VIAddVersionKey /LANG=1033 "LegalTrademarks" "Copyright © 2007 ReactOS Team"
VIAddVersionKey /LANG=1033 "LegalCopyright" "Copyright © 2007 ReactOS Team"
VIAddVersionKey /LANG=1033 "FileDescription" "${PRODUCT_NAME} Setup"
VIProductVersion "1.1.0.0"

CRCCheck force
SetCompressor /FINAL /SOLID lzma

!include "MUI2.nsh"
!include "InstallOptions.nsh"
!include "RosSourceDir.nsh"
!include "LogicLib.nsh"

;; MUI begin.

;;
;; Read our custom page ini, remove previous version.
;;
Function .onInit
    Call UninstallPrevious
    !insertmacro INSTALLOPTIONS_EXTRACT "RosSourceDir.ini"
FunctionEnd

;;
;; MUI Settings
;;
!define MUI_ABORTWARNING
!define MUI_ICON "Icons\rosbe.ico"
!define MUI_UNICON "Icons\uninstall.ico"
!define MUI_COMPONENTSPAGE_NODESC

!define MUI_WELCOMEPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_WELCOME
;!insertmacro MUI_PAGE_LICENSE "Root\License.txt"
!insertmacro MUI_PAGE_DIRECTORY

;;
;; ReactOS source directory page
;;
var REACTOS_SOURCE_DIRECTORY
!insertmacro CUSTOM_PAGE_ROSDIRECTORY

;;
;; Start menu page
;;
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "ReactOS Build Environment"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP

!insertmacro MUI_PAGE_COMPONENTS

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.pdf"
!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!insertmacro MUI_PAGE_FINISH

;;
;; Uninstaller pages
;;
!insertmacro MUI_UNPAGE_INSTFILES

;;
;;  Language and reserve files
;;
ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"
!insertmacro MUI_LANGUAGE "English"

;; MUI end.

Section -BaseFiles SEC01
    SetShellVarContext current
    SetOutPath "$INSTDIR"
    SetOverwrite try
    File /r Icons\mingw.ico
    File /r Icons\rosbe.ico
    File /r Root\README.pdf
    File /r Root\RosBE.mac
    File /r Root\changelog.txt
    File /r Root\LICENSE.txt
    File /r Root\MinGW.cmd
    File /r Root\Build.cmd
    File /r Root\Clean.cmd
    File /r Root\Help.cmd
    File /r Root\RosBE.cmd
    File /r Root\rosbe-gcc-env.cmd
    File /r Root\TimeDate.cmd
    SetOutPath "$INSTDIR\Tools"
    SetOverwrite try
    File /r Components\Tools\buildtime.exe
    File /r Components\Tools\chknewer.exe
    File /r Components\Tools\cpucount.exe
    File /r Components\Tools\flash.exe
    File /r Components\Tools\getdate.exe
    File /r Components\Tools\tee.exe
SectionEnd

Section -MinGWGCCNASM SEC02
    SetOutPath "$INSTDIR\4.1.3"
    SetOverwrite try
    File /r Components\4.1.3\*.*
SectionEnd

Section "RosBE Configurator (options)" SEC03
    SetShellVarContext current
    SetOutPath "$INSTDIR"
    SetOverwrite try
    File /r Root\options.cmd
    SetOutPath "$INSTDIR\Tools"
    SetOverwrite try
    File /r Components\Tools\options.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
        SetOutPath $INSTDIR
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Options.lnk" \
                       "$INSTDIR\Tools\options.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Subversion Tools (svn, ssvn)" SEC04
    SetShellVarContext current
    SetOutPath "$INSTDIR"
    SetOverwrite try
    File /r Root\sSVN.cmd
    SetOutPath "$INSTDIR\Tools"
    SetOverwrite try
    File /r Components\Tools\svn.exe
    File /r Components\Tools\intl3_svn.dll
    File /r Components\Tools\libapr.dll
    File /r Components\Tools\libeay32.dll
    File /r Components\Tools\ssleay32.dll
    File /r Components\Tools\libaprutil.dll
    File /r Components\Tools\libapriconv.dll
    File /r Components\Tools\libdb44.dll
    SetOutPath "$INSTDIR\Tools\licenses"
    SetOverwrite try
    File /r Components\Tools\licenses\*.*
SectionEnd

Section "Shortcut Tool (scut)" SEC05
    SetShellVarContext current
    SetOutPath "$INSTDIR"
    SetOverwrite try
    File /r Root\scut.cmd
    SetOutPath "$INSTDIR\Tools"
    SetOverwrite try
    File /r Components\Tools\scut.exe
SectionEnd

Section "ccache - Compiler Cache" SEC06
    SetShellVarContext current
    SetOutPath "$INSTDIR\4.1.3\bin"
    SetOverwrite try
    File /r Components\Tools\ccache.exe
    File /r Components\Tools\cygwin1.dll
SectionEnd

Section "GDB - The GNU Project Debugger" SEC07
    SetShellVarContext current
    SetOutPath "$INSTDIR\4.1.3\bin"
    SetOverwrite try
    File /r Components\Tools\gdb.exe
    File /r Components\Tools\gdbserver.exe
SectionEnd

Section "relAddr2Line Tool" SEC08
    SetShellVarContext current
    SetOutPath "$INSTDIR"
    SetOverwrite try
    File /r Root\reladdr2line.cmd
    SetOutPath "$INSTDIR\Tools"
    SetOverwrite try
    File /r Components\Tools\echoh.exe
    File /r Components\Tools\raddr2line.exe
    File /r Components\Tools\chkslash.exe
SectionEnd

Section "Other Tools (chdefdir, chdefgcc and config)" SEC09
    SetShellVarContext current
    SetOutPath "$INSTDIR"
    SetOverwrite try
    File /r Root\chdefdir.cmd
    File /r Root\chdefgcc.cmd
    File /r Root\Config.cmd
SectionEnd

Section -StartMenuShortcuts SEC10
    SetShellVarContext current

    ;;
    ;; Add our start menu shortcuts.
    ;;
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
        SetOutPath $REACTOS_SOURCE_DIRECTORY
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\ReactOS Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\RosBE.cmd"' "$INSTDIR\rosbe.ico"
        SetOutPath $PROFILE
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Standard MinGW Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\MinGW.cmd"' "$INSTDIR\mingw.ico"
        SetOutPath $INSTDIR
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall RosBE.lnk" \
                       "$INSTDIR\Uninstall-${PRODUCT_VERSION}.exe"
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Readme.lnk" \
                       "$INSTDIR\README.pdf"
    !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section /o "Desktop Shortcuts" SEC11
    SetShellVarContext current

    ;;
    ;; Add our desktop shortcuts.
    ;;
    SetOutPath $REACTOS_SOURCE_DIRECTORY
    CreateShortCut "$DESKTOP\ReactOS Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\RosBE.cmd"' "$INSTDIR\rosbe.ico"
    SetOutPath $PROFILE
    CreateShortCut "$DESKTOP\Standard MinGW Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\MinGW.cmd"' "$INSTDIR\mingw.ico"
SectionEnd

Section /o "Quick Launch Shortcuts" SEC12
    SetShellVarContext current

    ;;
    ;; Add our quick launch shortcuts.
    ;;
    SetOutPath $REACTOS_SOURCE_DIRECTORY
    CreateShortCut "$QUICKLAUNCH\ReactOS Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\RosBE.cmd"' "$INSTDIR\rosbe.ico"
    SetOutPath $PROFILE
    CreateShortCut "$QUICKLAUNCH\Standard MinGW Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\MinGW.cmd"' "$INSTDIR\mingw.ico"
SectionEnd

Section -Post SEC13
    WriteUninstaller "$INSTDIR\Uninstall-${PRODUCT_VERSION}.exe"
    WriteRegStr HKCU "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\RosBE.cmd"
    WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
    WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall-${PRODUCT_VERSION}.exe"
    WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
SectionEnd

Function un.onUninstSuccess
    HideWindow
    MessageBox MB_ICONINFORMATION|MB_OK \
               "ReactOS Build Environment was successfully removed from your computer."
FunctionEnd

Function un.onInit
    MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 \
               "Are you sure you want to completely remove ReactOS Build Environment and all of its components?" \
               IDYES +2
    Abort
FunctionEnd

Section Uninstall
    !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
    SetShellVarContext current

    ;;
    ;; Clean up installed files.
    ;;
    RMDir /r /REBOOTOK "$INSTDIR\4.1.3"
    RMDir /r /REBOOTOK "$INSTDIR\Tools"
    RMDir /r /REBOOTOK "$SMPROGRAMS\$ICONS_GROUP"
    IfFileExists "$PROFILE\RosBE\." 0 +2
        RMDir /r /REBOOTOK "$PROFILE\RosBE"
    IfFileExists "$APPDATA\RosBE\." 0 +2
        RMDir /r /REBOOTOK "$APPDATA\RosBE"
    Delete /REBOOTOK "$INSTDIR\Build.cmd"
    Delete /REBOOTOK "$INSTDIR\chdefdir.cmd"
    Delete /REBOOTOK "$INSTDIR\chdefgcc.cmd"
    Delete /REBOOTOK "$INSTDIR\Clean.cmd"
    Delete /REBOOTOK "$INSTDIR\Config.cmd"
    Delete /REBOOTOK "$INSTDIR\Help.cmd"
    Delete /REBOOTOK "$INSTDIR\options.cmd"
    Delete /REBOOTOK "$INSTDIR\reladdr2line.cmd"
    Delete /REBOOTOK "$INSTDIR\RosBE.cmd"
    Delete /REBOOTOK "$INSTDIR\rosbe-gcc-env.cmd"
    Delete /REBOOTOK "$INSTDIR\scut.cmd"
    Delete /REBOOTOK "$INSTDIR\sSVN.cmd"
    Delete /REBOOTOK "$INSTDIR\TimeDate.cmd"
    Delete /REBOOTOK "$INSTDIR\README.pdf"
    Delete /REBOOTOK "$INSTDIR\rosbe.ico"
    Delete /REBOOTOK "$INSTDIR\uninstall.ico"
    Delete /REBOOTOK "$INSTDIR\RosBE.mac"
    Delete /REBOOTOK "$INSTDIR\ChangeLog.txt"
    Delete /REBOOTOK "$INSTDIR\LICENSE.txt"
    Delete /REBOOTOK "$INSTDIR\MinGW.cmd"
    Delete /REBOOTOK "$INSTDIR\mingw.ico"
    Delete /REBOOTOK "$INSTDIR\srclist.txt"
    Delete /REBOOTOK "$INSTDIR\config.rbuild"
    Delete /REBOOTOK "$INSTDIR\Uninstall-${PRODUCT_VERSION}.exe"
    RMDir /r /REBOOTOK "$INSTDIR"
    IfFileExists "$DESKTOP\ReactOS Build Environment.lnk" 0 +2
        Delete /REBOOTOK "$DESKTOP\ReactOS Build Environment.lnk"
    IfFileExists "$QUICKLAUNCH\ReactOS Build Environment.lnk" 0 +2
        Delete /REBOOTOK "$QUICKLAUNCH\ReactOS Build Environment.lnk"
    IfFileExists "$DESKTOP\Standard MinGW Build Environment.lnk" 0 +2
        Delete /REBOOTOK "$DESKTOP\Standard MinGW Build Environment.lnk"
    IfFileExists "$QUICKLAUNCH\Standard MinGW Build Environment.lnk" 0 +2
        Delete /REBOOTOK "$QUICKLAUNCH\Standard MinGW Build Environment.lnk"

    ;;
    ;; Clean up the registry.
    ;;
    DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
    DeleteRegKey HKCU "${PRODUCT_DIR_REGKEY}"
    SetAutoClose true
SectionEnd

Function UninstallPrevious
    ReadRegStr $R0 HKCU \
               "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
               "UninstallString"
    ReadRegStr $R1 HKCU \
               "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
               "DisplayVersion"
    ${If} $R1 == "${PRODUCT_VERSION}"
        messageBox MB_OK|MB_ICONEXCLAMATION \
            "You already have the ${PRODUCT_NAME} v${PRODUCT_VERSION} installed. Please manually uninstall the ${PRODUCT_NAME} v${PRODUCT_VERSION} if you want to reinstall."
        Quit
    ${EndIf}
    ${If} $R0 == ""
        ReadRegStr $R0 HKLM \
                   "Software\Microsoft\Windows\CurrentVersion\Uninstall\ReactOS Build Environment" \
                   "UninstallString"
        ReadRegStr $R1 HKLM \
                   "Software\Microsoft\Windows\CurrentVersion\Uninstall\ReactOS Build Environment" \
                   "DisplayVersion"
        ${If} $R0 == ""
            Return
        ${EndIf}
    ${EndIf}
    MessageBox MB_YESNO|MB_ICONQUESTION  \
               "A previous version of the ${PRODUCT_NAME} was found. You must uninstall it before installing this version.$\n$\nDo you want to do that now?" \
               IDNO UninstallPrevious_no \
               IDYES UninstallPrevious_yes
    Abort
    UninstallPrevious_no:
        Quit
    UninstallPrevious_yes:
        Var /global PREVIOUSINSTDIR
        Push $R0
        Call GetParent
        Pop $PREVIOUSINSTDIR
        ExecWait '$R0 _?=$PREVIOUSINSTDIR'
FunctionEnd

Function GetParent
    Exch $R0
    Push $R1
    Push $R2
    Push $R3

    StrCpy $R1 0
    StrLen $R2 $R0

    loop:
        IntOp $R1 $R1 + 1
        IntCmp $R1 $R2 get 0 get
        StrCpy $R3 $R0 1 -$R1
        StrCmp $R3 "\" get
        Goto loop

    get:
        StrCpy $R0 $R0 -$R1

        Pop $R3
        Pop $R2
        Pop $R1
        Exch $R0
FunctionEnd
