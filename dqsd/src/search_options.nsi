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
!Define TITLE_AND_COPYRIGHT "Dave's Quick Search Deskbar$\nCopyright (c) 2002 David Bau$\nDistributed under the terms of the$\nGNU General Public License, Version 2"
!Define DQSD_CLSID "{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}"
!Define DQSD_TITLE "Dave's Quick Search Deskbar"

; Silent install
DirShow hide
CRCCheck on
SetDatablockOptimize on
Icon search.ico

; The file to write
OutFile "..\DQSD_opt.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\Quick Search Deskbar"

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKCR "CLSID\${DQSD_CLSID}" "InstallDir"

; The text to prompt the user to enter a directory
DirText "Choose a directory to install in to:"

InstType "Full"
InstType "United States"
InstType "United Kingdom"
InstType "France"
InstType "Canada"


ComponentText "Select what you wish to install."


; The stuff to install
Section "Quick Search Deskbar (required)"
  SectionIn 1 2 3 4 5 RO
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
  UnRegDLL $INSTDIR\DQSDTools.dll
  Delete /REBOOTOK $INSTDIR\DQSDTools.dll
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
  File "..\helpbox.js"
  File "..\helpmenu.js"
  File "..\history.js"
  File "..\holidays.ausnsw.xml"
  File "..\holidays.fr.xml"
  File "..\holidays.ca.xml"
  File "..\holidays.it.xml"
  File "..\holidays.nl.NL.xml"
  File "..\holidays.nl.EN.xml"
  File "..\holidays.sg.xml"
  File "..\holidays.us.xml"
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
  File "..\xplocalsearch.css"
  File "..\xptoolbarbot.bmp"
  File "..\xptoolbartop.bmp"
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

  ; Create files with user preferences if they doesn't exist
  SetOverwrite off
  SetOutPath "$INSTDIR"
  File "..\localaliases.txt"
  File "..\mrumenu.txt"
  File "..\localsearch.css"
  File "..\localprefs.js"
  SetOverwrite on
  
  ; Create subdirectory for user searches
  CreateDirectory "localsearches"

  ; old source files used to live here
  Delete $INSTDIR\license.txt
  Delete $INSTDIR\search.ico
  Delete $INSTDIR\search.nsi
  Delete $INSTDIR\dqsd.xml
  Delete $INSTDIR\search.xml

  ; old versions of searches to delete; most renamed or consolidated
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
  Delete $INSTDIR\searches\math.xml
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

Section "Standard Searches"
  SectionIn 1 2 3 4 5 RO
  SetOutPath "$INSTDIR\searches"
  File ..\searches\abt.xml
  File ..\searches\acx.xml
  File ..\searches\af.xml
  File ..\searches\aj.xml
  File ..\searches\alarm.xml
  File ..\searches\alistapart.xml
  File ..\searches\alm.xml
  File ..\searches\amaz.xml
  File ..\searches\amed.xml
  File ..\searches\amo.xml
  File ..\searches\ana.xml
  File ..\searches\anon.xml
  File ..\searches\anthelp.xml
  File ..\searches\aolim.xml
  File ..\searches\ar.xml
  File ..\searches\archi.xml
  File ..\searches\arin.xml
  File ..\searches\ascii.xml
  File ..\searches\atech.xml
  File ..\searches\atm.xml
  File ..\searches\av.xml
  File ..\searches\aw.xml
  File ..\searches\babel.xml
  File ..\searches\bb.xml
  File ..\searches\bfe.xml
  File ..\searches\bg.xml
  File ..\searches\bigchart.xml
  File ..\searches\bio.xml
  File ..\searches\blb.xml
  File ..\searches\bn.xml
  File ..\searches\bomis.xml
  File ..\searches\bq.xml
  File ..\searches\brit.xml
  File ..\searches\bta.xml
  File ..\searches\ccase.xml
  File ..\searches\cddb.xml
  File ..\searches\cdnow.xml
  File ..\searches\cdo.xml
  File ..\searches\chap.xml
  File ..\searches\cheapcd.xml
  File ..\searches\chem.xml
  File ..\searches\chmodconv.xml
  File ..\searches\clc.xml
  File ..\searches\cnn.xml
  File ..\searches\configure.xml
  File ..\searches\conv.xml
  File ..\searches\cowp.xml
  File ..\searches\cp.xml
  File ..\searches\cpan.xml
  File ..\searches\cpp.xml
  File ..\searches\crax.xml
  File ..\searches\cs.xml
  File ..\searches\ct.xml
  File ..\searches\curr.xml
  File ..\searches\daypop.xml
  File ..\searches\dg.xml
  File ..\searches\dic.xml
  File ..\searches\dl.xml
  File ..\searches\dlt.xml
  File ..\searches\dns.xml
  File ..\searches\dp.xml
  File ..\searches\dps.xml
  File ..\searches\dqsd.xml
  File ..\searches\drinkstreet.xml
  File ..\searches\dseek.xml
  File ..\searches\dyndns.xml
  File ..\searches\e2.xml
  File ..\searches\ebay.xml
  File ..\searches\ebuy.xml
  File ..\searches\egw.xml
  File ..\searches\email.xml
  File ..\searches\enc.xml
  File ..\searches\encarta.xml
  File ..\searches\ep.xml
  File ..\searches\evolt.xml
  File ..\searches\ex.xml
  File ..\searches\expd.xml
  File ..\searches\faq.xml
  File ..\searches\fd.xml
  File ..\searches\fe.xml
  File ..\searches\findlaw.xml
  File ..\searches\fm.xml
  File ..\searches\freedb.xml
  File ..\searches\freetrans.xml
  File ..\searches\fs.xml
  File ..\searches\fst.xml
  File ..\searches\fv.xml
  File ..\searches\gfaq.xml
  File ..\searches\gg.xml
  File ..\searches\googletrans.xml
  File ..\searches\gtwhois.xml
  File ..\searches\half.xml
  File ..\searches\hb.xml
  File ..\searches\hexconv.xml
  File ..\searches\howfar.xml
  File ..\searches\hs.xml
  File ..\searches\hsw.xml
  File ..\searches\htmlh.xml
  File ..\searches\httpd.xml
  File ..\searches\hv.xml
  File ..\searches\ia.xml
  File ..\searches\iep.xml
  File ..\searches\imdb.xml
  File ..\searches\infosec.xml
  File ..\searches\jdk.xml
  File ..\searches\js.xml
  File ..\searches\jsenc.xml
  File ..\searches\jsexec.xml
  File ..\searches\jsref.xml
  File ..\searches\lf.xml
  File ..\searches\lipsum.xml
  File ..\searches\lottogen.xml
  File ..\searches\mac.xml
  File ..\searches\mag.xml
  File ..\searches\manpage.xml
  File ..\searches\mathref.xml
  File ..\searches\md.xml
  File ..\searches\md5.xml
  File ..\searches\mf.xml
  File ..\searches\mg.xml
  File ..\searches\mrqe.xml
  File ..\searches\msda.xml
  File ..\searches\msdn.xml
  File ..\searches\msk.xml
  File ..\searches\msl.xml
  File ..\searches\multi.xml
  File ..\searches\mwd.xml
  File ..\searches\mwt.xml
  File ..\searches\mys.xml
  File ..\searches\mysimon.xml
  File ..\searches\nlstart.xml
  File ..\searches\npdr.xml
  File ..\searches\ns.xml
  File ..\searches\nup.xml
  File ..\searches\nyse.xml
  File ..\searches\ors.xml
  File ..\searches\ostk.xml
  File ..\searches\otn.xml
  File ..\searches\palmgear.xml
  File ..\searches\passgen.xml
  File ..\searches\pcm.xml
  File ..\searches\pg.xml
  File ..\searches\pgrab.xml
  File ..\searches\php.xml
  File ..\searches\phpb.xml
  File ..\searches\phsp.xml
  File ..\searches\pinp.xml
  File ..\searches\psi.xml
  File ..\searches\pw.xml
  File ..\searches\qrz.xml
  File ..\searches\qsfind.xml
  File ..\searches\queue.xml
  File ..\searches\quot.xml
  File ..\searches\radix.xml
  File ..\searches\reget.xml
  File ..\searches\rfc.xml
  File ..\searches\rgb.xml
  File ..\searches\rnum.xml
  File ..\searches\rot13.xml
  File ..\searches\rt.xml
  File ..\searches\run.xml
  File ..\searches\rw.xml
  File ..\searches\rz.xml
  File ..\searches\sams.xml
  File ..\searches\screenit.xml
  File ..\searches\sd.xml
  File ..\searches\sep.xml
  File ..\searches\setiq.xml
  File ..\searches\sf.xml
  File ..\searches\shack.xml
  File ..\searches\sl.xml
  File ..\searches\smlnk.xml
  File ..\searches\sp.xml
  File ..\searches\spcs.xml
  File ..\searches\sqlt.xml
  File ..\searches\stk.xml
  File ..\searches\sw.xml
  File ..\searches\tcc.xml
  File ..\searches\temp.xml
  File ..\searches\tgn.xml
  File ..\searches\thelist.xml
  File ..\searches\ths.xml
  File ..\searches\thw.xml
  File ..\searches\timer.xml
  File ..\searches\tinyurl.xml
  File ..\searches\tldp.xml
  File ..\searches\tm.xml
  File ..\searches\tr.xml
  File ..\searches\tucows.xml
  File ..\searches\tw.xml
  File ..\searches\txp.xml
  File ..\searches\ubl.xml
  File ..\searches\ubs.xml
  File ..\searches\ul.xml
  File ..\searches\ulwhois.xml
  File ..\searches\unip.xml
  File ..\searches\uns.xml
  File ..\searches\viam.xml
  File ..\searches\videoeta.xml
  File ..\searches\vir.xml
  File ..\searches\viv.xml
  File ..\searches\wa.xml
  File ..\searches\walmart.xml
  File ..\searches\wc.xml
  File ..\searches\webm.xml
  File ..\searches\webopedia.xml
  File ..\searches\webpoll.xml
  File ..\searches\wf.xml
  File ..\searches\wfb.xml
  File ..\searches\wgn.xml
  File ..\searches\whatis.xml
  File ..\searches\whois.xml
  File ..\searches\wik.xml
  File ..\searches\winf.xml
  File ..\searches\winres.xml
  File ..\searches\wn.xml
  File ..\searches\wotsit.xml
  File ..\searches\xref.xml
  File ..\searches\yg.xml
  File ..\searches\yh.xml
  File ..\searches\yhe.xml
  File ..\searches\ystocks.xml
  File ..\searches\zdn.xml
  
SectionEnd

Section "United States"
  SectionIn 1 2
SetOutPath "$INSTDIR\searches"
File ..\searches\abex.xml
File ..\searches\ac.xml
File ..\searches\anywho.xml
File ..\searches\dhl.xml
File ..\searches\dir.xml
File ..\searches\fandango.xml
File ..\searches\fedex.xml
File ..\searches\mb.xml
File ..\searches\mq.xml
File ..\searches\sb.xml
File ..\searches\tvg.xml
File ..\searches\ups.xml
File ..\searches\usps.xml
File ..\searches\wug.xml
File ..\searches\ym.xml
File ..\searches\yp.xml
File ..\searches\yweather.xml
File ..\searches\zc.xml

SectionEnd 

Section "United Kingdom (2 searches)"
  SectionIn 1 3
SetOutPath "$INSTDIR\searches"
File ..\searches\mm.xml
File ..\searches\uktv.xml

SectionEnd

Section "France (8 searches)"
  SectionIn 1 4
SetOutPath "$INSTDIR\searches"
File ..\searches\acine.xml
File ..\searches\cjfr.xml
File ..\searches\dicfr.xml
File ..\searches\encyfr.xml
File ..\searches\so.xml
File ..\searches\synfr.xml
File ..\searches\telfr.xml
File ..\searches\wpfr.xml

SectionEnd

Section "Canada (6 searches)"
  SectionIn 1 5
SetOutPath "$INSTDIR\searches"
File ..\searches\is.xml
File ..\searches\isb.xml
File ..\searches\isc.xml
File ..\searches\isp.xml
File ..\searches\ispb.xml
File ..\searches\twn.xml

SectionEnd

Section "Italy (1 search)"
 SectionIn 1
SetOutPath "$INSTDIR\searches"
File ..\searches\dicit.xml

SectionEnd

Section "Brazil (1 search)"
 SectionIn 1
File ..\searches\guiasp.xml
SectionEnd

Section "Australia (2 searches)"
 SectionIn 1
File ..\searches\newsau.xml
File ..\searches\smh.xml
 SectionEnd

; uncomment the following section after adding Kim's Swedish searches to CVS.
;Section "Sweden (2 searches)"
; SectionIn 1
;File ..\searches\pcbok.xml
;File ..\searches\sven.xml
;SectionEnd



; uninstall stuff

UninstallText "Click next to uninstall ${DQSD_TITLE}."

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
  Delete /REBOOTOK $INSTDIR\helpbox.js
  Delete /REBOOTOK $INSTDIR\helpmenu.js
  Delete /REBOOTOK $INSTDIR\history.js
  Delete /REBOOTOK $INSTDIR\holidays.ausnsw.xml
  Delete /REBOOTOK $INSTDIR\holidays.fr.xml
  Delete /REBOOTOK $INSTDIR\holidays.ca.xml
  Delete /REBOOTOK $INSTDIR\holidays.it.xml
  Delete /REBOOTOK $INSTDIR\holidays.nl.NL.xml
  Delete /REBOOTOK $INSTDIR\holidays.nl.EN.xml
  Delete /REBOOTOK $INSTDIR\holidays.sg.xml
  Delete /REBOOTOK $INSTDIR\holidays.us.xml
  Delete /REBOOTOK $INSTDIR\loader.js
  Delete /REBOOTOK $INSTDIR\maximized.htm
  Delete /REBOOTOK $INSTDIR\menu.txt
  Delete /REBOOTOK $INSTDIR\readme.txt
  Delete /REBOOTOK $INSTDIR\search.css
  Delete /REBOOTOK $INSTDIR\search.htm
  Delete /REBOOTOK $INSTDIR\search.xml
  Delete /REBOOTOK $INSTDIR\strings.js
  Delete /REBOOTOK $INSTDIR\tools.js
  Delete /REBOOTOK $INSTDIR\translate.js
  Delete /REBOOTOK $INSTDIR\version.js
  Delete /REBOOTOK $INSTDIR\version.xml
  Delete /REBOOTOK $INSTDIR\versiondialog.htm
  Delete /REBOOTOK $INSTDIR\xplocalsearch.css
  Delete /REBOOTOK $INSTDIR\xptoolbarbot.bmp
  Delete /REBOOTOK $INSTDIR\xptoolbartop.bmp
  RmDir /r $INSTDIR\src
  RmDir /r $INSTDIR\searches
  RmDir /r $INSTDIR\addons

  ; Remove preferences too
  Delete /REBOOTOK $INSTDIR\preferences.js

  ; MUST REMOVE UNINSTALLER, too
  Delete /REBOOTOK $INSTDIR\uninstall.exe

  ; Delete directory
  RmDir /r $INSTDIR

SectionEnd

; eof