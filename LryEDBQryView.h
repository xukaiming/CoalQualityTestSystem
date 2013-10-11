#if !defined(AFX_LRYEDBQRYVIEW_H__96235AB4_B091_4F6C_96D3_982BB4E5BB9D__INCLUDED_)
#define AFX_LRYEDBQRYVIEW_H__96235AB4_B091_4F6C_96D3_982BB4E5BB9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LryEDBQryView.h : header file
//
#include "LryEDBQryDoc.h"
#include "rmreportengine.h"	// Added by ClassView
#include "ReportSele.h"
/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


class CLryEDBQryView : public CXTListView
{
protected:
	CLryEDBQryView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLryEDBQryView)

// Form Data
public:
	//{{AFX_DATA(CLryEDBQryView)
	//enum { IDD = IDD_LRYEDBQRYVIEW_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CLryEDBQryDoc * GetDocument();
	afx_msg void OnUpdatDBDateTime(CCmdUI *pCmdUI); 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLryEDBQryView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLryEDBQryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif 
	
	afx_msg  void OnUpdateMenuState(CCmdUI* pCmdUI);
	afx_msg  void OnUpdateMenuState1(CCmdUI* pCmdUI);
	afx_msg  void OnUpdateCursorMove(CCmdUI* pCmdUI) ;
	// Generated message map functions
	//{{AFX_MSG(CLryEDBQryView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEModify();
	afx_msg void OnPrintReportSetting();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AverageReport(CReportSele *pDlg);
	void SingleReport(CReportSele *pDlg);
	CRMReportEngine pReportdlg;
	void FillList();
	
};

/////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG  // debug version in LRYCTRLView.cpp
inline CLryEDBQryDoc* CLryEDBQryView::GetDocument()
   { return (CLryEDBQryDoc*) m_pDocument; }
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYEDBQRYVIEW_H__96235AB4_B091_4F6C_96D3_982BB4E5BB9D__INCLUDED_)
