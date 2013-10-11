// EditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "EditListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if (_WIN32_WINNT < 0x501)
typedef struct tagNMLVSCROLL
{
	NMHDR   hdr;
	int     dx;
	int     dy;
} NMLVSCROLL, *LPNMLVSCROLL;

#define LVN_BEGINSCROLL          (LVN_FIRST-80)
#define LVN_ENDSCROLL            (LVN_FIRST-81)
#endif


#define MAX_COLS	2

#define LV_EDIT_CTRL_ID		1001

// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)
// CEditListCtrl message handlers
BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(LV_EDIT_CTRL_ID, OnEnKillFocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomDraw)
	ON_NOTIFY_REFLECT(LVN_ENDSCROLL, OnLvnEndScroll)
	ON_NOTIFY_REFLECT(LVN_BEGINSCROLL, OnLvnBeginScroll)
	ON_WM_CREATE() 
END_MESSAGE_MAP()

CEditListCtrl::CEditListCtrl()
{
	m_pctrlEdit			= NULL;
	m_pctrlComboBox		= NULL;
	m_pDateTimePicker	= NULL;
	m_iRow = 0;
	m_iCol = 1;

	m_iEditRow = -1;
	m_iEditCol = -1;
}

CEditListCtrl::~CEditListCtrl()
{

}
 
BOOL CEditListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)  //按TAB键盘
	{
		CommitEditCtrl();
		m_iEditRow = -1;
		m_iEditCol = -1;
		if (GetKeyState(VK_SHIFT) & 0x8000)        //如果按住了 shift键盘
		{
			m_iCol--;
			if (m_iCol < 1)
			{
				m_iCol = MAX_COLS - 1;
				m_iRow--;
				while (m_iRow >= 0 && GetItemData(m_iRow) & 1)
					m_iRow--;

				if (m_iRow < 0)
				{
					m_iCol = 1;
					m_iRow = 0;

					// cycle to prev sibling control
					CWnd* pWnd = GetWindow(GW_HWNDPREV);
					ASSERT( pWnd );
					if (pWnd)
					{
						pWnd->SetFocus();
						return CListCtrl::PreTranslateMessage(pMsg);
					}
				}
			}
		}
		else
		{
			m_iCol++;
			if (m_iCol >= MAX_COLS)
			{
				m_iCol = 1;
				m_iRow++;

				while (m_iRow < GetItemCount() && GetItemData(m_iRow) & 1)
					m_iRow++;

				ResetBottomPosition();
				if (m_iRow > GetItemCount() - 1)
				{
					m_iRow = GetItemCount() - 1;

					// cycle to next sibling control
					CWnd* pWnd = GetNextWindow();
					ASSERT( pWnd );
					if (pWnd)
					{
						pWnd->SetFocus();
						return CListCtrl::PreTranslateMessage(pMsg);
					}
				}
			}
		}

		VerifyScrollPos();
		ResetTopPosition();
		ResetBottomPosition();

		ShowCtrl();
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

BOOL CEditListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (((NMHDR*)lParam)->code)    //修改列表宽度的时候
	{
	case HDN_BEGINTRACKW:    
	case HDN_BEGINTRACKA:
		if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
			m_pctrlEdit->ShowWindow(SW_HIDE);
		if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
			m_pctrlComboBox->ShowWindow(SW_HIDE);
		if (m_pDateTimePicker && m_pDateTimePicker->IsWindowVisible())
			m_pDateTimePicker->ShowWindow(SW_HIDE);
		*pResult = 0;
		break;
	} 

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

// ShowEditCtrl
void CEditListCtrl::ShowEditCtrl(void)
{
	if (m_iRow >= GetItemCount())
		return;
	if (GetItemData(m_iRow) & 1)
	{
		if (m_pctrlEdit)
			m_pctrlEdit->ShowWindow(SW_HIDE);
		return;
	}

	CRect rect;
	GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect);
	if (m_pctrlEdit == NULL)
	{
		m_pctrlEdit = new CEdit;
		m_pctrlEdit->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL, rect, this, LV_EDIT_CTRL_ID);
		m_pctrlEdit->SetFont(GetFont());
		m_pctrlEdit->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pctrlEdit->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}

	TCHAR szItem[256] = {0};
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = m_iRow;
	item.iSubItem = m_iCol;
	item.cchTextMax =  sizeof(szItem)/sizeof(szItem[0]);
	item.pszText = szItem;
	GetItem(&item);
	m_pctrlEdit->SetWindowText(szItem);
	m_pctrlEdit->SetSel(0, -1);
	if (m_pctrlComboBox)
		m_pctrlComboBox->ShowWindow(SW_HIDE);	
	if (m_pDateTimePicker)
		m_pDateTimePicker->ShowWindow(SW_HIDE);
	m_pctrlEdit->SetFocus();

	m_iEditRow = m_iRow;
	m_iEditCol = m_iCol;
}

// ShowComboBoxCtrl
void CEditListCtrl::ShowComboBoxCtrl(void)
{
	CRect rect;
	GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect);
	rect.bottom += 100;
	if (!m_pctrlComboBox)
	{
		m_pctrlComboBox = new CComboBox;
		m_pctrlComboBox->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPSIBLINGS|WS_BORDER|CBS_DROPDOWN|WS_VSCROLL|WS_HSCROLL, rect, this, 1002);
		m_pctrlComboBox->ShowWindow(SW_SHOW);
		m_pctrlComboBox->SetHorizontalExtent(800);
		m_pctrlComboBox->SendMessage(CB_SETDROPPEDWIDTH, 600, 0);
	}
	else
	{
		m_pctrlComboBox->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		m_pctrlComboBox->SetCurSel(0);
	}
	if (m_pctrlEdit)
		m_pctrlEdit->ShowWindow(SW_HIDE);
	if (m_pDateTimePicker)
		m_pDateTimePicker->ShowWindow(SW_HIDE);
	m_pctrlComboBox->SetFocus();
}
//
//void  ShowComboBoxCtrl(void)
void CEditListCtrl::ShowDateTimePicker(void)
{
	CRect rect;
	GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect);
	rect.bottom += 2;
	if(!m_pDateTimePicker)
	{
		m_pDateTimePicker = new CDateTimeCtrl;
		m_pDateTimePicker->Create(WS_VISIBLE | WS_CHILD | DTS_LONGDATEFORMAT, rect, this, 1002);
		//COleDateTime dbTime	= COleDateTime::GetCurrentTime();
		m_pDateTimePicker->SendMessage(DTM_SETFORMAT,0,(LPARAM)_T("yyyy-mm-dd  HH:mm:ss")); 
		m_pDateTimePicker->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDateTimePicker->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}

	TCHAR szItem[256] = {0};
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = m_iRow;
	item.iSubItem = m_iCol;
	item.cchTextMax =  sizeof(szItem)/sizeof(szItem[0]);
	item.pszText = szItem;
	GetItem(&item); 
	CString  dtStr;
	COleDateTime time ;
	if(_tcscmp(szItem,_T(""))==0)
	{
		time = COleDateTime::GetCurrentTime();
	}
	else 
	{
		COleVariant vtime(szItem);     //将字符串转换为时间
		vtime.ChangeType(VT_DATE);
		time = vtime;
	}
	m_pDateTimePicker->SetTime(time);
	if (m_pctrlComboBox)
		m_pctrlComboBox->ShowWindow(SW_HIDE);	
	if (m_pctrlEdit)
		m_pctrlEdit->ShowWindow(SW_HIDE);
	m_pDateTimePicker->SetFocus();
	m_iEditRow = m_iRow;
	m_iEditCol = m_iCol;
}


// ResetTopPosition
void CEditListCtrl::ResetTopPosition()
{
	CWnd* pWnd = NULL;
	if (m_pctrlComboBox)
		pWnd = m_pctrlComboBox;
	else if (m_pctrlEdit)
		pWnd = m_pctrlEdit;
	else if (m_pDateTimePicker)
		pWnd = m_pDateTimePicker;
	if (pWnd)
	{
		CRect rcThis;
		GetWindowRect(rcThis);
		CRect rect;
		if (!GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect))
			return;
		ClientToScreen(rect);
		CPoint pt(rect.left, rect.top);
		if (!rcThis.PtInRect(pt))
			SendMessage(WM_VSCROLL, SB_LINEUP, 0);
	}
}

void CEditListCtrl::ResetBottomPosition()
{
	CWnd* pWnd = NULL;
	if (m_pctrlComboBox)
		pWnd = m_pctrlComboBox;
	else if (m_pctrlEdit)
		pWnd = m_pctrlEdit;
	else if (m_pDateTimePicker)
		pWnd = m_pDateTimePicker;
	if (pWnd)
	{
		CRect rcThis;
		GetWindowRect(rcThis);
		CRect rect;
		if (!GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect))
			return;
		ClientToScreen(rect);
		CPoint pt(rect.right, rect.bottom);
		if (!rcThis.PtInRect(pt))
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	}
}

void CEditListCtrl::VerifyScrollPos()
{
	int nTopRowVal = GetTopIndex();
	int nBottomVal = nTopRowVal + GetCountPerPage();
	if (m_iRow > nTopRowVal-1 && m_iRow < nBottomVal+1)
		;
	else
	{
		if (m_iRow < nTopRowVal)
		{
			for (int j = 0; j < -m_iRow + nTopRowVal; j++)
				SendMessage(WM_VSCROLL, SB_LINEUP, 0);
		}
		if (m_iRow > nBottomVal)
		{
			for (int j = 0; j < m_iRow - nTopRowVal; j++)
				SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		}
	}
}

void CEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode != SB_LINEDOWN && nSBCode != SB_LINEUP)
	{
		if (nSBCode == SB_ENDSCROLL)
		{
			if (m_pctrlEdit)
				ShowEditCtrl();
		}
		else if (m_pctrlEdit)
		{
			CommitEditCtrl();
			m_iEditRow = -1;
			m_iEditCol = -1;
			m_pctrlEdit->ShowWindow(SW_HIDE);
		}
	}

	if (m_pctrlComboBox)
		m_pctrlComboBox->ShowWindow(SW_HIDE);
	if (m_pDateTimePicker)
		m_pDateTimePicker->ShowWindow(SW_HIDE); 
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
 void CEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
 {
	 if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
		 m_pctrlEdit->ShowWindow(SW_HIDE);
	 if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
		 m_pctrlComboBox->ShowWindow(SW_HIDE);
	 if (m_pDateTimePicker && m_pDateTimePicker->IsWindowVisible())
		 m_pDateTimePicker->ShowWindow(SW_HIDE);
	 CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
 }
 void CEditListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
 {
	 if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
		 m_pctrlEdit->ShowWindow(SW_HIDE);
	 if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
		 m_pctrlComboBox->ShowWindow(SW_HIDE);
	 if (m_pDateTimePicker && m_pDateTimePicker->IsWindowVisible())
		 m_pDateTimePicker->ShowWindow(SW_HIDE);
	 *pResult = 0;
 } 

 void CEditListCtrl::OnDestroy()
 {
	 delete m_pctrlEdit;
	 delete m_pctrlComboBox;
	 delete m_pDateTimePicker;
	 CListCtrl::OnDestroy();
 }
 void CEditListCtrl::OnEnKillFocus()
 {
	 CommitEditCtrl();
	 m_iEditRow = -1;
	 m_iEditCol = -1;
	 if (m_pctrlEdit)
		 m_pctrlEdit->ShowWindow(SW_HIDE);
 }
 void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
 {
	 CListCtrl::OnLButtonDown(nFlags, point);

	 LVHITTESTINFO hti = {0};
	 hti.pt = point;
	 if (SubItemHitTest(&hti) == -1)
		 return;

	 int iItem = hti.iItem;
	 int iSubItem = hti.iSubItem;
	 if (iItem == -1 || iSubItem == -1)
		 return;

	 CRect rect;
	 GetSubItemRect(iItem, iSubItem, LVIR_LABEL, rect);
	 ClientToScreen(rect);

	 CPoint pt(rect.right, rect.bottom);
	 CRect rc;
	 GetWindowRect(rc);
	 if (!rc.PtInRect(pt))
		 SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	 pt.x = rect.left;
	 pt.y = rect.top;
	 if (!rc.PtInRect(pt))
		 SendMessage(WM_VSCROLL, SB_LINEUP, 0);

	 m_iRow = iItem;
	 m_iCol = iSubItem;

	 ShowCtrl();

 }
void CEditListCtrl::OnSetFocus(CWnd* pOldWnd)
 {
	 CListCtrl::OnSetFocus(pOldWnd);
	 if ((pOldWnd != m_pctrlEdit)
		 &&(pOldWnd != m_pctrlComboBox)
		 &&(pOldWnd != m_pDateTimePicker)) 
	 {
		 ShowCtrl();
	 }
	 /*
	 switch (m_iCol)
	 {
	 case 0:
		 break;
	 case 1:
		 if (pOldWnd != m_pctrlEdit)
			 ShowEditCtrl();
		 break;
	 }
	 */
 }
 void CEditListCtrl::OnNMCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	 if (pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	 {
		 *pResult = CDRF_NOTIFYITEMDRAW;
		 return;
	 }

	 if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	 {
		 DWORD dwItemData = pNMCD->nmcd.lItemlParam;
		 if (dwItemData & 1)
		 {
			 pNMCD->clrText = RGB(128,128,128);
			 pNMCD->clrTextBk = 0xFF000000;
		 }
		 else
		 {
			 pNMCD->clrText = 0xFF000000;
			 pNMCD->clrTextBk = 0xFF000000;
		 }
	 }

	 *pResult = CDRF_DODEFAULT;
 }
 void CEditListCtrl::OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 //LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	 if (m_pctrlEdit)
		 ShowEditCtrl();
	 *pResult = 0;
 }

 //滚动
 void CEditListCtrl::OnLvnBeginScroll(NMHDR *pNMHDR, LRESULT *pResult)
 {

	 //LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	 if (m_pctrlEdit)
	 {
		 CommitEditCtrl();
		 m_iEditRow = -1;
		 m_iEditCol = -1;
		 m_pctrlEdit->ShowWindow(SW_HIDE);
	 }
	 *pResult = 0;
 }

 void CEditListCtrl::CommitEditCtrl()
 {
	 if (m_iEditCol != -1 && m_iEditRow != -1)
	 {
		 if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
		 {
			 //CString strItem;
			 TCHAR strItem[MAX_PATH];
			 ::GetWindowText(m_pctrlEdit->GetSafeHwnd(),strItem,MAX_PATH);
		     //int GetWindowText(HWND hWnd,LPTSTR lpString,int nMaxCount);
			 //m_pctrlEdit->GetWindowText(strItem);			 
			 //strItem.TrimLeft();
			 //strItem.TrimRight();
			 ValidCheck(m_iEditRow, m_iEditCol, strItem);
			 SetItemText(m_iEditRow, m_iEditCol, strItem);
			 //			m_iEditRow = -1;
			 //			m_iEditCol = -1;
		 }
		 else if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
		 {
			 CString strItem;
			 m_pctrlComboBox->GetLBText(m_pctrlComboBox->GetCurSel(), strItem);
			 strItem.TrimLeft();
			 strItem.TrimRight();
			 SetItemText(m_iEditRow, m_iEditCol, strItem);
			 //			m_iEditRow = -1;
			 //			m_iEditCol = -1;
		 }else if(m_pDateTimePicker && m_pDateTimePicker->IsWindowVisible())
		 {
			 CString strItem;  
			 COleDateTime dt;
			 m_pDateTimePicker->GetTime(dt);
			 strItem = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
			 SetItemText(m_iEditRow, m_iEditCol, strItem);
		 }
	 }
 }


 int CEditListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
 {
	 if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		 return -1;

	 // TODO:  Add your specialized creation code here 	//
	  
	return 0;
 }



 void CEditListCtrl::ShowCtrl(void)
 { 
	 switch (m_iCol)
	 {
	 case 0: 
		 break;
	 case 1: 
		 ShowEditCtrl();
		 break;
	 case 2:
		 //ShowDateTimePicker();
		 ShowEditCtrl();
		 break;
	 case 3:
		 ShowEditCtrl();
	 case 4:
		 ShowEditCtrl();
		 break;
	 }
 }

 BOOL CEditListCtrl::ValidCheck(int nRow, int nCol, LPCTSTR lpszText)
 {
	 BOOL bResult=FALSE;
	 COleDateTime time = COleDateTime::GetCurrentTime();
	 COleVariant vtime(lpszText);     //将字符串转换为时间
	 double temp;
	 switch (nCol)
	 { 
	 case 1:  
		 break;
	 case 2:
		 try
		 {
			 vtime.ChangeType(VT_DATE);
			 time = vtime;
		 } 
		 catch (CException* e)
		 {
			 CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
			 _tcscpy((TCHAR*)lpszText,strTime);		 
		 }

		 break;
	 case 3:  //样品重量
		 temp = _tcstod(lpszText,NULL);
		 if((temp<100)&&(temp>5))
			_stprintf((TCHAR*)lpszText,_T("%2.1f"),temp);
		 else
			 _stprintf((TCHAR*)lpszText,_T("%2.1f"),50.0f); 
	 case 4:  //水分百分比 
		 temp = _tcstod(lpszText,NULL);
		 if(temp<100)
			 _stprintf((TCHAR*)lpszText,_T("%2.1f"),temp);
		 else
			 _stprintf((TCHAR*)lpszText,_T("%2.1f"),1.2f); 
		 break;
	 }
	 return TRUE;
 }

 //	GetDlgItem(IDC_DEVICE_DianHuoWenCha ) ->GetWindowText(str) ;
 //LryRdb.m_fFireOKtemp	= (str,NULL); 