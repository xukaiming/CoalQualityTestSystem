// ModifyWeight.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "ModifyWeight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyWeight dialog


CModifyWeight::CModifyWeight(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyWeight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyWeight)
	m_AutoNO = _T("");
	m_ModifyWeight = _T("");
	//}}AFX_DATA_INIT
}


void CModifyWeight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyWeight)
	DDX_Text(pDX, IDC_MODIFY_NUM, m_AutoNO);
	DDX_Text(pDX, IDC_YangPin, m_ModifyWeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyWeight, CDialog)
	//{{AFX_MSG_MAP(CModifyWeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyWeight message handlers

void CModifyWeight::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	try
	{
		GetDlgItem( IDC_YangPin)->GetWindowText(str); //硫含量
		m_RDB.fYangPinZhiLiang = _tcstod(str,NULL); 
		m_RDB.ReCalculateE(); 
		UpdateDB();	
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	
	CDialog::OnOK();
}

BOOL CModifyWeight::OnInitDialog() 
{
	CDialog::OnInitDialog(); 	
	try
	{
		SQL();
		RefreshDataE();
		UpWindowsText();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyWeight::SQL()
{
	CString strSQL;
	strSQL.Format(_T("select * from 热容量 where 试验编号='%s'"),m_AutoNO);

		Rs.CreateInstance("ADODB.Recordset");
		Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic, -1); 
 

}

void CModifyWeight::UpdateDB()
{
	m_RDB.UpdateERecordToDB(Rs); 
} 

void CModifyWeight::UpWindowsText()
{
	CString str;  
	GetDlgItem(IDC_MODIFY_NUM)->SetWindowText(m_RDB.strAutoNo) ; 
	str.Format(_T("%f"),m_RDB.fYangPinZhiLiang)	;
	GetDlgItem(IDC_YangPin)->SetWindowText(str) ; 
}

void CModifyWeight::RefreshDataE()
{
	m_RDB.LoadERecordFromDB(Rs); 
}
