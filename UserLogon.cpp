// UserLogon.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "UserLogon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLogon dialog


CUserLogon::CUserLogon(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLogon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLogon)
	m_UserName = _T("");
	m_UserPassword = _T("");
	//}}AFX_DATA_INIT
}


void CUserLogon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLogon)
	DDX_Text(pDX, IDC_REG_USERNAME, m_UserName);
	DDX_Text(pDX, IDC_REG_PASS, m_UserPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLogon, CDialog)
	//{{AFX_MSG_MAP(CUserLogon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserLogon message handlers

BOOL CUserLogon::OnInitDialog() 
{
	SQL();
	CDialog::OnInitDialog(); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserLogon::SQL()
{
	CString strSQL(_T("select * from 系统参数 "));
	try
	{

		Rs.CreateInstance("ADODB.Recordset");
		Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic, -1); 
		LoadFromDB();
		if(Rs->State==adStateOpen)
			Rs->Close();

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void CUserLogon::LoadFromDB()
{
	m_UserName				= VariantToStr(Rs->Fields->GetItem(_T("用户名"))->Value) ;
	m_DBUserPassword		= VariantToStr(Rs->Fields->GetItem(_T("口令"))->Value) ; 
}

//DEL void CUserLogon::UpdateWindowsText()
//DEL {
//DEL 	GetDlgItem(IDC_REG_USERNAME)->SetWindowText( m_UserName);
//DEL }
