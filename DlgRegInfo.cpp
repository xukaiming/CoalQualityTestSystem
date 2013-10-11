// DlgRegInfo.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DlgRegInfo.h" 
#include "ModBusServer.h"

// CDlgRegInfo dialog

IMPLEMENT_DYNAMIC(CDlgRegInfo, CDialog)

CDlgRegInfo::CDlgRegInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRegInfo::IDD, pParent)
{

}

CDlgRegInfo::~CDlgRegInfo()
{ 
	delete bNewSecretArray; 
}

void CDlgRegInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRegInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgRegInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CDlgRegInfo::OnBnClickedButtonCalc)
END_MESSAGE_MAP()


// CDlgRegInfo message handlers

void CDlgRegInfo::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgRegInfo::Query(void)
{
	pUserInfo = new CUserInfoDlg();
	pUserInfo->SQL();
	userCompany = pUserInfo->m_Company;
	delete pUserInfo;
}

BOOL CDlgRegInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	Query();
	dtToday = COleDateTime::GetCurrentTime();
	bNewSecretArray	= new BYTE[sizeof(Encpypt::SecruitInfo)]; 
	/////////////////////////////////////////////////////////
	CString strOldInfo = Hex2String(bOldSecretArray,sizeof(Encpypt::SecruitInfo));
	GetDlgItem(IDC_EDIT_OLD_INFO)->SetWindowText(strOldInfo);
	
	/////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRegInfo::OnBnClickedButtonCalc()
{
		int InfoSize = sizeof(Encpypt::SecruitInfo);
		int CrcSize  = sizeof(pInfo.pInfo->CRC); 
		ZeroMemory(bNewSecretArray,InfoSize); 
		//拷贝公司名
	#ifdef _DEBUG 
		ZeroMemory(pInfo.pInfo->cCompany,sizeof(pInfo.pInfo->cCompany));
		#ifdef _UNICODE 
		w2c(pInfo.pInfo->cCompany,
			userCompany.GetBuffer(sizeof(pInfo.pInfo->cCompany)),
			sizeof(pInfo.pInfo->cCompany)<userCompany.GetLength()<<1?
			sizeof(pInfo.pInfo->cCompany):userCompany.GetLength()<<1);
		userCompany.ReleaseBuffer();
		#else
		strcpy(pInfo.pInfo->cCompany,userCompany);
		#endif
	
		COleDateTime dt = COleDateTime::GetCurrentTime();
		pInfo.pInfo->lStartDate		= dt.m_dt;
		pInfo.pInfo->lCurrentDate	= dt.m_dt;
		CDateTimeCtrl  *pdtCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_LAST);
		pdtCtrl->GetTime(dt);
		pInfo.pInfo->lEndDate	= dt.m_dt;


		pInfo.encrypt((byte*)pInfo.pInfo,bNewSecretArray,InfoSize-CrcSize);  
 
		/////////////////////////////////////////////////////////////////////// 

		int CrcValue = CModBusServer::CRC_16(bNewSecretArray,InfoSize-CrcSize); 
		pInfo.pInfo->CRC[0] = CrcValue>>8;
		pInfo.pInfo->CRC[1] = CrcValue&0xFF;
		bNewSecretArray[InfoSize-CrcSize] = CrcValue>>8; 
		bNewSecretArray[InfoSize-CrcSize+1] = CrcValue&0xFF;
		CString strNewInfo = Hex2String(bNewSecretArray,sizeof(Encpypt::SecruitInfo));
		GetDlgItem(IDC_EDIT_NEW_INFO)->SetWindowText(strNewInfo);
	#endif	
		CString strNewRegCode;
		GetDlgItem(IDC_EDIT_NEW_INFO)->GetWindowText(strNewRegCode);
		int newRegcodeLen = strNewRegCode.GetLength();
		if(newRegcodeLen/2>sizeof(Encpypt::SecruitInfo))
		{
			AfxMessageBox(_T("长度有误!\n"),MB_ICONWARNING+MB_OK);
			return; 
		}
		StrToHex(strNewRegCode,(char*)bNewSecretArray);
	// 
}

CString CDlgRegInfo::Hex2String(BYTE* pInChar, int length)
{
	CString strOldInfo;
	CString strHex;
	for (int i=0;i<length;i++)
	{
		strHex.Format(_T("%02X"),pInChar[i]);
		strOldInfo+=strHex;	
	} 
	return strOldInfo;
}

int CDlgRegInfo::StrToHex(CString pInStr, char *oBuffer)
{
	unsigned int length = pInStr.GetLength()/2;
	CString substring;
	for (unsigned int i=0;i<length;i++)
	{
		substring		= pInStr.Mid((i<<1),(i<<1)+2);
		char hexvalue	= _tcstol(substring,NULL,16);
		oBuffer[i]		= hexvalue;
	}
	return length;
}
