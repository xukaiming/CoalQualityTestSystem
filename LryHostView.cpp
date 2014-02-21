// LryHostView.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "LryHostView.h" 
#include "MainFrm.h"

#include "ModBusServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLryHostView

IMPLEMENT_DYNCREATE(CLryHostView, CFormView)

CLryHostView::CLryHostView()
	: CFormView(CLryHostView::IDD)
{
	//{{AFX_DATA_INIT(CLryHostView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dlgPropSheet		= new CPropertySheet;
	m_pPageParm			= new CPageParam;
	m_pPageCurve		= new CPageCurve;
	m_pPageOut			= new CPageOut;
	m_pPageDebug		= new CPageDebug;
	m_pButtonImages     = new CImageList;
	pLryHostCtrl		= new CLryHostCtrl;
	pLryHostCtrl->m_pOwner = this;
	m_iTimer			= 0; 



}

CLryHostView::~CLryHostView()
{  
	delete pLryHostCtrl;
	delete m_dlgPropSheet;
	delete m_pPageParm;
	delete m_pPageCurve;
	delete m_pPageOut;
	delete m_pPageDebug;
	delete m_pButtonImages;

}

void CLryHostView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLryHostView)
	DDX_Control(pDX, IDC_TEST_PROGRESS, m_ProgBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLryHostView, CFormView)
	//{{AFX_MSG_MAP(CLryHostView)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TEST_START, OnTestStart)
	ON_BN_CLICKED(IDC_TEST_STOP, OnTestStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_FILE_CLOSE, OnFileClose)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_MESSAGE(WM_PARAMETE_CHANGED,OnParmChanged) 
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_START_PROGBAR,OnStartProgBar)
	ON_MESSAGE(WM_IMAGE_DATA_CHANGE,OnImageDataChange)
	ON_MESSAGE(WM_CALC_DATA,OnCalcData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLryHostView diagnostics

#ifdef _DEBUG
void CLryHostView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLryHostView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLryHostDoc * CLryHostView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLryHostDoc)));
	return (CLryHostDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLryHostView message handlers

void CLryHostView::OnInitialUpdate() 
{

	CFormView::OnInitialUpdate();
	
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);


	//加输入参数、升温曲线、输出信息分页
	m_dlgPropSheet->AddPage(m_pPageParm);
	m_dlgPropSheet->AddPage(m_pPageCurve);
	m_dlgPropSheet->AddPage(m_pPageOut);
	m_dlgPropSheet->AddPage(m_pPageDebug);
	m_dlgPropSheet->Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_dlgPropSheet->ModifyStyle(0, WS_TABSTOP);
	m_dlgPropSheet->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_dlgPropSheet->SetWindowPos(NULL,6,20,230,100,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
	//标题栏中加图标 
	m_pButtonImages->Create(16, 16, ILC_COLORDDB | ILC_MASK, 5, 0);
	m_pButtonImages->Add(AfxGetApp()->LoadIcon(IDI_PARM));
	m_pButtonImages->Add(AfxGetApp()->LoadIcon(IDI_CURVE));
	m_pButtonImages->Add(AfxGetApp()->LoadIcon(IDI_OUT));
	m_pButtonImages->Add(AfxGetApp()->LoadIcon(IDI_DEBUG));

	CTabCtrl *pTab=m_dlgPropSheet->GetTabControl();
	pTab->SetImageList(m_pButtonImages);

	TC_ITEM tcItem;
	tcItem.mask = TCIF_IMAGE;
	for(int i=0; i<4; i++)
	{
		tcItem.iImage=i;
		pTab->SetItem(i, &tcItem);
	}
	//使分页有效
 	m_dlgPropSheet->SetActivePage(3);	//硬件诊断
 	m_dlgPropSheet->SetActivePage(2);	//输出信息
 	m_dlgPropSheet->SetActivePage(1);	//升温曲线
	m_dlgPropSheet->SetActivePage(0);	//输入参数
 
	m_pPageParm->m_pCtrl  = pLryHostCtrl;
	m_pPageCurve->m_pCtrl = pLryHostCtrl;	
	m_pPageOut->m_pCtrl	  = pLryHostCtrl;	 
	m_pPageDebug->m_pCtrl = pLryHostCtrl;

	
	//m_ProgBar.SetShowText(false);
//	m_ProgBar.SetBkColour(RGB(150,150,150));
	m_ProgBar.SetForeColour(RGB(0,150,0));
	



	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();



		//初始化设备
	CLryHostDoc *pDoc	= GetDocument();
	
	LRYRDB     *pLryRdb	= &pDoc->LryRdb ;  
	if(!pLryHostCtrl->InitDevice(pLryRdb))
	{
		#ifndef _DEBUG 
		//初始化设备不成功100ms后关闭窗口，不能在此直接关闭窗口。
		SetTimer(TIME_EVENT_CLOSEWINDOWS,1000,NULL);		//1000 mS
		#endif		 
	} 
	m_pPageParm->InitParamFromDB(pLryRdb);
	InitTestParam();
	
	#ifndef _DEBUG 
	//	SetTimer(TIME_EVENT_ISLOGIN,10000,NULL);
	#endif
	

}

void CLryHostView::OnDestroy() 
{
	
	KillTimer(TIME_EVENT_PROCBAR);
	KillTimer(TIME_EVENT_CLOSEWINDOWS);
	KillTimer(TIME_EVENT_ISLOGIN);
	CFormView::OnDestroy(); 

}

LRESULT CLryHostView::OnParmChanged(WPARAM, LPARAM)
{
	InitTestParam();
	return 0;
}
  

void CLryHostView::OnTestStart() 
{
	// TODO: Add your control notification handler code here

	CString s;  
	s=CString(_T("正在连接"))+	pLryHostCtrl->pRdb->m_szName;
	StartProgBar(s, 60);
	m_pPageParm->GetParamFromDlg(pLryHostCtrl->pRdb); 
	m_pPageCurve->EnableUpdateCurve(FALSE,100000);
	pLryHostCtrl->AddCommand(COMMAND_CQD,0); 
	//pLryHostCtrl->AddCommand(COMMAND_RDF,0);
}

void CLryHostView::OnTestStop() 
{ 
	pLryHostCtrl->AddCommand(COMMAND_CTZ,0);  
}

 
void CLryHostView::OnTimer(UINT nIDEvent) 
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
 

LRESULT CLryHostView::OnStartProgBar(WPARAM sText, LPARAM lP)
{
	int iMax=lP&0x0000ffff;
	int iCommand= (lP>>16)&0x0000ffff;

	int iCurveXMax,iPriod;
	if((TCHAR*)sText!=_T("")) 
		StartProgBar((TCHAR*)sText, iMax);
	iCurveXMax = pLryHostCtrl->pRdb->m_sStartJiaoBanTime1+	
					pLryHostCtrl->pRdb->m_sStartJiaoBanTime2+
					pLryHostCtrl->pRdb->m_sEndtJiaoBanTime+14*60; 
	iPriod = iCurveXMax*1000/MAX_CURVE/pLryHostCtrl->pRdb->m_lScanPriod_mS;
	switch(iCommand)
	{
	case SCH_CMD_DISCURVE:			//显示温升曲线
		if(!m_pPageCurve->bShowCurve)
		{
			m_pPageCurve->EnableUpdateCurve(TRUE, iCurveXMax); 
			m_dlgPropSheet->SetActivePage(1);	//active curve window
		}
		//break;
	case SCH_CMD_DISTEST:			//关闭试验按扭
		GetDlgItem(IDC_TEST_START)->EnableWindow(false);
		GetDlgItem(IDC_TEST_STOP)->EnableWindow(true);
		//m_pPageParm->EnableAllParmCtrl(FALSE);
		break;
	case SCH_CMD_ENTEST:			//使能试验按扭
		m_pPageCurve->EnableUpdateCurve(FALSE , iCurveXMax);
		GetDlgItem(IDC_TEST_START)->EnableWindow(true);
		GetDlgItem(IDC_TEST_STOP)->EnableWindow(false);
		m_pPageParm->EnableAllParmCtrl(true);
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

void CLryHostView::StartProgBar(CString sText, int iMax)
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
		m_ProgBar.SetWindowText(sText);
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

LRESULT CLryHostView::OnImageDataChange(WPARAM sText, LPARAM lP)
{
	int count = m_dlgPropSheet->GetPageCount();
	int i;
	for(i=0;i<count;i++)
	{
		CPropertyPage *pPage = m_dlgPropSheet->GetPage(i);
		pPage->SendMessage(WM_IMAGE_DATA_CHANGE,0,0);
	}
	return 0;
}


 

LRESULT CLryHostView::OnCalcData(WPARAM wp, LPARAM lP)
{
	LRYRDB * pRDB = pLryHostCtrl->pRdb; 
	int ret;
	m_pPageParm->GetParamFromDlg(pRDB); 
	pRDB->SaveKeyPoint();
	//InitTestParam();
	if(pRDB->strWorkType ==_T("热容量"))
	{
		pRDB->CalculateE();	
		pLryHostCtrl->SaveResultToDatabase();
		ret=pLryHostCtrl->m_DataE.UpdateOutputRecordE();
		m_pPageOut->DisEListView(&pLryHostCtrl->m_DataE);
		 
		if(ret==PINXIN_OK)
		{
			//保存热容量
			pLryHostCtrl->m_DataE.UpdateE(pLryHostCtrl->pRdb->m_szName, pLryHostCtrl->pRdb->strYangDanBianHao,pLryHostCtrl->m_DataE.lAverageE);
			pLryHostCtrl->m_DataE.InitDataE(pLryHostCtrl->pRdb); 
			AfxMessageBox(_T("热容量试验成功 ！"),MB_ICONEXCLAMATION);
			//if(pLryHostCtrl->pLryRdb->m_bAutoPrint) 
			/*
				//m_lryCtrl.m_TestDb.bAutoPrint)	//打印热容量报告
			{
				m_iPrintSel=PRINT_EAVE;
				m_sName=m_lryCtrl.m_TestDb.strName;			//sDeviceName
				m_sAB=m_lryCtrl.m_TestDb.strYangDanBianHao;	//sAB
				m_sMode=m_lryCtrl.m_TestDb.strMode;			//sMode
				m_lGroupNum=m_lryCtrl.m_TestDb.lGroupNum;	//groupmum
//				CFormView::OnFilePrintPreview();			//debug
				CFormView::OnFilePrint();
			}
			m_lryCtrl.InitDataE(); 
			*/
		} 
		if(ret==PINXIN_FAIL)
		{
			pLryHostCtrl->m_DataE.InitDataE(pLryHostCtrl->pRdb);
			AfxMessageBox(_T("热容量试验失败 ！"),MB_ICONEXCLAMATION);
		}

	} 
	else if(pRDB->strWorkType ==_T("发热量"))
	{ 
		pRDB->CalculateQ();
		pLryHostCtrl->SaveResultToDatabase();
		ret=pLryHostCtrl->m_DataQ.UpdateOutputRecordQ();
		m_pPageOut->DisQListView(&pLryHostCtrl->m_DataQ);
		 
		/*
		if(!m_lryCtrl.m_TestDb.iPingXing)			//非平行样
		{
			if(m_lryCtrl.m_TestDb.bAutoPrint)		//打印发热量报告
			{
				m_iPrintSel=PRINT_Q;
				m_sTestNum=m_lryCtrl.m_TestDb.strShiYanBianHao;
//				CFormView::OnFilePrintPreview();	//debug
				CFormView::OnFilePrint();
			}
			m_lryCtrl.InitDataQ();
		}
		else if(e)		//平行样完成
		{
			if(m_lryCtrl.m_TestDb.bAutoPrint)	//打印发热量平行样报告
			{
				m_iPrintSel=PRINT_QAVE;
				m_sName=m_lryCtrl.m_TestDb.strName;				//sDeviceName
				m_sAB=m_lryCtrl.m_TestDb.strYangDanBianHao;		//sAB
				m_sMode=m_lryCtrl.m_TestDb.strMode;				//sMode
				m_lPingXingNum=m_lryCtrl.m_TestDb.lPingXingNum;	//lPingXingNum
//				CFormView::OnFilePrintPreview();	//debug
				CFormView::OnFilePrint();
			}
			m_lryCtrl.InitDataQ();
		}
		*/
	}

	m_dlgPropSheet->SetActivePage(2);	//跳到试验结果分页

	return 0L;
} 

 
 
 
 

void CLryHostView::OnFileClose() 
{   
	GetParent()->SendMessage(WM_CLOSE,0,0);//   
}

 

 

void CLryHostView::OnButton1() 
{
	pLryHostCtrl->AddCommand(COMMAND_RDF,0);
}

void CLryHostView::InitTestParam()
{
	CString s,s1;
	m_pPageParm->GetParamFromDlg(pLryHostCtrl->pRdb); 
	m_pPageParm->SetDlgItemText(IDC_TEST_TESTNUM,pLryHostCtrl->pRdb->strAutoNo);   //设置自动编号
	m_pPageParm->GetDlgItemText(IDC_TEST_TYPE,s);		//试验内容		
	if(s==_T("热容量"))
	{
		pLryHostCtrl->m_DataE.InitDataE(pLryHostCtrl->pRdb);
		m_pPageOut->DisEListView(&pLryHostCtrl->m_DataE); 
	} 
	else if(s==_T("发热量"))
	{
		pLryHostCtrl->m_DataQ.InitDataQ(pLryHostCtrl->pRdb);
		m_pPageOut->DisQListView(&pLryHostCtrl->m_DataQ); 
		m_pPageParm->InitParamFromDB( pLryHostCtrl->pRdb); 
	} 
}


 

void CLryHostView::OnDraw(CDC* pDC) 
{
	CString s;
	CFont font;
	CFont*	pOldFont;

	font.CreateFont(16, 8, 0, 0, FW_NORMAL, false, false,
	                0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                DEFAULT_PITCH | FF_MODERN, NULL);//_T("Times New Roman")
	pOldFont = (CFont*) (pDC->SelectObject(&font));
	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(RGB(100,100,100));
	pDC->TextOut(7, 5, _T("试验设备："));
	pDC->TextOut(250, 5, _T("试验方法："));

	pDC->SetTextColor(RGB(200,32,0));
	pDC->TextOut(75, 5, pLryHostCtrl->pRdb->m_szName); 
	pDC->TextOut(320, 5, pLryHostCtrl->pRdb->m_szAlgorithm);

	pDC->SelectObject(pOldFont);
}
 