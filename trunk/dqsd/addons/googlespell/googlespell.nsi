; googlespell.nsi
;
; Installs Add-on for Dave's Quick Search Deskbar
;

; The name of the installer
Name "GoogleSpell Add-on for Dave's Quick Search Deskbar"

; Silent install
DirShow show
CRCCheck on
SetDatablockOptimize on
SilentInstall normal
ShowInstDetails show

; The installation package to create
OutFile "..\googlespell.exe"

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKCR "CLSID\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}" "InstallDir"

; The stuff to install
Section "GoogleSpell"
  IfFileExists $INSTDIR\search.htm Installed
    SetAutoClose true
    MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to find installation of Dave's Quick Search Deskbar.$\n$\nPlease install this add-on after installing the latest version of Dave's Quick Search Deskbar."
    Quit

Installed:
  StrCpy $R1 "$INSTDIR\addons\googlespell"
  StrCpy $R3 "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"

  ; Set output path to the add-on installation directory.
  SetOutPath $R1

  StrCpy $R2 "$R1\uninstall.exe"
  WriteUninstaller $R2  

  SetOverwrite on

  File "googlespell.xml"
  File "webservice.htc"
  File "GoogleSearch.wsdl"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "DisplayName" "Dave's Quick Search Deskbar - GoogleSpell Add-on (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "UninstallString" '"$R2"'
  
  DetailPrint ""
  DetailPrint "Usage: 'googlespell <word or phrase to check>'"
  DetailPrint ""
  DetailPrint "Enter ! in the search bar to load this add-on."

SectionEnd

; Uninstall stuff

UninstallText "Click next to uninstall the GoogleSpell Add-on for Dave's Quick Search Deskbar."
Section "Uninstall"
  SetDetailsView show
  
  Delete /REBOOTOK $INSTDIR\*.*
  RMDir /r $INSTDIR

  IfRebootFlag rebootmsg norebootmsg
    rebootmsg:
    MessageBox MB_OK "The Add-on has been uninstalled.$\n$\nYou should reboot your computer now to$\ncomplete the removal.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2002 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  norebootmsg:

SectionEnd
