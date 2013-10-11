// DeivceViewPane.cpp: implementation of the CDeivceViewPane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "DeivceViewPane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeivceViewPane::CDeivceViewPane()
{

}

CDeivceViewPane::~CDeivceViewPane()
{

}

BEGIN_MESSAGE_MAP(CDeivceViewPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()  
	ON_NOTIFY(NM_DBLCLK, 0xFF, OnDblclk)
	ON_UPDATE_COMMAND_UI(ID_CORR_FACT,OnCorrFactUI)
	ON_MESSAGE(WM_PARAMETE_CHANGED,OnParmChanged)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CDeivceViewPane::AssertValid() const
{
	CWnd::AssertValid();
}

void CDeivceViewPane::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif //_DEBUG


int CDeivceViewPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////////////////////////////////////
	try
	{ 
		HRESULT hr; 
		hr = Rs.CreateInstance("ADODB.Recordset");
		TESTHR(hr);	
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	///////////////////////////////////////////////////////
	if (m_wndClassView.GetSafeHwnd() == 0)
	{
		if (!m_wndClassView.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CRect(0,0,0,0), this, 0xFF ))
		{
			TRACE(_T("Unable to create tree control.\n" ));
			return 0;
		}
		m_wndClassView.ModifyStyleEx(0, WS_EX_STATICEDGE); 
		if (!CreateImageList(m_ImageList, IDB_IL_TAB))
			return 0;		
		// Set the image list for the tree control.
		m_wndClassView.SetImageList( &m_ImageList, TVSIL_NORMAL );	 
		// Add the parent item
		LoadDevice();
		//////////////////////////////////////////////////////////// 
		CFont *pFont =	m_wndClassView.GetFont(); 
		CFont pFont2;
		LOGFONT		LogFont;
		//ZeroMemory(&LogFont,sizeof(LOGFONT)); 
		pFont->GetLogFont(&LogFont); 
		LogFont.lfHeight  *=1.5f; 
		pFont2.CreateFontIndirect(&LogFont);  

		m_wndClassView.SetFont(&pFont2,TRUE);  
		/////////////////////////////////////////////////////////////
		
	}
	return 0;
}


void CDeivceViewPane::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTreeCtrl *TreeCtrl; 
	HTREEITEM	   hItem ;
	HTREEITEM	   hParenItem ;   
	hItem      = m_wndClassView.GetSelectedItem();
	hParenItem = m_wndClassView.GetParentItem(hItem);
	if(hParenItem!=NULL)
		::SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_COMMAND,ID_TEST,NULL);
 	*pResult = 0;
}

void CDeivceViewPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);	
	int nTop = 0;	
	if (m_wndClassView.GetSafeHwnd())
	{
		m_wndClassView.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndClassView.Invalidate(FALSE);
	}
}

void CDeivceViewPane::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndClassView.SetFocus();
}

void CDeivceViewPane::AddDev(const HTREEITEM &ParenthtItem, const CString &str)
{
	CTreeCtrl* m_TreeCtrl;
	
	CString strDev;
	try
	{
		CString strSQL;
		strSQL.Format(_T("select * from %s"),str);
		Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockReadOnly, -1);
	}
	catch(_com_error &e)
	{
		TRACE(_T("Error AddDev"));
		dump_com_error(e);
		return ;
	}
	int i = Rs->RecordCount;
	m_TreeCtrl = (CTreeCtrl*)&m_wndClassView;
	while(!Rs->adoEOF)//
	{
		HTREEITEM htItem;
		_variant_t value; 
		//value = Rs->Fields->GetItem((short)0)->Value;
		//value = Rs->GetCollect("名称");
		value = Rs->Fields->GetItem("名称")->Value;
		strDev = VariantToStr(value);
		//AfxMessageBox(strDev);
		htItem = m_TreeCtrl->InsertItem(strDev,1,2,ParenthtItem,TVI_LAST);
		m_TreeCtrl->SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );
		Rs->MoveNext();
		
	} 
	if(Rs->State==adStateOpen)
		Rs->Close();
}

void CDeivceViewPane::LoadDevice()
{
	int i;
	CString strDevName[3];
	CTreeCtrl* m_TreeCtrl;
	HTREEITEM htItem1;
	
	strDevName[0].LoadString(ID_STR_DEV_LRY);
	strDevName[1].LoadString(ID_STR_DEV_CLY);
	strDevName[2].LoadString(ID_STR_DEV_GYFXY);
	//strDevName[0].LoadString(ID_STR_DEV_HRD);
	
	m_TreeCtrl = (CTreeCtrl*)&m_wndClassView;
	m_TreeCtrl->DeleteAllItems();
	// Create the image list for the tree control 
	
	m_TreeCtrl->SetImageList (&m_ImageList, TVSIL_NORMAL);
	// Add the parent item
	for(i=0;i<sizeof(strDevName)/sizeof(CString);i++)
	{ 
		htItem1 = m_TreeCtrl->InsertItem(strDevName[i]);	//量热仪
		m_TreeCtrl->SetItemState( htItem1, TVIS_BOLD, TVIS_BOLD );  
		AddDev(htItem1,strDevName[i]);
		m_TreeCtrl->Expand(htItem1, TVE_EXPAND); 
	} 
}


void CDeivceViewPane::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_POP_DEV);
	
	CMenu *pSubMenu;
	pSubMenu = Menu.GetSubMenu(0);
	
	CRect rect;
	CPoint pt;
	GetClientRect(&rect);
	pt = point;
	ScreenToClient(&pt);
	
	CWnd *pOwner = this;
	while(pOwner->GetStyle()&WS_CHILD)
	{
		pOwner = pOwner->GetParent();
	}
	
	
	if(PtInRect(rect,pt))
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pOwner); 		
	
}


void CDeivceViewPane::OnCorrFactUI(CCmdUI* pCmdUI)
{
	CString strDevCly;
	strDevCly.LoadString(ID_STR_DEV_CLY);
	if(GetCurrParentSeleString()==strDevCly)
		pCmdUI->Enable( TRUE ); 
	else
		pCmdUI->Enable( FALSE ); 
}

LRESULT  CDeivceViewPane::OnParmChanged(WPARAM wParam, LPARAM lParam)
{
	LoadDevice();
	return 0;
}

