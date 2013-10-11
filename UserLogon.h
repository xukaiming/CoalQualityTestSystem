#if !defined(AFX_USERLOGON_H__88FA9A23_94FA_43BD_B063_AD09E71AE917__INCLUDED_)
#define AFX_USERLOGON_H__88FA9A23_94FA_43BD_B063_AD09E71AE917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserLogon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserLogon dialog

class CUserLogon : public CDialog
{
// Construction
public:

	CUserLogon(CWnd* pParent = NULL);   // standard constructor
	CString	m_DBUserPassword;
// Dialog Data
	//{{AFX_DATA(CUserLogon)
	enum { IDD = IDD_LOGIN };
	CString	m_UserName;	
	CString	m_UserPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLogon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserLogon)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	_RecordsetPtr Rs;
	void SQL();
	void LoadFromDB();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLOGON_H__88FA9A23_94FA_43BD_B063_AD09E71AE917__INCLUDED_)
