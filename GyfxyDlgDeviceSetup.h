#pragma once


#include "Gyfxy1RDB.h" 
#include "DlgInstrumentSet.h"
#include "xlistctrl.h"
// CGyfxyDlgDeviceSetup dialog

class CGyfxyDlgDeviceSetup : public CDlgInstrumentSet
{
	DECLARE_DYNAMIC(CGyfxyDlgDeviceSetup)

public:
	CGyfxyDlgDeviceSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGyfxyDlgDeviceSetup();

// Dialog Data
	enum { IDD = IDD_DLG_GYFXY_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void UpdateWinText(void);
	void GetParamFromDlg(void);
	CXListCtrl mListAttrib; 
	CStringArray ColNameArray;
	bool m_bInitListCtl;

	LOGFONT   lf; 
	CFont	font;


	CStringArray strAlgorithm;

	virtual BOOL OnInitDialog();
	void InitList(void);
	afx_msg void OnBnClickedDeviceModify();
};
