; dictionaryapi.nsi
;
; Installs Add-on for Dave's Quick Search Deskbar
;

!Define APPNAME       "DictionaryPop Add-on for Dave's Quick Search Deskbar"
!Define ADDON_SUBDIR  "$INSTDIR\addons\dictionarypop"
!Define INSTALL_PKG   "..\dqsd_dictionarypop.exe"

!Define DQSD_UUID     "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"

; The name of the installer
Name "${APPNAME}"

; Silent install
DirShow show
CRCCheck on
SetDatablockOptimize on
SilentInstall normal
ShowInstDetails show

; The installation package to create
OutFile "${INSTALL_PKG}"

InstallDirRegKey HKCR "CLSID\${DQSD_UUID}" "InstallDir"

; The stuff to install
Section "DictionaryPop"
  IfFileExists $INSTDIR\search.htm Installed
    SetAutoClose true
    MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to find installation of Dave's Quick Search Deskbar.$\n$\nPlease install this add-on after installing the latest version of Dave's Quick Search Deskbar."
    Quit
  Installed:

  ; Confirm that the user really does want to install  
  MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2 "This will install the ${APPNAME}.  Would you like to continue?" IDYES userconfirmedinstall
  Quit
  userconfirmedinstall:

  StrCpy $R1 "${ADDON_SUBDIR}"
  StrCpy $R3 "${DQSD_UUID}"

  ; Set output path to the add-on installation directory.
  SetOutPath $R1

  StrCpy $R2 "$R1\uninstall.exe"
  WriteUninstaller $R2  

  SetOverwrite on

  File "dictionarypop.xml"
  File "webservice.htc"
  File "DictionaryService.wsdl"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "DisplayName" "${APPNAME} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "UninstallString" '"$R2"'
  
  DetailPrint ""
  DetailPrint "Author: Thad Kerosky"
  DetailPrint "This Add-on gives handy popup dictionary, synonyms, and stock information"
  DetailPrint "Usage: 'dictionarypop <word or phrase to check> [{/synonym | /news | /profile | /quote}]'"
  DetailPrint ""
  DetailPrint "Enter ! in the search bar to load this add-on."

SectionEnd

; Uninstall stuff

UninstallText "Click next to uninstall ${APPNAME}."
Section "Uninstall"
  SetDetailsView show
  
  Delete /REBOOTOK $INSTDIR\*.*
  RMDir /r $INSTDIR

  IfRebootFlag rebootmsg norebootmsg
    rebootmsg:
    MessageBox MB_OK "The ${APPNAME} has been uninstalled.$\n$\nYou should reboot your computer now to$\ncomplete the removal.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2002 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  norebootmsg:

SectionEnd
