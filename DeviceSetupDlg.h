#if !defined(AFX_DEVICESETUPDLG_H__8E5098BA_A167_4F48_A8F5_5BCC8433BA22__INCLUDED_)
#define AFX_DEVICESETUPDLG_H__8E5098BA_A167_4F48_A8F5_5BCC8433BA22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceSetupDlg.h : header file
//
#include "LRYRDB.H"
/////////////////////////////////////////////////////////////////////////////
// CDeviceSetupDlg dialog

class CDeviceSetupDlg : public CDialog     //¡ø»»“«
{
// Construction
public:
	CDeviceSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeviceSetupDlg)
	enum { IDD = IDD_SETUP_DEVICE };
	UINT	m_RowNO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeviceModify();
	afx_msg void OnDeviceAdd();
	afx_msg void OnDeviceDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	LRYRDB LryRdb;
	_RecordsetPtr	pRsDev;
	UINT	m_ROWNO;
	CString strSQL;	
private:
	CString GetCurrSeleParentString();
	void MoveDBPtr();
	CString GetCurrentSeleString();
	void SQL();
	void UpdateDB();
	void GetParamFromDlg();
	void UpdateWinText();
	void RefreshRDB();
public:
//	virtual HRESULT get_accHelpTopic(BSTR *pszHelpFile, VARIANT varChild, long *pidTopic);
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICESETUPDLG_H__8E5098BA_A167_4F48_A8F5_5BCC8433BA22__INCLUDED_)
