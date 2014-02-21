// DBQrySView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DBQrySView.h"
#include "ReportSele.h"

#include "CalCoffDlg.h"

#include "ClyRDB.h"
#include "ReCalS.h"
#include "MainFrm.h"
//#include "DBQryDocument.h"
// CDBQrySView

IMPLEMENT_DYNCREATE(CDBQrySView, CDBQryView)

CDBQrySView::CDBQrySView()
{

}

CDBQrySView::~CDBQrySView()
{
}

BEGIN_MESSAGE_MAP(CDBQrySView, CDBQryView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CAL_COFF, &CDBQrySView::OnCalCoff)
	ON_COMMAND(ID_ANALYSE_RECAL, &CDBQrySView::OnAnalyseRecal)
	ON_UPDATE_COMMAND_UI(ID_DELETE_RECORD,OnUpdateMenuState1) 
	ON_UPDATE_COMMAND_UI(ID_ANALYSE_RECAL,OnUpdateMenuState1) 
	ON_UPDATE_COMMAND_UI(ID_CAL_COFF,OnUpdateMenuState1) 
//	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// CDBQrySView diagnostics

#ifdef _DEBUG
void CDBQrySView::AssertValid() const
{
	CDBQryView::AssertValid();
}

#ifndef _WIN32_WCE
void CDBQrySView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBQrySView message handlers
 

CString CDBQrySView::DBVariantToStr(_variant_t var,CString ParamName)
{
	VARTYPE vt = var.vt;
	CString strResult("空白");
	if(ParamName==_T("样品质量"))
	{
		strResult.Format(_T("%.1f"),V_R8(&var));
	}
	else if(ParamName==_T("水分"))
	{
		strResult.Format(_T("%.3f%%"),V_R8(&var));
	} 
	else if(ParamName==_T("全硫Stad"))
	{
		strResult.Format(_T("%.3f%%"),V_R8(&var));
	}
	else if(ParamName==_T("全硫Std"))
	{
		strResult.Format(_T("%.3f%%"),V_R8(&var));
	} 
	else if(ParamName==_T("分析时间"))  //电量
	{
		strResult = COleDateTime(var).Format(_T("%M:%S"));
	}
	else if(ParamName==_T("电量"))  //试验日期
	{ 
		strResult.Format(_T("%.2f"),V_R4(&var)); 
	}
	else if(ParamName==_T("修正"))  //试验日期
	{
		strResult  = V_BOOL(&var) ? _T("是") : _T("否");
	}
	else

	{
		strResult = VariantToStr(var);
	}
	
	return strResult;
}

// 重载该函数以将所选行的整行高亮显示.
void CDBQrySView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDBQryView::DrawItem(lpDrawItemStruct); 
}

void CDBQrySView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu Menu;	
	CMenu *pMenu;
	Menu.LoadMenu(IDR_POPUP_S_DATABASE);
	pMenu = Menu.GetSubMenu(0);

	/*
	if(pReportdlg.m_hWnd==NULL)
	{
		pMenu->EnableMenuItem(ID_FILE_PRINT_PREVIEW,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_PRINT_REPORT_SETTING,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_FILE_PRINT_PREVIEW,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);	
	} 
	*/
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
		CDBQryView::OnContextMenu(pWnd,point); //弹出默认的滚动条菜单
}


void CDBQrySView::FileFilter()
{
	ReportSeleDlg.cFileName = _T("测硫仪*.rmf");
}

void CDBQrySView::ParsarReport()
{

	if(ReportSeleDlg.cFile==_T("测硫仪分析报表.rmf"))
	{ 
		AverageReport(&ReportSeleDlg); 
	}
	if(ReportSeleDlg.cFile==_T("测硫仪日报表.rmf"))
	{ 
		RiBao(&ReportSeleDlg);
	} 
	//查询当前日期,然后打开打印数据集 
	//if(ReportSeleDlg.cFile==_T("发热量试验结果日报表.rmf"))
}
// //
void CDBQrySView::AverageReport(CReportSele * pDlg)
{
	_variant_t strNO,strDev,strBombName,strSampleName;
	COleDateTime dt;
	CString strTime1,strTime2;
	int nItemIndex;
	long RecordCount;
	CDBQryDocument * pDoc = GetDocument();
	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		try
		{   
			pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
			strNO		= pDoc->Rs->GetCollect(_bstr_t("平行样号")); 
			strDev		= pDoc->Rs->GetCollect(_bstr_t("设备名称"));  
			dt			= pDoc->Rs->GetCollect(_bstr_t("试验日期")); 
			strSampleName = pDoc->Rs->GetCollect(_bstr_t("样品编号")); 
			strTime1.Format(_T("#%d-%d-%d#"),dt.GetYear(),dt.GetMonth(),dt.GetDay());
			dt= MakeDays(1)+dt;
			strTime2.Format(_T("#%d-%d-%d#"),dt.GetYear(),dt.GetMonth(),dt.GetDay());
			CString	strSQL;
			strSQL.Format(_T("select * from 测硫仪实验数据 where 平行样号=%s and 设备名称='%s' and 试验日期>=%s and 试验日期<=%s and 样品编号 ='%s'"),VariantToStr(strNO),VariantToStr(strDev) ,strTime1 ,strTime2,VariantToStr(strSampleName)); 

			HRESULT hr; 	
			if(pDoc->RsPrintData->State==adStateOpen)
				pDoc->RsPrintData->Close();

			hr = pDoc->RsPrintData->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			TESTHR(hr);   
			RecordCount = pDoc->RsPrintData->GetRecordCount();

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
			LPCTSTR dbName1  = _T("SReport");
			pReportdlg.AddAdoDataSet(dbName1,((CDBQryDocument*)GetDocument())->RsPrintData);
			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);	 
			LPCTSTR dbName2 = _T("CopyCly");
			pReportdlg.AddAdoDataSet(dbName2,((CDBQryDocument*)GetDocument())->RsTitle);
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

void CDBQrySView::RiBao(CReportSele * pDlg)
{
	_variant_t str;
	int nItemIndex;
	CDBQryDocument* pDoc = (CDBQryDocument*)GetDocument();
	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		//pDoc->Rs->Move(nItemIndex,(long)adBookmarkFirst);
		str = pDoc->Rs->GetCollect(_bstr_t("试验日期"));
		COleDateTime TestTime(str);
		CString StrDateStart = TestTime.Format(_T("%Y-%m-%d"));
		TestTime=TestTime+MakeDays(1);
		CString StrDateEnd    = TestTime.Format(_T("%Y-%m-%d"));
		CString	strSQL;
		strSQL.Format(_T("select * from 测硫仪实验数据 where 试验日期>=#%s# and 试验日期<#%s# ORDER BY 试验编号" ),StrDateStart,StrDateEnd); 
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
			LPCTSTR dbName1  = _T("SReport");
			pReportdlg.AddAdoDataSet(dbName1,((CDBQryDocument*)GetDocument())->RsPrintData);
			// 增加一个数据表 
			pReportdlg.SetModifyPrepared(TRUE);	
			LPCTSTR dbName2 = _T("CopyCly");
			pReportdlg.AddAdoDataSet(dbName2,((CDBQryDocument*)GetDocument())->RsTitle);

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

void CDBQrySView::OnUpdateMenuState1(CCmdUI* pCmdUI)
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
void CDBQrySView::OnCalCoff()
{ 
	CCalCoffDlg calDlg; 
	calDlg.pParentListView = this;
	calDlg.pParentListCtrl = &GetListCtrl();

	CString strSQL ;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strDev = _T("1#量热仪");
	int index = pFrame->m_wndComboBoxDevice->GetCurSel();
	pFrame->m_wndComboBoxDevice->GetLBText( index, strDev);
	if(strDev.Find(_T("测硫仪"),0)==-1)
	{ 
		MessageBox(_T("请将查询设备修改为\n你要修改系数的仪器"),_T("警告"),MB_ICONWARNING+MB_OK);
		return;
	} 
	calDlg.DoModal();
}

void CDBQrySView::OnAnalyseRecal()
{ 
	CClyRDB clyRdb;
	int nItemIndex;
	//clyRdb.InitRDB()
	CString strDevType;
	CString strAutoNO;
	CString strDeviceName;
	strDevType.LoadString(ID_STR_DEV_CLY);
	// 
	CReCalS dlg;
	dlg.pClyRdb = &clyRdb;
	//GetItemText()
	if(( nItemIndex=GetListCtrl().GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		strDeviceName	= GetItemText(nItemIndex,_T("设备名称"));
		strAutoNO		= GetItemText(nItemIndex,_T("试验编号"));
		clyRdb.LoadParamFromDB(strDeviceName,strDevType); 
		clyRdb.result.LoadResultFromDB(strAutoNO);
		if(ID_OK==dlg.DoModal())
		{
			clyRdb.result.UpdateResultToDB(); 
			::SendMessage(m_hWnd,WM_COMMAND,ID_RECORD_REFRESH,0);
			//GetListCtrl().SetFocus();
			GetListCtrl().SetItemState(nItemIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		}
		 
	}
	
}

