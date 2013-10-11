#if !defined(AFX_USERINFODLG_H__46C2DA27_4EFE_4785_98E1_B4A786DA35BA__INCLUDED_)
#define AFX_USERINFODLG_H__46C2DA27_4EFE_4785_98E1_B4A786DA35BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg dialog

class CUserInfoDlg : public CDialog
{
// Construction
public:
	void SaveToDB();
	CUserInfoDlg(CWnd* pParent = NULL);   // standard constructor
	void SQL();
// Dialog Data
	//{{AFX_DATA(CUserInfoDlg)
	enum { IDD = IDD_DIALOG_CHANGEUSERINFO };
	CString	m_UserName;
	CString	m_NewPwd;
	CString	m_ReNewPwd;
	CString	m_Company;
	//}}AFX_DATA
	_RecordsetPtr Rs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadFromDB();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERINFODLG_H__46C2DA27_4EFE_4785_98E1_B4A786DA35BA__INCLUDED_)
