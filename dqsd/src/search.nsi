; search.nsi
;
; Installs Dave's Quick Search Deskbar
;

; The name of the installer
Name "Dave's Quick Search Deskbar"

; Silent install
DirShow hide
CRCCheck on
SetDatablockOptimize on
Icon search.ico
SilentInstall silent

; The file to write
OutFile "..\dqsd.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\Quick Search Deskbar"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKCR "CLSID\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}" "InstallDir"

; The text to prompt the user to enter a directory
DirText "Choose a directory to install in to:"

; The stuff to install
Section "Quick Search Deskbar (required)"

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Old versions to delete
  UnRegDLL $INSTDIR\DQSDTools.dll
  Delete /REBOOTOK $INSTDIR\DQSDTools.dll
  UnRegDLL $INSTDIR\dqsdt253.dll
  Delete /REBOOTOK $INSTDIR\dqsdt253.dll
  UnRegDLL $INSTDIR\dqsdt254.dll
  Delete /REBOOTOK $INSTDIR\dqsdt254.dll

  ; install new DLL
  SetOverwrite try
  ClearErrors
  File "..\dqsdt255.dll"
  IfErrors seeifsame register

  ; If was unable to upgrade, see if files are the same anyway
  seeifsame:
  ClearErrors
  GetDLLVersionLocal "..\dqsdt255.dll" $1 $2
  GetDLLVersion $INSTDIR\dqsdt255.dll $3 $4
  IfErrors isdifferent
  IntCmpU $1 $3 test1 isdifferent isdifferent
  test1:
  IntCmpU $2 $4 test2 isdifferent isdifferent
  test2:
  ClearErrors
  GetFileTimeLocal "..\dqsdt255.dll" $1 $2
  GetFileTime $INSTDIR\dqsdt255.dll $3 $4
  IfErrors isdifferent
  IntCmpU $1 $3 test3 isdifferent isdifferent
  test3:
  IntCmpU $2 $4 register isdifferent isdifferent

  ; If different, stop the install and tell the user
  isdifferent:
  MessageBox MB_OK "Did not finish installing the deskbar because DQSDTools.dll is in use.$\n$\nTo finish installation, please logout and login again (or just reboot)$\nand then run this installer again."
  Abort

  ; We can register the dll and continue
  register:
  RegDLL $INSTDIR\dqsdt255.dll

  SetOverwrite on

  ; Determine if this is an upgrade
  StrCpy $8 "The Deskbar has been upgraded.$\n$\nRight-click in your taskbar and select$\n$\n    Toolbar > Add Quick Search...$\n$\nto add the Quick Search Deskbar to your taskbar.$\n$\nIf the search bar is already present on your$\ntaskbar, right-click on the handle to the left of$\nthe search bar and select 'Refresh' to reload it.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2002 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  IfFileExists "$INSTDIR\search.htm" upgradebar
  StrCpy $8 "The Deskbar has been installed.$\n$\nRight-click in your taskbar and select$\n$\n    Toolbar > Add Quick Search...$\n$\nto add the Quick Search Deskbar to your taskbar.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2001 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  upgradebar:
  IfRebootFlag rebootmsg norebootmsg
  rebootmsg:
  StrCpy $8 "The Deskbar has been upgraded.$\n$\nYou should reboot your computer now to$\ncomplete the installation.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2002 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  norebootmsg:

  

  WriteUninstaller "$INSTDIR\uninstall.exe"

  ; Put files there
  File "..\readme.txt"
  File "..\search.htm"
  File "..\calculate.js"
  File "..\calendar.js"
  File "..\clock.js"
  File "..\currency.js"
  File "..\helpbox.js"
  File "..\helpmenu.js"
  File "..\history.js"
  File "..\loader.js"
  File "..\phoneno.js"
  File "..\strings.js"
  File "..\tools.js"
  File "..\translate.js"
  File "..\aliases.txt"
  File "..\menu.txt"
  File "..\holidays.sg.xml"
  File "..\holidays.us.xml"
  SetOutPath "$INSTDIR\searches"
  File "..\searches\ac.xml"
  File "..\searches\acx.xml"
  File "..\searches\af.xml"
  File "..\searches\aim.xml"
  File "..\searches\alarm.xml"
  File "..\searches\alm.xml"
  File "..\searches\amaz.xml"
  File "..\searches\amed.xml"
  File "..\searches\ana.xml"
  File "..\searches\ask.xml"
  File "..\searches\at.xml"
  File "..\searches\atm.xml"
  File "..\searches\av.xml"
  File "..\searches\aw.xml"
  File "..\searches\bfe.xml"
  File "..\searches\bible.xml"
  File "..\searches\bomis.xml"
  File "..\searches\cddb.xml"
  File "..\searches\cdnow.xml"
  File "..\searches\cdo.xml"
  File "..\searches\cf.xml"
  File "..\searches\chem.xml"
  File "..\searches\clc.xml"
  File "..\searches\cnn.xml"
  File "..\searches\conv.xml"
  File "..\searches\cpan.xml"
  File "..\searches\crax.xml"
  File "..\searches\dic.xml"
  File "..\searches\dl.xml"
  File "..\searches\dlt.xml"
  File "..\searches\dp.xml"
  File "..\searches\ebay.xml"
  File "..\searches\email.xml"
  File "..\searches\enc.xml"
  File "..\searches\ex.xml"
  File "..\searches\faq.xml"
  File "..\searches\fc.xml"
  File "..\searches\fedex.xml"
  File "..\searches\fst.xml"
  File "..\searches\gg.xml"
  File "..\searches\gga.xml"
  File "..\searches\ggn.xml"
  File "..\searches\half.xml"
  File "..\searches\hs.xml"
  File "..\searches\hsw.xml"
  File "..\searches\ia.xml"
  File "..\searches\ifl.xml"
  File "..\searches\imdb.xml"
  File "..\searches\map.xml"
  File "..\searches\md.xml"
  File "..\searches\mf.xml"
  File "..\searches\mm.xml"
  File "..\searches\msdn.xml"
  File "..\searches\msk.xml"
  File "..\searches\mwd.xml"
  File "..\searches\mwt.xml"
  File "..\searches\mys.xml"
  File "..\searches\pcm.xml"
  File "..\searches\php.xml"
  File "..\searches\phsp.xml"
  File "..\searches\psi.xml"
  File "..\searches\pw.xml"
  File "..\searches\quot.xml"
  File "..\searches\run.xml"
  File "..\searches\sams.xml"
  File "..\searches\sb.xml"
  File "..\searches\sbb.xml"
  File "..\searches\sbc.xml"
  File "..\searches\sbp.xml"
  File "..\searches\screenit.xml"
  File "..\searches\sqlt.xml"
  File "..\searches\start.xml"
  File "..\searches\stk.xml"
  File "..\searches\ths.xml"
  File "..\searches\thw.xml"
  File "..\searches\timer.xml"
  File "..\searches\tvg.xml"
  File "..\searches\tvgc.xml"
  File "..\searches\unip.xml"
  File "..\searches\uns.xml"
  File "..\searches\ups.xml"
  File "..\searches\viv.xml"
  File "..\searches\wfb.xml"
  File "..\searches\wug.xml"
  File "..\searches\xref.xml"
  File "..\searches\yh.xml"
  File "..\searches\yhe.xml"
  File "..\searches\ym.xml"
  File "..\searches\zc.xml"
  File "..\searches\zdn.xml"
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
  File "DQSDTools\License.txt"
  File "DQSDTools\StdAfx.cpp"
  File "DQSDTools\StdAfx.h"
  File "DQSDTools\Utilities.h"
  File "DQSDTools\dlldatax.c"
  File "DQSDTools\dlldatax.h"
  File "DQSDTools\resource.h"


  ; Create prefs file if one doesn't exist
  SetOverwrite off
  SetOutPath "$INSTDIR"
  File "..\preferences.js"
  SetOverwrite on

  ; old source files used to live here
  Delete $INSTDIR\license.txt
  Delete $INSTDIR\search.ico
  Delete $INSTDIR\search.nsi
  Delete $INSTDIR\dqsd.xml
  Delete $INSTDIR\search.xml

  ; The unique uuid for our taskbar
  StrCpy $9 "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"

  ; Write the installation path into the registry
  WriteRegStr HKCR "CLSID\$9" "InstallDir" "$INSTDIR"
  WriteRegStr HKCR "CLSID\$9" "SecureFile" "$INSTDIR\search.htm"

  ; Registry settings needed to function
  WriteRegStr HKCR "CLSID\$9" "" "Quick Search"
  WriteRegStr HKCR "CLSID\$9" "MenuText" "Add Quick Search..."
  WriteRegStr HKCR "CLSID\$9" "HelpText" "Dave's Quick Search Deskbar"
  WriteRegStr HKCR "CLSID\$9\Implemented Categories\{00021492-0000-0000-C000-000000000046}" "" ""
  WriteRegStr HKCR "CLSID\$9\InprocServer32" "" "$SYSDIR\shdocvw.dll"
  WriteRegStr HKCR "CLSID\$9\InprocServer32" "ThreadingModel" "Apartment"
  WriteRegStr HKCR "CLSID\$9\Instance" "CLSID" "{7BA4C742-9E81-11CF-99D3-00AA004AE837}"
  WriteRegStr HKCR "CLSID\$9\Instance\InitPropertyBag" "Url" "$INSTDIR\search.htm"

  ; Uninstallation keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$9" "DisplayName" "Dave's Quick Search Deskbar (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$9" "UninstallString" '"$INSTDIR\uninstall.exe"'

  ; Message box
  MessageBox MB_OK "$8"

SectionEnd

; uninstall stuff

UninstallText "Click next to uninstall Dave's Quick Search Deskbar."

; special uninstall section.
Section "Uninstall"

  ; The unique uuid for our taskbar
  StrCpy $9 "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"

  ; Unegister DQSDTools
  UnRegDLL $INSTDIR\dqsdt255.dll

  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$9"
  DeleteRegKey HKCR "CLSID\$9"

  ; remove files
  Delete /REBOOTOK $INSTDIR\dqsdt255.dll
  Delete /REBOOTOK $INSTDIR\readme.txt
  Delete /REBOOTOK $INSTDIR\search.htm
  Delete /REBOOTOK $INSTDIR\search.xml
  Delete /REBOOTOK $INSTDIR\aliases.txt
  Delete /REBOOTOK $INSTDIR\menu.txt
  Delete /REBOOTOK $INSTDIR\calculate.js
  Delete /REBOOTOK $INSTDIR\calendar.js
  Delete /REBOOTOK $INSTDIR\clock.js
  Delete /REBOOTOK $INSTDIR\currency.js
  Delete /REBOOTOK $INSTDIR\helpbox.js
  Delete /REBOOTOK $INSTDIR\helpmenu.js
  Delete /REBOOTOK $INSTDIR\history.js
  Delete /REBOOTOK $INSTDIR\loader.js
  Delete /REBOOTOK $INSTDIR\phoneno.js
  Delete /REBOOTOK $INSTDIR\strings.js
  Delete /REBOOTOK $INSTDIR\tools.js
  Delete /REBOOTOK $INSTDIR\translate.js
  Delete /REBOOTOK $INSTDIR\holidays.sg.xml
  Delete /REBOOTOK $INSTDIR\holidays.us.xml
  RmDir /r $INSTDIR\src
  RmDir /r $INSTDIR\searches

  ; Remove preferences too
  Delete /REBOOTOK $INSTDIR\preferences.js

  ; MUST REMOVE UNINSTALLER, too
  Delete /REBOOTOK $INSTDIR\uninstall.exe

  ; Delete directory
  RmDir /r $INSTDIR

SectionEnd

; eof
