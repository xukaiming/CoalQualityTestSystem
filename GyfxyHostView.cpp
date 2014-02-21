// GyfxyHostView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyHostView.h"


// CGyfxyHostView

IMPLEMENT_DYNCREATE(CGyfxyHostView, CFormView) 
CGyfxyHostView::CGyfxyHostView()
: CFormView(CGyfxyHostView::IDD)
{
}

CGyfxyHostView::CGyfxyHostView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	pGyfxyHostCtrl = new CGyfxyHostCtrl();
	pGyfxyHostCtrl->m_pOwner = this;
}

CGyfxyHostView::~CGyfxyHostView()
{
	delete pGyfxyHostCtrl;
}

void CGyfxyHostView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_STATE, m_ProgBar); 
	DDX_Control(pDX, IDC_LIST_RESULT, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CGyfxyHostView, CFormView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CALC_DATA,OnCalcData)
	ON_MESSAGE(WM_IMAGE_DATA_CHANGE,OnImageDataChange)
	ON_MESSAGE(WM_SAMPLE_QUALITY_UPDATE,OnUpdateQuality)
	ON_MESSAGE(WM_START_PROGBAR,OnStartProgBar) 
	ON_BN_CLICKED(IDC_TEST_START, OnBnClickedTestStart)
	ON_WM_DESTROY() 
	ON_BN_CLICKED(ID_WEIGHT_POT,  OnBnClickedWeightPot)
	ON_BN_CLICKED(ID_WEIGHT_SAMPLE,  OnBnClickedWeightSample) 
	ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_RESULT, &CGyfxyHostView::OnNMCustomdrawListResult)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnCheckbox)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_NO,  OnBnClickedButtonAutoNo)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE_UD, OnBnClickedButtonSampleUd)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE_DN,  OnBnClickedButtonSampleDn)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_RESULT,OnLvnColumnclickListResult)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TEST_STOP, OnBnClickedTestStop) 
END_MESSAGE_MAP()


// CGyfxyHostView diagnostics

#ifdef _DEBUG
void CGyfxyHostView::AssertValid() const
{
	CFormView::AssertValid();
}


CGyfxyHostDoc  * CGyfxyHostView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGyfxyHostDoc)));
	return (CGyfxyHostDoc*)m_pDocument;
} 

#ifndef _WIN32_WCE
void CGyfxyHostView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
} 
#endif
#else
CGyfxyHostDoc  * CGyfxyHostView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGyfxyHostDoc)));
	return (CGyfxyHostDoc*)m_pDocument;
} 
#endif //_DEBUG


// CGyfxyHostView message handlers

void CGyfxyHostView::OnInitialUpdate()
{	
	CGyfxyHostDoc *pDoc	= GetDocument();	
	pDoc->pHostCtrl		= pGyfxyHostCtrl;
	if(!pGyfxyHostCtrl->InitDevice(pDoc->pRDB))
	{
#ifndef _DEBUG 
		//初始化设备不成功100ms后关闭窗口，不能在此直接关闭窗口。
		SetTimer(TIME_EVENT_CLOSEWINDOWS,1000,NULL);		//1000 mS
#endif		 
	} 
	pRdb = pGyfxyHostCtrl->pRdb;
	pImg = pGyfxyHostCtrl->pImageSlave;
	pSample = pGyfxyHostCtrl->pRdb->SampleArray.Sample;
	////////////////////////////////////////////////////////////
	CFormView::OnInitialUpdate();
	/////////////////////////////////////////////////////////////
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
	InitUI();
	//SendMessage(WM_COMMAND,IDC_BUTTON_SAMPLE_DN,0);
	/////////////////////////////////
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

void CGyfxyHostView::InitUI()
{
#ifdef _DEBUG	
	GetDlgItem(ID_WEIGHT_SAMPLE)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEST_START)->EnableWindow(TRUE);
#endif 
	InitListHeadCtrl(); 
	int rowcnt = pRdb->attrib.m_btMaxSampleCnt;
	pRdb->SampleArray.SetDefaultSample(m_ListCtrl,rowcnt);
}

void CGyfxyHostView::InitListHeadCtrl(void)
{
	int i =0;
	int iStrLen = 0;
	HRESULT hr= S_OK;
	_variant_t  str;
	CStringArray ColNameArray;
	BeginWaitCursor() ;
	try
	{ 
		_RecordsetPtr Rs ;
		CString strSQL;
		Rs.CreateInstance("ADODB.Recordset");
		strSQL = _T("select * from 工业分析仪数据");
		if(Rs->State==adStateOpen)
			Rs->Close();
		//IADORecordBindingPtr pIRB(pRsIED);
		hr = Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	
		TESTHR(hr);
		//得到所有字段名称
		GetAllFieldName(Rs,&ColNameArray); 
		SetRedraw(FALSE);
		//清除所有数据
		DWORD ExStyle = m_ListCtrl.GetExtendedStyle() ;
		m_ListCtrl.SetExtendedStyle(ExStyle| LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//
		m_ListCtrl.EnableToolTips(TRUE); 
		m_ListCtrl.DeleteAllItems();
		//清除所有标题
		do
		{
			m_ListCtrl.DeleteColumn(0);
		}
		while(m_ListCtrl.GetHeaderCtrl()->GetItemCount()>0); 
		//////////////////////////////////////////////////////////// 
		//写字段标题
		CRect rect;
		m_ListCtrl.GetWindowRect(&rect);

		int w = rect.Width() - 2;
		int i;
		int total_cx = 0;
		LV_COLUMN lvcolumn;
		memset(&lvcolumn, 0, sizeof(lvcolumn));

		w = w - ::GetSystemMetrics(SM_CXVSCROLL);	// width of vertical scroll bar

		// add columns
		for (i = 0;i<ColNameArray.GetSize() ; i++)
		{ 

			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.pszText = (TCHAR*)(LPCTSTR)ColNameArray.GetAt(i);
			lvcolumn.fmt =  LVCFMT_CENTER;

			lvcolumn.iSubItem = i;
			lvcolumn.cx = (i==ColNameArray.GetSize()-1) ? w - total_cx - 2 : (w * 8) / 64;
			total_cx += lvcolumn.cx;
			m_ListCtrl.InsertColumn(i, &lvcolumn);
		}

		///////////////////////////////////////////////////////////////
		int rowcnt = pRdb->attrib.m_btMaxSampleCnt;
		for(int i=0;i<rowcnt;i++)        //增加24个样品
		{			
			m_ListCtrl.InsertItem(i,_T(""));		
			//增加样位好
			CString str(_T("")) ;
			str.Format(_T("%d"),i+1);
			m_ListCtrl.SetItemText(i,1,str);
			//增加Edit CheckBox
			m_ListCtrl.SetEdit(i, 0); 
			m_ListCtrl.SetItemText(i,0,_T("样品编号"));

		}
		
		/////////////////////////////////////////////////////////////
		// create the image list from bitmap resource
		VERIFY(m_ListCtrl.m_cImageList.Create(18, 18, ILC_COLOR24, 3, 1));
		CBitmap bm;
		bm.LoadBitmap(IDB_CHECKBOXES);
		m_ListCtrl.m_cImageList.Add(&bm, RGB(255, 0, 255));
		m_ListCtrl.m_HeaderCtrl.SetImageList(&m_ListCtrl.m_cImageList);
		////CheckBox
		HDITEM hditem;
		TCHAR szText[40];
		hditem.pszText = szText;
		hditem.cchTextMax = 40;
		for (i = 0; i < m_ListCtrl.m_HeaderCtrl.GetItemCount(); i++)
		{
			hditem.mask = HDI_IMAGE | HDI_FORMAT|HDI_TEXT;
			m_ListCtrl.m_HeaderCtrl.GetItem(i, &hditem);
			hditem.fmt |=  HDF_IMAGE;
			if(CString(szText)==_T("水分")
				||CString(szText)==_T("挥发分")
				||CString(szText)==_T("灰分"))  
				hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
			else
				hditem.iImage = XHEADERCTRL_NO_IMAGE;

			m_ListCtrl.m_HeaderCtrl.SetItem(i, &hditem);
		} 
		//////////////////////////////////////设置字体高度
		CFont* pFont = (CFont*)m_ListCtrl.GetFont();
		if (pFont) {
			LOGFONT lf;
			pFont->GetLogFont(&lf);
			lf.lfHeight = -16;
			m_listFont.CreateFontIndirect(&lf);
			m_ListCtrl.SetFont(&m_listFont);
		} 
		///////////////////////////////////////////////////
		SetRedraw(TRUE);                    

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	EndWaitCursor();
}


void CGyfxyHostView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CWnd *pListCtrl		= GetDlgItem(IDC_LIST_RESULT);
	CWnd *pProgWnd		= GetDlgItem(IDC_PROGRESS_STATE); 
	/////////////////////////////////////////////////////////
	CWnd *pButStart		= GetDlgItem(IDC_TEST_START); 
	CWnd *pButStop		= GetDlgItem(IDC_TEST_STOP);
	CWnd *pButClose		= GetDlgItem(ID_FILE_CLOSE);

	CWnd *pButRst		= GetDlgItem(IDC_RESET); 
	CWnd *pButWPot 		= GetDlgItem(ID_WEIGHT_POT); 
	CWnd *pButWsample	= GetDlgItem(ID_WEIGHT_SAMPLE);  

	CWnd *pAutoNO		= GetDlgItem(IDC_BUTTON_AUTO_NO);
	CWnd *pDownload		= GetDlgItem(IDC_BUTTON_SAMPLE_DN);
	CWnd *pUpload		= GetDlgItem(IDC_BUTTON_SAMPLE_UD);

	CWnd *pLeftTempText		= GetDlgItem(IDC_STATIC_LEFT_TEMP);
	CWnd *pRightTempText	= GetDlgItem(IDC_STATIC_RIGHT_TEMP);
	CWnd *pWaterTempText	= GetDlgItem(IDC_STATIC_WATER_TEMP);
	CWnd *pInfo				= GetDlgItem(IDC_STATIC_INFO);

	if(pProgWnd!=NULL)
	{
		RECT rect, rectProgress;
		RECT rectStart,rectStop,rectClose,rectRst,rectWPot,rectWsample,rectList; 
		RECT rectAuto,rectDownload,rectUpload;

		RECT rectLeftTemp,rectRightTemp,rectWaterTemp,rectInfo;


		int iHeight;
		GetClientRect(&rect);
		pListCtrl->GetWindowRect(&rectList);
		pProgWnd->GetWindowRect(&rectProgress);  
		/////////////////////////////////////////
		pButStart->GetWindowRect(&rectStart); 
		pButClose->GetWindowRect(&rectClose); 
		pButRst->GetWindowRect(&rectRst);
		pButWPot->GetWindowRect(&rectWPot);
		pButWsample->GetWindowRect(&rectWsample); 

		pAutoNO->GetWindowRect(&rectAuto);
		pDownload->GetWindowRect(&rectDownload);
		pUpload->GetWindowRect(&rectUpload);


		pLeftTempText->GetWindowRect(&rectLeftTemp);		
		pRightTempText->GetWindowRect(&rectRightTemp);			
		pWaterTempText->GetWindowRect(&rectWaterTemp);			
		pInfo->GetWindowRect(&rectInfo);					
		/////////////////////////////////////////////
		ScreenToClient(&rectList);
		ScreenToClient(&rectProgress);  
		ScreenToClient(&rectStart); 
		ScreenToClient(&rectClose); 

		ScreenToClient(&rectRst);
		ScreenToClient(&rectWPot);
		ScreenToClient(&rectWsample); 

		ScreenToClient(&rectAuto);
		ScreenToClient(&rectDownload);
		ScreenToClient(&rectUpload); 

		ScreenToClient(&rectLeftTemp); 
		ScreenToClient(&rectRightTemp );
		ScreenToClient(&rectWaterTemp);
		ScreenToClient(&rectInfo); 
		///////////////////////////////////////////////////
		//按钮
		int iButHigh,iButWidth;
		iButHigh	= rectStart.bottom - rectStart.top;
		iButWidth	= rectStart.right  - rectStart.left;
		//
		rectRst.left		= 40;
		rectRst.right		= 40+iButWidth;
		rectRst.bottom		= rect.bottom-10;
		rectRst.top			= rect.bottom-10-iButHigh;
		pButRst->MoveWindow(&rectRst);
		pButRst->Invalidate(FALSE);

		//
		rectClose.right		= rect.right-40;
		rectClose.left		= rect.right-40-iButWidth;
		rectClose.bottom	= rect.bottom-10;;
		rectClose.top		=  rect.bottom-10-iButHigh;
		pButClose->MoveWindow(&rectClose);
		pButClose->Invalidate(FALSE);
		//称坩埚		 
		rectWPot.left		= 40+(rectClose.left-rectRst.left)/5;
		rectWPot.right		= rectWPot.left	+iButWidth;
		rectWPot.bottom		= rect.bottom-10;
		rectWPot.top		= rect.bottom-10-iButHigh;
		pButWPot->MoveWindow(&rectWPot);
		pButWPot->Invalidate(FALSE);
		//称样品
		rectWsample.left		= 40+(rectClose.left-rectRst.left)*2/5;
		rectWsample.right		= rectWsample.left	+iButWidth;
		rectWsample.bottom		= rect.bottom-10;
		rectWsample.top			= rect.bottom-10-iButHigh;
		pButWsample->MoveWindow(&rectWsample);
		pButWsample->Invalidate(FALSE);
		//		
		rectStart.left		= 40+(rectClose.left-rectRst.left)*3/5;
		rectStart.right		= rectStart.left	+iButWidth;
		rectStart.bottom		= rect.bottom-10;
		rectStart.top			= rect.bottom-10-iButHigh;
		pButStart->MoveWindow(&rectStart);
		pButStart->Invalidate(FALSE);  
		 
		rectStop.left		= 40+(rectClose.left-rectRst.left)*4/5;
		rectStop.right		= rectStop.left	+iButWidth;
		rectStop.bottom		= rect.bottom-10;
		rectStop.top		= rect.bottom-10-iButHigh;
		pButStop->MoveWindow(&rectStop);
		pButStop->Invalidate(FALSE);  
		//左温度,右温度
		//中间炉子温度
		//信息栏
		int staticHigh,staticWidth = 0;

		staticHigh			= rectLeftTemp.bottom - rectLeftTemp.top;
		staticWidth			= 80;
		rectLeftTemp.left	= 40; 
		rectLeftTemp.right	= 40+staticWidth;
		rectLeftTemp.bottom = (rect.bottom-100);
		rectLeftTemp.top    = (rectLeftTemp.bottom-staticHigh);

		pLeftTempText->MoveWindow(&rectLeftTemp);
		pLeftTempText->Invalidate(FALSE);

		rectRightTemp.left	= 40; 
		rectRightTemp.right	= 40+staticWidth;
		rectRightTemp.bottom = (rect.bottom-90+staticHigh);
		rectRightTemp.top    = (rectRightTemp.bottom-staticHigh);

		pRightTempText->MoveWindow(&rectRightTemp);
		pRightTempText->Invalidate(FALSE);


		rectWaterTemp.left	= rectRightTemp.left + 180;;
		rectWaterTemp.right	= rectWaterTemp.left+staticWidth;
		rectWaterTemp.bottom = (rect.bottom-100);
		rectWaterTemp.top    = (rectWaterTemp.bottom-staticHigh);

		pWaterTempText->MoveWindow(&rectWaterTemp);
		pWaterTempText->Invalidate(FALSE);

		rectInfo.left	= rectRightTemp.left + 180;; 
		rectInfo.right	= rectInfo.left+staticWidth;
		rectInfo.bottom = (rect.bottom-90+staticHigh);
		rectInfo.top    = (rectInfo.bottom-staticHigh);

		pInfo->MoveWindow(&rectInfo);
		pInfo->Invalidate(FALSE);
			//,,,;
		/*
		CWnd *pLeftTempText		= GetDlgItem(IDC_STATIC_LEFT_TEMP);
		CWnd *pRightTempText	= GetDlgItem(IDC_STATIC_RIGHT_TEMP);
		CWnd *	= GetDlgItem(IDC_STATIC_WATER_TEMP);
		CWnd *				= GetDlgItem(IDC_STATIC_INFO);
		*/

		//进度条
		iHeight = rectProgress.top-rectProgress.bottom;
		rectProgress.right = rect.right-rectProgress.left;		
		rectProgress.bottom = rect.bottom-40;
		rectProgress.top	= rectProgress.bottom +iHeight;
		pProgWnd->MoveWindow(&rectProgress);
		pProgWnd->Invalidate(FALSE);
		///////////////////////////////////////////////////
		iButHigh	= rectAuto.bottom - rectAuto.top;
		iButWidth	= rectAuto.right  - rectAuto.left;

		rectAuto.left		= 40;
		rectAuto.right		= 40+iButWidth;
		rectAuto.top		= rect.top+10;
		rectAuto.bottom		= rect.top+10+iButHigh;
		pAutoNO->MoveWindow(&rectAuto);
		pAutoNO->Invalidate(FALSE);

		
		rectUpload.right		= rect.right - 40;
		rectUpload.left			= rectUpload.right-iButWidth;
		rectUpload.top			= rect.top+10;
		rectUpload.bottom		= rect.top+10+iButHigh;
		pUpload->MoveWindow(&rectUpload);
		pUpload->Invalidate(FALSE);

		
		rectDownload.left		= (rectAuto.left+rectUpload.left)/2;
		rectDownload.right		= rectDownload.left	+iButWidth;
		rectDownload.top		= rect.top+10;
		rectDownload.bottom		= rect.top+10+iButHigh;
		pDownload->MoveWindow(&rectDownload);
		pDownload->Invalidate(FALSE);

		////结果报告 
		rectList.right	= rect.right-rectList.left;	
		rectList.top	= rectAuto.bottom +10;	
		rectList.bottom = (rect.bottom-120);
		pListCtrl->MoveWindow(&rectList);
		pListCtrl->Invalidate(FALSE); 
		///////////////////////////////////////////////////
	} 
}


LRESULT CGyfxyHostView::OnCalcData(WPARAM wp, LPARAM lP)
{ 
	return 0;
}


void CGyfxyHostView::OnBnClickedTestStart()
{
	int i = 0;
	i = pRdb->attrib.m_szalgorithm==_T("快速")?
		CGyfxyImageSlave::SDownParam::M_QK :CGyfxyImageSlave::SDownParam::M_GB;
		
	if(GetAsyncKeyState(VK_LCONTROL))	//快速实验
		pGyfxyHostCtrl->AddCommand(COMMAND_CQD,1 );
	else								//数据库设置
		pGyfxyHostCtrl->AddCommand(COMMAND_CQD,i);	
	// TODO: Add your control notification handler code here
}

LRESULT CGyfxyHostView::OnImageDataChange(WPARAM sText, LPARAM lP)
{
	CGyfxyHostDoc  *pDoc = GetDocument(); 
	pDoc->UpdateAllViews(NULL);
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////		
	return 0; 
}


//更新重量信息
LRESULT CGyfxyHostView::OnUpdateQuality(WPARAM cSamplePos, LPARAM lP)
{  
	//更新列表数据
	pRdb->SampleArray.OnUpdateQuality(cSamplePos,&m_ListCtrl); 
	return 0;
}

void CGyfxyHostView::StartProgBar(CString sText, int iMax)
{ 
	if((m_strProgBarText!=sText)&&(m_iTimer!=0))
	{ 
		KillTimer(TIME_EVENT_PROCBAR);
		m_iTimer=0;
	}
	if(iMax==0)
	{
		m_ProgBar.SetShowText(true);
		m_ProgBar.SetPos(0);
		m_ProgBar.SetWindowText((LPCTSTR)sText);
		return;
	}
	if(m_iTimer==0)
	{
		m_iTimer=SetTimer(TIME_EVENT_PROCBAR,1000,NULL);		//1 second
		m_iCount=0;
		m_strProgBarText=sText;
		m_iMax=iMax;
		ASSERT(m_iTimer!=0);	
		m_ProgBar.SetShowText(TRUE);
		m_ProgBar.SetRange(0,iMax);
		m_ProgBar.SetPos(0);
		m_ProgBar.SetStep(1); 
	}
}

LRESULT CGyfxyHostView::OnStartProgBar(WPARAM sText, LPARAM lP)
{
	int iMax=lP&0x0000ffff;
	int iCommand= (lP>>16)&0x0000ffff;
	
	int iCurveXMax,iPriod;
	if((TCHAR*)sText!=_T("")) 
		StartProgBar((TCHAR*)sText, iMax); 
	switch(iCommand)
	{ 
	case SCH_CMD_DISTEST:			//关闭试验按扭 
		GetDlgItem(IDC_TEST_START)->EnableWindow(false);  
		break;
	case SCH_CMD_ENTEST:			//使能试验按扭 
		GetDlgItem(IDC_TEST_START)->EnableWindow(true); 
		//m_pPageParm->EnableAllParmCtrl(true); 
		break;
	case SCH_CMD_ENSTOP:
		GetDlgItem(IDC_TEST_START)->EnableWindow(true);  
		GetDlgItem(IDC_TEST_START)->SetWindowText(_T("停止实验"));
	case SCH_CMD_CLOSESYS:
		AfxMessageBox(_T("该系统未注册，不能正常运行！\n\n请和厂家联系！"),MB_OK+MB_ICONERROR);
		//	gbIsLogin=false;
#if !defined(_DEBUG) 
		//SendMessage(WM_COMMAND,ID_FILE_CLOSE,0);
#endif
		break;
	case SCH_CMD_LOGOK:				//注册成功使能试验按扭
		//		gbIsLogin=true;
		//GetDlgItem(IDC_TEST_START)->EnableWindow(true); 
		GetDlgItem(IDC_TEST_START)->SetWindowText(_T("开始实验"));
		//		InitTest();
	}
	
	return 0;
}

void CGyfxyHostView::OnDestroy()
{
	KillTimer(TIME_EVENT_PROCBAR);
	KillTimer(TIME_EVENT_CLOSEWINDOWS);
	KillTimer(TIME_EVENT_ISLOGIN);
	pGyfxyHostCtrl->StopCommThread();
	CFormView::OnDestroy();	 

	// TODO: Add your message handler code here
}
 

void CGyfxyHostView::OnBnClickedWeightPot()
{
	//pRdb->SampleArray.GetSampleDataFromDlg(m_ListCtrl,pImg); 
	//pGyfxyHostCtrl->DownloadAllSample();
	pGyfxyHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_WEIGHT,CGyfxyHostCtrl::T_POT );
}

void CGyfxyHostView::OnBnClickedWeightSample()
{
	if(GetAsyncKeyState(VK_LCONTROL))	//快速称样	
		pGyfxyHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_WEIGHT,CGyfxyHostCtrl::T_SAMPLE,1 );
	else								//TODO: 此处修改为由数据库设置
		pGyfxyHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_WEIGHT,CGyfxyHostCtrl::T_SAMPLE, 0);
}


void CGyfxyHostView::OnBnClickedReset()
{
	pGyfxyHostCtrl->AddCommand(COMMAND_DLD,0);  
	pGyfxyHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_RESET_INST);  //复位设备
	
}

LRESULT CGyfxyHostView::OnCheckbox(WPARAM nItem, LPARAM nSubItem)
{ 
	pRdb->SampleArray.GetSampleDataFromDlg(nItem,nSubItem,m_ListCtrl,pImg);
	pGyfxyHostCtrl->AddCommand(COMMAND_SETQUALITY,nItem+1);	
	return 0;
}

LRESULT CGyfxyHostView::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
	return 0;
}

void CGyfxyHostView::OnBnClickedButtonAutoNo()
{
	pDlg.pRdb = pRdb;
	pDlg.pImg = pImg;
	if(pDlg.DoModal()==IDOK)
	{
		OnUpdateQuality(0, (LPARAM)&m_ListCtrl); 
		pGyfxyHostCtrl->AddCommand(COMMAND_SETQUALITY,0);	
	}
}

void CGyfxyHostView::OnBnClickedButtonSampleUd()
{
	pGyfxyHostCtrl->AddCommand(COMMAND_GETQUALITY,0);	
}

void CGyfxyHostView::OnBnClickedButtonSampleDn()
{ 

	pRdb->SampleArray.GetSampleDataFromDlg(m_ListCtrl,pImg);
	pGyfxyHostCtrl->AddCommand(COMMAND_SETQUALITY,0);	
}



void CGyfxyHostView::OnLvnColumnclickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nSubItem = pNMLV->iSubItem;
	int nCheckedState = m_ListCtrl.GetHeaderCheckedState(nSubItem); 
	// 0 = no checkbox
	if (nCheckedState != XHEADERCTRL_NO_IMAGE)
	{		
		PostMessage(WM_COMMAND,IDC_BUTTON_SAMPLE_DN,0);
	}
	*pResult = 0;
}
 
BOOL   CALLBACK   CGyfxyHostView::EnumChildProc(HWND   hwndChild,LPARAM   lParam)
{
	TCHAR   szWnd[1024]; 
	GetClassName(hwndChild,szWnd,1024);	
	CWnd *pWnd=CWnd::FromHandle(hwndChild); 
	if(_tcscmp(szWnd,_T("Static"))!=0) 
		pWnd->EnableWindow(lParam); 
	return TRUE;
}

void CGyfxyHostView::EnableAllParmCtrl(bool bEnable)
{
	EnumChildWindows(m_hWnd,(WNDENUMPROC)EnumChildProc,LPARAM(bEnable));
}


void CGyfxyHostView::OnTimer(UINT_PTR nIDEvent)
{
	CString Percent;
	switch(nIDEvent)
	{
	case TIME_EVENT_PROCBAR:
		if(m_iCount>=m_iMax)
		{ 
			KillTimer(TIME_EVENT_PROCBAR);
			m_iTimer=0;
			m_iCount=0;
			Beep(1,1);
			m_ProgBar.SetPos(0);
			m_ProgBar.SetWindowText(_T(""));
			m_ProgBar.SetShowText(false); 

		}
		else
		{
			m_iCount++;
			m_ProgBar.StepIt();
			CString str;
			if(m_iMax>1)
				str.Format(_T("%s   %dS"), m_strProgBarText, m_iCount);
			else
				str=m_strProgBarText;
			m_ProgBar.SetWindowText(str);
		}
		break;

	case TIME_EVENT_CLOSEWINDOWS :	//关闭设备窗口
	case TIME_EVENT_ISLOGIN:
		KillTimer(TIME_EVENT_CLOSEWINDOWS);
		GetDocument()->SetModifiedFlag(FALSE); 
		GetParent()->PostMessage(WM_CLOSE,0,0); //关闭窗口 
		break; 
	default:
		break;
	}	
	CFormView::OnTimer(nIDEvent);
}


void CGyfxyHostView::OnBnClickedTestStop()
{
	pGyfxyHostCtrl->AddCommand(COMMAND_CTZ,0);	
}


 