#if !defined(AFX_CRECALQ_H__0B9ED318_0D8F_4267_8446_9B0094221409__INCLUDED_)
#define AFX_CRECALQ_H__0B9ED318_0D8F_4267_8446_9B0094221409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CReCalQ.h : header file
//
#include "LRYRDB.H"
/////////////////////////////////////////////////////////////////////////////
// CCReCalQ dialog

class CCReCalQ : public CDialog
{
// Construction
public:
	CCReCalQ(CWnd* pParent = NULL);   // standard constructor  
	CString m_szTestNum; 
// Dialog Data
	//{{AFX_DATA(CCReCalQ)
	enum { IDD = IDD_RECAL_Q };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCReCalQ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCReCalQ)
	virtual BOOL OnInitDialog();
	afx_msg void OnRecaleCal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LRYRDB  m_RDB;
	void UpdateDB();
	void GetParamFromDlg();
	void RefreshDataQ();
	void UpWindowsText();
	_RecordsetPtr Rs;
	void SQL();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRECALQ_H__0B9ED318_0D8F_4267_8446_9B0094221409__INCLUDED_)
