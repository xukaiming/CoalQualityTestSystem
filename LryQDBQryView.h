#if !defined(AFX_LRYDBQRYVIEW_H__0A6DF75A_4540_49B3_B9F3_08AFF1FB2B52__INCLUDED_)
#define AFX_LRYDBQRYVIEW_H__0A6DF75A_4540_49B3_B9F3_08AFF1FB2B52__INCLUDED_

#include "rmreportengine.h"	// Added by ClassView
#include "ReportSele.h"
#include "LryQDBQryDoc.h"
 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LryDBQryView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "DBListView.h"

#undef  CListView
#define CListView CDBListView

class CLryQDBQryView : public CListView
{
protected:
	CLryQDBQryView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLryQDBQryView)

// Form Data
public:
	//{{AFX_DATA(CLryQDBQryView)
	//enum { IDD = IDD_DIALOG_COMM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CLryQDBQryDoc * GetDocument();
// Operations
public:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLryQDBQryView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLryQDBQryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg void OnUpdateCursorMove(CCmdUI* pCmdUI) ;
	afx_msg void OnUpdateMenuState(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuState1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuAvgCheck(CCmdUI* pCmdUI); 
	afx_msg void OnUpdateMenuAverage(CCmdUI* pCmdUI);
	// Generated message map functions
	//{{AFX_MSG(CLryQDBQryView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPrintReportSetting();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnAnalyseDantong();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRMReportEngine pReportdlg;
	void OnAnalyseGaoDiWei();
	void PXResult(CReportSele *pDlg);
	void SingleResult(CReportSele *pDlg);
	void RiBao(CReportSele *pDlg); 
	void FillList();
private:
	
public:
	CString DBVariantToStr(_variant_t var, CString ParamName);
	
};

/////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG  // debug version in LRYCTRLView.cpp
inline CLryQDBQryDoc* CLryQDBQryView::GetDocument()
   { return (CLryQDBQryDoc*) m_pDocument; }
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYDBQRYVIEW_H__0A6DF75A_4540_49B3_B9F3_08AFF1FB2B52__INCLUDED_)
