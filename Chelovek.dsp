# Microsoft Developer Studio Project File - Name="Chelovek" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Chelovek - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Chelovek.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Chelovek.mak" CFG="Chelovek - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Chelovek - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Chelovek - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Chelovek - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x418 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Chelovek - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x418 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Chelovek - Win32 Release"
# Name "Chelovek - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "RiKB Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RiKB\Ri.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiCNF.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiCNF_B.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiData.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiKB.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiNode.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiRules.cpp
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiSpace.cpp
# End Source File
# End Group
# Begin Group "Chelovek Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Chelovek.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\Chelovek.hpj
USERDEP__CHELO="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	

!IF  "$(CFG)" == "Chelovek - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=Chelovek
InputPath=.\hlp\Chelovek.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "Chelovek - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=Chelovek
InputPath=.\hlp\Chelovek.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Chelovek.rc
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RiDataSourceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RiDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RiListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RiOptAnalysisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RiOptInductionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RiProcessAnalysisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RiProcessInductionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RiTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Lk Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Lk\LkCrispCNF.cpp
# End Source File
# Begin Source File

SOURCE=.\Lk\LkFuzzyCNF.cpp
# End Source File
# Begin Source File

SOURCE=.\Lk\LkNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Lk\LkSegmCNF.cpp
# End Source File
# Begin Source File

SOURCE=.\Lk\LkVector.cpp
# End Source File
# End Group
# Begin Group "Kb Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Kb\KbDataSourceCSV.cpp
# End Source File
# Begin Source File

SOURCE=.\Kb\KbKB.cpp
# End Source File
# Begin Source File

SOURCE=.\Kb\KbSpace.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "RiKB Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RiKB\Ri.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiCNF.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiData.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiKB.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiNode.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiRules.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiSpace.h
# End Source File
# Begin Source File

SOURCE=.\RiKB\RiTypes.h
# End Source File
# End Group
# Begin Group "Chelovek Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Chelovek.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RiDataSourceDlg.h
# End Source File
# Begin Source File

SOURCE=.\RiDoc.h
# End Source File
# Begin Source File

SOURCE=.\RiListView.h
# End Source File
# Begin Source File

SOURCE=.\RiOptAnalysisDlg.h
# End Source File
# Begin Source File

SOURCE=.\RiOptInductionDlg.h
# End Source File
# Begin Source File

SOURCE=.\RiProcessAnalysisDlg.h
# End Source File
# Begin Source File

SOURCE=.\RiProcessInductionDlg.h
# End Source File
# Begin Source File

SOURCE=.\RiTreeView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Lk Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Lk\Lk.h
# End Source File
# Begin Source File

SOURCE=.\Lk\LkCrispCNF.h
# End Source File
# Begin Source File

SOURCE=.\Lk\LkFuzzyCNF.h
# End Source File
# Begin Source File

SOURCE=.\Lk\LkNode.h
# End Source File
# Begin Source File

SOURCE=.\Lk\LkSegmCNF.h
# End Source File
# Begin Source File

SOURCE=.\Lk\LkVector.h
# End Source File
# End Group
# Begin Group "Kb Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Kb\Kb.h
# End Source File
# Begin Source File

SOURCE=.\Kb\KbDataSourceCSV.h
# End Source File
# Begin Source File

SOURCE=.\Kb\KbKB.h
# End Source File
# Begin Source File

SOURCE=.\Kb\KbSpace.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Chelovek.ico
# End Source File
# Begin Source File

SOURCE=.\res\Chelovek.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ChelovekDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\COMP0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP13.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP15.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COMP9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DS_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IF_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\KB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\KB_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RULE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RULE_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SEM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SEM_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SYN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SYN_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\THEN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\THEN_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VAL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VAL_SEL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VAR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VAR_SEL.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Chelovek.cnt
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# End Group
# Begin Group "Object Files"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
