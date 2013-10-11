// LRYCTRLView.h : interface of the CLRYCTRLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LRYCTRLVIEW_H__F9EB3CE4_7FDF_4D98_8997_75D7E69B4EE5__INCLUDED_)
#define AFX_LRYCTRLVIEW_H__F9EB3CE4_7FDF_4D98_8997_75D7E69B4EE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLRYCTRLView : public CView
{
protected: // create from serialization only
	CLRYCTRLView();
	DECLARE_DYNCREATE(CLRYCTRLView)

// Attributes
public:
	CLRYCTRLDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLRYCTRLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLRYCTRLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLRYCTRLView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LRYCTRLView.cpp
inline CLRYCTRLDoc* CLRYCTRLView::GetDocument()
   { return (CLRYCTRLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYCTRLVIEW_H__F9EB3CE4_7FDF_4D98_8997_75D7E69B4EE5__INCLUDED_)
