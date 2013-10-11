// LryEDBQryView.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "LryEDBQryView.h"
#include "ModifyWeight.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryView

IMPLEMENT_DYNCREATE(CLryEDBQryView, CXTListView)

CLryEDBQryView::CLryEDBQryView()
	: CXTListView( )
{
	//{{AFX_DATA_INIT(CLryEDBQryView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLryEDBQryView::~CLryEDBQryView()
{
}

void CLryEDBQryView::DoDataExchange(CDataExchange* pDX)
{
	CXTListView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLryEDBQryView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLryEDBQryView, CXTListView)
	//ID_PRINT_REPORT_SETTING  //ID_FILE_PRINT_PREVIEW ID_FILE_PRINT
	ON_UPDATE_COMMAND_UI(ID_PRINT_REPORT_SETTING,OnUpdateMenuState )
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW,OnUpdateMenuState )
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,OnUpdateMenuState )

	ON_UPDATE_COMMAND_UI(ID_DELETE_RECORD,OnUpdateMenuState1 )
	ON_UPDATE_COMMAND_UI(IDC_E_MODIFY,OnUpdateMenuState1)
	//IDC_E_MODIFY
	//ON_UPDATE_COMMAND_UI(ID_MODIFY_RECORD)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV, OnUpdateCursorMove)
	
	//{{AFX_MSG_MAP(CLryEDBQryView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_E_MODIFY, OnEModify)
	ON_COMMAND(ID_PRINT_REPORT_SETTING, OnPrintReportSetting)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryView diagnostics

#ifdef _DEBUG
void CLryEDBQryView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CLryEDBQryView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}

CLryEDBQryDoc * CLryEDBQryView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLryEDBQryDoc)));
	return (CLryEDBQryDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryView message handlers
void CLryEDBQryView::FillList()
{
	int iStrLen = 0;
	int i		= 0;
	_variant_t  str;
	CStringArray ColNameArray;
	BeginWaitCursor() ;
	try
	{
		CLryEDBQryDoc* pDoc  = (CLryEDBQryDoc*)GetDocument();
		_RecordsetPtr pRs = pDoc->Rs;
			//得到所有字段名称
		GetAllFieldName(pRs,&ColNameArray);
		
		CListCtrl * ListCtrl = (CListCtrl  *)&GetListCtrl(); 
		ListCtrl->SetRedraw(FALSE);
		//清除所有数据
		
		ListCtrl->DeleteAllItems();
		//清除所有标题
		do
		{
			ListCtrl->DeleteColumn(0);
		}
		while(ListCtrl->GetHeaderCtrl()->GetItemCount()>0); 
		//写字段标题
		for(i=ColNameArray.GetSize()-1;i>=0;i--)
		{
			CDC *pDC = GetDC();
			iStrLen	= pDC->GetTextExtent(ColNameArray.GetAt(i)).cx*2;				
			ListCtrl->InsertColumn(0,ColNameArray.GetAt(i),LVCFMT_CENTER,iStrLen ,-1);
			ReleaseDC(pDC);	
		}
		//
		LONG RecordCount = 0;
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
					if(i==0)
						ListCtrl->InsertItem(0,VariantToStr(str));
					else
						ListCtrl->SetItemText(0,i,VariantToStr(str));
				}
				pRs->MovePrevious();
			}  
			pDoc->MoveFirst();
		}
		ListCtrl->SetRedraw(TRUE); 
		pDoc->UpdateAllViews(this);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	EndWaitCursor();
}

void CLryEDBQryView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	int iCursor;
	CListCtrl * ListCtrl = (CListCtrl *)&GetListCtrl();
	CLryEDBQryDoc *pDoc = (CLryEDBQryDoc *)GetDocument();
	iCursor = ListCtrl->GetNextItem(-1, LVNI_SELECTED );
	if(iCursor>-1)
		pDoc->Move(iCursor,(long)adBookmarkFirst);  
	*pResult = 0;
}

int CLryEDBQryView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1; 
	CListCtrl * ListCtrl = (CListCtrl  *)&GetListCtrl();	
	LONG style = GetWindowLong(ListCtrl->m_hWnd,GWL_STYLE);
	style |= LVS_REPORT|LVS_SHOWSELALWAYS |LVS_SINGLESEL;
	SetWindowLong(ListCtrl->m_hWnd,GWL_STYLE,style); 
	ListCtrl->SetExtendedStyle(ListCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	return 0;
}

void CLryEDBQryView::OnInitialUpdate() 
{
	CXTListView::OnInitialUpdate();
	//打印设置初始化
	LPCTSTR dbName = _T("HRDRECORD");

	if(pReportdlg.Create(_T(""),0,CRect(0,0,0,0),this,IDC_RMREPORTENGINE)==FALSE)
	{
		pReportdlg.DestroyWindow();
		MessageBox(_T("RMReport.ocx尚未注册!\n无法打印报表!"),_T("警告"),MB_OK|MB_ICONWARNING);
	}	
}

void CLryEDBQryView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl  *ListCtrl = (CListCtrl  *)&GetListCtrl();
	CLryEDBQryDoc *pDoc = (CLryEDBQryDoc *)GetDocument();
	CLryEDBQryDoc::CRowObj *rowInfo;
	int i;
	switch(lHint)
	{
	case 0:
		FillList();			//填写表格
		break;
	case 1:
		try
		{
			i = pDoc->Rs->AbsolutePosition-1;
			ListCtrl->EnsureVisible(i,TRUE);		
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
		break;
	case 2:
		rowInfo = (CLryEDBQryDoc::CRowObj*)pHint;
		ListCtrl->DeleteItem(rowInfo->RowNO);
		ListCtrl->Update(rowInfo->RowNO);
		break;
	default:
		break;
	}	
	
}

void CLryEDBQryView::OnUpdatDBDateTime(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
 
void CLryEDBQryView::OnDraw(CDC* pDC) 
{
	CDocument* pDoc = GetDocument();	
		
}

void CLryEDBQryView::OnUpdateMenuState(CCmdUI* pCmdUI)
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
  
void CLryEDBQryView::OnUpdateMenuState1(CCmdUI* pCmdUI)
{ 
	if( GetListCtrl().GetNextItem(-1,LVNI_SELECTED) ==-1)
	{ 
		pCmdUI->Enable( FALSE ); 
	} 
	else
	{
		pCmdUI->Enable( TRUE ); 
	}
}
 
 

void CLryEDBQryView::OnUpdateCursorMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here 
	int i ;
	CLryEDBQryDoc *pDoc = (CLryEDBQryDoc *)GetDocument();
	CListCtrl  *ListCtrl = (CListCtrl  *)&GetListCtrl();
	try
	{ 
		if(pDoc->Rs->GetRecordCount()>0)
		{
			i = pDoc->Rs->AbsolutePosition-1;
			ListCtrl->SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
		} 
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		UNREFERENCED_PARAMETER(e); // 避免編譯時的警告
	
	} 
 	pCmdUI->ContinueRouting();    

}
 

void CLryEDBQryView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu Menu;	
	CMenu *pMenu;
	Menu.LoadMenu(IDR_POPUP_E_DATABASE);
	pMenu = Menu.GetSubMenu(0);

	if(pReportdlg.m_hWnd==NULL)
	{
		pMenu->EnableMenuItem(ID_FILE_PRINT_PREVIEW,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_PRINT_REPORT_SETTING,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_FILE_PRINT_PREVIEW,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);	
	} 
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
		pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pOwner); 
		//弹出自己的右键菜单
	else
		CXTListView::OnContextMenu(pWnd,point); //弹出默认的滚动条菜单		
}

void CLryEDBQryView::OnEModify() 
{ 
	//#include "ModifyWeight.h"	
	CModifyWeight dlg; 
	int nItemIndex=-1;	//当前所选记录的索引  
	if((nItemIndex = GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		dlg.m_AutoNO = GetListCtrl().GetItemText(nItemIndex,0);
		dlg.DoModal();
	} 
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0); 
}

void CLryEDBQryView::OnPrintReportSetting() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if(pReportdlg.m_hWnd==NULL)
		return;
	LPCTSTR dbName = _T("        ");
	TCHAR path[1024];	
	GetCurrentDirectory(sizeof(path),path);
	_tcscat(path,_T("\\reports\\热容量试验结果报告报单.rmf"));
    dbName = path;
	pReportdlg.SetReportFile(dbName);
	pReportdlg.DesignReport();		
}

void CLryEDBQryView::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	CReportSele	ReportSeleDlg;
	CString ReportFile;
	CString strSQL;
	ReportSeleDlg.cFileName = _T("热容量*.rmf");
	if(ReportSeleDlg.DoModal()==IDOK)
	{
		if(ReportSeleDlg.cFile==_T("热容量平均试验结果报告单.rmf"))
		{
			AverageReport(&ReportSeleDlg);
			//RiBao(&ReportSeleDlg);
		}
		if(ReportSeleDlg.cFile==_T("热容量试验结果报告报单.rmf"))
		{ 
			SingleReport(&ReportSeleDlg);
			//SingleResult(&ReportSeleDlg);
		} 
		//查询当前日期,然后打开打印数据集 
		//if(ReportSeleDlg.cFile==_T("发热量试验结果日报表.rmf"))
	}	
}

void CLryEDBQryView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	OnFilePrintPreview();	
}

void CLryEDBQryView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnClick(pNMHDR,pResult);	
	*pResult = 0;
}

void CLryEDBQryView::AverageReport(CReportSele *pDlg)
{
	_variant_t strNO,strDev,strBombName;
	COleDateTime dt;
	CString strTime1,strTime2;
	int nItemIndex;
	CLryEDBQryDoc * pDoc = GetDocument();
	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		try
		{   
			pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
			strNO		= pDoc->Rs->GetCollect(_bstr_t("组号")); 
			strDev		= pDoc->Rs->GetCollect(_bstr_t("设备名称")); 
			strBombName = pDoc->Rs->GetCollect(_bstr_t("氧弹编号")); 
			dt			= pDoc->Rs->GetCollect(_bstr_t("试验日期")); 
			strTime1.Format(_T("#%d-%d-%d#"),dt.GetYear(),dt.GetMonth(),dt.GetDay());
			dt= MakeDays(1)+dt;
			strTime2.Format(_T("#%d-%d-%d#"),dt.GetYear(),dt.GetMonth(),dt.GetDay());
			CString	strSQL;
			strSQL.Format(_T("select * from 热容量 where 组号=%s and 设备名称='%s' and 氧弹编号='%s' and 试验日期>=%s and 试验日期<=%s"),VariantToStr(strNO),VariantToStr(strDev) ,VariantToStr(strBombName) ,strTime1 ,strTime2); 

			HRESULT hr; 	
			if(pDoc->RsPrintData->State==adStateOpen)
				pDoc->RsPrintData->Close();
			
			hr = pDoc->RsPrintData->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			TESTHR(hr);   
			
			if(pDoc->RsTitle->State==adStateOpen)
				pDoc->RsTitle->Close();
			strSQL = "select * from 系统参数";
			hr = GetDocument()->RsTitle->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			TESTHR(hr); 
			
			
			
			pReportdlg.Init(0);
			pReportdlg.SetModifyPrepared(TRUE);
			CString File(pDlg->cPath);
			File+=_T("reports\\")+pDlg->cFile;
			LPCTSTR path = File.GetBuffer(File.GetLength());
			pReportdlg.SetReportFile(path);
			File.ReleaseBuffer();
			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);
			LPCTSTR dbName1  = _T("SReportHeat");
			pReportdlg.AddAdoDataSet(dbName1,((CLryEDBQryDoc*)GetDocument())->RsPrintData);
			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);	
			LPCTSTR dbName2 = _T("CopyEmit");
			pReportdlg.AddAdoDataSet(dbName2,((CLryEDBQryDoc*)GetDocument())->RsTitle);
			if(pDlg->m_iSelect==0)
				pReportdlg.ShowReport();       //设计报表 
			else
				pReportdlg.DesignReport();		// 预览报表 
			
		}
		catch(_com_error &e)
		{ 
			dump_com_error(e);
		} 
	}
}

void CLryEDBQryView::SingleReport(CReportSele *pDlg)
{
	_variant_t str;
	int nItemIndex;
	CLryEDBQryDoc* pDoc = GetDocument();
	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
		str = pDoc->Rs->GetCollect(_bstr_t("试验编号")); 		 
		CString	strSQL;
		strSQL.Format(_T("select * from 热容量 where  试验编号='%s'"),VariantToStr(str)); 		
		try
		{ 
			
			HRESULT hr; 
			
			if(pDoc->RsPrintData->State==adStateOpen)
				pDoc->RsPrintData->Close();			
			hr = pDoc->RsPrintData->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			TESTHR(hr); 
			
			if(pDoc->RsTitle->State==adStateOpen)
				pDoc->RsTitle->Close();
			strSQL = _T("select * from 系统参数");
			hr = GetDocument()->RsTitle->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			TESTHR(hr); 
			
			pReportdlg.Init(0);
			pReportdlg.SetModifyPrepared(TRUE);	
			LPCTSTR dbName2 = _T("CopyHeat");
			pReportdlg.AddAdoDataSet(dbName2,((CLryEDBQryDoc*)GetDocument())->RsTitle);
			pReportdlg.SetModifyPrepared(TRUE);
			LPCTSTR dbName1  = _T("SReportHeat");
			pReportdlg.AddAdoDataSet(dbName1,((CLryEDBQryDoc*)GetDocument())->RsPrintData);
			//nItemIndex = pDoc->RsPrintData->GetRecordCount();	
			//pDoc->RsPrintData->MoveFirst();
			CString File(pDlg->cPath);
			File+=_T("reports\\")+pDlg->cFile;
			LPCTSTR path = File.GetBuffer(File.GetLength());
			pReportdlg.SetReportFile(path);
			File.ReleaseBuffer();
			if(pDlg->m_iSelect==0)
				pReportdlg.ShowReport();       //设计报表 
			else
				pReportdlg.DesignReport();		// 预览报表 
		}
		catch(_com_error &e)
		{ 
			dump_com_error(e);
		} 
	}	
}