// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__79F7CB45_BB49_46DA_8BC1_0D616B9C5B1E__INCLUDED_)
#define AFX_STDAFX_H__79F7CB45_BB49_46DA_8BC1_0D616B9C5B1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(8)
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

/****************************************************************************
* support new platform ( zou xinyu)
***************************************************************************/
//#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 // so the code would compile
#endif

#define WINVER   0x0500    // 如果上面的语句不够，可以加上这两句
#undef  _WIN32_IE
#define _WIN32_IE 0x0600


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#include <errno.h>
#include <afxmt.h>
#include <locale.h>
#include <math.h>

 
//GDI+
 
#include "gdiplus.h"
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus; 

//ADO数据库
#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
   no_namespace rename("EOF", "adoEOF")
//导入    MSJRO  压缩数据库    
#import   "C:\PROGRAM FILES\COMMON FILES\System\ado\MSJRO.DLL"   no_namespace  
#import   "C:\Program Files\Common Files\System\ado\msadox.dll"
#include <XTToolkitPro.h>   // Codejock Software Components 
#include <afxcview.h>
#include <afx.h>


#ifdef TRACE
#undef TRACE
#endif
#define TRACE AfxTrace 

#endif // _AFX_NO_AFXCMN_SUPPORT

 
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
void dump_com_error(_com_error &e);
#endif // !defined(AFX_STDAFX_H__79F7CB45_BB49_46DA_8BC1_0D616B9C5B1E__INCLUDED_)
