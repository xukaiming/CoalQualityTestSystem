// CLyHostDoc.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "CLyHostDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ChildFrm.h"
#include "ClyDebugView.h"
/////////////////////////////////////////////////////////////////////////////
// CClyHostDoc

IMPLEMENT_DYNCREATE(CClyHostDoc, CIEDDocument)

CClyHostDoc::CClyHostDoc()
{
	pDebugWnd = NULL;
}

BOOL CClyHostDoc::OnNewDocument()
{
	pRDB = &ClyRDB;
	if (!CIEDDocument::OnNewDocument())
		return FALSE;	
	return TRUE;
}

CClyHostDoc::~CClyHostDoc()
{
}


BEGIN_MESSAGE_MAP(CClyHostDoc, CIEDDocument)
	//{{AFX_MSG_MAP(CClyHostDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DEBUG, &CClyHostDoc::OnDebug)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClyHostDoc diagnostics

#ifdef _DEBUG
void CClyHostDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClyHostDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClyHostDoc serialization

void CClyHostDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{ 
	}
	else
	{ 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CClyHostDoc commands

BOOL CClyHostDoc::GetHostParam( CString sDeviceName, CString sDeviceType)
{ 
 	ASSERT(ClyRDB.IsKindOf(RUNTIME_CLASS(CClyRDB)));
	
	BOOL bResult = TRUE;
	CString strSQL;		  
	return ClyRDB.LoadParamFromDB(sDeviceName,sDeviceType);  
}

void CClyHostDoc::OnDebug()
{ 
	if((pDebugWnd==NULL)||(pDebugWnd!=NULL&&!IsWindow(pDebugWnd->m_hWnd)))
	{	
		CDocTemplate *pDocTemplate = new CMultiDocTemplate(
			IDR_CLYHOSTVIEW_TMPL,
			RUNTIME_CLASS(CClyHostDoc),		// document class
			RUNTIME_CLASS(CChildFrame),		// frame class MID fixsize
			RUNTIME_CLASS(CClyDebugView));		// view class
		AfxGetApp()->AddDocTemplate(pDocTemplate);  //没有这个,退出时会报告资源泄露
		//不添加为私有视图
		pDebugWnd  =  pDocTemplate->CreateNewFrame(this,NULL); 
		pDebugWnd->InitialUpdateFrame(this,TRUE);
		
	}

}
