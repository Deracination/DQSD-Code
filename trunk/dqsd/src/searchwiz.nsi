; searchwiz.nsi
;
; Installs Dave's Quick Search Deskbar Search Wizard
;

!include "dqsd_clsid.nsh"

!define APPNAME       "Dave's Quick Search Deskbar Search Wizard"
!define ADDON_SUBDIR  "$INSTDIR\Search Wizard"
!define INSTALL_PKG   "..\dqsdwiz.exe"

; The name of the installer
Name "${APPNAME}"

; Silent install
; DirShow show ; DirShow no longer works in NSIS 2.0
CRCCheck on
SetDatablockOptimize on
Icon search.ico
SilentInstall normal
ShowInstDetails show

; The installation package to create
OutFile "${INSTALL_PKG}"

InstallDirRegKey HKCR "CLSID\${DQSD_CLSID}" "InstallDir"

; The stuff to install
Section "DQSDSearchWizard"
  ; Confirm that the user really does want to install  
  MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2 "This will install Dave's Quick Search Deskbar Search Wizard.  Would you like to continue?" IDYES userconfirmedinstall
  Quit
  userconfirmedinstall:

  MessageBox MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2 "Please close all Internet Explorer windows before proceeding.  If any IE windows are open during the install, you will probably have to reboot.  Press OK to continue after you have closed all IE windows." IDOK userconfirmedcloseie
  Quit
  userconfirmedcloseie:

  IfFileExists $INSTDIR\search.htm Installed
    SetAutoClose true
    MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to find installation of Dave's Quick Search Deskbar.$\n$\nPlease install the latest version of Dave's Quick Search Deskbar."
    Quit

Installed:
  StrCpy $R1 "${ADDON_SUBDIR}"
  StrCpy $R3 "${DQSD_CLSID}"

  ; Set output path to the add-on installation directory.
  SetOutPath $R1

  StrCpy $R2 "$R1\uninstall.exe"
  WriteUninstaller $R2  

  SetOverwrite on

  File /oname=DQSDSearchWiz.dll.ins "..\DQSDSearchWiz.dll"
  Rename /REBOOTOK "${ADDON_SUBDIR}\DQSDSearchWiz.dll.ins" "${ADDON_SUBDIR}\DQSDSearchWiz.dll"
  RegDLL "${ADDON_SUBDIR}\DQSDSearchWiz.dll"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "DisplayName" "${APPNAME} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "UninstallString" '"$R2"'
  
  IfRebootFlag rebootmsg norebootmsg
    rebootmsg:
    MessageBox MB_OK "${APPNAME} has been installed.$\n$\nYou should reboot your computer now to$\ncomplete the installation.$\n$\nDave's Quick Search Deskbar Search Wizard$\nCopyright (c) 2002 Glenn Carr$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  norebootmsg:

SectionEnd

; Uninstall stuff

UninstallText "Click next to uninstall ${APPNAME}."
UninstallIcon search.ico

Section "Uninstall"
  SetDetailsView show
  
  UnRegDLL $INSTDIR\DQSDSearchWiz.dll
  Delete /REBOOTOK $INSTDIR\*.*
  RMDir /r $INSTDIR

  IfRebootFlag rebootmsg norebootmsg
    rebootmsg:
    MessageBox MB_OK "${APPNAME} has been uninstalled.$\n$\nYou should reboot your computer now to$\ncomplete the removal.$\n$\nDave's Quick Search Deskbar Search Wizard$\nCopyright (c) 2002 Glenn Carr$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  norebootmsg:

SectionEnd
