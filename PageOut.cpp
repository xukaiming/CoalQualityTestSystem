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
	IDS_COL_1,	//������
	IDS_COL_6,	//��Ʒ����
	IDS_COL_7,	//������
	IDS_COL_9,	//ȡ��
//	IDS_COL_8,	//����
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
	IDS_COL_3,	//��Ʒ���
	IDS_COL_6,	//��Ʒ����
	IDS_COL_10,	//��Ͳ������
	IDS_COL_11,	//��λ������
	IDS_COL_12,	//��λ������
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
	//��ʼ���б�ؼ��ķ��
    m_listView.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);      //����������|ѡ������
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
	while (m_listView.DeleteColumn(0));		//ɾ���б���������

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
		str[3].Format(_T("%s"),pD->D[i].bOk ? _T("�ϸ�"):_T("����"));
		FormatList(i,str,iCol);
	}
	//��ʾƽ��ֵ
	m_lMaxDiffE=pD->lMaxDiff;
	m_fSErrorE=pD->fSError;
	m_fRSErrorE=pD->fRSError;
	m_lAverageE=pD->lAverageE;
	m_sTest=_T("������");
	Invalidate(true);
}

//��ʾ������������Ϣ
void CPageOut::DisQListView(DATA_Q *pQ)
{
	CString str[5];
	int iCol;

	m_listView.DeleteAllItems();
	while (m_listView.DeleteColumn(0));		//ɾ���б���������

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
	//��ʾƽ��ֵ
	m_lAverageQ=pQ->lAverageQbad;
	m_lMaxDiffQ=pQ->lMaxDiffQbad;
	m_sTest=_T("������");
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
//DEL 	//SetItemData������������Ϊһ������һ����ǩ���Ժ�ͨ���ñ�ǩ�����һ���һ�У���ǩ��һ����ֵ��
//DEL 	//SetItemData(index,index)��˼������һ���ǵڼ��оͰѱ�ǩ��Ϊ����ΪʲôҪ���ñ�ǩ�أ��Ժ����������õ���ǩ��
//DEL  
//DEL 	 if( pNMListView->iSubItem == m_nSortedCol )     //�����������к���һ�ε��������ͬһ�еĻ�
//DEL 	  m_fAsc = !m_fAsc;                             //����ʽȡ�������ϴ���������ξ�Ϊ�����ϴν�����ξ�Ϊ����
//DEL 	 else                                                   //�����������к���һ�ε�����в���ͬһ�еĻ�
//DEL 	 {
//DEL 	  m_fAsc = TRUE;                               //����ʽΪ����
//DEL 	  m_nSortedCol = pNMListView->iSubItem;        //����ε��������ű��浽m_nSortedCol ������
//DEL 	 }
//DEL 	 m_listView.SortItems( ListCompare, (DWORD)this );    
//DEL 	//SortItems�����������������ListCompare�ǻص�������SortItems��������ô˻ص��������������ж�
//DEL 
//DEL 
//DEL  
//DEL 	*pResult = 0;
//DEL }

//DEL int CALLBACK CPageOut::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
//DEL {
//DEL 	//TODO:�ȽϺ���
//DEL 	return TRUE;
//DEL }

void CPageOut::OnPaint() 
{ 
	CPaintDC dc(this); // device context for painting
	
	CFont font;
	CFont*	pOldFont;
	CString s;

	//���༭������ڵ�ɫ
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

	if(m_sTest==_T("������"))
	{
		pControlDC->TextOut(5, 3, "����:                      ��׼��:                      ��Ա�׼��:");
		pControlDC->TextOut(5, 20, "ƽ��������:");
		s.Format(_T("%d  J/K"),m_lMaxDiffE);
		pControlDC->TextOut(35, 2, s);
		s.Format(_T("%2.2f  J/K"),m_fSErrorE);
		pControlDC->TextOut(140, 2, s);
		s.Format(_T("%2.2f %%"),m_fRSErrorE*100);
		pControlDC->TextOut(270, 2, s);
		s.Format(_T("%6d  J/K"),m_lAverageE);
		pControlDC->TextOut(70, 19, s);
	}
	else if(m_sTest==_T("������"))
	{
		pControlDC->TextOut(5, 3, _T("����:"));
		pControlDC->TextOut(5, 20, "ƽ��������:");
		s.Format(_T("%4d J/g"),m_lMaxDiffQ);
		pControlDC->TextOut(35, 2, s);
		s.Format(_T("%4d J/g"),m_lAverageQ);
		pControlDC->TextOut(70, 19, s);
	}
	pControlDC->SelectObject(pOldFont);
	pWnd->ReleaseDC(pControlDC);
}
