; search.nsi
;
; Installs Dave's Quick Search Deskbar
;

; The name of the installer
Name "Dave's Quick Search Deskbar"

; What is the minimum required version of IE?
!Define IE_MAJOR_REQUIRED  5
!Define IE_MINOR_REQUIRED  5
!Define HOW_TO_TURN_ON_TOOLBAR "Right-click in your taskbar and select$\n$\n    Toolbar > Add Quick Search...$\n$\nto add the Quick Search Deskbar to your taskbar."
!Define TITLE_AND_COPYRIGHT "Dave's Quick Search Deskbar$\nCopyright © 2002-2004 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
!Define DQSD_CLSID "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"
!Define DQSD_TITLE "Dave's Quick Search Deskbar"

; Silent install
;DirShow hide
CRCCheck on
SetDatablockOptimize on
Icon search.ico
SilentInstall silent
;ShowInstDetails show

; The file to write
OutFile "..\dqsd.exe"

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

  ; Confirm that the user really does want to install  
  MessageBox MB_YESNO|MB_ICONINFORMATION|MB_DEFBUTTON2 "This will install ${DQSD_TITLE}.  Would you like to continue?" IDYES userconfirmedinstall
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
  File "..\ChangeLog.txt"
  File "..\aliases.txt"
  File "..\calculate.js"
  File "..\calendar.js"
  File "..\categoryExpander.htc"
  File "..\clock.js"
  File "..\defer_tools.js"
  File "..\dqsd.png"  
  File "..\helpbox.js"
  File "..\helpmenu.js"
  File "..\history.js"
  File "..\holidays.*.xml"
  File "..\loader.js"
  File "..\maximized.htm"
  File "..\preferences.js"
  File "..\readme.txt"
  File "..\savesearchsettings.js"
  File "..\search.css"
  File "..\search.htm"
  File "..\strings.js"
  File "..\tools.js"
  File "..\translate.js"
  File "..\version.js"
  File "..\version.xml"
  File "..\versiondialog.htm"
  File "..\xptoolbar2_default.bmp"
  File "..\xptoolbar1_default.bmp"
  File "..\xptoolbar1_silver.bmp"
  File "..\xptoolbar2_silver.bmp"
  File "..\xptoolbar1_olive.bmp"
  File "..\xptoolbar2_olive.bmp"
  File "..\xptoolbar1_blue.bmp"
  File "..\xptoolbar2_blue.bmp"
  File "..\localsearch_default.css"
  File "..\localsearch_silver.css"
  File "..\localsearch_olive.css"
  File "..\localsearch_blue.css"
  
  SetOutPath "$INSTDIR\searches"
  File "..\searches\*.xml"
  
  SetOutPath "$INSTDIR\src"
  File "search.ico"
  File "search.nsi"
  File "build.cmd"
  File "clean.cmd"
  File "scrub.cmd"
  File "dqsd.xml"
  
  SetOutPath "$INSTDIR\src\DQSDTools"
  File "DQSDTools\DQSDTools.cpp"
  File "DQSDTools\DQSDTools.def"
  File "DQSDTools\DQSDTools.dep"
  File "DQSDTools\DQSDTools.dsp"
  File "DQSDTools\DQSDTools.dsw"
  File "DQSDTools\DQSDTools.idl"
  File "DQSDTools\DQSDTools.mak"
  File "DQSDTools\DQSDTools.rc"
  File "DQSDTools\Launcher.cpp"
  File "DQSDTools\Launcher.h"
  File "DQSDTools\Launcher.rgs"
  File "DQSDTools\MenuBuilder.rgs"
  File "DQSDTools\MenuBuilder.h"
  File "DQSDTools\MenuBuilder.cpp"
  File "DQSDTools\KeyboardHook.h"
  File "DQSDTools\KeyboardHook.cpp"
  File "DQSDTools\License.txt"
  File "DQSDTools\StdAfx.cpp"
  File "DQSDTools\StdAfx.h"
  File "DQSDTools\Utilities.h"
  File "DQSDTools\Utilities.cpp"
  File "DQSDTools\resource.h"

  ; Create files with user preferences if they don't exist
  SetOverwrite off
  SetOutPath "$INSTDIR"
  File "..\localaliases.txt"
  File "..\mrumenu.txt"
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

  ; Remove stuff introduced by 3.1.8 beta-1
  Delete $INSTDIR\dqsd.gif
  Delete $INSTDIR\httpinst.js

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
  Delete $INSTDIR\searches\mb.xml
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
  Delete $INSTDIR\searches\start.xml
  Delete $INSTDIR\searches\tel.xml
  Delete $INSTDIR\searches\tvgc.xml
  Delete $INSTDIR\searches\webpoll.xml
  Delete $INSTDIR\searches\jsexec.xml
  Delete $INSTDIR\searches\timer.xml

  ; Write the installation path into the registry
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "InstallDir" "$INSTDIR"

  ; ??? Keep this for a while for backward compatibility
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "SecureFile" "$INSTDIR\search.htm"
  WriteRegDWORD HKCR "CLSID\${DQSD_CLSID}\SecureFiles" "$INSTDIR\search.htm" 0
  WriteRegDWORD HKCR "CLSID\${DQSD_CLSID}\SecureFiles" "$INSTDIR\settings.htm" 0

  ; Registry settings needed to function
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "" "Quick Search"
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "MenuText" "Add Quick Search..."
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}" "HelpText" "${DQSD_TITLE}"
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}\Implemented Categories\{00021492-0000-0000-C000-000000000046}" "" ""
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}\InprocServer32" "" "$SYSDIR\shdocvw.dll"
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}\InprocServer32" "ThreadingModel" "Apartment"
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}\Instance" "CLSID" "{7BA4C742-9E81-11CF-99D3-00AA004AE837}"
  WriteRegStr HKCR "CLSID\${DQSD_CLSID}\Instance\InitPropertyBag" "Url" "$INSTDIR\search.htm"

  ; Add dqsd clsid to approved shell extensions - irrelevant on non-NT based OS - but doesn't hurt anything per
  ; http://msdn.microsoft.com/library/en-us/shellcc/platform/Shell/programmersguide/shell_int/shell_int_extending/extensionhandlers/shell_ext.asp
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved" "${DQSD_CLSID}" "${DQSD_TITLE}"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DQSD_CLSID}" "DisplayName" "${DQSD_TITLE} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DQSD_CLSID}" "UninstallString" '"$INSTDIR\uninstall.exe"'

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

UninstallText "Click next to uninstall ${DQSD_TITLE}."
UninstallIcon search.ico

; special uninstall section.
Section "Uninstall"

  ; Unregister DQSDTools
  UnRegDLL $INSTDIR\DQSDTools.dll

  ; remove approved shell extension regkey
  DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved" "${DQSD_CLSID}"

  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${DQSD_CLSID}"
  DeleteRegKey HKCR "CLSID\${DQSD_CLSID}"

  ; remove files
  Delete /REBOOTOK $INSTDIR\DQSDTools.dll
  Delete /REBOOTOK $INSTDIR\ChangeLog.txt
  Delete /REBOOTOK $INSTDIR\aliases.txt
  Delete /REBOOTOK $INSTDIR\calculate.js
  Delete /REBOOTOK $INSTDIR\calendar.js
  Delete /REBOOTOK $INSTDIR\categoryExpander.htc
  Delete /REBOOTOK $INSTDIR\clock.js
  Delete /REBOOTOK $INSTDIR\defer_tools.js
  Delete /REBOOTOK $INSTDIR\dqsd.png
  Delete /REBOOTOK $INSTDIR\helpbox.js
  Delete /REBOOTOK $INSTDIR\helpmenu.js
  Delete /REBOOTOK $INSTDIR\history.js
  Delete /REBOOTOK $INSTDIR\holidays.*.xml
  Delete /REBOOTOK $INSTDIR\loader.js
  Delete /REBOOTOK $INSTDIR\maximized.htm
  Delete /REBOOTOK $INSTDIR\menu.txt
  Delete /REBOOTOK $INSTDIR\readme.txt
  Delete /REBOOTOK $INSTDIR\savesearchsettings.js
  Delete /REBOOTOK $INSTDIR\search.css
  Delete /REBOOTOK $INSTDIR\search.htm
  Delete /REBOOTOK $INSTDIR\search.xml
  Delete /REBOOTOK $INSTDIR\strings.js
  Delete /REBOOTOK $INSTDIR\tools.js
  Delete /REBOOTOK $INSTDIR\translate.js
  Delete /REBOOTOK $INSTDIR\version.js
  Delete /REBOOTOK $INSTDIR\version.xml
  Delete /REBOOTOK $INSTDIR\versiondialog.htm
  Delete /REBOOTOK $INSTDIR\xptoolbar2_default.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar1_default.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar1_silver.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar2_silver.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar1_olive.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar2_olive.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar1_blue.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbar2_blue.bmp
  Delete /REBOOTOK $INSTDIR\localsearch_default.css
  Delete /REBOOTOK $INSTDIR\localsearch_silver.css
  Delete /REBOOTOK $INSTDIR\localsearch_olive.css
  Delete /REBOOTOK $INSTDIR\localsearch_blue.css
  Delete /REBOOTOK $INSTDIR\preferences.js
  Delete /REBOOTOK $INSTDIR\uninstall.exe

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
