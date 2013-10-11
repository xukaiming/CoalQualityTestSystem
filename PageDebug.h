#if !defined(AFX_PAGEDEBUG_H__B7DD97BD_FEC8_422C_9253_84A0F285762B__INCLUDED_)
#define AFX_PAGEDEBUG_H__B7DD97BD_FEC8_422C_9253_84A0F285762B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDebug.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDebug dialog
#include "LryHostCtrl.h"
class CPageDebug : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageDebug)

// Construction
public:

	CLryHostCtrl * m_pCtrl;
	CPageDebug();
	~CPageDebug();

// Dialog Data
	//{{AFX_DATA(CPageDebug)
	enum { IDD = IDD_PAGE_DEBUG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDebug)
	afx_msg void OnDebugDianhuo();
	afx_msg void OnDebugJinshui();
	afx_msg void OnDebugFangshui();
	afx_msg void OnDebugJiaoban();
	afx_msg void OnDebugOutjiaoban();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LRESULT OnImageDataChange(WPARAM sText, LPARAM lP);
public:
	afx_msg void OnBnClickedDebugModifyid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDEBUG_H__B7DD97BD_FEC8_422C_9253_84A0F285762B__INCLUDED_)
