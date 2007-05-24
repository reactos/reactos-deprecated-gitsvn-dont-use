!define PRODUCT_NAME "ReactOS Test Environment"
!define PRODUCT_VERSION "0.1"
!define PRODUCT_PUBLISHER "ReactOS"
!define PRODUCT_WEB_SITE "http://www.reactos.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\RosTE"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define SOURCE_DIR "D:\ReactOS\trunk\tools\RosTE\files"
!define MODULE_NAME "RosTE"

;; Basic installer options
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "RosTE-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\RosTE"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

;; Add version/product information metadata to the installation file.
VIAddVersionKey /LANG=1033 "FileVersion" "0.0.0.1"
VIAddVersionKey /LANG=1033 "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=1033 "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=1033 "Comments" "This installer was written by Ged Murphy using Nullsoft Scriptable Install System (http://nsis.sourceforge.net/)"
VIAddVersionKey /LANG=1033 "CompanyName" "ReactOS Team"
VIAddVersionKey /LANG=1033 "LegalTrademarks" "Copyright © 2007 ReactOS Team"
VIAddVersionKey /LANG=1033 "LegalCopyright" "Copyright © 2007 Ged Murphy (gedmurphy@reactos.org)"
VIAddVersionKey /LANG=1033 "FileDescription" "${PRODUCT_NAME} Setup"
VIProductVersion "0.0.1.0"

CRCCheck force
SetCompressor /FINAL /SOLID lzma

; MUI 1.67 compatible
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "ros.ico"
!define MUI_UNICON "unros.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "D:\ReactOS\trunk\tools\RosTE\LICENSE.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\GUI\QemuGUI.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

ShowInstDetails show
ShowUnInstDetails show

!macro Check_DotNet
  ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\.NETFramework" "InstallRoot"
	
  StrCmp $R0 "" NotFound VerifyVersion
		
  VerifyVersion:
    IfFileExists "$R0\v2.0.50727\*.*" Found NotFound
	
  NotFound:
    MessageBox MB_OK|MB_ICONEXCLAMATION "This application requires the .NET 2.0 framework."
    Quit
		
  Found:
macroend


Section "InstallGui" SEC01
  !insertmacro Check_DotNet
  SetOutPath "$INSTDIR\GUI"
  SetOverwrite ifnewer
  File "${SOURCE_DIR}\GUI\QemuGUI.exe"
  File "${SOURCE_DIR}\GUI\config.xml"
  CreateDirectory "$SMPROGRAMS\RosTE"
  CreateShortCut "$SMPROGRAMS\RosTE\RosTE.lnk" "$INSTDIR\GUI\QemuGUI.exe"
  CreateShortCut "$DESKTOP\RosTE.lnk" "$INSTDIR\GUI\QemuGUI.exe"
  WriteRegStr HKCU "Software\RosTE" "DefaultConfig" "$INSTDIR\GUI\config.xml"
SectionEnd
  
Section "InstallQemu" SEC02
  SetOutPath "$INSTDIR\QEmu"
  SetOverwrite ifnewer
  File "${SOURCE_DIR}\qemu-0.9.0-windows\bios.bin"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\fmod.dll"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\libusb0.dll"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\linux_boot.bin"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\pxe-ne2k_pci.bin"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\pxe-pcnet.bin"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\pxe-rtl8139.bin"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu-doc.html"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu-img.exe"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu-system-x86_64.exe"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu-tech.html"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu-win.bat"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu-x86_64.bat"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\qemu.exe"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\README-en.txt"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\README-ja.txt"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\SDL.dll"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\vgabios-cirrus.bin"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\vgabios.bin"
  SetOutPath "$INSTDIR\QEmu\keymaps"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\ar"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\common"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\da"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\de"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\de-ch"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\en-gb"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\en-us"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\es"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\et"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\fi"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\fo"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\fr"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\fr-be"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\fr-ca"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\fr-ch"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\hr"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\hu"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\is"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\it"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\ja"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\lt"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\lv"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\mk"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\modifiers"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\nl"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\nl-be"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\no"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\pl"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\pt"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\pt-br"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\ru"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\sl"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\sv"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\th"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\keymaps\tr"
  SetOutPath "$INSTDIR\QEmu\License"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\License\LICENSE"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\License\README-SDL.txt"
  File "${SOURCE_DIR}\qemu-0.9.0-windows\License\README.TXT"
SectionEnd

Section "Installvdk" SEC03
  SetOutPath "$INSTDIR\VDK"
  SetOverwrite ifnewer
  File "${SOURCE_DIR}\vdk\COPYING.TXT"
  File "${SOURCE_DIR}\vdk\README.TXT"
  File "${SOURCE_DIR}\vdk\READMEJ.TXT"
  File "${SOURCE_DIR}\vdk\vdk.exe"
  File "${SOURCE_DIR}\vdk\vdk.sys"
SectionEnd

Section "InstallQemu" SEC04
  SetOutPath "$INSTDIR\Kqemu"
  SetOverwrite ifnewer
  File "${SOURCE_DIR}\Kqemu\LICENSE"
  File "${SOURCE_DIR}\Kqemu\COPYING"
  File "${SOURCE_DIR}\Kqemu\kqemu-doc.html"
  File "${SOURCE_DIR}\Kqemu\kqemu-tech.html"
  SetOutPath "$SYSDIR\drivers"
  File "${SOURCE_DIR}\Kqemu\kqemu.sys"
  SetOutPath "$SMPROGRAMS"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\RosTE\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\RosTE\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  ;; Start kqemu after install
  ;; Service control manager plugin is used. Inf file isn't used.
  nsSCM::Install /NOUNLOAD "kqemu" "KQEmu virtualisation module for QEmu" 1 2 "$SYSDIR\drivers\kqemu.sys" "" "" "" ""
  Pop $0
  DetailPrint "snSCM::Install : $0"
  nsSCM::Start /NOUNLOAD "kqemu"
  Pop $0
  DetailPrint "nsSCM::Start : $0"

  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\GUI\QemuGUI.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\GUI\QemuGUI.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$DESKTOP\RosTE.lnk"
  RMDir /r /REBOOTOK "$SMPROGRAMS\RosTE"
  RMDir /r /REBOOTOK "$INSTDIR\GUI"
  RMDir /r /REBOOTOK "$INSTDIR\QEmu"
  RMDir /r /REBOOTOK "$INSTDIR\VDK"
  RMDir /r /REBOOTOK "$INSTDIR\Kqemu"
  RMDir /r /REBOOTOK "$INSTDIR"

  ;; Stop kqemu
  nsSCM::Stop /NOUNLOAD "kqemu"
  Pop $0
  DetailPrint "nsSCM::Stop : $0"
  nsSCM::Remove /NOUNLOAD "kqemu"
  Pop $0
  DetailPrint "nsSCM::Remove : $0"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  ; delete user defined default config value
  DeleteRegKey HKCU "Software\RosTE"
  SetAutoClose false
SectionEnd
