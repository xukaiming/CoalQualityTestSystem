// CReCalQ.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "CReCalQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCReCalQ dialog


CCReCalQ::CCReCalQ(CWnd* pParent /*=NULL*/)
	: CDialog(CCReCalQ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCReCalQ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCReCalQ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCReCalQ)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCReCalQ, CDialog)
	//{{AFX_MSG_MAP(CCReCalQ)
	ON_BN_CLICKED(IDC_RECALE_CAL, OnRecaleCal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCReCalQ message handlers

BOOL CCReCalQ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SQL();
	RefreshDataQ();
	UpWindowsText();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCReCalQ::SQL()
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

void CCReCalQ::UpWindowsText()
{
	CString str;  
	GetDlgItem(IDC_RECALE_TESTNUM)->SetWindowText(m_RDB.strAutoNo) ; 
	GetDlgItem(IDC_RECALE_DATE)->SetWindowText(m_RDB.m_tDate.Format(_T("%Y-%m-%d %H:%M:%S"))) ;
	GetDlgItem(IDC_RECALE_MODE)->SetWindowText(m_RDB.m_szAlgorithm);
	GetDlgItem(IDC_RECALE_DEVICE)->SetWindowText(m_RDB.m_szName);
	GetDlgItem(IDC_RECALE_AB)->SetWindowText(m_RDB.strYangDanBianHao); 
	GetDlgItem(IDC_RECALE_YANGPINNUM)->SetWindowText(m_RDB.strYangPinBianHao);  
	SetDlgItemInt(IDC_RECALE_E,m_RDB.m_HeatCapacity); 

	str.Format(_T("%0.4f"),m_RDB.fYangPinZhiLiang);
	GetDlgItem( IDC_RECALE_YANGPINM)->SetWindowText(str); 
	str.Format(_T("%0.4f"),m_RDB.fTianJiaWuZhiLiang1);  
	GetDlgItem(IDC_RECALE_M1)->SetWindowText(str) ;  
	str.Format(_T("%0.4f"),m_RDB.fTianJiaWuZhiLiang2);
	GetDlgItem(IDC_RECALE_M2)->SetWindowText(str) ; 
	str.Format(_T("%0.4f"),m_RDB.fDianHuoSiZhiLiang);
	GetDlgItem(IDC_RECALE_MDH)->SetWindowText(str) ; 
	
	SetDlgItemInt(IDC_RECALE_DUPLICATE_NO, m_RDB.lPingXingNum ) ; 
	SetDlgItemInt(IDC_RECALE_Q,m_RDB.lQbad	);
  
}

void CCReCalQ::RefreshDataQ()
{
	m_RDB.LoadQRecordFromDB(Rs); 
}

void CCReCalQ::OnRecaleCal() 
{
	// TODO: Add your control notification handler code here
	GetParamFromDlg();
	m_RDB.ReCalculateQ();	 
	UpWindowsText();
	UpdateDB();
}

void CCReCalQ::GetParamFromDlg()
{
	//
	//IDC_RECALE_YANGPINM
	//IDC_RECALE_M1
	//IDC_RECALE_M2
	//IDC_RECALE_MDH
	//IDC_RECALE_DUPLICATE_NO
	CString str;
	GetDlgItem(IDC_RECALE_E)->GetWindowText(str) ;
	m_RDB.m_HeatCapacity		= _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_YANGPINM)->GetWindowText(str) ;
	m_RDB.fYangPinZhiLiang		= _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_M1)->GetWindowText(str) ;
	m_RDB.fTianJiaWuZhiLiang1	= _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_M2)->GetWindowText(str) ;
	m_RDB.fTianJiaWuZhiLiang2	= _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_MDH)->GetWindowText(str) ;
	m_RDB.fDianHuoSiZhiLiang	= _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_DUPLICATE_NO)->GetWindowText(str) ; 
	m_RDB.lPingXingNum			= _tcstod(str,NULL);

}

void CCReCalQ::UpdateDB()
{ 
	m_RDB.UpdateQRecordToDB(Rs);
}
