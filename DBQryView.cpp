// DBQryView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DBQryView.h"


// CDBQryView

IMPLEMENT_DYNCREATE(CDBQryView, CListView)//CXTListView

CDBQryView::CDBQryView()
{
	m_dwDefaultStyle |= ( LVS_REPORT | LVS_OWNERDRAWFIXED );
}

CDBQryView::~CDBQryView()
{
}

BEGIN_MESSAGE_MAP(CDBQryView, CListView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_PRINT_REPORT_SETTING,OnUpdateMenuState1) 
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW,OnUpdateMenuState1) 
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,OnUpdateMenuState1) 
	 

	//	ON_WM_SETFOCUS()
	ON_COMMAND(ID_PRINT_REPORT_SETTING, &CDBQryView::OnPrintReportSetting)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDBQryView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, &CDBQryView::OnFilePrint)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CDBQryView::OnNMCustomdraw)
	ON_WM_MEASUREITEM_REFLECT()

END_MESSAGE_MAP()


// CDBQryView diagnostics

#ifdef _DEBUG
void CDBQryView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CDBQryView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif

CDBQryDocument * CDBQryView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBQryDocument)));
	return (CDBQryDocument*)m_pDocument;
}
#endif //_DEBUG


// CDBQryView message handlers

void CDBQryView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	LPCTSTR dbName = _T("HRDRECORD");

	if(pReportdlg.Create(_T(""),0,CRect(0,0,0,0),this,IDC_RMREPORTENGINE)==FALSE)
	{
		pReportdlg.DestroyWindow();
		MessageBox(_T("RMReport.ocx��δע��!\n�޷���ӡ����!"),_T("����"),MB_OK|MB_ICONWARNING);
	}
	CListCtrl *pListCtrl = &GetListCtrl(); 
	//pListCtrl->SetItemHeight(36); 

	//
	//HWND hWndHeader = pListCtrl->GetHeaderCtrl()->GetSafeHwnd();
	//m_header.SubclassWindow(hWndHeader); 
	/*
	CListCtrl *pListCtrl = &GetListCtrl();
	HWND hWndHeader = pListCtrl->GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader); 
	m_header.SetTheme(new CXTHeaderCtrlThemeOfficeXP());

	m_header.SetMinSize(15);
	m_header.EnableAutoSize(TRUE);
	
	CXTHeaderCtrlTheme* pTheme = m_header.GetTheme();
	
	if (pTheme)
	{
		pTheme->SetDrawStyle(XTTHEME_WINXPTHEMES|XTTHEME_HOTTRACKING, &m_header);
		RedrawWindow(NULL, NULL,
			RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
	}
	*/
}


void CDBQryView::FillHeader()
{
	
	
	CDBQryDocument* pDoc  = (CDBQryDocument*)GetDocument(); 
	CListCtrl * ListCtrl = (CListCtrl  *)&GetListCtrl(); 
	//CXTListCtrl * ListCtrl = (CXTListCtrl  *)&GetListCtrl(); 
	_RecordsetPtr pRs = pDoc->Rs;

	int i,iStrLen;

	GetAllFieldName(pRs,&ColNameArray);
	ListCtrl->DeleteAllItems();
	//������б���
	do
	{
		ListCtrl->DeleteColumn(0);
	}
	while(ListCtrl->GetHeaderCtrl()->GetItemCount()>0); 
	//д�ֶα���
	for(  i=ColNameArray.GetSize()-1;i>=0;i--)
	{
		CDC *pDC = GetDC();
		iStrLen	= pDC->GetTextExtent(ColNameArray.GetAt(i)).cx*2;				
		ListCtrl->InsertColumn(0,ColNameArray.GetAt(i),LVCFMT_CENTER,iStrLen ,35);
		ReleaseDC(pDC);	
	}
}

void CDBQryView::FillData()
{
	CDBQryDocument* pDoc  = (CDBQryDocument*)GetDocument();
	CListCtrl * ListCtrl = (CListCtrl  *)&GetListCtrl(); 
	//CXTListCtrl * ListCtrl = (CXTListCtrl  *)&GetListCtrl(); 
	_RecordsetPtr pRs = pDoc->Rs;

	LONG RecordCount = 0;
	_variant_t  str;
	CString strData;
	int i;
	RecordCount = pRs->GetRecordCount();
	ListCtrl->SetItemCount(RecordCount);

	if(RecordCount>0)
	{
		if(!pRs->adoEOF)
			pRs->MoveLast();

		while(!pRs->BOF)
		{		
			for(i=0;i<pRs->Fields->Count;i++)
			{
				str = pRs->GetCollect((_bstr_t)ColNameArray.GetAt(i));
				strData = DBVariantToStr(str,ColNameArray.GetAt(i)); 
				if(i==0)
					ListCtrl->InsertItem(0,strData);
				else
					ListCtrl->SetItemText(0,i,strData);
			}
			pRs->MovePrevious();
		}  
		pDoc->MoveFirst();
	}
	
}
void CDBQryView::FillList()
{
	int i =0;
	int iStrLen = 0;
	
	
	BeginWaitCursor() ;
	try
	{
		CDBQryDocument* pDoc  = (CDBQryDocument*)GetDocument();
		//_RecordsetPtr pRs = pDoc->Rs;
		//�õ������ֶ����� 
		CListCtrl * ListCtrl = (CListCtrl  *)&GetListCtrl(); 
		//CXTListCtrl * ListCtrl = (CXTListCtrl  *)&GetListCtrl(); 
		ListCtrl->SetRedraw(FALSE);
		FillHeader();		// 
		FillData();
		ListCtrl->SetRedraw(TRUE); 
		pDoc->UpdateAllViews(this);		
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	EndWaitCursor();
}

void CDBQryView::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	
	int iCursor;
	CListCtrl * ListCtrl = (CListCtrl *)&GetListCtrl();
	//CXTListCtrl * ListCtrl = (CXTListCtrl  *)&GetListCtrl(); 
	CDBQryDocument *pDoc = (CDBQryDocument *)GetDocument();
	iCursor = ListCtrl->GetNextItem(-1, LVNI_SELECTED ); 

	if((iCursor>-1)&&(GetAsyncKeyState(VK_LCONTROL)==0))
		pDoc->Move(iCursor,(long)adBookmarkFirst); 

	*pResult = 0;
}

int CDBQryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{ 

	// TODO:  Add your specialized creation code here
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1; 
 
	CListCtrl * ListCtrl = (CListCtrl *)&GetListCtrl();
	LONG style = ListCtrl->GetStyle(); 
	style |= LVS_REPORT |LVS_SHOWSELALWAYS ;//|LVS_SINGLESEL;;//
	
	SetWindowLong(ListCtrl->m_hWnd,GWL_STYLE,style); 
 
	//ModifyExtendedStyle(0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_TRACKSELECT  );
	//SetTextBkColor(COLOR_3DHIGHLIGHT);
	ListCtrl->SetExtendedStyle(ListCtrl->GetExtendedStyle()
		|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP
		|LVS_EX_GRIDLINES|LVS_EX_BORDERSELECT );//
	
	CFont *pFont = GetFont();
	::ZeroMemory(&lf,   sizeof(lf)); 
	pFont->GetLogFont(&lf);
	lf.lfHeight   =   -16; 
	/*
	
	lf.lfWidth     =   0; 
	lf.lfEscapement   =   0; 
	lf.lfOrientation   =   0; 
	lf.lfWeight   =   0;//FW_BOLD; 
	lf.lfItalic   =   FALSE; 
	lf.lfUnderline   =   FALSE; 
	lf.lfCharSet   =   DEFAULT_CHARSET; 
	lf.lfOutPrecision   =   OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision   =   CLIP_DEFAULT_PRECIS; 
	lf.lfQuality   =   ANTIALIASED_QUALITY; 
	*/
	font.CreateFontIndirect(&lf);  
	SetFont(&font); 
	
	//////////////////////////////  
	return 0;		
}

void CDBQryView::OnUpdate(CView* pSender, LPARAM lHint, CObject *pHint)
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl  *ListCtrl = (CListCtrl  *)&GetListCtrl();  
	CDBQryDocument *pDoc = (CDBQryDocument *)GetDocument();
	CDBQryDocument::CRowObj *rowInfo;
	int i,j;
	switch(lHint)
	{
	case 0:
		FillList();			//��д���
		break;
	case 1:
		try
		{
			i = pDoc->Rs->AbsolutePosition-1; 
			ListCtrl->SetFocus();
			 
			for(j=0;j<ListCtrl->GetItemCount();j++)
				ListCtrl-> SetItemState(j,0,LVIS_SELECTED|LVIS_FOCUSED);

			ListCtrl-> SetItemState(i,   LVIS_SELECTED|LVIS_FOCUSED    ,   LVIS_SELECTED|LVIS_FOCUSED); 
			ListCtrl->EnsureVisible(i,TRUE);	
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
		break;
	case 2:
		rowInfo = (CDBQryDocument::CRowObj*)pHint;
		ListCtrl->DeleteItem(rowInfo->RowNO);
		ListCtrl->Update(rowInfo->RowNO);
		break;
	default:
		break;
	} 
}

CString CDBQryView::DBVariantToStr(_variant_t var,CString ParamName)
{
	return VariantToStr(var);
}


void CDBQryView::OnUpdateMenuState1(CCmdUI* pCmdUI)
{
	if(( GetListCtrl().GetNextItem(-1,LVNI_SELECTED) ==-1)||(pReportdlg.m_hWnd==NULL) )
	{ 
		pCmdUI->Enable( FALSE ); 
	} 
	else
	{ 
		pCmdUI->Enable( TRUE ); 
	}
}

void CDBQryView::OnPrintReportSetting()
{
	// TODO: Add your command handler code here
	OnFilePrintPreview();
}

void CDBQryView::OnFilePrintPreview()
{
	
	CString ReportFile;
	CString strSQL;
	//ReportSeleDlg.cFileName = _T("������*.rmf");
	FileFilter();
	if(ReportSeleDlg.DoModal()==IDOK)
	{
		ParsarReport();

	}	
}

void CDBQryView::OnFilePrint()
{
	// TODO: Add your command handler code here
	OnFilePrintPreview();
}

void CDBQryView::FileFilter()
{
}

void CDBQryView::ParsarReport()
{
}

CString CDBQryView::GetItemText(int nItem, CString strColName)
{
	int i;
	CListCtrl *pCtrl = &GetListCtrl();
	CString pstrColName;
	for(  i=ColNameArray.GetSize()-1;i>=0;i--)
	{  
		pstrColName= ColNameArray.GetAt(i);
		if(pstrColName==strColName)
			break;
	}
	return pCtrl->GetItemText(nItem,i); 
}

void CDBQryView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CListCtrl *pListCtrl = (CListCtrl*)&GetListCtrl();
	// ȡ����ʾ�豸������ָ��
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// ���ñ�־
	UINT uiFlags(ILD_TRANSPARENT);

	// �������Ƿ�ѡ��
	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
	{
		//�������ѡ�������ֺͱ�������ɫ��Ϊ������ʾʱ����ɫ
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		uiFlags |= ILD_BLEND50;
	}
	else 
	{
		//���򽫽����ֺͱ�������ɫ��Ϊ������ɫ
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}

	

	// �õ���������ʾ����
	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(&lvi));
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lpDrawItemStruct->itemID;
	pListCtrl->GetItem(&lvi);
	 
	// �õ��ͼ���б�ָ��
	CImageList* plistImages =  pListCtrl->GetImageList(LVSIL_SMALL);
	// ������ѡ�е�ͼ���б�
	if (plistImages) {
		CPoint ptAt(lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top);
		if ( ! plistImages->Draw(pDC, 0//lvi.iImage
		, ptAt, uiFlags) ) {
			int x = 0;
			x++;
		}
	}
 

	// �������ڻ��Ʊ���ߵĻ���
	CPen pen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
	CPen* penOld = pDC->SelectObject(&pen);

	TCHAR szText[256];     // �����ı�����,����ı����Ƚϴ�,�������Ӹ�����ĳ���
	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(lvc));
	lvc.mask = LVCF_WIDTH;

	for (int nColumn = 0; pListCtrl->GetColumn(nColumn, &lvc); nColumn++) 
	{
		CRect rTextClip;

		// ����ǻ���һ��
		if (0 == nColumn) 
		{
			//����һ��ʱ�����ͼ���б��򽫻����ı��ľ��������Կճ�����ͼ�εĿռ�
			rTextClip.left   = lpDrawItemStruct->rcItem.left +
				(plistImages ? 16 : 0);
			rTextClip.top    = lpDrawItemStruct->rcItem.top;
			rTextClip.right  = lpDrawItemStruct->rcItem.left + lvc.cx;
			rTextClip.bottom = lpDrawItemStruct->rcItem.bottom;
		}
		else 
		{
			// ������ǵ�һ�����ı�������������һ���ı����ε��Ҳ�
			rTextClip.left  = rTextClip.right;
			rTextClip.right = rTextClip.left + lvc.cx;
		}

		// ����ı�
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = nColumn;
		lvi.pszText = szText;
		lvi.cchTextMax = sizeof(szText);
		pListCtrl->GetItem(&lvi);

		// �õ��ı�����
		int nTextCount(lstrlen(lvi.pszText));

		//�����ı�
		CSize sizText(pDC->GetOutputTextExtent(lvi.pszText,
			nTextCount));

		//ʹ�ı�������СһЩ��ʹ�м���ַָ�,��������ı��ϳ�������м��ı���������
		rTextClip.right -= 4;

		//����ı���ȴ��ڻ��ƾ��εĿ��
		if (sizText.cx >= rTextClip.Width()) 
		{

			// ���...�ĳ���
			CSize sizEllipse = pDC->GetOutputTextExtent(_T("..."), 3);

			// �����϶̵��ı�
			while (sizText.cx+sizEllipse.cx > (rTextClip.Width())  &&
				nTextCount > 1) 
		 {
			 lvi.pszText[--nTextCount] = 0;
			 sizText = pDC->GetOutputTextExtent(lvi.pszText,
				 nTextCount);
			}

			// ���ı������ "...".
			lvi.pszText[nTextCount] = '.';
			lvi.pszText[nTextCount + 1] = '.';
			lvi.pszText[nTextCount + 2] = '.';
			lvi.pszText[nTextCount + 3] = 0;
		}

		// �ָ���������
		rTextClip.right += 4;

		// �����ı�
		//pDC->ExtTextOut(rTextClip.left + 2, rTextClip.top-1,
		//	ETO_CLIPPED | ETO_OPAQUE, &rTextClip, lvi.pszText,
		//	lstrlen(lvi.pszText), NULL);
		pDC->ExtTextOut(rTextClip.left + 2, rTextClip.top+1,
			ETO_CLIPPED | ETO_OPAQUE, &rTextClip, lvi.pszText,
			lstrlen(lvi.pszText), NULL);

		//���Ʊ����
		//      pDC->MoveTo(rTextClip.left+1, rTextClip.bottom-1);
		//      pDC->LineTo(rTextClip.right-1, rTextClip.bottom-1);
		//      pDC->MoveTo(rTextClip.right-1, rTextClip.bottom-1);
		//      pDC->LineTo(rTextClip.right-1, rTextClip.top-1);
	}

	// ������ɫ
	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
	{
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}

	// ������б�ѡ�����ѡ�����
	if (lpDrawItemStruct->itemState & ODS_FOCUS) 
	{
		CRect rTextClip(lpDrawItemStruct->rcItem);
		rTextClip.left += (plistImages ? 16 : 0);
		pDC->DrawFocusRect(&rTextClip);
	}

	// �ָ�ԭ�л���
	pDC->SelectObject(penOld);
}

void CDBQryView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR); 
	// TODO: Add your control notification handler code here
	*pResult = 0;

}

void CDBQryView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{  
	//ʵ����CListview�ĸ߶ȿ���
	lpMeasureItemStruct->itemHeight = 18;
} 
