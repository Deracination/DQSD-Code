; genealogy.nsi
;
; Installs Add-on for Dave's Quick Search Deskbar
;

!Define APPNAME       "Genealogy Add-on for Dave's Quick Search Deskbar"
!Define ADDON_SUBDIR  "$INSTDIR\addons\genealogy"
!Define INSTALL_PKG   "..\dqsd_genealogy.exe"

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
Section "Genealogy"
  IfFileExists $INSTDIR\search.htm Installed
    SetAutoClose true
    MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to find installation of Dave's Quick Search Deskbar.$\n$\nPlease install this add-on after installing the latest version of Dave's Quick Search Deskbar."
    Quit

Installed:
  StrCpy $R1 "${ADDON_SUBDIR}"
  StrCpy $R3 "${DQSD_UUID}"

  ; Set output path to the add-on installation directory.
  SetOutPath $R1

  StrCpy $R2 "$R1\uninstall.exe"
  WriteUninstaller $R2  

  SetOverwrite on

  File "accgen.xml"
  File "ancestry.xml"
  File "ancmb.xml"
  File "blm.xml"
  File "bwo.xml"
  File "cadi.xml"
  File "cemetery.xml"
  File "census.xml"
  File "cousin.xml"
  File "cyndi.xml"
  File "everton.xml"
  File "genforum.xml"
  File "genlookup.xml"
  File "gentoday.xml"
  File "ghl.xml"
  File "gnis.xml"
  File "gwar.xml"
  File "gwcp.xml"
  File "iddi.xml"
  File "ildi.xml"
  File "illp.xml"
  File "ilmi.xml"
  File "interment.xml"
  File "kydi.xml"
  File "lds.xml"
  File "medi.xml"
  File "melvyl.xml"
  File "mi1870.xml"
  File "midi.xml"
  File "mimi.xml"
  File "raogk.xml"
  File "rsl.xml"
  File "rwres.xml"
  File "sampub.xml"
  File "sndx.xml"
  File "ssdi.xml"
  File "surhelp.xml"
  File "town.xml"
  File "txdi.xml"
  File "usgw.xml"
  File "usgx.xml"
  File "vital.xml"
  File "vitalrec.xml"
  File "wcon.xml"
  File "wsmarr.xml"
  File "genealogy_lib.js"
  File "genealogy.nsi"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "DisplayName" "${APPNAME} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "UninstallString" '"$R2"'
  
  DetailPrint ""
  DetailPrint "Usage: 'See the Genealogy section of the menu for usage details'"
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
