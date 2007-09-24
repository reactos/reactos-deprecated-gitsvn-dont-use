!define PRODUCT_NAME "ReactOS Build Environment for Windows"
!define PRODUCT_VERSION "0.3.8.1"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\RosBE.cmd"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKCU"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

;;
;; Basic installer options
;;
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "RosBE-${PRODUCT_VERSION}.exe"
InstallDir "C:\RosBE"
InstallDirRegKey HKCU "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

;;
;; Add version/product information metadata to the installation file.
;;
VIAddVersionKey /LANG=1033 "FileVersion" "0.3.8.1"
VIAddVersionKey /LANG=1033 "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=1033 "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=1033 "Comments" "This installer was written by Peter Ward and Daniel Reimer using Nullsoft Scriptable Install System (http://nsis.sourceforge.net/)"
VIAddVersionKey /LANG=1033 "CompanyName" "ReactOS Team"
VIAddVersionKey /LANG=1033 "LegalTrademarks" "Copyright © 2007 ReactOS Team"
VIAddVersionKey /LANG=1033 "LegalCopyright" "Copyright © 2007 ReactOS Team"
VIAddVersionKey /LANG=1033 "FileDescription" "${PRODUCT_NAME} Setup"
VIProductVersion "0.3.8.1"

CRCCheck force
SetCompressor /FINAL /SOLID lzma

!include "MUI.nsh"
!include "RosSourceDir.nsh"
!include "LogicLib.nsh"

;; MUI begin.

;;
;; Read our custom page ini, remove previous version and check if the user has administrative privileges.
;;
Function .onInit
    Call CheckAdminOrCurrent
    Call UninstallPrevious
    !insertmacro MUI_INSTALLOPTIONS_EXTRACT "RosSourceDir.ini"
FunctionEnd

;;
;; MUI Settings
;;
!define MUI_ABORTWARNING
!define MUI_ICON "Icons\reactos.ico"
!define MUI_UNICON "Icons\reactos.ico"
!define MUI_COMPONENTSPAGE_NODESC

!define MUI_WELCOMEPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_WELCOME
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
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
!insertmacro MUI_LANGUAGE "English"

;; MUI end.

;;
;; Add sections 1, 2 and 3 which contain the file lists for GCC4 and Miscellaneous.
;;
!include "RosBE.Files-4.1.3.nsh"
!include "RosBE.Files-Miscellaneous.nsh"

Section -StartMenuShortcuts SEC03
    SetShellVarContext current

    ;;
    ;; Add our start menu shortcuts.
    ;;
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
        SetOutPath $REACTOS_SOURCE_DIRECTORY
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\ReactOS Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\RosBE.cmd"' "$INSTDIR\reactos.ico"
        SetOutPath $INSTDIR
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall RosBE.lnk" \
                       "$INSTDIR\Uninstall-${PRODUCT_VERSION}.exe"
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Readme.lnk" \
                       "$INSTDIR\readme.pdf"
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Options.lnk" \
                       "$INSTDIR\options.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section /o "Desktop Shortcuts" SEC04
    SetShellVarContext current

    ;;
    ;; Add our desktop shortcuts.
    ;;
    SetOutPath $REACTOS_SOURCE_DIRECTORY
    CreateShortCut "$DESKTOP\ReactOS Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\RosBE.cmd"' "$INSTDIR\reactos.ico"
SectionEnd

Section /o "Quick Launch Shortcuts" SEC05
    SetShellVarContext current

    ;;
    ;; Add our quick launch shortcuts.
    ;;
    SetOutPath $REACTOS_SOURCE_DIRECTORY
    CreateShortCut "$QUICKLAUNCH\ReactOS Build Environment.lnk" "$SYSDIR\cmd.exe" '/k "$INSTDIR\RosBE.cmd"' "$INSTDIR\reactos.ico"
SectionEnd

Section -Post SEC07
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
    Call un.CheckAdminOrCurrent
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
    Delete /REBOOTOK "$INSTDIR\Build.cmd"
    Delete /REBOOTOK "$INSTDIR\Build-Multi.cmd"
    Delete /REBOOTOK "$INSTDIR\Clean.cmd"
    Delete /REBOOTOK "$INSTDIR\Help.cmd"
    Delete /REBOOTOK "$INSTDIR\RosBE.cmd"
    Delete /REBOOTOK "$INSTDIR\reladdr2line.cmd"
    Delete /REBOOTOK "$INSTDIR\Config.cmd"
    Delete /REBOOTOK "$INSTDIR\sSVN.cmd"
    Delete /REBOOTOK "$INSTDIR\Build-Shared.cmd"
    Delete /REBOOTOK "$INSTDIR\RAddr.cmd"
    Delete /REBOOTOK "$INSTDIR\options.exe"
    Delete /REBOOTOK "$INSTDIR\TimeDate.cmd"
    Delete /REBOOTOK "$INSTDIR\ChangeLog.txt"
    Delete /REBOOTOK "$INSTDIR\LICENSE.txt"
    Delete /REBOOTOK "$INSTDIR\scut.cmd"
    Delete /REBOOTOK "$INSTDIR\srclist.xml"
    Delete /REBOOTOK "$INSTDIR\README.txt"
    Delete /REBOOTOK "$INSTDIR\readme.pdf"
    Delete /REBOOTOK "$INSTDIR\reactos.ico"
    Delete /REBOOTOK "$INSTDIR\RosBE.mac"
    Delete /REBOOTOK "$INSTDIR\config.rbuild"
    Delete /REBOOTOK "$INSTDIR\Uninstall-${PRODUCT_VERSION}.exe"
    IfFileExists "$DESKTOP\ReactOS Build Environment.lnk" 0 +2
        Delete /REBOOTOK "$DESKTOP\ReactOS Build Environment.lnk"
    IfFileExists "$QUICKLAUNCH\ReactOS Build Environment.lnk" 0 +2
        Delete /REBOOTOK "$QUICKLAUNCH\ReactOS Build Environment.lnk"

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
        Return
    ${EndIf}
    MessageBox MB_YESNO|MB_ICONQUESTION  \
               "A previous version of the ${PRODUCT_NAME} was found. You must uninstall it before installing this version.$\n$\nDo you want to do that now?" \
               IDNO UninstallPrevious_no \
               IDYES UninstallPrevious_yes
    Abort
    UninstallPrevious_no:
        Quit
    UninstallPrevious_yes:
        ExecWait '$R0 _?=$INSTDIR'
FunctionEnd

Function CheckAdminOrCurrent
    userInfo::getAccountType
    pop $R0
    ${If} $R0 == "Admin"
        Return
    ${else}
        messageBox MB_OK|MB_ICONEXCLAMATION \
            "You do not have administrative privileges. You need to set a folder with writing rights to install your BE and Code to. NO Vista Support possible!"
        Return
    ${EndIf}
FunctionEnd

Function un.CheckAdminOrCurrent
    userInfo::getAccountType
    pop $R0
    ${If} $R0 == "Admin"
        Return
    ${else}
        Return
    ${EndIf}
FunctionEnd
