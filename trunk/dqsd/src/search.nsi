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
  UnRegDLL $INSTDIR\dqsdt255.dll
  Delete /REBOOTOK $INSTDIR\dqsdt255.dll

  ; install new DLL
  SetOverwrite try
  ClearErrors
  File "..\dqsdt257.dll"
  IfErrors seeifsame register

  ; If was unable to upgrade, see if files are the same anyway
  seeifsame:
  ClearErrors
  GetDLLVersionLocal "..\dqsdt257.dll" $1 $2
  GetDLLVersion $INSTDIR\dqsdt257.dll $3 $4
  IfErrors isdifferent
  IntCmpU $1 $3 test1 isdifferent isdifferent
  test1:
  IntCmpU $2 $4 test2 isdifferent isdifferent
  test2:
  ClearErrors
  GetFileTimeLocal "..\dqsdt257.dll" $1 $2
  GetFileTime $INSTDIR\dqsdt257.dll $3 $4
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
  RegDLL $INSTDIR\dqsdt257.dll

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
  File "..\helpbox.js"
  File "..\helpmenu.js"
  File "..\history.js"
  File "..\loader.js"
  File "..\strings.js"
  File "..\tools.js"
  File "..\defer_tools.js"
  File "..\translate.js"
  File "..\aliases.txt"
  File "..\menu.txt"
  File "..\categoryExpander.htc"
  File "..\holidays.sg.xml"
  File "..\holidays.us.xml"
  File "..\holidays.ca.xml"
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
  File "DQSDTools\License.txt"
  File "DQSDTools\StdAfx.cpp"
  File "DQSDTools\StdAfx.h"
  File "DQSDTools\Utilities.h"
  File "DQSDTools\dlldatax.c"
  File "DQSDTools\dlldatax.h"
  File "DQSDTools\resource.h"


  ; Create files with user preferences if they doesn't exist
  SetOverwrite off
  SetOutPath "$INSTDIR"
  File "..\preferences.js"
  File "..\localaliases.txt"
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
  
  ; ??? Keep this for a while for backward compatibility
  WriteRegStr HKCR "CLSID\$9" "SecureFile" "$INSTDIR\search.htm"
  WriteRegDWORD HKCR "CLSID\$9\SecureFiles" "$INSTDIR\search.htm" 0
  WriteRegDWORD HKCR "CLSID\$9\SecureFiles" "$INSTDIR\settings.htm" 0

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
  UnRegDLL $INSTDIR\dqsdt257.dll

  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$9"
  DeleteRegKey HKCR "CLSID\$9"

  ; remove files
  Delete /REBOOTOK $INSTDIR\dqsdt257.dll
  Delete /REBOOTOK $INSTDIR\readme.txt
  Delete /REBOOTOK $INSTDIR\search.htm
  Delete /REBOOTOK $INSTDIR\search.xml
  Delete /REBOOTOK $INSTDIR\aliases.txt
  Delete /REBOOTOK $INSTDIR\menu.txt
  Delete /REBOOTOK $INSTDIR\calculate.js
  Delete /REBOOTOK $INSTDIR\calendar.js
  Delete /REBOOTOK $INSTDIR\clock.js
  Delete /REBOOTOK $INSTDIR\helpbox.js
  Delete /REBOOTOK $INSTDIR\helpmenu.js
  Delete /REBOOTOK $INSTDIR\history.js
  Delete /REBOOTOK $INSTDIR\loader.js
  Delete /REBOOTOK $INSTDIR\strings.js
  Delete /REBOOTOK $INSTDIR\tools.js
  Delete /REBOOTOK $INSTDIR\defer_tools.js
  Delete /REBOOTOK $INSTDIR\translate.js
  Delete /REBOOTOK $INSTDIR\categoryExpander.htc
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
