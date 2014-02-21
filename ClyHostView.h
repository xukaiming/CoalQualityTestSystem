#if !defined(AFX_CLYHOSTVIEW_H__9041F39D_0DB0_4D29_9D33_A32E204EED88__INCLUDED_)
#define AFX_CLYHOSTVIEW_H__9041F39D_0DB0_4D29_9D33_A32E204EED88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClyHostView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClyHostView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TextProgressCtrl.h"
#include "CurveEdit.h"
#include "ClyEditListCtrl.h"

#include "CLYHostCtrl.h" 
#include "CLyHostDoc.h"
#include "afxwin.h"
#include "afxcmn.h"

class CClyHostView : public CFormView
{
protected:
	CClyHostView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CClyHostView)

// Form Data
public:
	//{{AFX_DATA(CClyHostView)
	enum { IDD = IDD_CLYHOSTVIEW_FORM };
	CTextProgressCtrl	m_ProgBar;
	//}}AFX_DATA

// Attributes
public:
	CCLYHostCtrl *pCLYHostCtrl;
// Operations
public:
	CClyHostDoc  *GetDocument() ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClyHostView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CClyHostView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CClyHostView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPumbUp();
	afx_msg void OnJb();
	afx_msg void OnTestStart();
	afx_msg void OnTestStop();
	afx_msg void OnSysRst();
	afx_msg void OnWarmUp();
	//}}AFX_MSG
	afx_msg LRESULT OnImageDataChange(WPARAM sText, LPARAM lP);
	afx_msg LRESULT OnStartProgBar(WPARAM sText, LPARAM lP);
	DECLARE_MESSAGE_MAP()
private:
	CString m_strProgBarText;
	int m_iTimer;
	int m_iMax;
	int m_iCount;
	CStringArray comboArray;
private:
	BOOL IsEdit( CWnd* pWnd );

	void StartProgBar(CString sText, int iMax);
	void InitTestParam();
	void InitParamFromDB();
public:
	// 绘制电流曲线
	//int DrawCurve(void);
//	afx_msg void OnPaint();
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	// //显示曲线
	CCurveEdit m_CurveEdit;   
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	// //CEditListCtrl 显示结果
	CClyEditListCtrl m_ResultListCtrl;
public:
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedAddSample();
	afx_msg void OnDeleteRecord(); 
	afx_msg void OnAddRecord();
	afx_msg LRESULT OnCalcData(WPARAM wp, LPARAM lP);
	//响应热键
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI); 
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy() ;
	afx_msg	void OnEditPaste();
	afx_msg	void OnEditCut();
private:
	// //void CPageParam
	void GetParamFromDlg(CClyRDB * pRDB);
public:
	afx_msg void OnBnClickedButtonR();
private:
	void UpdateRDBResult(CClyRDB * pRDB);  
	
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};

#ifndef _DEBUG  // debug version in LRYCTRLView.cpp
inline CClyHostDoc* CClyHostView::GetDocument()
{ return (CClyHostDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLYHOSTVIEW_H__9041F39D_0DB0_4D29_9D33_A32E204EED88__INCLUDED_)
