// ReCalS.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "ReCalS.h"


// CReCalS dialog

IMPLEMENT_DYNAMIC(CReCalS, CDialog)

CReCalS::CReCalS(CWnd* pParent /*=NULL*/)
	: CDialog(CReCalS::IDD, pParent)
{

}

CReCalS::~CReCalS()
{
}

void CReCalS::DoDataExchange(CDataExchange* pDX)
{
   	CDialog::DoDataExchange(pDX);
 	DDX_Control(pDX, IDC_DATETIMEPICKER, m_DateTimePicker);
 	DDX_Control(pDX, IDC_DATETIME_MEASURE, m_DateTimeTest);
}


BEGIN_MESSAGE_MAP(CReCalS, CDialog)
	ON_BN_CLICKED(IDCAL, &CReCalS::OnBnClickedCal)
END_MESSAGE_MAP()


// CReCalS message handlers

BOOL CReCalS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_DateTimePicker.SetFormat(_T("yyyy-MM-dd HH:mm")); 
	m_DateTimeTest.SetFormat(_T("yyyy-MM-dd HH:mm")); 
	UpWindowsText();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CReCalS::OnBnClickedCal()
{
	// TODO: Add your control notification handler code here
	GetParamFromDlg();
	pClyRdb->result.CalResultS();
	UpWindowsText();

}

void CReCalS::UpWindowsText()
{
	CString str;  
	CClyRDB::Result *pResult = &pClyRdb->result; 
	m_DateTimePicker.SetTime(pResult->m_PickSampleDate);
	m_DateTimeTest.SetTime(pResult->m_dtTestDate);

	GetDlgItem(IDC_RECALE_TESTNUM)->SetWindowText(pResult->m_szTestAutoNO) ;
	GetDlgItem(IDC_RECALE_YANGPINNUM)->SetWindowText(pResult->m_szSampleNO) ;

	str = pResult->m_dtAnalysisTime.Format(_T("%M:%S"));  //·ÖÎöÊ±¼ä

	str.Format(_T("%f"),pResult->m_dSampleWeight);
	GetDlgItem(IDC_RECALE_YANGPINM)->SetWindowText(str);

	SetDlgItemInt(IDC_RECALE_DUPLICATE_NO,pResult->m_lPingXingNO);
	GetDlgItem(IDC_RECALE_IED)->SetWindowText(pResult->m_szName) ;
	str.Format(_T("%f"),pResult->m_dWater);
	GetDlgItem(IDC_RECALE_Mad)->SetWindowText(str);
	str.Format(_T("%f"),pResult->m_dStad);
	GetDlgItem(IDC_RECALE_Sad)->SetWindowText(str);
	str.Format(_T("%f"),pResult->m_dStd);
	GetDlgItem(IDC_RECALE_Sd)->SetWindowText(str);
}

void CReCalS::GetParamFromDlg()
{
	CString str;
	
	CClyRDB::Result *pResult = &pClyRdb->result;
	m_DateTimePicker.GetTime(pResult->m_PickSampleDate);
	m_DateTimeTest.GetTime(pResult->m_dtTestDate);

	GetDlgItem(IDC_RECALE_TESTNUM)->GetWindowText(pResult->m_szTestAutoNO) ;
	GetDlgItem(IDC_RECALE_YANGPINNUM)->GetWindowText(pResult->m_szSampleNO) ;
	GetDlgItem(IDC_RECALE_YANGPINM)->GetWindowText(str);
	pResult->m_dSampleWeight = _tcstod(str,NULL);

	pResult->m_lPingXingNO = GetDlgItemInt(IDC_RECALE_DUPLICATE_NO);  
	GetDlgItem(IDC_RECALE_IED)->GetWindowText(pResult->m_szName) ;

	GetDlgItem(IDC_RECALE_Mad)->GetWindowText(str);
	pResult->m_dWater = _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_Sad)->GetWindowText(str);
	pResult->m_dStad = _tcstod(str,NULL);
	GetDlgItem(IDC_RECALE_Sd)->GetWindowText(str);
	pResult->m_dStd = _tcstod(str,NULL);  
}