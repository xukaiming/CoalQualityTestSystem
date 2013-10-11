#if !defined(AFX_CLYCHILDWND_H__39806078_350D_4D48_9AFA_168C98B70842__INCLUDED_)
#define AFX_CLYCHILDWND_H__39806078_350D_4D48_9AFA_168C98B70842__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XSplitterWnd.h"
// ClyChildWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClyChildWnd frame

class CClyChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CClyChildWnd)
protected:
	CClyChildWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClyChildWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CClyChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CClyChildWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CXSplitterWnd m_wndSplitter;
	//CSplitterWnd m_wndSplitter;
	BOOL m_bMainSplitterReady ;

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLYCHILDWND_H__39806078_350D_4D48_9AFA_168C98B70842__INCLUDED_)
