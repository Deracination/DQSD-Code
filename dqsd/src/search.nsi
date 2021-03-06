; search.nsi
;
; Installs Dave's Quick Search Deskbar

!include "dqsd_clsid.nsh"

; The name of the installer
Name "Dave's Quick Search Deskbar"

; What is the minimum required version of IE?
!define IE_MAJOR_REQUIRED  5
!define IE_MINOR_REQUIRED  5
!define HOW_TO_TURN_ON_TOOLBAR "Right-click in your taskbar and select$\n$\n    Toolbars > Quick Search$\n$\nto add the Quick Search Deskbar to your taskbar."
!define TITLE_AND_COPYRIGHT "Dave's Quick Search Deskbar$\nCopyright � 2002-2011 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
!define DQSD_TITLE "Dave's Quick Search Deskbar"

; Silent install
;DirShow hide
CRCCheck on
SetDatablockOptimize on
Icon search.ico
SilentInstall silent
;ShowInstDetails show

; The file to write
OutFile "..\dqsd.exe"

;--------------------------------
; GetWindowsXPTheme
; returns "" for windows classic
; returns "blue" for xp blue
; returns "silver" for xp silver
; returns "olive" for xp olive
Function GetWindowsXPTheme
  Push $0
  Push $1

  ReadRegStr $1 HKCU "Software\Microsoft\Windows\CurrentVersion\ThemeManager" "ThemeActive"
  StrCmp $1 "1" 0 noXPTheme

  ;has XP Theme
  ReadRegStr $1 HKCU "Software\Microsoft\Windows\CurrentVersion\ThemeManager" "ColorName"
  ; blue check
  StrCmp $1 "NormalColor" 0 +3
  StrCpy $0 "blue"
  goto doneXPTheme
  ; olive check
  StrCmp $1 "HomeStead" 0 +3
  StrCpy $0 "olive"
  goto doneXPTheme
  ; silver check
  StrCmp $1 "Metallic" 0 +3
  StrCpy $0 "silver"
  Goto doneXPTheme
  ; unknown
  StrCpy $0 ""
  goto doneXPTheme

  noXPTheme:
    ; no theme active
    StrCpy $0 ""
    Goto doneXPTheme

  doneXPTheme:
    Pop $1
    Exch $0
FunctionEnd

; The default installation directory
InstallDir "$PROGRAMFILES\Quick Search Deskbar"

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKCR "CLSID\${DQSD_CLSID}" "InstallDir"

; The text to prompt the user to enter a directory
DirText "Choose a directory to install in to:"

; The stuff to install
Section "Quick Search Deskbar (required)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; If the currently installed version is less 4.0, then force them to uninstall before proceeding.
  IfFileExists "$INSTDIR\search.htm" +1 noUninstallNeeded ; If they don't have a search.htm, assume this is a clean installation
  IfFileExists "$INSTDIR\version.xml" +1 uninstallNeeded  ; If they don't have a version.xml, this is really old installation
  IfFileExists "$INSTDIR\DQSDTools.dll" +1 uninstallNeeded  ; If they don't have a DQSDTools.dll, this is really old installation
  GetDLLVersion "$INSTDIR\DQSDTools.dll" $R0 $R1
  IntOp $R2 $R0 >> 16
  IntOp $R2 $R2 & 0x0000FFFF ; $R2 now contains major version
  IntCmp $R2 4 noUninstallNeeded +1 noUninstallNeeded
  uninstallNeeded:
    MessageBox MB_OK|MB_ICONINFORMATION "Before installing this version of ${DQSD_TITLE}, you must uninstall the current installation."
    Abort
  noUninstallNeeded:

  ; Check if Administrators
  # call userInfo plugin to get user info.  The plugin puts the result in the stack
  userInfo::getAccountType
  pop $0
  strCmp $0 "Admin" +3
  MessageBox MB_OK|MB_ICONINFORMATION "You need to run this installer as an administrator of this machine."
    Abort

  ; Confirm that the user really does want to install  
  MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2 "This will install ${DQSD_TITLE}.$\n$\nWould you like to continue?" IDYES userconfirmedinstall
    Abort
  userconfirmedinstall:

  ; Verify that they've got the correct version of IE installed.
  GetDLLVersion "shdocvw.dll" $1 $4
  IntOp $2 $1 & 0xffff0000
  IntOp $2 $2 / 0x00010000
  IntOp $3 $1 & 0x0000ffff
  IntOp $5 $4 & 0xffff0000
  IntOp $5 $5 / 0x00010000
  IntOp $6 $4 & 0x0000ffff
  DetailPrint 'IE Version: $2.$3.$5.$6: OK'
  IntCmp ${IE_MAJOR_REQUIRED} $2 ieMajorVersionOK ieVersionOK ieVersionNotOK
  ieMajorVersionOK:
  IntCmp ${IE_MINOR_REQUIRED} $3 ieVersionOK ieVersionOK ieVersionNotOK

  ieVersionNotOK:
    MessageBox MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2 "This version of ${DQSD_TITLE} requires IE version ${IE_MAJOR_REQUIRED}.${IE_MINOR_REQUIRED} or higher.  You are currently running version $2.$3.$\n$\nWould you like to continue with the installation anyway?" IDYES ieVersionOK
    Abort
  ieVersionOK:

  ; Old versions to delete
  UnRegDLL $INSTDIR\dqsdt253.dll
  Delete /REBOOTOK $INSTDIR\dqsdt253.dll
  UnRegDLL $INSTDIR\dqsdt254.dll
  Delete /REBOOTOK $INSTDIR\dqsdt254.dll
  UnRegDLL $INSTDIR\dqsdt255.dll
  Delete /REBOOTOK $INSTDIR\dqsdt255.dll
  UnRegDLL $INSTDIR\dqsdt257.dll
  Delete /REBOOTOK $INSTDIR\dqsdt257.dll
  UnRegDLL $INSTDIR\dqsdt258.dll
  Delete /REBOOTOK $INSTDIR\dqsdt258.dll

  ; install new DLL
  SetOverwrite try
  ClearErrors
  File "..\DQSDTools.dll"
  IfErrors seeifsame register

  ; If was unable to upgrade, see if files are the same anyway
  seeifsame:
  ClearErrors
  GetDLLVersionLocal "..\DQSDTools.dll" $1 $2
  GetDLLVersion $INSTDIR\DQSDTools.dll $3 $4
  IfErrors isdifferent
  IntCmpU $1 $3 test1 isdifferent isdifferent
  test1:
  IntCmpU $2 $4 test2 isdifferent isdifferent
  test2:
  ClearErrors
  GetFileTimeLocal "..\DQSDTools.dll" $1 $2
  GetFileTime $INSTDIR\DQSDTools.dll $3 $4
  IfErrors isdifferent
  IntCmpU $1 $3 test3 isdifferent isdifferent
  test3:
  IntCmpU $2 $4 register isdifferent isdifferent

  SetOverwrite on

  ; If different, install as another name and rename to correct file on reboot
  isdifferent:
  File /oname=DQSDTools.dll.ins "..\DQSDTools.dll"
  Rename /REBOOTOK "$INSTDIR\DQSDTools.dll.ins" "$INSTDIR\DQSDTools.dll"

  ; We can register the dll and continue
  register:
  RegDLL $INSTDIR\DQSDTools.dll
  
  ; Determine if this is an upgrade
  StrCpy $8 "The Deskbar has been upgraded.  $\nIf the search bar is already present $\non your taskbar, right-click on $\nthe handle to the left of the search bar $\nand select 'Refresh' to reload it.  $\nIf the search bar is not present, $\n${HOW_TO_TURN_ON_TOOLBAR}$\n$\n${TITLE_AND_COPYRIGHT}"
  IfFileExists "$INSTDIR\search.htm" upgradebar
  StrCpy $8 "The Deskbar has been installed.$\n$\n${HOW_TO_TURN_ON_TOOLBAR}$\n$\n${TITLE_AND_COPYRIGHT}"

  upgradebar:
  WriteUninstaller "$INSTDIR\uninstall.exe"

  ; Put files there
  File "..\aliases.txt"
  File "..\categoryExpander.htc"
  File "..\ChangeLog.txt"
  File "..\readme.txt"
  File "..\dqsd.png"
  File "..\version.xml"
  File "..\versiondialog.htm"
  File "..\maximized.htm"
  File "..\search.htm"
  File "..\xptoolbar*.bmp"
  File "..\holidays.*.xml"

  File "..\calculate.js"
  File "..\calendar.js"
  File "..\clock.js"
  File "..\defer_tools.js"
  File "..\helpbox.js"
  File "..\helpmenu.js"
  File "..\history.js"
  File "..\loader.js"
  File "..\preferences.js"
  File "..\savesearchsettings.js"
  File "..\strings.js"
  File "..\tools.js"
  File "..\translate.js"
  File "..\version.js"

  File "..\search.css"
  File "..\theme.css"
  File "..\theme_blue.css"
  File "..\theme_classic.css"
  File "..\theme_energy_blue.css"
  File "..\theme_olive.css"
  File "..\theme_royale.css"
  File "..\theme_royale_noir.css"
  File "..\theme_royale_zune.css"
  File "..\theme_silver.css"

  
  SetOutPath "$INSTDIR\searches"
  File "..\searches\*.xml"
  
  SetOutPath "$INSTDIR\src"
  File "search.ico"
  File "button-dqsd.gif"
  File "button-dqsd2.gif"

  File "search.nsi"
  File "dqsd_clsid.nsh"

  File "build32.cmd"
  File "build64.cmd"
  File "build64.exclude.txt"
  File "build_all.cmd"
  File "publish_release.cmd"
  File "scrub.cmd"
  
  File "release-procedure.txt"

  File "zip.exe"
  File "pscp.exe"
  
  
  SetOutPath "$INSTDIR\src\DQSDTools"
  File "DQSDTools\*.cpp"
  File "DQSDTools\*.def"
  File "DQSDTools\*.idl"
  File "DQSDTools\*.rc"
  File "DQSDTools\*.h"
  File "DQSDTools\*.rgs"
  File "DQSDTools\*.sln"
  File "DQSDTools\*.vcproj"
  File "DQSDTools\License.txt"

  ; Create files with user preferences if they don't exist
  SetOverwrite off
  SetOutPath "$INSTDIR"
  File "..\localaliases.txt"
  File "..\localsearch.css"
  File "..\localprefs.js"
  
  ; Create subdirectory for user searches
  CreateDirectory "$INSTDIR\localsearches"
  SetOverwrite on

  ; Remove deprecated stuff
  Delete $INSTDIR\license.txt
  Delete $INSTDIR\search.ico
  Delete $INSTDIR\search.nsi
  Delete $INSTDIR\dqsd.xml
  Delete $INSTDIR\search.xml
  Delete $INSTDIR\xplocalsearch.css
  Delete $INSTDIR\xptoolbarbot.bmp
  Delete $INSTDIR\xptoolbartop.bmp
  Delete $INSTDIR\localsearch_default.css
  Delete $INSTDIR\localsearch_silver.css
  Delete $INSTDIR\localsearch_olive.css
  Delete $INSTDIR\localsearch_blue.css
  Delete $INSTDIR\xptoolbar2_default.bmp
  Delete $INSTDIR\xptoolbar1_default.bmp
  Delete $INSTDIR\src\dqsd.xml
  
  ; Remove stuff introduced by 3.1.8 beta-1
  Delete $INSTDIR\dqsd.gif
  Delete $INSTDIR\httpinst.js
  
  ; Remove DQSDHost.dll, it's been merged into DQSDTools
  Delete /REBOOTOK $INSTDIR\DQSDHost.dll

  ; Old versions of searches to delete; most renamed or consolidated
  Delete $INSTDIR\searches\aim.xml
  Delete $INSTDIR\searches\ask.xml
  Delete $INSTDIR\searches\at.xml
  Delete $INSTDIR\searches\base.xml
  Delete $INSTDIR\searches\bible.xml
  Delete $INSTDIR\searches\cf.xml
  Delete $INSTDIR\searches\chart.xml
  Delete $INSTDIR\searches\day.xml
  Delete $INSTDIR\searches\drink.xml
  Delete $INSTDIR\searches\fc.xml
  Delete $INSTDIR\searches\foldoc.xml
  Delete $INSTDIR\searches\gga.xml
  Delete $INSTDIR\searches\ggi.xml
  Delete $INSTDIR\searches\ggc.xml
  Delete $INSTDIR\searches\ggn.xml
  Delete $INSTDIR\searches\gguk.xml
  Delete $INSTDIR\searches\gwd.xml
  Delete $INSTDIR\searches\ifl.xml
  Delete $INSTDIR\searches\law.xml
  Delete $INSTDIR\searches\lottery.xml
  Delete $INSTDIR\searches\man.xml
  Delete $INSTDIR\searches\map.xml
  Delete $INSTDIR\searches\palm.xml
  Delete $INSTDIR\searches\phone.xml
  Delete $INSTDIR\searches\roman.xml
  Delete $INSTDIR\searches\sbb.xml
  Delete $INSTDIR\searches\sbc.xml
  Delete $INSTDIR\searches\sbp.xml
  Delete $INSTDIR\searches\short.xml
  Delete $INSTDIR\searches\slash.xml
  Delete $INSTDIR\searches\smlnk.xml
  Delete $INSTDIR\searches\start.xml
  Delete $INSTDIR\searches\tel.xml
  Delete $INSTDIR\searches\tvgc.xml
  Delete $INSTDIR\searches\webpoll.xml
  Delete $INSTDIR\searches\jsexec.xml
  Delete $INSTDIR\searches\timer.xml
  Delete $INSTDIR\searches\nwtsr.xml
  Delete $INSTDIR\searches\httpd.xml
  Delete $INSTDIR\searches\encyfr.xml
  Delete $INSTDIR\searches\pcm.xml
  Delete $INSTDIR\searches\wiq.xml
  Delete $INSTDIR\searches\sp.xml
  Delete $INSTDIR\searches\tvtm.xml
  Delete $INSTDIR\searches\gtwhois.xml

  ; Remove any searches that were possibly disabled.  They should be reinstalled anyway
  Delete $INSTDIR\searches\*.disabled*

  ; Write the installation path into the registry
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "InstallDir" "$INSTDIR"
  
  ; Registry settings needed to function
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "" "Quick Search"
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "HelpText" "${DQSD_TITLE}"

  ; DQSD COM objects check that the current URL is in the set of files listed below.
  ; Add new ones if necessary, though search.htm should be sufficient
  WriteRegDWORD HKCR "CLSID\${DQSD_CLSID}\SecureFiles" "$INSTDIR\search.htm" 0

  
  ; Add dqsd clsid to approved shell extensions - irrelevant on non-NT based OS - but doesn't hurt anything per
  ; http://msdn.microsoft.com/library/en-us/shellcc/platform/Shell/programmersguide/shell_int/shell_int_extending/extensionhandlers/shell_ext.asp
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved" "${DQSD_CLSID}" "${DQSD_TITLE}"
  
  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DQSD_CLSID}" "DisplayName" "${DQSD_TITLE} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DQSD_CLSID}" "UninstallString" '"$INSTDIR\uninstall.exe"'

  ; xp theme css installer
  Call GetWindowsXPTheme
  Pop $0
  StrCmp $0 "" defaultTheme
  StrCmp $0 "blue" 0 +3
  CopyFiles /SILENT /FILESONLY $INSTDIR\theme_blue.css $INSTDIR\theme.css
  goto doneXPTheme
  StrCmp $0 "olive" 0 +3
  CopyFiles /SILENT /FILESONLY $INSTDIR\theme_olive.css $INSTDIR\theme.css
  goto doneXPTheme
  StrCmp $0 "silver" 0 +3
  CopyFiles /SILENT /FILESONLY $INSTDIR\theme_silver.css $INSTDIR\theme.css
  goto doneXPTheme
  defaultTheme:
  ; none
  doneXPTheme:

  ; Message box
  IfRebootFlag rebootmsg norebootmsg
    rebootmsg:
      MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON1 "The Deskbar has been upgraded.$\nYou must reboot before it will work correctly.$\nAfter rebooting, if the search bar$\nis not visible, ${HOW_TO_TURN_ON_TOOLBAR}$\n$\nWould you like to reboot now (recommended)?" IDYES doreboot
      ; MessageBox MB_OK|MB_ICONINFORMATION "Please remember to reboot before using the search bar.$\n$\n${TITLE_AND_COPYRIGHT}"
      Goto endinstall
    
    norebootmsg:
      MessageBox MB_OK|MB_ICONINFORMATION "$8"
      Goto endinstall

    doreboot:
      Reboot

   endinstall:
SectionEnd

; uninstall stuff

UninstallText "Click Uninstall to remove ${DQSD_TITLE}."
UninstallIcon search.ico

; special uninstall section.
Section "Uninstall"

  ; Unregister DLLs
  UnRegDLL $INSTDIR\DQSDTools.dll

  ; remove approved shell extension regkey
  DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved" "${DQSD_CLSID}"

  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DQSD_CLSID}"
  DeleteRegKey HKCR "CLSID\${DQSD_CLSID}"

  ; remove files
  Delete /REBOOTOK $INSTDIR\DQSDTools.dll
  Delete /REBOOTOK $INSTDIR\ChangeLog.txt
  Delete /REBOOTOK $INSTDIR\aliases.deprecated
  Delete /REBOOTOK $INSTDIR\menu.txt
  Delete /REBOOTOK $INSTDIR\readme.txt
  Delete /REBOOTOK $INSTDIR\uninstall.exe

  Delete /REBOOTOK $INSTDIR\aliases.txt
  Delete /REBOOTOK $INSTDIR\categoryExpander.htc
  Delete /REBOOTOK $INSTDIR\ChangeLog.txt
  Delete /REBOOTOK $INSTDIR\readme.txt
  Delete /REBOOTOK $INSTDIR\dqsd.png
  Delete /REBOOTOK $INSTDIR\version.xml
  Delete /REBOOTOK $INSTDIR\versiondialog.htm
  Delete /REBOOTOK $INSTDIR\maximized.htm
  Delete /REBOOTOK $INSTDIR\search.htm
  Delete /REBOOTOK $INSTDIR\xptoolbar*.bmp
  Delete /REBOOTOK $INSTDIR\holidays.*.xml

  Delete /REBOOTOK $INSTDIR\calculate.js
  Delete /REBOOTOK $INSTDIR\calendar.js
  Delete /REBOOTOK $INSTDIR\clock.js
  Delete /REBOOTOK $INSTDIR\defer_tools.js
  Delete /REBOOTOK $INSTDIR\helpbox.js
  Delete /REBOOTOK $INSTDIR\helpmenu.js
  Delete /REBOOTOK $INSTDIR\history.js
  Delete /REBOOTOK $INSTDIR\loader.js
  Delete /REBOOTOK $INSTDIR\preferences.js
  Delete /REBOOTOK $INSTDIR\savesearchsettings.js
  Delete /REBOOTOK $INSTDIR\strings.js
  Delete /REBOOTOK $INSTDIR\tools.js
  Delete /REBOOTOK $INSTDIR\translate.js
  Delete /REBOOTOK $INSTDIR\version.js

  Delete /REBOOTOK $INSTDIR\search.css
  Delete /REBOOTOK $INSTDIR\theme.css
  Delete /REBOOTOK $INSTDIR\theme_blue.css
  Delete /REBOOTOK $INSTDIR\theme_classic.css
  Delete /REBOOTOK $INSTDIR\theme_energy_blue.css
  Delete /REBOOTOK $INSTDIR\theme_olive.css
  Delete /REBOOTOK $INSTDIR\theme_royale.css
  Delete /REBOOTOK $INSTDIR\theme_royale_noir.css
  Delete /REBOOTOK $INSTDIR\theme_royale_zune.css
  Delete /REBOOTOK $INSTDIR\theme_silver.css


; Ask to delete local files
  MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2 "Uninstall local settings too?" IDYES fulluninstall IDNO partialuninstall
  fulluninstall:
  RmDir /r $INSTDIR

  partialuninstall:
  RmDir /r $INSTDIR\src
  RmDir /r $INSTDIR\searches
  RmDir /r $INSTDIR\addons

  ; Remove localsearches if it's empty (i.e. no /r)
  RmDir $INSTDIR\localsearches

  ; Remove install directory if it's empty (hardly ever will be)
  RmDir /REBOOTOK $INSTDIR

SectionEnd
