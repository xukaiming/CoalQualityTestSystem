// CalGaoDiQDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "CalGaoDiQDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalGaoDiQDlg dialog


CCalGaoDiQDlg::CCalGaoDiQDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalGaoDiQDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalGaoDiQDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCalGaoDiQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalGaoDiQDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalGaoDiQDlg, CDialog)
	//{{AFX_MSG_MAP(CCalGaoDiQDlg)
	ON_BN_CLICKED(IDC_CAL, OnCal)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalGaoDiQDlg message handlers

void CCalGaoDiQDlg::OnCal() 
{ 
	GetParamFromDlg();
	m_RDB.ReCalculateQ();	 
	UpWindowsText();
	UpdateDB();	
}

void CCalGaoDiQDlg::SQL()
{
	CString strSQL;
	strSQL.Format(_T("select * from 发热量 where 试验编号='%s'"),m_szTestNum);
	try
	{

		Rs.CreateInstance("ADODB.Recordset");
		Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic, -1); 
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}


void CCalGaoDiQDlg::UpWindowsText()
{
	CString str;  
	GetDlgItem(IDC_RECALE_TESTNUM)->SetWindowText(m_RDB.strAutoNo) ; 
	GetDlgItem(IDC_RECALE_DATE)->SetWindowText(m_RDB.m_tDate.Format(_T("%Y-%m-%d %H:%M:%S"))) ;
	GetDlgItem(IDC_RECALE_MODE)->SetWindowText(m_RDB.m_szAlgorithm);
	GetDlgItem(IDC_RECALE_DEVICE)->SetWindowText(m_RDB.m_szName);
	GetDlgItem(IDC_RECALE_AB)->SetWindowText(m_RDB.strYangDanBianHao); 
	GetDlgItem(IDC_RECALE_YANGPINNUM)->SetWindowText(m_RDB.strYangPinBianHao);  
	SetDlgItemInt(IDC_RECALE_E,m_RDB.m_HeatCapacity); 
	str.Format(_T("%0.4f"),m_RDB.fSad);							//硫含量
	GetDlgItem( IDC_CAL_St)->SetWindowText(str); 
	str.Format(_T("%0.4f"),m_RDB.fMad);							//空干基水分
	GetDlgItem( IDC_CAL_Mad)->SetWindowText(str); 
	str.Format(_T("%0.4f"),m_RDB.fMt);							//全水分
	GetDlgItem( IDC_CAL_Mt)->SetWindowText(str); 
	str.Format(_T("%0.4f"),m_RDB.fHad);							//空干基氢含量Had
	GetDlgItem( IDC_CAL_Had)->SetWindowText(str);  
	SetDlgItemInt(IDC_CAL_Qbad,m_RDB.lQbad);					//弹筒发热量
	SetDlgItemInt(IDC_CAL_Qgrad,m_RDB.lQgrad);					
	SetDlgItemInt(IDC_CAL_Qgrd,m_RDB.lQgrd);					
	SetDlgItemInt(IDC_CAL_Qnetvar,m_RDB.lQnetvad);	 
}

BOOL CCalGaoDiQDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SQL();
	RefreshDataQ();
	UpWindowsText();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalGaoDiQDlg::RefreshDataQ()
{
	m_RDB.LoadQRecordFromDB(Rs); 
}

void CCalGaoDiQDlg::UpdateDB()
{
	m_RDB.UpdateQRecordToDB(Rs);
}

void CCalGaoDiQDlg::GetParamFromDlg()
{ 
	CString str; 						
	GetDlgItem( IDC_CAL_St)->GetWindowText(str); //硫含量
	m_RDB.fSad = _tcstod(str,NULL); 				
	GetDlgItem( IDC_CAL_Mad)->GetWindowText(str); //空干基水分
	m_RDB.fMad = _tcstod(str,NULL); 		
	GetDlgItem( IDC_CAL_Mt)->GetWindowText(str); //全水分 		
	m_RDB.fMt = _tcstod(str,NULL);
	GetDlgItem( IDC_CAL_Had)->GetWindowText(str); //空干基氢含量Had 
	m_RDB.fHad = _tcstod(str,NULL); 
	m_RDB.lQbad		= GetDlgItemInt(IDC_CAL_Qbad);					//弹筒发热量
	m_RDB.lQgrad	= GetDlgItemInt(IDC_CAL_Qgrad );					
	m_RDB.lQgrd		= GetDlgItemInt(IDC_CAL_Qgrd );					
	m_RDB.lQnetvad  = GetDlgItemInt(IDC_CAL_Qnetvar );	  
}

void CCalGaoDiQDlg::OnSave() 
{ 
	UpdateDB();		
}

 
