# Microsoft Developer Studio Generated NMAKE File, Based on DQSDTools.dsp
!IF "$(CFG)" == ""
CFG=DQSDTools - Win32 Debug
!MESSAGE No configuration specified. Defaulting to DQSDTools - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DQSDTools - Win32 Debug" && "$(CFG)" != "DQSDTools - Win32 Release MinDependency"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DQSDTools.mak" CFG="DQSDTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DQSDTools - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DQSDTools - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DQSDTools - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\DQSDTools.dll" ".\DQSDTools.tlb" ".\DQSDTools.h" ".\DQSDTools_i.c" "$(OUTDIR)\DQSDTools.bsc" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\DQSDTools.obj"
	-@erase "$(INTDIR)\DQSDTools.pch"
	-@erase "$(INTDIR)\DQSDTools.res"
	-@erase "$(INTDIR)\DQSDTools.sbr"
	-@erase "$(INTDIR)\KeyboardHook.obj"
	-@erase "$(INTDIR)\KeyboardHook.sbr"
	-@erase "$(INTDIR)\Launcher.obj"
	-@erase "$(INTDIR)\Launcher.sbr"
	-@erase "$(INTDIR)\MenuBuilder.obj"
	-@erase "$(INTDIR)\MenuBuilder.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\DQSDTools.bsc"
	-@erase "$(OUTDIR)\DQSDTools.dll"
	-@erase "$(OUTDIR)\DQSDTools.exp"
	-@erase "$(OUTDIR)\DQSDTools.ilk"
	-@erase "$(OUTDIR)\DQSDTools.lib"
	-@erase "$(OUTDIR)\DQSDTools.pdb"
	-@erase ".\DQSDTools.h"
	-@erase ".\DQSDTools.tlb"
	-@erase ".\DQSDTools_i.c"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DQSDTools.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\DQSDTools.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DQSDTools.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DQSDTools.sbr" \
	"$(INTDIR)\KeyboardHook.sbr" \
	"$(INTDIR)\Launcher.sbr" \
	"$(INTDIR)\MenuBuilder.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Utilities.sbr"

"$(OUTDIR)\DQSDTools.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shlwapi.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /base:"0x4000000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\DQSDTools.pdb" /debug /machine:I386 /def:".\DQSDTools.def" /out:"$(OUTDIR)\DQSDTools.dll" /implib:"$(OUTDIR)\DQSDTools.lib" /pdbtype:sept 
DEF_FILE= \
	".\DQSDTools.def"
LINK32_OBJS= \
	"$(INTDIR)\DQSDTools.obj" \
	"$(INTDIR)\KeyboardHook.obj" \
	"$(INTDIR)\Launcher.obj" \
	"$(INTDIR)\MenuBuilder.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\DQSDTools.res"

"$(OUTDIR)\DQSDTools.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\DQSDTools.dll
InputPath=.\Debug\DQSDTools.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "DQSDTools - Win32 Release MinDependency"

OUTDIR=.\ReleaseMinDependency
INTDIR=.\ReleaseMinDependency
# Begin Custom Macros
OutDir=.\ReleaseMinDependency
# End Custom Macros

ALL : "$(OUTDIR)\DQSDTools.dll" "$(OUTDIR)\DQSDTools.bsc" ".\ReleaseMinDependency\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\DQSDTools.obj"
	-@erase "$(INTDIR)\DQSDTools.pch"
	-@erase "$(INTDIR)\DQSDTools.res"
	-@erase "$(INTDIR)\DQSDTools.sbr"
	-@erase "$(INTDIR)\KeyboardHook.obj"
	-@erase "$(INTDIR)\KeyboardHook.sbr"
	-@erase "$(INTDIR)\Launcher.obj"
	-@erase "$(INTDIR)\Launcher.sbr"
	-@erase "$(INTDIR)\MenuBuilder.obj"
	-@erase "$(INTDIR)\MenuBuilder.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DQSDTools.bsc"
	-@erase "$(OUTDIR)\DQSDTools.dll"
	-@erase "$(OUTDIR)\DQSDTools.exp"
	-@erase "$(OUTDIR)\DQSDTools.lib"
	-@erase ".\DQSDTools.h"
	-@erase ".\DQSDTools.tlb"
	-@erase ".\DQSDTools_i.c"
	-@erase ".\ReleaseMinDependency\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DQSDTools.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\DQSDTools.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DQSDTools.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DQSDTools.sbr" \
	"$(INTDIR)\KeyboardHook.sbr" \
	"$(INTDIR)\Launcher.sbr" \
	"$(INTDIR)\MenuBuilder.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Utilities.sbr"

"$(OUTDIR)\DQSDTools.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shlwapi.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /base:"0x4000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\DQSDTools.pdb" /machine:I386 /def:".\DQSDTools.def" /out:"$(OUTDIR)\DQSDTools.dll" /implib:"$(OUTDIR)\DQSDTools.lib" 
DEF_FILE= \
	".\DQSDTools.def"
LINK32_OBJS= \
	"$(INTDIR)\DQSDTools.obj" \
	"$(INTDIR)\KeyboardHook.obj" \
	"$(INTDIR)\Launcher.obj" \
	"$(INTDIR)\MenuBuilder.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\DQSDTools.res"

"$(OUTDIR)\DQSDTools.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\DQSDTools.dll
InputPath=.\ReleaseMinDependency\DQSDTools.dll
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL_PROJ=

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("DQSDTools.dep")
!INCLUDE "DQSDTools.dep"
!ELSE 
!MESSAGE Warning: cannot find "DQSDTools.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "DQSDTools - Win32 Debug" || "$(CFG)" == "DQSDTools - Win32 Release MinDependency"
SOURCE=.\DQSDTools.cpp

"$(INTDIR)\DQSDTools.obj"	"$(INTDIR)\DQSDTools.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DQSDTools.pch"


SOURCE=.\DQSDTools.idl
MTL_SWITCHES=/tlb ".\DQSDTools.tlb" /h "DQSDTools.h" /iid "DQSDTools_i.c" /Oicf 

".\DQSDTools.tlb"	".\DQSDTools.h"	".\DQSDTools_i.c" : $(SOURCE) "$(INTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


SOURCE=.\DQSDTools.rc

"$(INTDIR)\DQSDTools.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\KeyboardHook.cpp

"$(INTDIR)\KeyboardHook.obj"	"$(INTDIR)\KeyboardHook.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DQSDTools.pch"


SOURCE=.\Launcher.cpp

"$(INTDIR)\Launcher.obj"	"$(INTDIR)\Launcher.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DQSDTools.pch"


SOURCE=.\MenuBuilder.cpp

"$(INTDIR)\MenuBuilder.obj"	"$(INTDIR)\MenuBuilder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DQSDTools.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "DQSDTools - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DQSDTools.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\DQSDTools.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DQSDTools - Win32 Release MinDependency"

CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DQSDTools.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\DQSDTools.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Utilities.cpp

"$(INTDIR)\Utilities.obj"	"$(INTDIR)\Utilities.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DQSDTools.pch"



!ENDIF 

