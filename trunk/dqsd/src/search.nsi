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

  ; Try to upgrade the DLL
  UnRegDLL $INSTDIR\DQSDTools.dll
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

  ; If different, stop the install and tell the user
  isdifferent:
  MessageBox MB_OK "Did not finish installing the deskbar because DQSDTools.dll is in use.$\nThe DLL has now been unregistered.$\n$\nPlease quit the Windows explorer (logout/login or simply restart)$\nand run this installer again."
  Abort

  ; We can register the dll and continue
  register:
  RegDLL $INSTDIR\DQSDTools.dll

  SetOverwrite on

  ; Determine if this is an upgrade
  StrCpy $8 "The Deskbar has been upgraded.$\n$\nRight-click on the handle to the left of the$\nsearch deskbar and select 'Refresh' to$\nrefresh the deskbar.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2001 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  IfFileExists "$INSTDIR\search.htm" upgradebar
  StrCpy $8 "The Deskbar has been installed.$\n$\nRight-click in your taskbar and select$\n$\n    Toolbar > Add Quick Search...$\n$\nto add the Quick Search Deskbar to your taskbar.$\n$\nDave's Quick Search Deskbar$\nCopyright (c) 2001 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
  upgradebar:

  

  WriteUninstaller "$INSTDIR\uninstall.exe"

  ; Put files there
  File "..\readme.txt"
  File "..\search.htm"
  File "..\search.xml"
  SetOutPath "$INSTDIR\src"
  File "search.ico"
  File "search.nsi"
  File "build.cmd"
  File "clean.cmd"
  File "dqsd.xml"
  SetOutPath "$INSTDIR\src\DQSDTools"
  File "DQSDTools\DQSDTools.cpp"
  File "DQSDTools\DQSDTools.def"
  File "DQSDTools\DQSDTools.dsp"
  File "DQSDTools\DQSDTools.dsw"
  File "DQSDTools\DQSDTools.idl"
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

  ; redundant with readme
  Delete $INSTDIR\license.txt

  ; The unique uuid for our taskbar
  StrCpy $9 "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"

  ; Write the installation path into the registry
  WriteRegStr HKCR "CLSID\$9" "InstallDir" "$INSTDIR"

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
  UnRegDLL $INSTDIR\DQSDTools.dll

  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$9"
  DeleteRegKey HKCR "CLSID\$9"

  ; remove files
  Delete $INSTDIR\readme.txt
  Delete $INSTDIR\search.htm
  Delete $INSTDIR\search.nsi
  Delete $INSTDIR\search.ico
  Delete $INSTDIR\search.xml
  Delete $INSTDIR\dqsd.xml
  Delete $INSTDIR\DQSDTools.dll
  Delete $INSTDIR\src\build.cmd
  Delete $INSTDIR\src\clean.cmd
  Delete $INSTDIR\src\DQSDTools\DQSDTools.cpp
  Delete $INSTDIR\src\DQSDTools\DQSDTools.def
  Delete $INSTDIR\src\DQSDTools\DQSDTools.dsp
  Delete $INSTDIR\src\DQSDTools\DQSDTools.dsw
  Delete $INSTDIR\src\DQSDTools\DQSDTools.idl
  Delete $INSTDIR\src\DQSDTools\DQSDTools.rc
  Delete $INSTDIR\src\DQSDTools\Launcher.cpp
  Delete $INSTDIR\src\DQSDTools\Launcher.h
  Delete $INSTDIR\src\DQSDTools\Launcher.rgs
  Delete $INSTDIR\src\DQSDTools\License.txt
  Delete $INSTDIR\src\DQSDTools\StdAfx.cpp
  Delete $INSTDIR\src\DQSDTools\StdAfx.h
  Delete $INSTDIR\src\DQSDTools\Utilities.h
  Delete $INSTDIR\src\DQSDTools\dlldatax.c
  Delete $INSTDIR\src\DQSDTools\dlldatax.h
  Delete $INSTDIR\src\DQSDTools\resource.h

  ; Remove preferences too
  Delete $INSTDIR\preferences.js

  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe

  ; Delete directory
  RmDir /r $INSTDIR

SectionEnd

; eof
