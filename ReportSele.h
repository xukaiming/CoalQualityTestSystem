#if !defined(AFX_REPORTSELE_H__890DA06D_69E5_4125_B9C6_AD64B98454E2__INCLUDED_)
#define AFX_REPORTSELE_H__890DA06D_69E5_4125_B9C6_AD64B98454E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportSele.h : header file
//
#undef CListBox
/////////////////////////////////////////////////////////////////////////////
// CReportSele dialog

class CReportSele : public CDialog
{
// Construction
public:
	CReportSele(CWnd* pParent = NULL);   // standard constructor
	TCHAR cPath[MAX_PATH] ;
	CString cFileName;
//	CString cFile;
// Dialog Data
	//{{AFX_DATA(CReportSele)
	enum { IDD = IDD_DIALOG_SELE };
	CListBox	m_ListFileName;
	CString	cFile;
	int		m_iSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportSele)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportSele)
	virtual BOOL OnInitDialog(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void FindFile();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTSELE_H__890DA06D_69E5_4125_B9C6_AD64B98454E2__INCLUDED_)
