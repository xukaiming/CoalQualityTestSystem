// PageOut.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "PageOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageOut property page

static int _gEColText[] =
{
	IDS_COL_1,	//试验编号
	IDS_COL_6,	//样品质量
	IDS_COL_7,	//热容量
	IDS_COL_9,	//取舍
//	IDS_COL_8,	//极差
};

static int _gEColSize[] =
{
	90,
	60,
	70,
	60,
	40,
};

static int _gQColText[] =
{
	IDS_COL_3,	//样品编号
	IDS_COL_6,	//样品质量
	IDS_COL_10,	//弹筒发热量
	IDS_COL_11,	//高位发热量
	IDS_COL_12,	//底位发热量
};

static int _gQColSize[] =
{
	60,
	60,
	72,
	72,
	72,
};
///////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CPageOut, CPropertyPage)

CPageOut::CPageOut() : CPropertyPage(CPageOut::IDD)
{
	//{{AFX_DATA_INIT(CPageOut)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageOut::~CPageOut()
{
}



void CPageOut::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageOut)
	DDX_Control(pDX, IDC_EDIT_OUT, m_editView);
	DDX_Control(pDX, IDC_LIST_OUT, m_listView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageOut, CPropertyPage)
	//{{AFX_MSG_MAP(CPageOut)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageOut message handlers

BOOL CPageOut::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listView.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
// 	m_listView.ModifyStyleEx(0, WS_EX_CLIENTEDGE|LVS_SORTASCENDING);
//	AddExStyle (LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	AddExStyle (LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	BuildColumns(sizeof(_gEColText)/sizeof(int), _gEColSize, _gEColText);
	//初始化列表控件的风格
    m_listView.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);      //生成网格线|选择整行
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOut::AddExStyle(DWORD dwNewStyle)
{
	// Returns the current extended style ( a DWORD ).
	DWORD dwStyle = ::SendMessage (m_listView.m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

	// Add the full row select and grid line style to the existing extended styles.
	dwStyle |= dwNewStyle;

	// Sets the current extended style ( a DWORD ).
	::SendMessage (m_listView.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

 

void CPageOut::BuildColumns(int nCols, int *nWidth, int *iCol)
{
	ASSERT(nCols);
	ASSERT(nWidth);
	ASSERT(iCol);
	ASSERT(m_listView);

	//insert columns
	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp;
	for(i = 0; i < nCols; i++)
	{
		lvc.iSubItem = i;
		strTemp.LoadString(iCol[i]);
		lvc.pszText = (TCHAR*)(LPCTSTR)strTemp;
		lvc.cx = nWidth[i];
		lvc.fmt = LVCFMT_LEFT;
		m_listView.InsertColumn(i,&lvc);
	}
}

void CPageOut::DisEListView(DATA_E *pD)
{
	CString str[5];
	int iCol;

	m_listView.DeleteAllItems();
	while (m_listView.DeleteColumn(0));		//删除列表中所有列

	iCol=sizeof(_gEColText)/sizeof(int);
	BuildColumns(iCol, _gEColSize, _gEColText);

	for(int i=0;i<MAX_DATA_E;i++)
	{
		if(!pD->D[i].bActive)
			break;
		str[0]=pD->D[i].strTestNum;
		str[1].Format(_T("%1.4f"),pD->D[i].fYangPinZhiLiang);
		str[2].Format(_T("%d"),pD->D[i].lE);
//		str[3].Format(_T("%d"),pD->D[i].lMaxDiff);
		str[3].Format(_T("%s"),pD->D[i].bOk ? _T("合格"):_T("超差"));
		FormatList(i,str,iCol);
	}
	//显示平均值
	m_lMaxDiffE=pD->lMaxDiff;
	m_fSErrorE=pD->fSError;
	m_fRSErrorE=pD->fRSError;
	m_lAverageE=pD->lAverageE;
	m_sTest=_T("热容量");
	Invalidate(true);
}

//显示发热量试验信息
void CPageOut::DisQListView(DATA_Q *pQ)
{
	CString str[5];
	int iCol;

	m_listView.DeleteAllItems();
	while (m_listView.DeleteColumn(0));		//删除列表中所有列

	iCol=sizeof(_gQColText)/sizeof(int);
	BuildColumns(iCol, _gQColSize, _gQColText);

	for(int i=0;i<MAX_DATA_Q;i++)
	{
		if(!pQ->D[i].bActive)
			break;
		str[0]=pQ->D[i].strYangPinNum;
		str[1].Format(_T("%1.4f"),pQ->D[i].fYangPinM);
		str[2].Format(_T("%4d"),pQ->D[i].lQbad);
		str[3].Format(_T("%4d"),pQ->D[i].lQgrad);
		str[4].Format(_T("%4d"),pQ->D[i].lQnetvad);
		FormatList(i,str,iCol);
	}
	//显示平均值
	m_lAverageQ=pQ->lAverageQbad;
	m_lMaxDiffQ=pQ->lMaxDiffQbad;
	m_sTest=_T("发热量");
	Invalidate(true);
}



void CPageOut::FormatList(int iItem, CString *csText, int iCol)
{  
	LV_ITEM	lvi;

	lvi.mask = LVIF_TEXT | LVIF_STATE;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;
	lvi.iImage = 0;
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(1);
	lvi.pszText = (TCHAR*)(LPCTSTR)csText[0];		//first col
	int result = m_listView.InsertItem(&lvi);
	if (result != -1)
	{
		for(int i=1;i<iCol;i++)
			m_listView.SetItemText(iItem, i, csText[i]);
	}  
}

//DEL void CPageOut::OnColumnclickListOut(NMHDR* pNMHDR, LRESULT* pResult) 
//DEL {
//DEL 	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR; 
//DEL 	 // TODO: Add your control notification handler code here
//DEL 	for (int index=0;index<m_listView.GetItemCount();index++)
//DEL 	  m_listView.SetItemData(index,index); 
//DEL 	//SetItemData函数的作用是为一行设置一个标签，以后通过该标签就能找回这一行，标签是一个数值。
//DEL 	//SetItemData(index,index)意思就是这一行是第几列就把标签设为几。为什么要设置标签呢？以后的排序必须用到标签。
//DEL  
//DEL 	 if( pNMListView->iSubItem == m_nSortedCol )     //如果被点击的列和上一次点击的列是同一列的话
//DEL 	  m_fAsc = !m_fAsc;                             //排序方式取反，即上次是升序这次就为降序，上次降序这次就为升序
//DEL 	 else                                                   //如果被点击的列和上一次点击的列不是同一列的话
//DEL 	 {
//DEL 	  m_fAsc = TRUE;                               //排序方式为升序
//DEL 	  m_nSortedCol = pNMListView->iSubItem;        //把这次点击的列序号保存到m_nSortedCol 变量中
//DEL 	 }
//DEL 	 m_listView.SortItems( ListCompare, (DWORD)this );    
//DEL 	//SortItems函数进行排序操作，ListCompare是回调函数，SortItems函数会调用此回调函数进行排序判断
//DEL 
//DEL 
//DEL  
//DEL 	*pResult = 0;
//DEL }

//DEL int CALLBACK CPageOut::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//DEL {
//DEL 	//TODO:比较函数
//DEL 	return TRUE;
//DEL }

void CPageOut::OnPaint() 
{ 
	CPaintDC dc(this); // device context for painting
	
	CFont font;
	CFont*	pOldFont;
	CString s;

	//填充编辑输出窗口底色
	CWnd *pWnd=GetDlgItem(IDC_EDIT_OUT);

	CDC * pControlDC=pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	CBrush *brush;
	brush = new CBrush(RGB(245,240,240));				// Create a solid or hatched brush.
	CBrush* oldBrush = pControlDC->SelectObject(brush);	// Select the new brush into the DC.
	CRect rc;
	m_editView.GetClientRect(rc);
	pControlDC->Rectangle(rc);				// Draw the rectangle.
	pControlDC->SelectObject(oldBrush);		// Restore the DC and delete the brush.
	delete brush;

	font.CreateFont(15, 0, 0, 0, FW_NORMAL, false, false,
	                0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                DEFAULT_PITCH | FF_MODERN, _T("Times New Roman"));
	pOldFont = (CFont*) (pControlDC->SelectObject(&font));
	pControlDC->SetBkMode(TRANSPARENT);
	pControlDC->SetTextColor(RGB(128,32,0));

	if(m_sTest==_T("热容量"))
	{
		pControlDC->TextOut(5, 3, "极差:                      标准差:                      相对标准差:");
		pControlDC->TextOut(5, 20, "平均热容量:");
		s.Format(_T("%d  J/K"),m_lMaxDiffE);
		pControlDC->TextOut(35, 2, s);
		s.Format(_T("%2.2f  J/K"),m_fSErrorE);
		pControlDC->TextOut(140, 2, s);
		s.Format(_T("%2.2f %%"),m_fRSErrorE*100);
		pControlDC->TextOut(270, 2, s);
		s.Format(_T("%6d  J/K"),m_lAverageE);
		pControlDC->TextOut(70, 19, s);
	}
	else if(m_sTest==_T("发热量"))
	{
		pControlDC->TextOut(5, 3, _T("极差:"));
		pControlDC->TextOut(5, 20, "平均发热量:");
		s.Format(_T("%4d J/g"),m_lMaxDiffQ);
		pControlDC->TextOut(35, 2, s);
		s.Format(_T("%4d J/g"),m_lAverageQ);
		pControlDC->TextOut(70, 19, s);
	}
	pControlDC->SelectObject(pOldFont);
	pWnd->ReleaseDC(pControlDC);
}
