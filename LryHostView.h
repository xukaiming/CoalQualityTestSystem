#if !defined(AFX_LRYHOSTVIEW_H__152B1212_9323_4CF6_81C4_07A238794802__INCLUDED_)
#define AFX_LRYHOSTVIEW_H__152B1212_9323_4CF6_81C4_07A238794802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LryHostView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLryHostView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "LryHostDoc.h"	// Added by ClassView
#include "PageParam.h"
#include "PageCurve.h"
#include "PageOut.h"
#include "PageDebug.h"
#include "TextProgressCtrl.h"
#include "LryHostCtrl.h" 



#define TIME_EVENT_PROCBAR    1
#define TIME_EVENT_CLOSEWINDOWS 2
#define TIME_EVENT_ISLOGIN		3
 
class CLryHostView : public CFormView
{
protected:
	CLryHostView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLryHostView)

// Form Data
public:
	//{{AFX_DATA(CLryHostView)
	enum { IDD = IDD_LRY_FORM };
	CTextProgressCtrl	m_ProgBar;
	//}}AFX_DATA
	int m_iTimer;
	int m_iMax;
	int m_iCount;
	
// Attributes
public:
	CLryHostDoc * GetDocument() ;
// Operations
public: 
	void InitTestParam();
	void StartProgBar(CString sText, int iMax);
	LRESULT OnStartProgBar(WPARAM sText, LPARAM lP);
	CPropertySheet * m_dlgPropSheet;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLryHostView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT OnParmChanged(WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnImageDataChange(WPARAM sText, LPARAM lP);
	afx_msg LRESULT OnCalcData(WPARAM wp, LPARAM lP);
	virtual ~CLryHostView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLryHostView)
	afx_msg void OnDestroy();
	afx_msg void OnTestStart();
	afx_msg void OnTestStop();
	afx_msg void OnTimer(UINT nIDEvent); 
	afx_msg void OnFileClose();
	afx_msg void OnButton1();
//	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	CLryHostCtrl *pLryHostCtrl;
	CPageParam	* m_pPageParm;
	CPageCurve	* m_pPageCurve;
	CPageOut	* m_pPageOut;
	CPageDebug	* m_pPageDebug;
	CImageList  * m_pButtonImages ; 

	CString m_strProgBarText;
};

#ifndef _DEBUG  // debug version in hrdView.cpp
inline CLryHostDoc* CLryHostView::GetDocument()
   { return (CLryHostDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYHOSTVIEW_H__152B1212_9323_4CF6_81C4_07A238794802__INCLUDED_)
