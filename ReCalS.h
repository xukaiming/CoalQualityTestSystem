#pragma once
#include "ClyRDB.h"
#include "afxdtctl.h"

// CReCalS dialog

class CReCalS : public CDialog
{
	DECLARE_DYNAMIC(CReCalS)

public:
	CReCalS(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReCalS();

// Dialog Data
	enum { IDD = IDD_DIALOG_RECAL_S };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CClyRDB *pClyRdb;
	virtual BOOL OnInitDialog();
	void UpWindowsText();
	void GetParamFromDlg();
	afx_msg void OnBnClickedCal();
	CDateTimeCtrl m_DateTimePicker;
	CDateTimeCtrl m_DateTimeTest;
};
