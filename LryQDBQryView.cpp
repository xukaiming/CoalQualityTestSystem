// LryDBQryView.cpp : implementation file
//

#include "stdafx.h" 
#include "LRYCTRL.h"
#include "LryQDBQryView.h"
#include "LryQDBQryDoc.h"
#include "CReCalQ.h"
#include "CalGaoDiQDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryView

IMPLEMENT_DYNCREATE(CLryQDBQryView, CListView)

CLryQDBQryView::CLryQDBQryView()
	: CListView( )
{
	//{{AFX_DATA_INIT(CLryQDBQryView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLryQDBQryView::~CLryQDBQryView()
{
}

void CLryQDBQryView::DoDataExchange(CDataExchange* pDX)
{
	CListView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLryQDBQryView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLryQDBQryView, CListView)
	//{{AFX_MSG_MAP(CLryQDBQryView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PRINT_REPORT_SETTING, OnPrintReportSetting)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_ANALYSE_DANTONG, OnAnalyseDantong)
	ON_COMMAND(ID_ANALYSE_GAODIWEI, OnAnalyseGaoDiWei)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(ID_DELETE_RECORD,ID_ANALYSE_GAODIWEI,OnUpdateMenuAvgCheck ) 
	ON_UPDATE_COMMAND_UI(ID_PRINT_REPORT_SETTING,OnUpdateMenuState1 ) 
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW,OnUpdateMenuState1 ) 
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,OnUpdateMenuState1 ) 
	ON_UPDATE_COMMAND_UI(ID_AVERAGE_MENU,OnUpdateMenuAverage) 
	ON_UPDATE_COMMAND_UI_RANGE(ID_DELETE_RECORD,ID_ANALYSE_GAODIWEI,OnUpdateMenuState ) 
	//ON_UPDATE_COMMAND_UI(ID_MODIFY_RECORD)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV, OnUpdateCursorMove)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryView diagnostics

#ifdef _DEBUG
void CLryQDBQryView::AssertValid() const
{
	CListView::AssertValid();
}

void CLryQDBQryView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CLryQDBQryDoc * CLryQDBQryView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLryQDBQryDoc)));
	return (CLryQDBQryDoc*)m_pDocument;
}

#endif //_DEBUG

 
/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryView message handlers

void CLryQDBQryView::FillList()
{
	int i =0;
	int iStrLen = 0;
	_variant_t  str;
	CStringArray ColNameArray;
	CString strData;
	BeginWaitCursor() ;
	try
	{
		CLryQDBQryDoc* pDoc  = (CLryQDBQryDoc*)GetDocument();
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
		for(  i=ColNameArray.GetSize()-1;i>=0;i--)
		{
			CDC *pDC = GetDC();
			iStrLen	= pDC->GetTextExtent(ColNameArray.GetAt(i)).cx+10;		 
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
		ListCtrl->SetRedraw(TRUE); 
		pDoc->UpdateAllViews(this);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	EndWaitCursor();
}

void CLryQDBQryView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
	CListCtrl  *ListCtrl = (CListCtrl  *)&GetListCtrl();
	CLryQDBQryDoc *pDoc = (CLryQDBQryDoc *)GetDocument();
	CLryQDBQryDoc::CRowObj *rowInfo;
	int i;
	switch(lHint)
	{
	case 0:
			FillList();			//填写表格 
			break;
	case 1:
		i = pDoc->Rs->AbsolutePosition-1;
		ListCtrl->EnsureVisible(i,TRUE);
		break;
	case 2:
		rowInfo = (CLryQDBQryDoc::CRowObj*)pHint;
		ListCtrl->DeleteItem(rowInfo->RowNO);
		ListCtrl->Update(rowInfo->RowNO);
		break;
	default:break;
	}
}



int CLryQDBQryView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CListCtrl * ListCtrl = (CListCtrl  *)&GetListCtrl();	
	LONG style = GetWindowLong(ListCtrl->m_hWnd,GWL_STYLE);
	style |= LVS_REPORT|LVS_SHOWSELALWAYS |LVS_SINGLESEL;
	SetWindowLong(ListCtrl->m_hWnd,GWL_STYLE,style); 
	ListCtrl->SetExtendedStyle(ListCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	return 0;
}

void CLryQDBQryView::OnUpdateMenuState(CCmdUI* pCmdUI)
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

void CLryQDBQryView::OnUpdateMenuState1(CCmdUI* pCmdUI)
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


void CLryQDBQryView::OnUpdateMenuAvgCheck(CCmdUI* pCmdUI)
{
	CString strAvr ;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	int index = pFrame->m_wndComboBoxAvr->GetCurSel();
	pFrame->m_wndComboBoxAvr->GetLBText( index, strAvr);
	if((strAvr==_T("原始记录")&&( GetListCtrl().GetNextItem(-1,LVNI_SELECTED) !=-1)))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
void CLryQDBQryView::OnUpdateMenuAverage(CCmdUI* pCmdUI)
{
	CString strAvr ;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	int index = pFrame->m_wndComboBoxAvr->GetCurSel();
	pFrame->m_wndComboBoxAvr->GetLBText( index, strAvr);
	if(strAvr==_T("平均记录"))
	{
		pCmdUI->SetText(_T("原始记录"));
	}
	else
	{
		pCmdUI->SetText(_T("平均记录"));
	}
	if( GetListCtrl().GetNextItem(-1,LVNI_SELECTED) !=-1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CLryQDBQryView::OnUpdateCursorMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here 
	int i ;
	CLryQDBQryDoc *pDoc = (CLryQDBQryDoc *)GetDocument();
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

void CLryQDBQryView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iCursor;
	CListCtrl * ListCtrl = (CListCtrl *)&GetListCtrl();
	CLryQDBQryDoc *pDoc = (CLryQDBQryDoc *)GetDocument();
	iCursor = ListCtrl->GetNextItem(-1, LVNI_SELECTED );
	if(iCursor>-1)
		pDoc->Move(iCursor,(long)adBookmarkFirst); 
//	 
	*pResult = 0;
}

 

void CLryQDBQryView::OnDraw(CDC* pDC) 
{
	
	CDocument* pDoc = GetDocument();	
	
}

 

 

void CLryQDBQryView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	
	//打印设置初始化
	LPCTSTR dbName = _T("HRDRECORD");

	if(pReportdlg.Create(_T(""),0,CRect(0,0,0,0),this,IDC_RMREPORTENGINE)==FALSE)
	{
		pReportdlg.DestroyWindow();
		MessageBox(_T("RMReport.ocx尚未注册!\n无法打印报表!"),_T("警告"),MB_OK|MB_ICONWARNING);
	}
 
}

void CLryQDBQryView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu Menu;	
	CMenu *pMenu;
	Menu.LoadMenu(IDR_POPUP_Q_DATABASE);
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
		CListView::OnContextMenu(pWnd,point); //弹出默认的滚动条菜单	
}

void CLryQDBQryView::OnPrintReportSetting() 
{
	// TODO: Add your command handler code here
	if(pReportdlg.m_hWnd==NULL)
		return;
	LPCTSTR dbName = _T("        ");
	TCHAR path[1024];	
	GetCurrentDirectory(sizeof(path),path);
	_tcscat(path,_T("\\reports\\发热量试验结果报告单.rmf"));
    dbName = path;
	pReportdlg.SetReportFile(dbName);
	pReportdlg.DesignReport();			
}

void CLryQDBQryView::OnFilePrintPreview() 
{ 
	CReportSele	ReportSeleDlg;
	CString ReportFile;
	CString strSQL;
	ReportSeleDlg.cFileName = _T("发热量*.rmf");
	if(ReportSeleDlg.DoModal()==IDOK)
	{
 		if(ReportSeleDlg.cFile==_T("发热量试验结果日报表.rmf"))
		{
			RiBao(&ReportSeleDlg);
		}
		if(ReportSeleDlg.cFile==_T("发热量试验结果报告单.rmf"))
		{ 
			SingleResult(&ReportSeleDlg);
		}
 		if(ReportSeleDlg.cFile==_T("发热量平行样试验结果报告单.rmf"))
		{ 
			PXResult(&ReportSeleDlg);
		}
		//查询当前日期,然后打开打印数据集 
		//if(ReportSeleDlg.cFile==_T("发热量试验结果日报表.rmf"))
	}	
}

void CLryQDBQryView::RiBao(CReportSele *pDlg)
{
	_variant_t str;
	int nItemIndex;
	CLryQDBQryDoc* pDoc = (CLryQDBQryDoc*)GetDocument();
 	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		//pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
		str = pDoc->Rs->GetCollect(_bstr_t("试验日期"));
		COleDateTime TestTime(str);
		CString StrDateStart = TestTime.Format(_T("%Y-%m-%d"));
		TestTime=TestTime+MakeDays(1);
		CString StrDateEnd    = TestTime.Format(_T("%Y-%m-%d"));
		CString	strSQL;
		strSQL.Format(_T("select * from 发热量 where 试验日期>=#%s# and 试验日期<#%s# ORDER BY 试验编号" ),StrDateStart,StrDateEnd); 
		try
		{   
			//报表栏
			HRESULT hr; 	
			if(pDoc->RsPrintData->State==adStateOpen)
				pDoc->RsPrintData->Close();

			hr = pDoc->RsPrintData->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
								adOpenStatic,
								adLockOptimistic,
								-1);
			TESTHR(hr);   
			//报表标题栏
			if(pDoc->RsTitle->State==adStateOpen)
				pDoc->RsTitle->Close();
			strSQL = "select * from 系统参数";
			hr = GetDocument()->RsTitle->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);
			TESTHR(hr); 
			//nItemIndex = pDoc->RsPrintData->GetRecordCount();	
			//初始化
			pReportdlg.Init(0); 
			//报表模板文件
			CString File(pDlg->cPath);
			File+=_T("reports\\")+pDlg->cFile;
			LPCTSTR path = File.GetBuffer(File.GetLength());
			pReportdlg.SetReportFile(path);
			File.ReleaseBuffer();

			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);
			LPCTSTR dbName1  = _T("SReportEmit");
			pReportdlg.AddAdoDataSet(dbName1,((CLryQDBQryDoc*)GetDocument())->RsPrintData);
			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);	
			LPCTSTR dbName2 = _T("CopyEmit");
			pReportdlg.AddAdoDataSet(dbName2,((CLryQDBQryDoc*)GetDocument())->RsTitle);

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

void CLryQDBQryView::SingleResult(CReportSele *pDlg)
{
	_variant_t str;
	int nItemIndex;
	CString	strSQL;
	CLryQDBQryDoc* pDoc = GetDocument();
 	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		try
		{ 			
			HRESULT hr; 
			pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
			try
			{
				str = pDoc->Rs->GetCollect(_bstr_t("试验编号")); 
			}
			catch(_com_error &e)
			{ 
				//dump_com_error(e);
				AfxMessageBox(_T("请选择原始记录"),MB_OK+MB_ICONINFORMATION); 
				return;
			} 

			strSQL.Format(_T("select * from 发热量 where  试验编号='%s'"),VariantToStr(str)); 		

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
			LPCTSTR dbName2 = _T("CopyEmit");
			pReportdlg.AddAdoDataSet(dbName2,((CLryQDBQryDoc*)GetDocument())->RsTitle);
			pReportdlg.SetModifyPrepared(TRUE);
			LPCTSTR dbName1  = _T("SReportEmit");
			pReportdlg.AddAdoDataSet(dbName1,((CLryQDBQryDoc*)GetDocument())->RsPrintData);
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

void CLryQDBQryView::PXResult(CReportSele *pDlg)
{
	_variant_t str;
	int nItemIndex;
	CLryQDBQryDoc * pDoc = GetDocument();
 	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
		str = pDoc->Rs->GetCollect(_bstr_t("平行样号")); 
		CString	strSQL;
		strSQL.Format(_T("select * from 发热量 where 平行样号=%s"),VariantToStr(str) ); 
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
			LPCTSTR dbName1  = _T("SReportEmit");
			pReportdlg.AddAdoDataSet(dbName1,((CLryQDBQryDoc*)GetDocument())->RsPrintData);
			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);	
			LPCTSTR dbName2 = _T("CopyEmit");
			pReportdlg.AddAdoDataSet(dbName2,((CLryQDBQryDoc*)GetDocument())->RsTitle);
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

void CLryQDBQryView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_COMMAND,ID_FILE_PRINT_PREVIEW,NULL);
}

void CLryQDBQryView::OnAnalyseDantong() 
{
	// TODO: Add your command handler code here

	CCReCalQ ReCalQDlg;
	int nItemIndex=-1;	//当前所选记录的索引 
	if((nItemIndex = GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		ReCalQDlg.m_szTestNum = GetListCtrl().GetItemText(nItemIndex,0);
		ReCalQDlg.DoModal();
	}
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0);
}

void CLryQDBQryView::OnAnalyseGaoDiWei()
{
	CCalGaoDiQDlg  dlg; 
	int nItemIndex=-1;	//当前所选记录的索引  
	if((nItemIndex = GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		dlg.m_szTestNum = GetListCtrl().GetItemText(nItemIndex,0);
		dlg.DoModal();
	} 
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0);
}

void CLryQDBQryView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnClick(pNMHDR,  pResult) ;
	*pResult = 0;
}

CString CLryQDBQryView::DBVariantToStr(_variant_t var, CString ParamName)
{
	VARTYPE vt = var.vt;
	CString strResult("空白");
	if(ParamName==_T("平均弹筒发热量"))
	{
		strResult.Format(_T("%.0f"),V_R8(&var));
	}
	else if(ParamName==_T("平均高位发热量"))
	{
		strResult.Format(_T("%.0f"),V_R8(&var));
	}
	else if(ParamName==_T("平均干基高位发热量"))
	{
		strResult.Format(_T("%.0f"),V_R8(&var));
	}
	else if(ParamName==_T("平均低位发热量"))
	{
		strResult.Format(_T("%.0f"),V_R8(&var));
	}
	else 
	{
		strResult = VariantToStr(var);
	} 
	return strResult;
}


