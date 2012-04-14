# Microsoft Developer Studio Generated NMAKE File, Based on BrowserHooker.dsp
!IF "$(CFG)" == ""
CFG=BrowserHooker - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BrowserHooker - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BrowserHooker - Win32 Release" && "$(CFG)" != "BrowserHooker - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BrowserHooker.mak" CFG="BrowserHooker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BrowserHooker - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BrowserHooker - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "BrowserHooker - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\BrowserHooker.exe" "$(OUTDIR)\BrowserHooker.bsc"


CLEAN :
	-@erase "$(INTDIR)\BHNavigator.obj"
	-@erase "$(INTDIR)\BHNavigator.sbr"
	-@erase "$(INTDIR)\BrowserHooker.obj"
	-@erase "$(INTDIR)\BrowserHooker.pch"
	-@erase "$(INTDIR)\BrowserHooker.res"
	-@erase "$(INTDIR)\BrowserHooker.sbr"
	-@erase "$(INTDIR)\BrowserHookerDoc.obj"
	-@erase "$(INTDIR)\BrowserHookerDoc.sbr"
	-@erase "$(INTDIR)\BrowserHookerView.obj"
	-@erase "$(INTDIR)\BrowserHookerView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Socket.obj"
	-@erase "$(INTDIR)\Socket.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\BrowserHooker.bsc"
	-@erase "$(OUTDIR)\BrowserHooker.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BrowserHooker.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\BrowserHooker.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BrowserHooker.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BHNavigator.sbr" \
	"$(INTDIR)\BrowserHooker.sbr" \
	"$(INTDIR)\BrowserHookerDoc.sbr" \
	"$(INTDIR)\BrowserHookerView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Socket.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BrowserHooker.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\BrowserHooker.pdb" /machine:I386 /out:"$(OUTDIR)\BrowserHooker.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BHNavigator.obj" \
	"$(INTDIR)\BrowserHooker.obj" \
	"$(INTDIR)\BrowserHookerDoc.obj" \
	"$(INTDIR)\BrowserHookerView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Socket.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BrowserHooker.res"

"$(OUTDIR)\BrowserHooker.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BrowserHooker - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\BrowserHooker.exe" "$(OUTDIR)\BrowserHooker.bsc"


CLEAN :
	-@erase "$(INTDIR)\BHNavigator.obj"
	-@erase "$(INTDIR)\BHNavigator.sbr"
	-@erase "$(INTDIR)\BrowserHooker.obj"
	-@erase "$(INTDIR)\BrowserHooker.pch"
	-@erase "$(INTDIR)\BrowserHooker.res"
	-@erase "$(INTDIR)\BrowserHooker.sbr"
	-@erase "$(INTDIR)\BrowserHookerDoc.obj"
	-@erase "$(INTDIR)\BrowserHookerDoc.sbr"
	-@erase "$(INTDIR)\BrowserHookerView.obj"
	-@erase "$(INTDIR)\BrowserHookerView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Socket.obj"
	-@erase "$(INTDIR)\Socket.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\BrowserHooker.bsc"
	-@erase "$(OUTDIR)\BrowserHooker.exe"
	-@erase "$(OUTDIR)\BrowserHooker.ilk"
	-@erase "$(OUTDIR)\BrowserHooker.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BrowserHooker.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\BrowserHooker.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BrowserHooker.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BHNavigator.sbr" \
	"$(INTDIR)\BrowserHooker.sbr" \
	"$(INTDIR)\BrowserHookerDoc.sbr" \
	"$(INTDIR)\BrowserHookerView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Socket.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\BrowserHooker.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\BrowserHooker.pdb" /debug /machine:I386 /out:"$(OUTDIR)\BrowserHooker.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BHNavigator.obj" \
	"$(INTDIR)\BrowserHooker.obj" \
	"$(INTDIR)\BrowserHookerDoc.obj" \
	"$(INTDIR)\BrowserHookerView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Socket.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BrowserHooker.res"

"$(OUTDIR)\BrowserHooker.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("BrowserHooker.dep")
!INCLUDE "BrowserHooker.dep"
!ELSE 
!MESSAGE Warning: cannot find "BrowserHooker.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BrowserHooker - Win32 Release" || "$(CFG)" == "BrowserHooker - Win32 Debug"
SOURCE=.\BHNavigator.cpp

"$(INTDIR)\BHNavigator.obj"	"$(INTDIR)\BHNavigator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BrowserHooker.pch"


SOURCE=.\BrowserHooker.cpp

"$(INTDIR)\BrowserHooker.obj"	"$(INTDIR)\BrowserHooker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BrowserHooker.pch"


SOURCE=.\BrowserHooker.rc

"$(INTDIR)\BrowserHooker.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\BrowserHookerDoc.cpp

"$(INTDIR)\BrowserHookerDoc.obj"	"$(INTDIR)\BrowserHookerDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BrowserHooker.pch"


SOURCE=.\BrowserHookerView.cpp

"$(INTDIR)\BrowserHookerView.obj"	"$(INTDIR)\BrowserHookerView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BrowserHooker.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BrowserHooker.pch"


SOURCE=.\Socket.cpp

"$(INTDIR)\Socket.obj"	"$(INTDIR)\Socket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\BrowserHooker.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "BrowserHooker - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BrowserHooker.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BrowserHooker.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "BrowserHooker - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\BrowserHooker.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\BrowserHooker.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

