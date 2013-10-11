// DBListView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DBListView.h"

//CCustomMenuHeaderCtrl
IMPLEMENT_DYNCREATE(CCustomMenuHeaderCtrl, CXTHeaderCtrl)
CCustomMenuHeaderCtrl::CCustomMenuHeaderCtrl()
{

}

CCustomMenuHeaderCtrl::~CCustomMenuHeaderCtrl()
{

}
BEGIN_MESSAGE_MAP(CCustomMenuHeaderCtrl, CXTHeaderCtrl)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()
// CDBListView

IMPLEMENT_DYNCREATE(CDBListView, CListView)
#ifdef _DEBUG
void CCustomMenuHeaderCtrl::AssertValid() const
{
	CXTHeaderCtrl::AssertValid();
}

#ifndef _WIN32_WCE
void CCustomMenuHeaderCtrl::Dump(CDumpContext& dc) const
{
	CXTHeaderCtrl::Dump(dc);
}
#endif
#endif //_DEBUG
//////////////////////////////////////////
CDBListView::CDBListView()
{

}

CDBListView::~CDBListView()
{
}

BEGIN_MESSAGE_MAP(CDBListView, CListView)
END_MESSAGE_MAP()


// CDBListView diagnostics

#ifdef _DEBUG
void CDBListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CDBListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBListView message handlers

void CDBListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate(); 
	CListCtrl *pListCtrl = &GetListCtrl();
	HWND hWndHeader = pListCtrl->GetHeaderCtrl()->GetSafeHwnd();
	m_HeaderCtrl.SubclassWindow(hWndHeader);    
	
	CXTHeaderCtrlTheme* pTheme = m_HeaderCtrl.GetTheme();
	if (pTheme)
	{
		DWORD dwStyle = pTheme->GetDrawStyle() & ~XTTHEME_SORTARROW; 
		dwStyle|=XTTHEME_WINXPTHEMES;	

		pTheme->SetDrawStyle(dwStyle, &m_HeaderCtrl);
		m_HeaderCtrl.RedrawWindow();
	}
	//
	m_HeaderCtrl.EnablePopupMenus(TRUE) ; 
	UpdateHeaderMenu();

	
	// TODO: Add your specialized code here and/or call the base class
}

void CDBListView::UpdateHeaderMenu(void)
{

}

void CCustomMenuHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//* */CXTHeaderCtrl::OnRButtonDown(nFlags, point);
	CPoint pt = m_pt = point;
	ClientToScreen(&pt);

	// If no sort headers are defined for the parent control or popup menus
	// has been disabled, call the base class and return.
	CWnd* pParentWnd = GetParent();
	if (pParentWnd->GetStyle() & LVS_NOSORTHEADER || m_bEnableMenus == FALSE)
	{
		CHeaderCtrl::OnRButtonDown(nFlags, point);
		return;
	}
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_HEAD)); 
	CMenu* pPopup = menu.GetSubMenu(0);

	ASSERT(pPopup != NULL);
	if (!pPopup)
		return;
	UpdateHeaderMenu();
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		pt.x, pt.y, GetOwner());
	
}

void CCustomMenuHeaderCtrl::UpdateHeaderMenu(void)
{
	HeadMenuName menuName;
	HDITEM item;
	RECT rect ;
	int ColCnt = GetItemCount();
	ZeroMemory(&item,sizeof(HDITEM));

	TCHAR  lpBuffer[MAX_PATH]=_T("");

	item.mask = HDI_TEXT;
	item.pszText = lpBuffer;
	item.cchTextMax = MAX_PATH;

	MenuArray.RemoveAll();
	for(int i=0;i<ColCnt;i++)
	{
		menuName.m_iSort = i; 
		GetItem(i,&item);
		GetItemRect(i,&rect);
		menuName.m_strColName  = item.pszText;
		menuName.bCheck = rect.right-rect.left>0?TRUE:FALSE;
		MenuArray.Add(menuName);
	}
}
