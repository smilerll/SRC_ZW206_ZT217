# Microsoft Developer Studio Project File - Name="ResOver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ResOver - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ResOver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ResOver.mak" CFG="ResOver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ResOver - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ResOver - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ResOver"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ResOver - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /I "../Include" /I "D:\Workspace\GDIPlus\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 SciDllProducer.lib BZP.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"../../../../Release/ResOver/Bin/ResOver.exe" /libpath:"../../../../Common/Lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=Resover.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ResOver - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SciDllProducer.lib BZP.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../../../../Release/ResOver/Bin/ResOver.exe" /pdbtype:sept /libpath:"../../../../Common/Lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=ResoverD.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ResOver - Win32 Release"
# Name "ResOver - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BasicExcelVC6.cpp

!IF  "$(CFG)" == "ResOver - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "ResOver - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BMParser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBkmk.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNtac.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReport.cpp
# End Source File
# Begin Source File

SOURCE=.\etapp.cpp
# End Source File
# Begin Source File

SOURCE=.\excel9.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgView.cpp
# End Source File
# Begin Source File

SOURCE=.\NAParser.cpp
# End Source File
# Begin Source File

SOURCE=.\ResOver.cpp
# End Source File
# Begin Source File

SOURCE=.\ResOver.rc
# End Source File
# Begin Source File

SOURCE=.\ResOverDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SpreadSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SpreadSheetBE.cpp
# End Source File
# Begin Source File

SOURCE=.\SpreadSheetET.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StrSet.cpp
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BasicExcelVC6.h
# End Source File
# Begin Source File

SOURCE=.\BMParser.h
# End Source File
# Begin Source File

SOURCE=.\DlgBkmk.h
# End Source File
# Begin Source File

SOURCE=.\DlgNtac.h
# End Source File
# Begin Source File

SOURCE=.\DlgReport.h
# End Source File
# Begin Source File

SOURCE=.\etapp.h
# End Source File
# Begin Source File

SOURCE=.\excel9.h
# End Source File
# Begin Source File

SOURCE=.\ImgView.h
# End Source File
# Begin Source File

SOURCE=.\NAParser.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResOver.h
# End Source File
# Begin Source File

SOURCE=.\ResOverDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResParser.h
# End Source File
# Begin Source File

SOURCE=.\SpreadSheet.h
# End Source File
# Begin Source File

SOURCE=.\SpreadSheetBE.h
# End Source File
# Begin Source File

SOURCE=.\SpreadSheetET.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrSet.h
# End Source File
# Begin Source File

SOURCE=.\XAboutDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bk_grid.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ResOver.ico
# End Source File
# Begin Source File

SOURCE=.\res\ResOver.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\ring.png
# End Source File
# End Target
# End Project
