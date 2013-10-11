// UserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "UserInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg dialog


CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInfoDlg)
	m_UserName = _T("");
	m_NewPwd = _T("");
	m_ReNewPwd = _T("");
	m_Company = _T("");
	//}}AFX_DATA_INIT
}


void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInfoDlg)
	DDX_Text(pDX, ID_USERNAME, m_UserName);
	DDX_Text(pDX, ID_NEWPWD, m_NewPwd);
	DDX_Text(pDX, ID_REPWD, m_ReNewPwd);
	DDX_Text(pDX, ID_UNIT, m_Company);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CUserInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg message handlers

void CUserInfoDlg::SQL()
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
		//if(Rs->State==adStateOpen)
		//	Rs->Close();

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

BOOL CUserInfoDlg::OnInitDialog() 
{
	SQL();
	CDialog::OnInitDialog(); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

 
void CUserInfoDlg::LoadFromDB()
{
	_variant_t var;
	m_UserName			= VariantToStr(Rs->Fields->GetItem(_T("用户名"))->Value) ;
	//strAutoNo			= VariantToStr(Rs->Fields->GetItem(_T("试验编号"))->Value) ;
	m_Company			= VariantToStr(Rs->Fields->GetItem(_T("单位名称"))->Value) ;   
}

void CUserInfoDlg::SaveToDB()
{
	try
	{
		Rs->PutCollect("用户名",_variant_t(m_UserName));  
		Rs->PutCollect("单位名称",_variant_t(m_Company)); 
		Rs->PutCollect("口令",_variant_t(m_NewPwd)); 	
		Rs->Update();
	}
	catch (_com_error &e) {
		dump_com_error(e);
	}
}
