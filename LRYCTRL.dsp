# Microsoft Developer Studio Project File - Name="LRYCTRL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LRYCTRL - Win32 UNICODE Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LRYCTRL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LRYCTRL.mak" CFG="LRYCTRL - Win32 UNICODE Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LRYCTRL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LRYCTRL - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "LRYCTRL - Win32 UNICODE Debug" (based on "Win32 (x86) Application")
!MESSAGE "LRYCTRL - Win32 UNICODE Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LRYCTRL - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /libpath:"Lib"

!ELSEIF  "$(CFG)" == "LRYCTRL - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "Include" /I "PropertyGrid" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"GDIPlus"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "LRYCTRL - Win32 UNICODE Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UNICODE Debug"
# PROP BASE Intermediate_Dir "UNICODE Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UniDebug"
# PROP Intermediate_Dir "UniDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_AFXDLL" /FAcs /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"Lib"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /libpath:"Lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "LRYCTRL - Win32 UNICODE Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LRYCTRL___Win32_UNICODE_Release"
# PROP BASE Intermediate_Dir "LRYCTRL___Win32_UNICODE_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UniRelease"
# PROP Intermediate_Dir "UniRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_AFXDLL" /FAcs /FR /Yu"stdafx.h" /FD /Zm100 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /libpath:"LIB"
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /machine:I386 /libpath:"Lib"
# SUBTRACT LINK32 /debug /nodefaultlib

!ENDIF 

# Begin Target

# Name "LRYCTRL - Win32 Release"
# Name "LRYCTRL - Win32 Debug"
# Name "LRYCTRL - Win32 UNICODE Debug"
# Name "LRYCTRL - Win32 UNICODE Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CalGaoDiQDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClyChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ClyDebugView.cpp
# End Source File
# Begin Source File

SOURCE=.\CLYHostCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CLyHostDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ClyHostView.cpp
# End Source File
# Begin Source File

SOURCE=.\ClyRDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ComPort.cpp
# End Source File
# Begin Source File

SOURCE=.\CReCalQ.cpp
# End Source File
# Begin Source File

SOURCE=.\DeivceViewPane.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HostCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\IEDDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\LRYCTRL.cpp
# End Source File
# Begin Source File

SOURCE=.\LRYCTRL.rc
# End Source File
# Begin Source File

SOURCE=.\LryEDBQryDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LryEDBQryView.cpp
# End Source File
# Begin Source File

SOURCE=.\LryHostCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LryHostDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LryHostView.cpp
# End Source File
# Begin Source File

SOURCE=.\LryQDBQryDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LryQDBQryView.cpp
# End Source File
# Begin Source File

SOURCE=.\LRYRDB.CPP
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ModBusServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyWeight.cpp
# End Source File
# Begin Source File

SOURCE=.\PageCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\PageOut.cpp
# End Source File
# Begin Source File

SOURCE=.\PageParam.cpp
# End Source File
# Begin Source File

SOURCE=.\RDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportSele.cpp
# End Source File
# Begin Source File

SOURCE=.\rmband.cpp
# End Source File
# Begin Source File

SOURCE=.\rmcellpage.cpp
# End Source File
# Begin Source File

SOURCE=.\rmdialogpage.cpp
# End Source File
# Begin Source File

SOURCE=.\rmgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\rmgridpage.cpp
# End Source File
# Begin Source File

SOURCE=.\rmparser.cpp
# End Source File
# Begin Source File

SOURCE=.\rmpreviewoptions.cpp
# End Source File
# Begin Source File

SOURCE=.\rmreportengine.cpp
# End Source File
# Begin Source File

SOURCE=.\rmreportpage.cpp
# End Source File
# Begin Source File

SOURCE=.\rmsavereportoptions.cpp
# End Source File
# Begin Source File

SOURCE=.\rmview.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserLogon.cpp
# End Source File
# Begin Source File

SOURCE=.\XSplitterWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CalGaoDiQDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClyChildWnd.h
# End Source File
# Begin Source File

SOURCE=.\ClyDebugView.h
# End Source File
# Begin Source File

SOURCE=.\CLYHostCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CLyHostDoc.h
# End Source File
# Begin Source File

SOURCE=.\ClyHostView.h
# End Source File
# Begin Source File

SOURCE=.\ClyRDB.h
# End Source File
# Begin Source File

SOURCE=.\ComPort.h
# End Source File
# Begin Source File

SOURCE=.\CReCalQ.h
# End Source File
# Begin Source File

SOURCE=.\DeivceViewPane.h
# End Source File
# Begin Source File

SOURCE=.\DeviceSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\HostCtrl.h
# End Source File
# Begin Source File

SOURCE=.\IEDDocument.h
# End Source File
# Begin Source File

SOURCE=.\PropertyGrid\Includes.h
# End Source File
# Begin Source File

SOURCE=.\LRYCTRL.h
# End Source File
# Begin Source File

SOURCE=.\LryEDBQryDoc.h
# End Source File
# Begin Source File

SOURCE=.\LryEDBQryView.h
# End Source File
# Begin Source File

SOURCE=.\LryHostCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LryHostDoc.h
# End Source File
# Begin Source File

SOURCE=.\LryHostView.h
# End Source File
# Begin Source File

SOURCE=.\LryImageSlave.H
# End Source File
# Begin Source File

SOURCE=.\LryQDBQryDoc.h
# End Source File
# Begin Source File

SOURCE=.\LryQDBQryView.h
# End Source File
# Begin Source File

SOURCE=.\LRYRDB.H
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ModBusServer.h
# End Source File
# Begin Source File

SOURCE=.\ModifyWeight.h
# End Source File
# Begin Source File

SOURCE=.\PageCurve.h
# End Source File
# Begin Source File

SOURCE=.\PageDebug.h
# End Source File
# Begin Source File

SOURCE=.\PageOut.h
# End Source File
# Begin Source File

SOURCE=.\PageParam.h
# End Source File
# Begin Source File

SOURCE=.\RDB.h
# End Source File
# Begin Source File

SOURCE=.\ReportSele.h
# End Source File
# Begin Source File

SOURCE=.\PropertyGrid\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rmband.h
# End Source File
# Begin Source File

SOURCE=.\rmcellpage.h
# End Source File
# Begin Source File

SOURCE=.\rmdialogpage.h
# End Source File
# Begin Source File

SOURCE=.\rmgrid.h
# End Source File
# Begin Source File

SOURCE=.\rmgridpage.h
# End Source File
# Begin Source File

SOURCE=.\rmparser.h
# End Source File
# Begin Source File

SOURCE=.\rmpreviewoptions.h
# End Source File
# Begin Source File

SOURCE=.\rmreportengine.h
# End Source File
# Begin Source File

SOURCE=.\rmreportpage.h
# End Source File
# Begin Source File

SOURCE=.\rmsavereportoptions.h
# End Source File
# Begin Source File

SOURCE=.\rmview.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Template.H
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\UserInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserLogon.h
# End Source File
# Begin Source File

SOURCE=.\XSplitterWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\adown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Attr.ico
# End Source File
# Begin Source File

SOURCE=.\res\aup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bleft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bright.bmp
# End Source File
# Begin Source File

SOURCE=.\CLY.ico
# End Source File
# Begin Source File

SOURCE=.\res\CLY.ico
# End Source File
# Begin Source File

SOURCE=.\res\CURVE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\db.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Debug.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_lrye.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_lryh.ico
# End Source File
# Begin Source File

SOURCE=.\res\il_class.bmp
# End Source File
# Begin Source File

SOURCE=.\res\il_tab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\imagelis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LRYCTRL.ico
# End Source File
# Begin Source File

SOURCE=.\res\LRYCTRL.rc2
# End Source File
# Begin Source File

SOURCE=.\res\LRYCTRLDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\mdi.ico
# End Source File
# Begin Source File

SOURCE=.\res\OUTPUT.ICO
# End Source File
# Begin Source File

SOURCE=.\res\PARRM.ICO
# End Source File
# Begin Source File

SOURCE=.\sdi.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\TraceWin.h
# End Source File
# Begin Source File

SOURCE=".\res\Î´ÃüÃû.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\LRYCTRL.EXE.MANIFEST
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section LRYCTRL : {9288EA99-2CE2-4323-9808-508B5DA0A065}
# 	2:5:Class:CRMGrid
# 	2:10:HeaderFile:rmgrid.h
# 	2:8:ImplFile:rmgrid.cpp
# End Section
# Section LRYCTRL : {2BAA4731-39BD-420D-A2DD-A6BBD843883A}
# 	2:5:Class:CRMPreviewOptions
# 	2:10:HeaderFile:rmpreviewoptions.h
# 	2:8:ImplFile:rmpreviewoptions.cpp
# End Section
# Section LRYCTRL : {2389756D-6FED-4E2F-9534-29DEFAA2A81D}
# 	2:5:Class:C_S7_Modbus_Server
# 	2:10:HeaderFile:_s7_modbus_server.h
# 	2:8:ImplFile:_s7_modbus_server.cpp
# End Section
# Section LRYCTRL : {E6338A1A-ED45-4BA0-942A-7373FD7821EF}
# 	2:5:Class:CRMDialogPage
# 	2:10:HeaderFile:rmdialogpage.h
# 	2:8:ImplFile:rmdialogpage.cpp
# End Section
# Section LRYCTRL : {5105F934-C228-42A0-9B68-F00589E7AA09}
# 	2:5:Class:CRMSaveReportOptions
# 	2:10:HeaderFile:rmsavereportoptions.h
# 	2:8:ImplFile:rmsavereportoptions.cpp
# End Section
# Section LRYCTRL : {030650B4-9016-4268-A823-E4C8E9A59261}
# 	2:21:DefaultSinkHeaderFile:rmreportengine.h
# 	2:16:DefaultSinkClass:CRMReportEngine
# End Section
# Section LRYCTRL : {36291A25-F170-46C0-91EE-3FD968E7E5E1}
# 	2:21:DefaultSinkHeaderFile:_s7_modbus_server.h
# 	2:16:DefaultSinkClass:C_S7_Modbus_Server
# End Section
# Section LRYCTRL : {51EA12D0-EE4C-4503-97DC-15981DCA77C0}
# 	2:5:Class:CRMParser
# 	2:10:HeaderFile:rmparser.h
# 	2:8:ImplFile:rmparser.cpp
# End Section
# Section LRYCTRL : {4DB2C8C3-EF5C-4BB1-BAE8-0F223D648821}
# 	2:5:Class:CRMGridPage
# 	2:10:HeaderFile:rmgridpage.h
# 	2:8:ImplFile:rmgridpage.cpp
# End Section
# Section LRYCTRL : {8CDBE5E8-F166-429C-A8E8-FEE89E0E2D20}
# 	2:5:Class:CRMReportEngine
# 	2:10:HeaderFile:rmreportengine.h
# 	2:8:ImplFile:rmreportengine.cpp
# End Section
# Section LRYCTRL : {597C02BE-0461-41B6-9E52-C77768DED396}
# 	2:5:Class:CRMBand
# 	2:10:HeaderFile:rmband.h
# 	2:8:ImplFile:rmband.cpp
# End Section
# Section LRYCTRL : {8CA6A60A-99F5-4564-AA90-9AF339BC0DEA}
# 	2:5:Class:CRMCellPage
# 	2:10:HeaderFile:rmcellpage.h
# 	2:8:ImplFile:rmcellpage.cpp
# End Section
# Section LRYCTRL : {435EDE67-EDBB-4F82-87C7-B6C990DFFA3E}
# 	2:5:Class:CRMView
# 	2:10:HeaderFile:rmview.h
# 	2:8:ImplFile:rmview.cpp
# End Section
# Section LRYCTRL : {87A6AFD1-4598-48CB-B2AC-F2F0EFCB8860}
# 	2:5:Class:CRMReportPage
# 	2:10:HeaderFile:rmreportpage.h
# 	2:8:ImplFile:rmreportpage.cpp
# End Section
