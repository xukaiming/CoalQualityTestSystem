// IEDDocument.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "IEDDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// CIEDDocument


IMPLEMENT_DYNCREATE(CIEDDocument, CDocument)

CIEDDocument::CIEDDocument()
{
}

BOOL CIEDDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE; 
	CString sDeviceType;
	sDeviceName = GetCurrentSeleString(sDeviceType);	
	if(!GetHostParam(sDeviceName,sDeviceType))
	{		
		::MessageBox(NULL,_T("��ȡ���ݿ�ʧ��,�豸��ʼ��ʧ��"),_T("����"),MB_OK+MB_ICONERROR);
		return FALSE;
	}
	else
	{
		SetTitle(sDeviceName);
		SetModifiedFlag(TRUE);  //�ر�ʱ�����رնԻ�  
		return TRUE;
	}
	return TRUE;
}

CIEDDocument::~CIEDDocument()
{
}


BEGIN_MESSAGE_MAP(CIEDDocument, CDocument)
	//{{AFX_MSG_MAP(CIEDDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEDDocument diagnostics

#ifdef _DEBUG
void CIEDDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CIEDDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIEDDocument serialization

void CIEDDocument::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CIEDDocument commands

BOOL CIEDDocument::GetHostParam( CString sDeviceName, CString sDeviceType)
{
	return FALSE;
}

CString CIEDDocument::GetCurrentSeleString(CString &strParent)
{
	CMainFrame * pFrame    = (CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CTreeCtrl  * pTreeCtrl = &((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_wndDeviceView.m_wndClassView; 
	HTREEITEM    hItem     = pTreeCtrl->GetSelectedItem();
	HTREEITEM    hParentItem =pTreeCtrl->GetParentItem(hItem);
	strParent = pTreeCtrl->GetItemText(hParentItem);
	return pTreeCtrl->GetItemText(hItem);
}
BOOL CIEDDocument::SaveModified() 
{
	
	CString strIEDName; 
	//ASSERT(pRDB!=(void*)0xcdcdcdcd);		//û�г�ʼ��
	strIEDName.Format(_T("ȷ��ֹͣ%sʵ�鲢�˳���"),pRDB->m_szName);
	if((!IsModified()||
		(IDOK==MessageBox(AfxGetMainWnd()->m_hWnd,strIEDName,_T("��ʾ"),MB_OKCANCEL|MB_ICONQUESTION|MB_TASKMODAL))))
	{ 
		
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CIEDDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}
