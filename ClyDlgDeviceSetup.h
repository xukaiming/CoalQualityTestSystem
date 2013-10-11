#pragma once

#include "ClyRDB.h" 
#include "DlgInstrumentSet.h"
// CClyDlgDeviceSetup dialog


class CClyDlgDeviceSetup : public CDlgInstrumentSet
{
	DECLARE_DYNAMIC(CClyDlgDeviceSetup)

public:
	CClyDlgDeviceSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClyDlgDeviceSetup(); 
	enum { IDD = IDD_DIALOG_CLY_SETUP };
protected: 
	virtual void UpdateWinText(void);
	virtual void GetParamFromDlg(void); 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support 
	DECLARE_MESSAGE_MAP()
public: 
//	afx_msg void OnBnClickedDeviceAdd(); 
//	afx_msg void OnBnClickedDeviceModify();
//	afx_msg void OnBnClickedDeviceAdd();
};
