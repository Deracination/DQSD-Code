; timesync.nsi
; I have "stolen" this nsi file from Brent Beardsley and modified it to my needs.  Thanks Brent!
;
; Installs Add-on for Dave's Quick Search Deskbar
;

!Define APPNAME       "TimeSync Add-on for Dave's Quick Search Deskbar"
!Define ADDON_SUBDIR  "$INSTDIR\addons\timesync"
!Define INSTALL_PKG   "..\dqsd_timesync.exe"

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
Section "TimeSync"
  IfFileExists $INSTDIR\search.htm Installed
    SetAutoClose true
    MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to find installation of Dave's Quick Search Deskbar.$\n$\nPlease install this add-on after installing the latest version of Dave's Quick Search Deskbar."
    Quit
  Installed:

  ; Confirm that the user really does want to install
  MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2 "This will install the ${APPNAME}.  This installation includes installing and registering a freeware SNTP ActiveX Control.  Would you like to continue?" IDYES userconfirmedinstall
  Quit
  userconfirmedinstall:

  StrCpy $R1 "${ADDON_SUBDIR}"
  StrCpy $R3 "${DQSD_UUID}"

  ; Set output path to the add-on installation directory.
  SetOutPath $R1

  StrCpy $R2 "$R1\uninstall.exe"
  WriteUninstaller $R2

  SetOverwrite on

  File "tsync.xml"
  File "tsync.js"
  File "readme.txt"


  SetOutPath $WINDIR\system32

  SetOverwrite ifnewer
  IfFileExists CTimeSynch.ocx Ureg Reg
  Ureg:
  Exec "regsvr32 /u /s CTimeSynch.ocx"

  Reg:
  File "CTimeSynch.ocx"
  Exec "regsvr32 /s CTimeSynch.ocx"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "DisplayName" "${APPNAME} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$R3" "UninstallString" '"$R2"'

  Exec "notepad $R1\readme.txt"

  DetailPrint ""
  DetailPrint "Author: John W. Bairen, Jr."
  DetailPrint "Usage: See the readme.txt and Functions section of the menu for usage details."
  DetailPrint ""
  DetailPrint "Enter ! in the search bar to load this add-on."
  DetailPrint ""

  MessageBox MB_OK "Don't forget to set your default time server by modifying localprefs.js like this: $\n$\nServerName=''time.nist.gov'';"

  Exec "notepad $INSTDIR\localprefs.js"

  DetailPrint ""
  DetailPrint "You may need to reboot this PC to be able to use the SNTP ActiveX Control."
  DetailPrint ""
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