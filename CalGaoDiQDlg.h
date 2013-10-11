#if !defined(AFX_CALGAODIQDLG_H__53AEB5D2_F973_4972_880B_9E404FAEA7B5__INCLUDED_)
#define AFX_CALGAODIQDLG_H__53AEB5D2_F973_4972_880B_9E404FAEA7B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalGaoDiQDlg.h : header file
//
#include "LRYRDB.h"
/////////////////////////////////////////////////////////////////////////////
// CCalGaoDiQDlg dialog

class CCalGaoDiQDlg : public CDialog
{
// Construction
public: 

	CString m_szTestNum; 
	CCalGaoDiQDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalGaoDiQDlg)
	enum { IDD = IDD_CAL_GAODIQ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalGaoDiQDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalGaoDiQDlg)
	afx_msg void OnCal();
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void GetParamFromDlg();
	LRYRDB  m_RDB;
	void UpdateDB();
	void RefreshDataQ();
	void UpWindowsText();
	_RecordsetPtr Rs;
	void SQL();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALGAODIQDLG_H__53AEB5D2_F973_4972_880B_9E404FAEA7B5__INCLUDED_)
