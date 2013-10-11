#if !defined(AFX_MODIFYWEIGHT_H__CC70A762_170A_4687_BB2A_CB3A693D5D34__INCLUDED_)
#define AFX_MODIFYWEIGHT_H__CC70A762_170A_4687_BB2A_CB3A693D5D34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyWeight.h : header file
//
#include "LRYRDB.H"
/////////////////////////////////////////////////////////////////////////////
// CModifyWeight dialog

class CModifyWeight : public CDialog
{
// Construction
public:
	CModifyWeight(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModifyWeight)
	enum { IDD = IDD_MODEFY_YANGPIN };
	CString	m_AutoNO;
	CString	m_ModifyWeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyWeight)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyWeight)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void RefreshDataE();
	void UpWindowsText();
	void UpdateDB();
	void SQL();
	LRYRDB  m_RDB;
	_RecordsetPtr Rs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYWEIGHT_H__CC70A762_170A_4687_BB2A_CB3A693D5D34__INCLUDED_)
