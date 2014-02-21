// ClyHostView.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "ClyHostView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClyHostView
#include "CLyHostDoc.h"

IMPLEMENT_DYNCREATE(CClyHostView, CFormView)

CClyHostView::CClyHostView()
	: CFormView(CClyHostView::IDD)
{
	//{{AFX_DATA_INIT(CClyHostView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pCLYHostCtrl = new CCLYHostCtrl();
	pCLYHostCtrl->m_pOwner = this;
}

CClyHostView::~CClyHostView()
{
	delete pCLYHostCtrl;
}

void CClyHostView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClyHostView) 
	DDX_Control(pDX, IDC_PROGRESS_STATE, m_ProgBar);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PNG, m_CurveEdit);
	DDX_Control(pDX, IDC_LIST_RESULT, m_ResultListCtrl);
}


BEGIN_MESSAGE_MAP(CClyHostView, CFormView)
	//{{AFX_MSG_MAP(CClyHostView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PUMB_UP, OnPumbUp)
	ON_BN_CLICKED(IDC_JB, OnJb)
	ON_BN_CLICKED(IDC_TEST_START, OnTestStart)
	ON_BN_CLICKED(IDC_TEST_STOP, OnTestStop)
	ON_BN_CLICKED(IDC_SYS_RST, OnSysRst)
	ON_BN_CLICKED(IDC_WARM_UP, OnWarmUp)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IMAGE_DATA_CHANGE,OnImageDataChange)
	ON_MESSAGE(WM_START_PROGBAR,OnStartProgBar) 
	ON_MESSAGE(WM_CALC_DATA,OnCalcData)
	ON_WM_TIMER() 
	ON_BN_CLICKED(IDC_ADD_SAMPLE, &CClyHostView::OnBnClickedAddSample)
	ON_COMMAND(ID_DELETE_RECORD, &CClyHostView::OnDeleteRecord) 

	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_COPY, ID_EDIT_CUT ,OnUpdateNeedSel)// 
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE,OnUpdateNeedClip) 
	ON_COMMAND(ID_EDIT_COPY, &CClyHostView::OnEditCopy) 
	ON_COMMAND(ID_EDIT_CUT, &CClyHostView::OnEditCut) 
	ON_COMMAND(ID_EDIT_PASTE , &CClyHostView::OnEditPaste)  

	ON_BN_CLICKED(IDC_BUTTON_R, &CClyHostView::OnBnClickedButtonR)
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID__DELE_SAMPLE, &CClyHostView::OnDeleteRecord)  
	ON_COMMAND(ID__ADD_SAMPLE, &CClyHostView::OnAddRecord) 
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClyHostView diagnostics

#ifdef _DEBUG
void CClyHostView::AssertValid() const
{
	CFormView::AssertValid();
}

void CClyHostView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CClyHostDoc  * CClyHostView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClyHostDoc)));
	return (CClyHostDoc*)m_pDocument;
} 

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClyHostView message handlers



void CClyHostView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE); 
	CClyHostDoc *pDoc	= GetDocument();	
	pDoc->pHostCtrl		= pCLYHostCtrl;
	if(!pCLYHostCtrl->InitDevice(pDoc->pRDB))
	{
#ifndef _DEBUG 
		//初始化设备不成功100ms后关闭窗口，不能在此直接关闭窗口。
		SetTimer(TIME_EVENT_CLOSEWINDOWS,1000,NULL);		//1000 mS
#endif		 
	} 

	//
	LONG style = GetWindowLong(m_ResultListCtrl.m_hWnd,GWL_STYLE);
	style |= LVS_REPORT|LVS_SHOWSELALWAYS |LVS_SINGLESEL;
	SetWindowLong(m_ResultListCtrl.m_hWnd,GWL_STYLE,style); 
	m_ResultListCtrl.SetExtendedStyle(m_ResultListCtrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	m_ResultListCtrl.SetTextBkColor(RGB(0x0,0xff,0xff)); 
	//m_ResultListCtrl.SetBkColor(RGB(0x0,0xff,0x7f));

	InitParamFromDB();
	InitTestParam();
	m_CurveEdit.ClearData();
	m_CurveEdit.pRDB=(CClyRDB*)pDoc->pRDB;
	m_ResultListCtrl.FillHeaderCtrl(pDoc);
	m_ResultListCtrl.SetColumnWidth(3,120);
#ifndef _DEBUG 
//	SetTimer(TIME_EVENT_ISLOGIN,10000,NULL);
#endif
	OnBnClickedAddSample();
}

void CClyHostView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CWnd *pProgWnd	= GetDlgItem(IDC_PROGRESS_STATE);
	CWnd *pPngWnd	= GetDlgItem(IDC_EDIT_PNG);
	CWnd *pResultWnd= GetDlgItem(IDC_LIST_RESULT);

	CWnd *pButStart = GetDlgItem(IDC_TEST_START);
	CWnd *pButStop  = GetDlgItem(IDC_TEST_STOP);
	CWnd *pButClose = GetDlgItem(ID_FILE_CLOSE);

	CWnd *pButRst	 = GetDlgItem(IDC_SYS_RST); 
	CWnd *pButWarm 	 = GetDlgItem(IDC_WARM_UP); 
	CWnd *pButPumb	 = GetDlgItem(IDC_PUMB_UP); 
	CWnd *pButJB	 = GetDlgItem(IDC_JB); 
	CWnd *pButAddSample	 = GetDlgItem(IDC_ADD_SAMPLE); 
	CWnd *pButReadResult = GetDlgItem(IDC_BUTTON_R); 
	
	if(pProgWnd!=NULL)
	{
		RECT rect, rectProgress,rectPng,rectResult;
		RECT rectStart,rectStop,rectClose,rectReadResult;
		RECT rectRst,rectWarm,rectPumb,rectJB,rectPutSample;
		double iHeight;
		GetClientRect(&rect);
		pProgWnd->GetWindowRect(&rectProgress);
		pPngWnd->GetWindowRect(&rectPng);
		pResultWnd->GetWindowRect(&rectResult);
		pButReadResult->GetWindowRect(&rectReadResult);
		/////////////////////////////////////////
		pButStart->GetWindowRect(&rectStart);
		pButStop->GetWindowRect(&rectStop);
		pButClose->GetWindowRect(&rectClose);

		///////////////////////////////////////
		pButRst->GetWindowRect(&rectRst);
		pButWarm->GetWindowRect(&rectWarm);
		pButPumb->GetWindowRect(&rectPumb);
		pButJB->GetWindowRect(&rectJB);
		pButAddSample->GetWindowRect(&rectPutSample);
		/////////////////////////////////////////////
		ScreenToClient(&rectProgress);
		ScreenToClient(&rectPng);
		ScreenToClient(&rectResult);
		ScreenToClient(&rectStart);
		ScreenToClient(&rectStop);
		ScreenToClient(&rectClose);
		ScreenToClient(&rectReadResult);

		ScreenToClient(&rectRst);
		ScreenToClient(&rectWarm);
		ScreenToClient(&rectPumb);
		ScreenToClient(&rectJB);
		ScreenToClient(&rectPutSample);
		///////////////////////////////////////////////////
		//按钮
		int iButHigh,iButWidth;
		iButHigh	= rectStart.bottom - rectStart.top;
		iButWidth	= rectStart.right  - rectStart.left;
		//
		rectStart.left		= 40;
		rectStart.right		= 40+iButWidth;
		rectStart.bottom	= rect.bottom-10;
		rectStart.top		= rect.bottom-10-iButHigh;
		pButStart->MoveWindow(&rectStart);
		pButStart->Invalidate(FALSE);
		//
		rectClose.right		= rect.right-40;
		rectClose.left		= rect.right-40-iButWidth;
		rectClose.bottom	= rectStart.bottom;
		rectClose.top		= rectStart.top;
		pButClose->MoveWindow(&rectClose);
		pButClose->Invalidate(FALSE);
		//
		rectStop.left		= (rect.right-iButWidth)/2;
		rectStop.right		= (rect.right+iButWidth)/2;
		rectStop.bottom		= rectStart.bottom;
		rectStop.top		= rectStart.top;
		pButStop->MoveWindow(&rectStop);
		pButStop->Invalidate(FALSE);
		//
		int rWidth			= rectReadResult.right-rectReadResult.left;
		rectReadResult.right= rect.right-10;
		rectReadResult.left = rect.right-10-rWidth;
		rectReadResult.bottom	= rectStart.bottom;
		rectReadResult.top		= rectStart.top;
		pButReadResult->MoveWindow(&rectReadResult);
		pButReadResult->Invalidate(FALSE);
		//
		rectRst.right		= rect.right-20;
		rectRst.left		= rect.right-20-iButWidth;
		rectWarm.right		= rect.right-20;
		rectWarm.left		= rect.right-20-iButWidth;
		rectPumb.right		= rect.right-20;
		rectPumb.left		= rect.right-20-iButWidth;
		rectJB.right		= rect.right-20;
		rectJB.left			= rect.right-20-iButWidth;	
		rectPutSample.right = rect.right-20;
		rectPutSample.left	= rect.right-20-iButWidth;
		
		 
		pButRst	->MoveWindow(&rectRst);
		pButWarm->MoveWindow(&rectWarm);
		pButPumb->MoveWindow(&rectPumb);
		pButJB	->MoveWindow(&rectJB);
		pButAddSample->MoveWindow(&rectPutSample);
		//进度条
		iHeight = rectProgress.top-rectProgress.bottom;
		rectProgress.right = rect.right-rectProgress.left;		
		rectProgress.bottom = rect.bottom-40;
		rectProgress.top	= rectProgress.bottom +iHeight;
		pProgWnd->MoveWindow(&rectProgress);
		pProgWnd->Invalidate(FALSE);
		///////////////////////////////////////////////////
		//图片框
		rectPng.right	= rect.right-rectPng.left-120;	
		rectPng.bottom = (rect.right-rect.left-120)*0.5;
		pPngWnd->MoveWindow(&rectPng);
		pPngWnd->Invalidate(FALSE);
		///////////////////////////////////////////////////
		//结果报告 
		rectResult.top		= rectPng.bottom+5;
		rectResult.right	= rect.right-rectResult.left;
		rectResult.bottom	= rectProgress.top-5;
		pResultWnd->MoveWindow(&rectResult);
		pResultWnd->Invalidate(FALSE);
		///////////////////////////////////////////////////
	} 
	
}

void CClyHostView::OnDestroy() 
{
	KillTimer(TIME_EVENT_PROCBAR);
	KillTimer(TIME_EVENT_CLOSEWINDOWS);
	KillTimer(TIME_EVENT_ISLOGIN);
	pCLYHostCtrl->StopCommThread();
	CFormView::OnDestroy();	
	 
}

void CClyHostView::InitParamFromDB()
{	
	comboArray.RemoveAll();
	CString strNO;
	for (int i=1;i<=pCLYHostCtrl->pRdb->attrib.m_MaxSampleCnt;i++)
	{
		strNO.Format(_T("%d"),i);
		comboArray.Add(strNO);
	}
	

}

void CClyHostView::InitTestParam()
{

}

LRESULT CClyHostView::OnStartProgBar(WPARAM sText, LPARAM lP)
{
	int iMax=lP&0x0000ffff;
	int iCommand= (lP>>16)&0x0000ffff;
	
	int iCurveXMax,iPriod;
	if((TCHAR*)sText!=_T("")) 
		StartProgBar((TCHAR*)sText, iMax);
	/*
	iCurveXMax = pCLYHostCtrl->pRdb->m_sStartJiaoBanTime1/60+	
		pLryHostCtrl->pLryRdb->m_sStartJiaoBanTime2/60+
		pLryHostCtrl->pLryRdb->m_sEndtJiaoBanTime/60+10; 
	iPriod = iCurveXMax*60*1000/MAX_CURVE/pLryHostCtrl->pLryRdb->m_lScanPriod_mS;
	*/
	switch(iCommand)
	{
	case SCH_CMD_DISCURVE:			//显示温升曲线
		/*
		if(!m_pPageCurve->bShowCurve)
		{
			m_pPageCurve->EnableUpdateCurve(TRUE,iPriod , iCurveXMax); 
			m_dlgPropSheet->SetActivePage(1);	//active curve window
		}
		*/
		break;
	case SCH_CMD_DISTEST:			//关闭试验按扭
		
		GetDlgItem(IDC_TEST_START)->EnableWindow(false);
		GetDlgItem(IDC_TEST_STOP)->EnableWindow(true);
		
		break;
	case SCH_CMD_ENTEST:			//使能试验按扭
		
		//m_pPageCurve->EnableUpdateCurve(FALSE,iPriod , iCurveXMax);
		GetDlgItem(IDC_TEST_START)->EnableWindow(true);
		GetDlgItem(IDC_TEST_STOP)->EnableWindow(false);
		//m_pPageParm->EnableAllParmCtrl(true);
		
		break;
	case SCH_CMD_CLOSESYS:
		AfxMessageBox(_T("该系统未注册，不能正常运行！\n\n请和厂家联系！"),MB_OK+MB_ICONERROR);
		//	gbIsLogin=false;
#if !defined(_DEBUG) 
		//SendMessage(WM_COMMAND,ID_FILE_CLOSE,0);
#endif
		break;
	case SCH_CMD_LOGOK:				//注册成功使能试验按扭
		//		gbIsLogin=true;
		GetDlgItem(IDC_TEST_START)->EnableWindow(true);
		GetDlgItem(IDC_TEST_STOP)->EnableWindow(false);
		//		InitTest();
	}
	
	return 0;
}

BOOL CClyHostView::IsEdit( CWnd* pWnd )
{
	ASSERT( pWnd != NULL );
	HWND hWnd = pWnd->GetSafeHwnd();
	if (hWnd == NULL)
		return FALSE;

	TCHAR szClassName[6];
	return ::GetClassName(hWnd, szClassName, 6) &&
		_tcsicmp(szClassName, _T("Edit")) == 0; 
}

void CClyHostView::StartProgBar(CString sText, int iMax)
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

void CClyHostView::OnPumbUp() 
{ 
	CClyImageSlave::_OutputCoil *pCoil =  &pCLYHostCtrl->pImageSlave->OutputCoil;
	pCLYHostCtrl->AddCommand(COMMAND_JDQ,CMD_DC_PUMB|(pCoil->DC_PUMB?0:0x80));	
}

void CClyHostView::OnJb() 
{ 
	CClyImageSlave::_OutputCoil *pCoil =  &pCLYHostCtrl->pImageSlave->OutputCoil;
	pCLYHostCtrl->AddCommand(COMMAND_JDQ,CMD_DC_JB|(pCoil->DC_JB?0:0x80));		
}
 

LRESULT CClyHostView::OnImageDataChange(WPARAM sText, LPARAM lP)
{
	CClyImageSlave::_OutputCoil *pCoil	=  &pCLYHostCtrl->pImageSlave->OutputCoil;
	
	AI_CONTROL *pCtrl					=  &pCLYHostCtrl->pImageSlave->Ai_CTRL;

	//CWnd *pWnd	= GetDlgItem(IDC_EDIT_PNG);
	//pWnd->Invalidate(TRUE);

	SetDlgItemText(IDC_PUMB_UP,pCoil->DC_PUMB?_T("气泵关"):_T("气泵开"));
	SetDlgItemText(IDC_JB,pCoil->DC_JB?_T("停止搅拌"):_T("开始搅拌")); 
	SetDlgItemText(IDC_WARM_UP,pCtrl->WarmFlag?_T("停止加热"):_T("开始加热"));
	 
	CClyHostDoc  *pDoc = GetDocument();
	m_CurveEdit.AddData(pCLYHostCtrl->pRdb->status.DJCurrent);
	pDoc->UpdateAllViews(this);
	return 0;
}

void CClyHostView::OnTestStart() 
{
	CClyRDB * pRDB = pCLYHostCtrl->pRdb; 
	//
	if(pRDB->result.m_ResultNO>=m_ResultListCtrl.GetItemCount())       //如果样品数目不够，添加一条样品记录
	{
		SendMessage(WM_COMMAND,IDC_ADD_SAMPLE,0);
	} 
	GetParamFromDlg(pRDB);  //得到 样位信息
	//
	if((pCLYHostCtrl->pRdb->status.Temperature<pCLYHostCtrl->pRdb->attrib.m_sDestTemp-10)&&
		(ID_CANCEL==AfxMessageBox(_T("貌似还没有到设定温度\n是否强制开始试验?"),MB_OKCANCEL+MB_ICONQUESTION))
		)
		return;
	pCLYHostCtrl->pRdb->status.m_bStartTest = TRUE;
	m_CurveEdit.ClearData();  
	
	pCLYHostCtrl->AddCommand(COMMAND_MTR_POS,0);	//停止
	pCLYHostCtrl->AddCommand(COMMAND_MTR_POS,pRDB->result.m_pos);	
	
	pCLYHostCtrl->AddCommand(COMMAND_CQD,0);	
}

void CClyHostView::OnTestStop() 
{ 
	pCLYHostCtrl->AddCommand(COMMAND_CTZ,0);
	pCLYHostCtrl->pRdb->status.m_bStartTest  = FALSE;
}

void CClyHostView::OnSysRst() 
{ 
	pCLYHostCtrl->AddCommand(COMMAND_RES,0);	
}

void CClyHostView::OnWarmUp() 
{ 	
	AI_CONTROL *pCtrl =  &pCLYHostCtrl->pImageSlave->Ai_CTRL;
#ifdef _DEBUG
	if(0==pCtrl->WarmFlag)
	{  //
		//((CClyHostDoc*)GetDocument())->GetHostParam((->m_szName,_T("测硫仪"));
		//pCLYHostCtrl->pRdb->LoadParamFromDB(pCLYHostCtrl->pRdb->m_szName,_T("测硫仪"));
		pCLYHostCtrl->AddCommand(COMMAND_DLD,0);
	}
#endif
	pCLYHostCtrl->AddCommand(COMMAND_WUP,pCtrl->WarmFlag?0:1);
}


void CClyHostView::OnDraw(CDC* pDC)
{ 
}

void CClyHostView::OnTimer(UINT_PTR nIDEvent)
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


void CClyHostView::OnBnClickedAddSample()
{
	int n = m_ResultListCtrl.GetItemCount(); 
	CString strNO;
	int maxcnt = pCLYHostCtrl->pRdb->attrib.m_MaxSampleCnt;
	strNO.Format(_T("%d"),n%maxcnt+1);
	m_ResultListCtrl.InsertItem(n,_T(""));
	m_ResultListCtrl.SetItemText(n,2,strNO);
	//m_ResultListCtrl.SetComboBox(n,2,TRUE,&comboArray,n,n%maxcnt+1,FALSE);
 

	m_ResultListCtrl.SetItemText(n,3,COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
	m_ResultListCtrl.SetItemText(n,4,_T("50"));
	m_ResultListCtrl.SetItemText(n,5,_T("0"));
	m_ResultListCtrl.EnsureVisible(n,FALSE);
	m_ResultListCtrl.SetItemState(n,LVIS_SELECTED, LVIS_SELECTED);

}



void CClyHostView::OnAddRecord()
{
	POSITION pos = m_ResultListCtrl.GetFirstSelectedItemPosition();
	int n = m_ResultListCtrl.GetNextSelectedItem(pos); 
	n++;
	m_ResultListCtrl.InsertItem(n,_T(""));
	m_ResultListCtrl.SetItemText(n,2,COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
	m_ResultListCtrl.SetItemText(n,3,_T("50"));
	m_ResultListCtrl.SetItemText(n,4,_T("0"));
	m_ResultListCtrl.EnsureVisible(n,FALSE);
	m_ResultListCtrl.SetItemState(n,LVIS_SELECTED, LVIS_SELECTED);

}
void CClyHostView::OnDeleteRecord()
{ 
	/*
	DWORD dwPos = GetMessagePos(); 
	CPoint point(LOWORD(dwPos),HIWORD(dwPos)); 
	m_ResultListCtrl.ScreenToClient(&point);   
	LVHITTESTINFO   lvinfo; 
	lvinfo.pt		=   point; 
	lvinfo.flags	=   LVHT_ABOVE; 
	int   nItem   =   m_ResultListCtrl.SubItemHitTest(&lvinfo); 
	CString   strtemp; 
	//strtemp.Format(_T( "单击的是第%d行第%d列 "),   lvinfo.iItem,   lvinfo.iSubItem); 
	m_ResultListCtrl.DeleteItem(lvinfo.iItem);
	*/
	int nItemIndex;
	CString strAutoNO;
	int *iNO = &pCLYHostCtrl->pRdb->result.m_ResultNO;
	if(GetFocus()!=&m_ResultListCtrl)   //防止在编辑状态删除记录
	{
		GetFocus()->SendMessage(WM_KEYDOWN, VK_DELETE, 0 );//
		return;
	}
	if((nItemIndex = m_ResultListCtrl.GetNextItem(-1,LVNI_SELECTED))!=-1)
	{
		if(((strAutoNO=m_ResultListCtrl.GetItemText(nItemIndex,0))!=_T(""))&&
			(ID_OK==::MessageBox(NULL,
							_T("确认删除记录吗?"),
							_T("警告!"),
							MB_OKCANCEL+MB_ICONWARNING+MB_TASKMODAL)
							))
		{
			(*iNO)--; //数据库中删除该记录
			pCLYHostCtrl->pRdb->result.DelResultFromDB(strAutoNO);
		}
		m_ResultListCtrl.DeleteItem(nItemIndex);
	}
	
}

LRESULT CClyHostView::OnCalcData(WPARAM wp, LPARAM lP)
{
	CClyRDB * pRDB = pCLYHostCtrl->pRdb;
	CClyImageSlave *pImg = pCLYHostCtrl->pImageSlave;
	GetParamFromDlg(pRDB);
	pRDB->result.CalResult(pImg);
	UpdateRDBResult(pRDB);
	pRDB->result.SaveResult2DB();
	return 0;
}
// //void CPageParam
void CClyHostView::GetParamFromDlg(CClyRDB * pRDB)
{

	int i = pRDB->result.m_ResultNO;
	pRDB->result.BuildAutoNO();  
	pRDB->result.m_szSampleNO		= m_ResultListCtrl.GetItemText(i,1); //样品编号
	CString strSampleNO				= m_ResultListCtrl.GetItemText(i,2); //样位位置 
	pRDB->result.m_pos				= _tcstol(strSampleNO,NULL,10);
	CString strPickDate				= m_ResultListCtrl.GetItemText(i,3); //取样日期
	pRDB->result.m_dSampleWeight	= _tcstod(m_ResultListCtrl.GetItemText(i,4),NULL); //样品重量
	pRDB->result.m_dWater			= _tcstod(m_ResultListCtrl.GetItemText(i,5),NULL); //分析水
	pRDB->result.m_dtTestDate		= COleDateTime::GetCurrentTime();			//测试时刻
	pRDB->result.m_szName			= pRDB->m_szName;
	COleVariant vDateTime(strPickDate);
	TRY
	{
		vDateTime.ChangeType(VT_DATE); 
		pRDB->result.m_PickSampleDate	= vDateTime;
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL
}

void CClyHostView::OnBnClickedButtonR()
{
	SendMessage(WM_CALC_DATA,0,0);
}

void CClyHostView::UpdateRDBResult(CClyRDB * pRDB)
{
	int *iNO = &pRDB->result.m_ResultNO;
	CString strTemp ; 
	m_ResultListCtrl.SetItemText(*iNO,0,pRDB->result.m_szTestAutoNO);
	strTemp.Format(_T("%.3f%%"),pRDB->result.m_dStad);
	m_ResultListCtrl.SetItemText(*iNO,6,strTemp)	 ;	//分析基硫	
	strTemp.Format(_T("%.3f%%"),pRDB->result.m_dStd);
	m_ResultListCtrl.SetItemText(*iNO,7,strTemp); 
	strTemp = pRDB->result.m_dtAnalysisTime.Format(_T("%M:%S"));	//实验时间			//
	m_ResultListCtrl.SetItemText(*iNO,8,strTemp); 	
	strTemp.Format(_T("%.3f"),pRDB->result.m_fCoulomb_ByVF); //电量
	m_ResultListCtrl.SetItemText(*iNO,9,strTemp); 
	strTemp.Format(_T("%.3f"),pRDB->result.m_fCoulomb_ByDA); //电量
	m_ResultListCtrl.SetItemText(*iNO,10,strTemp); 
	m_ResultListCtrl.SetItemText(*iNO,11,pRDB->result.m_szName); //设备名称
	strTemp = pRDB->result.m_dtTestDate.Format(_T("%Y-%m-%d %H:%M:%S"));//测试时刻
	m_ResultListCtrl.SetItemText(*iNO,12,strTemp);  
	m_ResultListCtrl.SetItemText(*iNO,13,pRDB->result.m_bFix?_T("是"):_T("否")); //修正
	strTemp.Format(_T("%d"),pRDB->result.m_lPingXingNO);//平行样编号
	m_ResultListCtrl.SetItemText(*iNO,14,strTemp); 
	(*iNO)  ++; 
	if(((*iNO)>=m_ResultListCtrl.GetItemCount())
		||!(pCLYHostCtrl->pRdb->attrib.m_bAutoCly))       //所有样品测试完毕
	{
		pRDB->status.m_bStartTest = FALSE;            //
	}
	else
		m_CurveEdit.ClearData();					 //没有实验完毕 

}

void CClyHostView::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	// get the current focus & determine if its on a CEdit control
	CWnd* pWnd = GetFocus();
	if (NULL == pWnd || !IsEdit( pWnd ))
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		CEdit* pEdit = (CEdit*)pWnd;
		int nBeg, nEnd;

		pEdit->GetSel( nBeg, nEnd );
		pCmdUI->Enable( nBeg != nEnd );
	} 
}

void CClyHostView::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	// get the current focus & determine if its on a CEdit control
	// also check to see if the control is read-only.
	CWnd* pWnd = GetFocus();
	if (  NULL == pWnd ||
		!IsEdit( pWnd ) ||
		(pWnd->GetStyle() & ES_READONLY) != 0 )
	{
		pCmdUI->Enable( FALSE );
	}
	else
		pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
}

void CClyHostView::OnEditCopy()  
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	pEdit->Copy();
}

 
void CClyHostView::OnEditCut()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	pEdit->Cut();
}

void CClyHostView::OnEditPaste()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	ASSERT(::IsClipboardFormatAvailable(CF_TEXT));
	pEdit->Paste();
}

void CClyHostView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu Menu;	
	CMenu *pMenu;
	Menu.LoadMenu(IDR_MENU_S_SAMPLE);
	pMenu = Menu.GetSubMenu(0);
 
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
		CFormView::OnContextMenu(pWnd,point); //弹出默认的滚动条菜单
}
