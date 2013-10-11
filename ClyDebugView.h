#if !defined(AFX_CLYDEBUGVIEW_H__0F779335_70A4_45B1_9924_90BE8CE42290__INCLUDED_)
#define AFX_CLYDEBUGVIEW_H__0F779335_70A4_45B1_9924_90BE8CE42290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClyDebugView.h : header file
//
#include "ClyRDB.h"	// Added by ClassView
#include "CLYHostCtrl.h"
#include "Picture.h"
/////////////////////////////////////////////////////////////////////////////
// CClyDebugView view

class CClyDebugView : public CFormView
{
protected:
	CClyDebugView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CClyDebugView)
// Form Data
public:
	//{{AFX_DATA(CCLYDebugView)
	enum { IDD = IDD_DIALOG_CLY_DEBUG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
// Attributes
public:

// Operations
public: 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClyDebugView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	virtual ~CClyDebugView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CClyDebugView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CClyRDB *pClyRdb;
	CCLYHostCtrl *pHostCtrl;
	CFont	*pHexFont; 
public: 
	afx_msg void OnBnClickedBtnId();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonFreqAdj();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnPosMov();
//	afx_msg void OnEnChangeEditCurrPos();
	afx_msg void OnBnClickedBtnPosMov2();
	CPicture m_pPos1;
	CPicture m_pPosN;
	CPicture m_pPosHighTemp;
	CPicture m_pPosPutSamplePos;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CLYDEBUGVIEW_H__0F779335_70A4_45B1_9924_90BE8CE42290__INCLUDED_)
