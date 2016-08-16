# Microsoft Developer Studio Generated NMAKE File, Based on Chelovek.dsp
!IF "$(CFG)" == ""
CFG=Chelovek - Win32 Debug
!MESSAGE Keine Konfiguration angegeben. Chelovek - Win32 Debug wird als\
 Standard verwendet.
!ENDIF 

!IF "$(CFG)" != "Chelovek - Win32 Release" && "$(CFG)" !=\
 "Chelovek - Win32 Debug"
!MESSAGE Ungultige Konfiguration "$(CFG)" angegeben.
!MESSAGE Sie konnen beim Ausfuhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "Chelovek.mak" CFG="Chelovek - Win32 Debug"
!MESSAGE 
!MESSAGE Fur die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "Chelovek - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "Chelovek - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 
!ERROR Eine ungultige Konfiguration wurde angegeben.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Chelovek - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
TargetName=Chelovek
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OutDir)\$(TargetName).hlp" "$(OUTDIR)\Chelovek.exe"

!ELSE 

ALL : "$(OutDir)\$(TargetName).hlp" "$(OUTDIR)\Chelovek.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Chelovek.obj"
	-@erase "$(INTDIR)\Chelovek.pch"
	-@erase "$(INTDIR)\Chelovek.res"
	-@erase "$(INTDIR)\KbDataSourceCSV.obj"
	-@erase "$(INTDIR)\KbKB.obj"
	-@erase "$(INTDIR)\KbSpace.obj"
	-@erase "$(INTDIR)\LkCrispCNF.obj"
	-@erase "$(INTDIR)\LkFuzzyCNF.obj"
	-@erase "$(INTDIR)\LkNode.obj"
	-@erase "$(INTDIR)\LkSegmCNF.obj"
	-@erase "$(INTDIR)\LkVector.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Ri.obj"
	-@erase "$(INTDIR)\RiCNF.obj"
	-@erase "$(INTDIR)\RiCNF_B.obj"
	-@erase "$(INTDIR)\RiData.obj"
	-@erase "$(INTDIR)\RiDataSourceDlg.obj"
	-@erase "$(INTDIR)\RiDoc.obj"
	-@erase "$(INTDIR)\RiKB.obj"
	-@erase "$(INTDIR)\RiListView.obj"
	-@erase "$(INTDIR)\RiNode.obj"
	-@erase "$(INTDIR)\RiOptAnalysisDlg.obj"
	-@erase "$(INTDIR)\RiOptInductionDlg.obj"
	-@erase "$(INTDIR)\RiProcessAnalysisDlg.obj"
	-@erase "$(INTDIR)\RiProcessInductionDlg.obj"
	-@erase "$(INTDIR)\RiRules.obj"
	-@erase "$(INTDIR)\RiSpace.obj"
	-@erase "$(INTDIR)\RiTreeView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Chelovek.exe"
	-@erase "$(OutDir)\$(TargetName).hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\Chelovek.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Chelovek.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Chelovek.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Chelovek.pdb" /machine:I386 /out:"$(OUTDIR)\Chelovek.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Chelovek.obj" \
	"$(INTDIR)\Chelovek.res" \
	"$(INTDIR)\KbDataSourceCSV.obj" \
	"$(INTDIR)\KbKB.obj" \
	"$(INTDIR)\KbSpace.obj" \
	"$(INTDIR)\LkCrispCNF.obj" \
	"$(INTDIR)\LkFuzzyCNF.obj" \
	"$(INTDIR)\LkNode.obj" \
	"$(INTDIR)\LkSegmCNF.obj" \
	"$(INTDIR)\LkVector.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Ri.obj" \
	"$(INTDIR)\RiCNF.obj" \
	"$(INTDIR)\RiCNF_B.obj" \
	"$(INTDIR)\RiData.obj" \
	"$(INTDIR)\RiDataSourceDlg.obj" \
	"$(INTDIR)\RiDoc.obj" \
	"$(INTDIR)\RiKB.obj" \
	"$(INTDIR)\RiListView.obj" \
	"$(INTDIR)\RiNode.obj" \
	"$(INTDIR)\RiOptAnalysisDlg.obj" \
	"$(INTDIR)\RiOptInductionDlg.obj" \
	"$(INTDIR)\RiProcessAnalysisDlg.obj" \
	"$(INTDIR)\RiProcessInductionDlg.obj" \
	"$(INTDIR)\RiRules.obj" \
	"$(INTDIR)\RiSpace.obj" \
	"$(INTDIR)\RiTreeView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Chelovek.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Chelovek - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
TargetName=Chelovek
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OutDir)\$(TargetName).hlp" "$(OUTDIR)\Chelovek.exe"

!ELSE 

ALL : "$(OutDir)\$(TargetName).hlp" "$(OUTDIR)\Chelovek.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Chelovek.obj"
	-@erase "$(INTDIR)\Chelovek.pch"
	-@erase "$(INTDIR)\Chelovek.res"
	-@erase "$(INTDIR)\KbDataSourceCSV.obj"
	-@erase "$(INTDIR)\KbKB.obj"
	-@erase "$(INTDIR)\KbSpace.obj"
	-@erase "$(INTDIR)\LkCrispCNF.obj"
	-@erase "$(INTDIR)\LkFuzzyCNF.obj"
	-@erase "$(INTDIR)\LkNode.obj"
	-@erase "$(INTDIR)\LkSegmCNF.obj"
	-@erase "$(INTDIR)\LkVector.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Ri.obj"
	-@erase "$(INTDIR)\RiCNF.obj"
	-@erase "$(INTDIR)\RiCNF_B.obj"
	-@erase "$(INTDIR)\RiData.obj"
	-@erase "$(INTDIR)\RiDataSourceDlg.obj"
	-@erase "$(INTDIR)\RiDoc.obj"
	-@erase "$(INTDIR)\RiKB.obj"
	-@erase "$(INTDIR)\RiListView.obj"
	-@erase "$(INTDIR)\RiNode.obj"
	-@erase "$(INTDIR)\RiOptAnalysisDlg.obj"
	-@erase "$(INTDIR)\RiOptInductionDlg.obj"
	-@erase "$(INTDIR)\RiProcessAnalysisDlg.obj"
	-@erase "$(INTDIR)\RiProcessInductionDlg.obj"
	-@erase "$(INTDIR)\RiRules.obj"
	-@erase "$(INTDIR)\RiSpace.obj"
	-@erase "$(INTDIR)\RiTreeView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Chelovek.exe"
	-@erase "$(OUTDIR)\Chelovek.ilk"
	-@erase "$(OUTDIR)\Chelovek.pdb"
	-@erase "$(OutDir)\$(TargetName).hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /Fp"$(INTDIR)\Chelovek.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Chelovek.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Chelovek.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Chelovek.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Chelovek.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Chelovek.obj" \
	"$(INTDIR)\Chelovek.res" \
	"$(INTDIR)\KbDataSourceCSV.obj" \
	"$(INTDIR)\KbKB.obj" \
	"$(INTDIR)\KbSpace.obj" \
	"$(INTDIR)\LkCrispCNF.obj" \
	"$(INTDIR)\LkFuzzyCNF.obj" \
	"$(INTDIR)\LkNode.obj" \
	"$(INTDIR)\LkSegmCNF.obj" \
	"$(INTDIR)\LkVector.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Ri.obj" \
	"$(INTDIR)\RiCNF.obj" \
	"$(INTDIR)\RiCNF_B.obj" \
	"$(INTDIR)\RiData.obj" \
	"$(INTDIR)\RiDataSourceDlg.obj" \
	"$(INTDIR)\RiDoc.obj" \
	"$(INTDIR)\RiKB.obj" \
	"$(INTDIR)\RiListView.obj" \
	"$(INTDIR)\RiNode.obj" \
	"$(INTDIR)\RiOptAnalysisDlg.obj" \
	"$(INTDIR)\RiOptInductionDlg.obj" \
	"$(INTDIR)\RiProcessAnalysisDlg.obj" \
	"$(INTDIR)\RiProcessInductionDlg.obj" \
	"$(INTDIR)\RiRules.obj" \
	"$(INTDIR)\RiSpace.obj" \
	"$(INTDIR)\RiTreeView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Chelovek.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Chelovek - Win32 Release" || "$(CFG)" ==\
 "Chelovek - Win32 Debug"
SOURCE=.\RiKB\Ri.cpp
DEP_CPP_RI_CP=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RI_CP=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\Ri.obj" : $(SOURCE) $(DEP_CPP_RI_CP) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiCNF.cpp
DEP_CPP_RICNF=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RICNF=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiCNF.obj" : $(SOURCE) $(DEP_CPP_RICNF) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiCNF_B.cpp
DEP_CPP_RICNF_=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RICNF_=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiCNF_B.obj" : $(SOURCE) $(DEP_CPP_RICNF_) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiData.cpp
DEP_CPP_RIDAT=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RIDAT=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiData.obj" : $(SOURCE) $(DEP_CPP_RIDAT) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiKB.cpp
DEP_CPP_RIKB_=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RIKB_=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiKB.obj" : $(SOURCE) $(DEP_CPP_RIKB_) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiNode.cpp
DEP_CPP_RINOD=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RINOD=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiNode.obj" : $(SOURCE) $(DEP_CPP_RINOD) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiRules.cpp
DEP_CPP_RIRUL=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RIRUL=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiRules.obj" : $(SOURCE) $(DEP_CPP_RIRUL) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RiKB\RiSpace.cpp
DEP_CPP_RISPA=\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_RISPA=\
	".\RiKB\stdafx.h"\
	

"$(INTDIR)\RiSpace.obj" : $(SOURCE) $(DEP_CPP_RISPA) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Chelovek.cpp
DEP_CPP_CHELO=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\MainFrm.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiListView.h"\
	".\RiTreeView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Chelovek.obj" : $(SOURCE) $(DEP_CPP_CHELO) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\hlp\Chelovek.hpj
USERDEP__CHELOV="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	

!IF  "$(CFG)" == "Chelovek - Win32 Release"

OutDir=.\Release
ProjDir=.
TargetName=Chelovek
InputPath=.\hlp\Chelovek.hpj

"$(OutDir)\$(TargetName).hlp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"\
 $(USERDEP__CHELOV)
	call "$(ProjDir)\makehelp.bat"

!ELSEIF  "$(CFG)" == "Chelovek - Win32 Debug"

OutDir=.\Debug
ProjDir=.
TargetName=Chelovek
InputPath=.\hlp\Chelovek.hpj

"$(OutDir)\$(TargetName).hlp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"\
 $(USERDEP__CHELOV)
	call "$(ProjDir)\makehelp.bat"

!ENDIF 

SOURCE=.\Chelovek.rc
DEP_RSC_CHELOVE=\
	".\res\Chelovek.ico"\
	".\res\Chelovek.rc2"\
	".\res\ChelovekDoc.ico"\
	".\res\COMP0.bmp"\
	".\res\COMP1.bmp"\
	".\res\COMP10.bmp"\
	".\res\COMP11.bmp"\
	".\res\COMP12.bmp"\
	".\res\COMP13.bmp"\
	".\res\COMP14.bmp"\
	".\res\COMP15.bmp"\
	".\res\COMP16.bmp"\
	".\res\COMP2.bmp"\
	".\res\COMP3.bmp"\
	".\res\COMP4.bmp"\
	".\res\COMP5.bmp"\
	".\res\COMP6.bmp"\
	".\res\COMP7.bmp"\
	".\res\COMP8.bmp"\
	".\res\COMP9.bmp"\
	".\res\DS.bmp"\
	".\res\DS_SEL.bmp"\
	".\res\IF.bmp"\
	".\res\IF_SEL.bmp"\
	".\res\KB.bmp"\
	".\res\KB_SEL.bmp"\
	".\res\RULE.bmp"\
	".\res\RULE_SEL.bmp"\
	".\res\SEM.bmp"\
	".\res\SEM_SEL.bmp"\
	".\res\SYN.bmp"\
	".\res\SYN_SEL.bmp"\
	".\res\THEN.bmp"\
	".\res\THEN_SEL.bmp"\
	".\res\Toolbar.bmp"\
	".\res\VAL.bmp"\
	".\res\VAL_SEL.bmp"\
	".\res\VAR.bmp"\
	".\res\VAR_SEL.bmp"\
	

"$(INTDIR)\Chelovek.res" : $(SOURCE) $(DEP_RSC_CHELOVE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\Chelovek.h"\
	".\MainFrm.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiListView.h"\
	".\RiTreeView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiDataSourceDlg.cpp
DEP_CPP_RIDATA=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDataSourceDlg.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiDataSourceDlg.obj" : $(SOURCE) $(DEP_CPP_RIDATA) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiDoc.cpp
DEP_CPP_RIDOC=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDataSourceDlg.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiProcessAnalysisDlg.h"\
	".\RiProcessInductionDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiDoc.obj" : $(SOURCE) $(DEP_CPP_RIDOC) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiListView.cpp
DEP_CPP_RILIS=\
	".\Chelovek.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiListView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiListView.obj" : $(SOURCE) $(DEP_CPP_RILIS) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiOptAnalysisDlg.cpp
DEP_CPP_RIOPT=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiOptAnalysisDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiOptAnalysisDlg.obj" : $(SOURCE) $(DEP_CPP_RIOPT) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiOptInductionDlg.cpp
DEP_CPP_RIOPTI=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiOptInductionDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiOptInductionDlg.obj" : $(SOURCE) $(DEP_CPP_RIOPTI) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiProcessAnalysisDlg.cpp
DEP_CPP_RIPRO=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiOptAnalysisDlg.h"\
	".\RiProcessAnalysisDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiProcessAnalysisDlg.obj" : $(SOURCE) $(DEP_CPP_RIPRO) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiProcessInductionDlg.cpp
DEP_CPP_RIPROC=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiOptInductionDlg.h"\
	".\RiProcessInductionDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiProcessInductionDlg.obj" : $(SOURCE) $(DEP_CPP_RIPROC) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\RiTreeView.cpp
DEP_CPP_RITRE=\
	".\Chelovek.h"\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiDoc.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	".\RiTreeView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RiTreeView.obj" : $(SOURCE) $(DEP_CPP_RITRE) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Chelovek - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /Fp"$(INTDIR)\Chelovek.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Chelovek.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Chelovek - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\Chelovek.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Chelovek.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Lk\LkCrispCNF.cpp
DEP_CPP_LKCRI=\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	
NODEP_CPP_LKCRI=\
	".\Lk\stdafx.h"\
	

"$(INTDIR)\LkCrispCNF.obj" : $(SOURCE) $(DEP_CPP_LKCRI) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Lk\LkFuzzyCNF.cpp
DEP_CPP_LKFUZ=\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	
NODEP_CPP_LKFUZ=\
	".\Lk\stdafx.h"\
	

"$(INTDIR)\LkFuzzyCNF.obj" : $(SOURCE) $(DEP_CPP_LKFUZ) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Lk\LkNode.cpp
DEP_CPP_LKNOD=\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	
NODEP_CPP_LKNOD=\
	".\Lk\stdafx.h"\
	

"$(INTDIR)\LkNode.obj" : $(SOURCE) $(DEP_CPP_LKNOD) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Lk\LkSegmCNF.cpp
DEP_CPP_LKSEG=\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	
NODEP_CPP_LKSEG=\
	".\Lk\stdafx.h"\
	

"$(INTDIR)\LkSegmCNF.obj" : $(SOURCE) $(DEP_CPP_LKSEG) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Lk\LkVector.cpp
DEP_CPP_LKVEC=\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	
NODEP_CPP_LKVEC=\
	".\Lk\stdafx.h"\
	

"$(INTDIR)\LkVector.obj" : $(SOURCE) $(DEP_CPP_LKVEC) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Kb\KbDataSourceCSV.cpp
DEP_CPP_KBDAT=\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_KBDAT=\
	".\Kb\stdafx.h"\
	

"$(INTDIR)\KbDataSourceCSV.obj" : $(SOURCE) $(DEP_CPP_KBDAT) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Kb\KbKB.cpp
DEP_CPP_KBKB_=\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_KBKB_=\
	".\Kb\stdafx.h"\
	

"$(INTDIR)\KbKB.obj" : $(SOURCE) $(DEP_CPP_KBKB_) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Kb\KbSpace.cpp
DEP_CPP_KBSPA=\
	".\Kb\Kb.h"\
	".\Kb\KbDataSourceCSV.h"\
	".\Kb\KbKB.h"\
	".\Lk\Lk.h"\
	".\Lk\LkCrispCNF.h"\
	".\Lk\LkFuzzyCNF.h"\
	".\Lk\LkISTreeAssocLog.h"\
	".\Lk\LkNode.h"\
	".\Lk\LkSegmCNF.h"\
	".\Lk\LkVector.h"\
	".\RiKB\Ri.h"\
	".\RiKB\RiCNF.h"\
	".\RiKB\RiData.h"\
	".\RiKB\RiKB.h"\
	".\RiKB\RiNode.h"\
	".\RiKB\RiRules.h"\
	".\RiKB\RiSpace.h"\
	".\RiKB\RiTypes.h"\
	
NODEP_CPP_KBSPA=\
	".\Kb\stdafx.h"\
	

"$(INTDIR)\KbSpace.obj" : $(SOURCE) $(DEP_CPP_KBSPA) "$(INTDIR)"\
 "$(INTDIR)\Chelovek.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

