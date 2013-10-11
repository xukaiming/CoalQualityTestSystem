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
		MessageBox(_T("RMReport.ocx尚未注册!\n无法打印报表!"),_T("警告"),MB_OK|MB_ICONWARNING);
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
	//清除所有标题
	do
	{
		ListCtrl->DeleteColumn(0);
	}
	while(ListCtrl->GetHeaderCtrl()->GetItemCount()>0); 
	//写字段标题
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
		//得到所有字段名称 
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
		FillList();			//填写表格
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
	//ReportSeleDlg.cFileName = _T("热容量*.rmf");
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
	// 取得显示设备描述表指针
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// 设置标志
	UINT uiFlags(ILD_TRANSPARENT);

	// 检查该行是否被选择
	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
	{
		//如果被被选择则将文字和背景的颜色设为高亮显示时的颜色
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		uiFlags |= ILD_BLEND50;
	}
	else 
	{
		//否则将将文字和背景的颜色设为正常颜色
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}

	

	// 得到将高亮显示的行
	LV_ITEM lvi;
	memset(&lvi, 0, sizeof(&lvi));
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lpDrawItemStruct->itemID;
	pListCtrl->GetItem(&lvi);
	 
	// 得到活动图形列表指针
	CImageList* plistImages =  pListCtrl->GetImageList(LVSIL_SMALL);
	// 绘制所选行的图形列表
	if (plistImages) {
		CPoint ptAt(lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top);
		if ( ! plistImages->Draw(pDC, 0//lvi.iImage
		, ptAt, uiFlags) ) {
			int x = 0;
			x++;
		}
	}
 

	// 创建用于绘制表格线的画笔
	CPen pen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
	CPen* penOld = pDC->SelectObject(&pen);

	TCHAR szText[256];     // 设置文本长度,如果文本长度较大,可以增加该数组的长度
	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(lvc));
	lvc.mask = LVCF_WIDTH;

	for (int nColumn = 0; pListCtrl->GetColumn(nColumn, &lvc); nColumn++) 
	{
		CRect rTextClip;

		// 如果是画第一列
		if (0 == nColumn) 
		{
			//画第一列时如果有图形列表则将绘制文本的矩形右移以空出绘制图形的空间
			rTextClip.left   = lpDrawItemStruct->rcItem.left +
				(plistImages ? 16 : 0);
			rTextClip.top    = lpDrawItemStruct->rcItem.top;
			rTextClip.right  = lpDrawItemStruct->rcItem.left + lvc.cx;
			rTextClip.bottom = lpDrawItemStruct->rcItem.bottom;
		}
		else 
		{
			// 如果不是第一列则将文本矩形右移至上一次文本矩形的右侧
			rTextClip.left  = rTextClip.right;
			rTextClip.right = rTextClip.left + lvc.cx;
		}

		// 获得文本
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = nColumn;
		lvi.pszText = szText;
		lvi.cchTextMax = sizeof(szText);
		pListCtrl->GetItem(&lvi);

		// 得到文本长度
		int nTextCount(lstrlen(lvi.pszText));

		//绘制文本
		CSize sizText(pDC->GetOutputTextExtent(lvi.pszText,
			nTextCount));

		//使文本矩形略小一些以使列间出现分隔,以免如果文本较长而造成列间文本连接起来
		rTextClip.right -= 4;

		//如果文本宽度大于绘制矩形的宽度
		if (sizText.cx >= rTextClip.Width()) 
		{

			// 获得...的长度
			CSize sizEllipse = pDC->GetOutputTextExtent(_T("..."), 3);

			// 制做较短的文本
			while (sizText.cx+sizEllipse.cx > (rTextClip.Width())  &&
				nTextCount > 1) 
		 {
			 lvi.pszText[--nTextCount] = 0;
			 sizText = pDC->GetOutputTextExtent(lvi.pszText,
				 nTextCount);
			}

			// 向文本后添加 "...".
			lvi.pszText[nTextCount] = '.';
			lvi.pszText[nTextCount + 1] = '.';
			lvi.pszText[nTextCount + 2] = '.';
			lvi.pszText[nTextCount + 3] = 0;
		}

		// 恢复整个矩形
		rTextClip.right += 4;

		// 绘制文本
		//pDC->ExtTextOut(rTextClip.left + 2, rTextClip.top-1,
		//	ETO_CLIPPED | ETO_OPAQUE, &rTextClip, lvi.pszText,
		//	lstrlen(lvi.pszText), NULL);
		pDC->ExtTextOut(rTextClip.left + 2, rTextClip.top+1,
			ETO_CLIPPED | ETO_OPAQUE, &rTextClip, lvi.pszText,
			lstrlen(lvi.pszText), NULL);

		//绘制表格线
		//      pDC->MoveTo(rTextClip.left+1, rTextClip.bottom-1);
		//      pDC->LineTo(rTextClip.right-1, rTextClip.bottom-1);
		//      pDC->MoveTo(rTextClip.right-1, rTextClip.bottom-1);
		//      pDC->LineTo(rTextClip.right-1, rTextClip.top-1);
	}

	// 重设颜色
	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
	{
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}

	// 如果该行被选则绘制选择矩形
	if (lpDrawItemStruct->itemState & ODS_FOCUS) 
	{
		CRect rTextClip(lpDrawItemStruct->rcItem);
		rTextClip.left += (plistImages ? 16 : 0);
		pDC->DrawFocusRect(&rTextClip);
	}

	// 恢复原有画笔
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
	//实现了CListview的高度控制
	lpMeasureItemStruct->itemHeight = 18;
} 
