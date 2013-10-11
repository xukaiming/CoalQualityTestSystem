// GyfxyHostDoc.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyHostDoc.h"
#include "GyfxyRDB.h"

#include "GyfxyBlanceView.h"
// CGyfxyHostDoc

IMPLEMENT_DYNCREATE(CGyfxyHostDoc, CIEDDocument)

CGyfxyHostDoc::CGyfxyHostDoc()
{
	pBalanceWnd = pDebugWnd = NULL;
}

BOOL CGyfxyHostDoc::OnNewDocument()
{
	pRDB = &GyfxyRDB; 
	if (!CIEDDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CGyfxyHostDoc::~CGyfxyHostDoc()
{
}


BEGIN_MESSAGE_MAP(CGyfxyHostDoc, CIEDDocument)
	ON_COMMAND(ID_DEBUG, &CGyfxyHostDoc::OnDebug)
	ON_COMMAND(ID_BALANCE, &CGyfxyHostDoc::OnBalance)
END_MESSAGE_MAP()


// CGyfxyHostDoc diagnostics

#ifdef _DEBUG
void CGyfxyHostDoc::AssertValid() const
{
	CIEDDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CGyfxyHostDoc::Dump(CDumpContext& dc) const
{
	CIEDDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CGyfxyHostDoc serialization

void CGyfxyHostDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// //virtual GetHostParam(CString sDeviceName,CString sDeviceType);

BOOL CGyfxyHostDoc::GetHostParam(CString sDeviceName , CString sDeviceType)
{
	ASSERT(GyfxyRDB.IsKindOf(RUNTIME_CLASS(CGyfxyRDB_G5200))); 
	return GyfxyRDB.LoadParamFromDB(sDeviceName,sDeviceType);  
}

void CGyfxyHostDoc::OnDebug()
{ 
	if((pDebugWnd==NULL)||(pDebugWnd!=NULL&&!IsWindow(pDebugWnd->m_hWnd)))
	{	
		CDocTemplate *pDocTemplate = new CMultiDocTemplate(
			IDR_GYFXYHOSTVIEW_TMPL,
			RUNTIME_CLASS(CGyfxyHostDoc),		// document class
			RUNTIME_CLASS(CChildFrame),			// frame class MID fixsize
			RUNTIME_CLASS(CGyfxyDebugView));		// view class
		AfxGetApp()->AddDocTemplate(pDocTemplate); 
		pDebugWnd  =  pDocTemplate->CreateNewFrame(this,NULL); 
		pDebugWnd->InitialUpdateFrame(this,TRUE);

	}
	else
	{ 
		((CMDIFrameWnd *)AfxGetMainWnd())->MDIActivate(pDebugWnd);
	}
}

void CGyfxyHostDoc::OnBalance()
{
	// BugList:为什么有时候天平实时弹出不正常捏?
	
	if((pBalanceWnd==NULL)||(pBalanceWnd!=NULL&&!IsWindow(pBalanceWnd->m_hWnd)))
	{	
		CDocTemplate *pDocTemplate = new CMultiDocTemplate(
			IDR_GYFXYHOSTVIEW_TMPL,
			RUNTIME_CLASS(CGyfxyHostDoc),		// document class
			RUNTIME_CLASS(CChildFrame),			// frame class MID fixsize
			RUNTIME_CLASS(CGyfxyBlanceView));		// view class
		AfxGetApp()->AddDocTemplate(pDocTemplate); 
		pBalanceWnd  =  pDocTemplate->CreateNewFrame(this,NULL); 
		pBalanceWnd->InitialUpdateFrame(this,TRUE);
		}
	else
	{ 
		((CMDIFrameWnd *)AfxGetMainWnd())->MDIActivate(pBalanceWnd);
	}
}
