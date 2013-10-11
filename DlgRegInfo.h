#pragma once


// CDlgRegInfo dialog 
#include ".\tea\Encpypt.h" 
#include "UserInfoDlg.h"
class CDlgRegInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgRegInfo)

public:
	CDlgRegInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRegInfo();
	Encpypt pInfo; 

	CString userCompany;
	COleDateTime dtToday;
	BYTE *bOldSecretArray;
	BYTE *bNewSecretArray;
	CUserInfoDlg *pUserInfo;
// Dialog Data
	enum { IDD = IDD_REG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void Query(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCalc();
	CString Hex2String(BYTE* pInChar, int length);
	int StrToHex(CString pInStr, char *oBuffer);
};
 