// PageParam.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "PageParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageParam property page

IMPLEMENT_DYNCREATE(CPageParam, CPropertyPage)

CPageParam::CPageParam() : CPropertyPage(CPageParam::IDD)
{
	//{{AFX_DATA_INIT(CPageParam)
	m_HeatWireQuality = 0.005;
	m_SampeQuality = 1.0;
	m_AutoNO = _T("");
	//}}AFX_DATA_INIT
}

CPageParam::~CPageParam()
{
}

void CPageParam::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageParam) 
	DDX_Control(pDX, IDC_TEST_PINGXINGYANG, m_PingXing);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SAMPLE, m_SampleDate);
	DDX_Control(pDX, IDC_TEST_YANGDANID, m_YangDanIdBox);
	DDX_Control(pDX, IDC_TEST_TYPE, m_TestType);
	DDX_Control(pDX, IDC_TEST_YANGPINNUM, m_YangPin);
	DDX_Text(pDX, IDC_TEST_DIANHUOSIZHILIANG, m_HeatWireQuality);
	DDX_Text2(pDX, IDC_TEST_YANGPINZHILIANG, m_SampeQuality);
	DDV_MinMaxDouble(pDX, m_SampeQuality, 0.1, 2.);
	DDX_Text(pDX, IDC_TEST_TESTNUM, m_AutoNO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageParam, CPropertyPage)
	//{{AFX_MSG_MAP(CPageParam)
	ON_BN_CLICKED(IDC_TEST_PINGXINGYANG, OnTestPingxingyang)
	ON_WM_CREATE()
	ON_CBN_EDITCHANGE(IDC_TEST_YANGPINNUM, OnEditchangeTestYangpinnum)
	ON_CBN_SELCHANGE(IDC_TEST_TYPE, OnSelchangeTestType)
	ON_CBN_SELCHANGE(IDC_TEST_YANGDANID, OnSelchangeTestYangdanid) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageParam message handlers

void CPageParam::OnTestPingxingyang() 
{
	CString s;

	GetDlgItemText(IDC_TEST_TYPE,s);
	if(s!=_T("发热量"))	
	{
		m_PingXing.SetCheck(0);
		return;
	}
	if(m_PingXing.GetCheck())
	{
		GetDlgItemText(IDC_TEST_YANGPINNUM,s);
		if(s==_T(""))
		{
			AfxMessageBox(_T("请输入样品编号！"),MB_ICONEXCLAMATION);
			m_PingXing.SetCheck(0);
			return;
		}
	}
	else
	{
		if(IDYES==AfxMessageBox(_T("平行样试验未完成，要继续吗？"),MB_YESNO+MB_ICONQUESTION))
		{
			m_PingXing.SetCheck(1);
			return;			//重新输入
		}
	}
	
	::SendMessage(this->GetParent()->GetParent()->m_hWnd,  WM_PARAMETE_CHANGED,0,0		 );
}

 

int CPageParam::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	 

	return 0;
}

BOOL CPageParam::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	((CComboBox*)GetDlgItem(IDC_TEST_YANGPINNUM))->SetCurSel(0);
	// TODO: Add extra initialization here
	m_SampleDate.SetFormat(_T("yyyy-MM-dd HH:mm")); 
	m_YangDanIdBox.SetCurSel(0);
	m_TestType.SetCurSel(0);  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageParam::OnEditchangeTestYangpinnum() 
{
	// TODO: Add your control notification handler code here 
	m_PingXing.SetCheck(0);	
	//SendMessage(WM_COMMAND,IDC_TEST_PINGXINGYANG,0); 
	::SendMessage(this->GetParent()->GetParent()->m_hWnd,  WM_PARAMETE_CHANGED,0,0);
}
 
BOOL   CALLBACK   CPageParam::EnumChildProc(HWND   hwndChild,LPARAM   lParam)
{
	TCHAR   szWnd[1024]; 
	GetClassName(hwndChild,szWnd,1024);	
	CWnd *pWnd=CWnd::FromHandle(hwndChild); 
	if(_tcscmp(szWnd,_T("Static"))!=0) 
		pWnd->EnableWindow(lParam); 
	return TRUE;
}

void CPageParam::EnableAllParmCtrl(bool bEnable)
{
	EnumChildWindows(m_hWnd,(WNDENUMPROC)EnumChildProc,LPARAM(bEnable));
}


void CPageParam::OnSelchangeTestType() 
{
	CString WorkType;
	GetDlgItemText(IDC_TEST_TYPE,WorkType);
	if(WorkType==_T("发热量"))
	{
		GetDlgItem(IDC_DATETIMEPICKER_SAMPLE)->EnableWindow(TRUE);	
	}
	else 
	{		
		GetDlgItem(IDC_DATETIMEPICKER_SAMPLE)->EnableWindow(FALSE);
	}	
	::SendMessage(this->GetParent()->GetParent()->m_hWnd,  WM_PARAMETE_CHANGED,0,0		 );
}

void CPageParam::GetParamFromDlg(LRYRDB *pRDB )
{
	CString WorkType,OxgenBombNO;
	GetDlgItemText(IDC_TEST_TYPE,WorkType);
	GetDlgItemText( IDC_TEST_YANGDANID,OxgenBombNO);
	pRDB->strAutoNo = pRDB->BuildAutoNO(WorkType,OxgenBombNO);  //实验编号
  
	GetDlgItem(IDC_TEST_YANGPINNUM)->GetWindowText(pRDB->strYangPinBianHao);;		//样品编号
	pRDB->strWorkType = WorkType;
	GetDlgItem(IDC_TEST_YANGDANID)->GetWindowText(pRDB->strYangDanBianHao);			//氧弹编号
	GetDlgItem(IDC_TEST_LAIYANGQINGKUANG)->GetWindowText(pRDB->strLaiYang);			//来样情况
	((CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_SAMPLE))->GetTime(pRDB->m_datePickSample );
	pRDB->m_tDate = COleDateTime::GetCurrentTime();
	CString	strQuality ;  
	GetDlgItem(IDC_TEST_YANGPINZHILIANG)->GetWindowText(strQuality);		//点火丝质量  	 
	pRDB->fYangPinZhiLiang    = _tcstod(strQuality,NULL);					//点火丝质量
	GetDlgItem(IDC_TEST_DIANHUOSIZHILIANG)->GetWindowText(strQuality);		//点火丝质量  	 
	pRDB->fDianHuoSiZhiLiang    = _tcstod(strQuality,NULL);					//点火丝质量
	GetDlgItem(IDC_TEST_TIANJIAWUZHILIANG)->GetWindowText(strQuality);		//1#添加物质量
	pRDB->fTianJiaWuZhiLiang1   = _tcstod(strQuality,NULL);;				//1#添加物质量
	GetDlgItem(IDC_TEST_TIANJIAWUZHILIANG2)->GetWindowText(strQuality);		//2#添加物质量
	pRDB->fTianJiaWuZhiLiang2	= _tcstod(strQuality,NULL);;		;		//2#添加物质量
	GetDlgItem(IDC_TEST_KONGGANJISHUIFEN)->GetWindowText(strQuality);		//空干基水分
	pRDB->fMad					= _tcstod(strQuality,NULL);;				//空干基水分Mad
	GetDlgItem(IDC_TEST_QUANSHUIFEN)->GetWindowText(strQuality);			//全水分Mt
	pRDB->fMt					= _tcstod(strQuality,NULL);;		 
	GetDlgItem(IDC_TEST_LIUHANLIANG)->GetWindowText(strQuality);			//硫含量	
	pRDB->fSad					= _tcstod(strQuality,NULL);;				//硫含量Sad
	GetDlgItem(IDC_TEST_QINHANLIANG)->GetWindowText(strQuality);
	pRDB->fHad					= _tcstod(strQuality,NULL);;				//氢含量Had

	//
	GetDlgItem(IDC_TEST_E)->GetWindowText(strQuality);						//热容量
	pRDB->m_HeatCapacity		= _tcstol(strQuality,NULL,10);;				 
	pRDB->iPingXing				= m_PingXing.GetCheck();					//1=做平行样		 　　
}
 

 

void CPageParam::OnSelchangeTestYangdanid() 
{
	CString BombID;
	GetDlgItemText(IDC_TEST_YANGDANID,BombID);
	if(BombID==_T("A氧弹"))
	{
		m_pCtrl->pRdb->m_HeatCapacity	=	m_pCtrl->pRdb->m_AOxygenHeatCapacity 	;
	}
	else
	{
		m_pCtrl->pRdb->m_HeatCapacity	=	m_pCtrl->pRdb->m_BOxygenHeatCapacity 	;
	}
	SetDlgItemInt(IDC_TEST_E,m_pCtrl->pRdb->m_HeatCapacity);
	::SendMessage(this->GetParent()->GetParent()->m_hWnd,  WM_PARAMETE_CHANGED,0,0);		
}

void CPageParam::InitParamFromDB(LRYRDB *pRDB)
{
	SetDlgItemInt(IDC_TEST_E,pRDB->m_HeatCapacity);
}
 