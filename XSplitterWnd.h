 #if !defined(AFX_XSPLITTERWND_H__56998EE9_A70F_48ED_9DFA_71B632FC8D96__INCLUDED_)
#define AFX_XSPLITTERWND_H__56998EE9_A70F_48ED_9DFA_71B632FC8D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XSplitterWnd.h : header file
//
#define  NULLMODE			 0
#define  HIDEMODE_LEFTEDGE	 1
#define  HIDEMODE_RIGHTEDGE	 2
#define  HIDEMODE_TOPEDGE	 3
#define  HIDEMODE_BOTTOMEDGE 4
#define	 DEFAULTSIZE		 10
#define	 DEFAULTHIDESIZE     0

/////////////////////////////////////////////////////////////////////////////
// CXSplitterWnd window
//切分窗体控制条扩展类　设计者：vcfly
// http://www.vcfly.net
class CXSplitterWnd : public CSplitterWnd
{
// Construction
public:
	CXSplitterWnd();
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
// Attributes
public:
	static void DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
		CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);
	static void LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
		int nMax, int nSize, int nSizeSplitter);
	
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXSplitterWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Set4Bitmaps(UINT left,UINT right,UINT down,UINT up);
	void SetHideMode(int newmode);
	void EnableResize(BOOL canresize);
	BOOL IsCanResize();
	void Toggle();
	void DoRestore();
	void DoHide();
	BOOL IsHiden();
	BOOL CreateStatic( CWnd* pParentWnd, int nRows, int nCols, int HideMode=NULLMODE, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = AFX_IDW_PANE_FIRST );
	int GetHideMode();
	virtual BOOL DoKeyboardSplit();
	virtual void OnInvertTracker(const CRect& rect);
	//virtual void RecalcLayout();
	virtual ~CXSplitterWnd();

	// Generated message map functions
protected:
	void GetValidRect(CRect * rect);
	//{{AFX_MSG(CXSplitterWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void SwitchOppositeDirection();
	void DrawMovingRect(CDC *pDC);
	BOOL IsCurCursorInRect();
	BOOL IsCursorInValidRect(CPoint pt);
	BOOL IsInHitTest(CPoint pt);
	int		mHideMode;
	BOOL	m_hidenstate;
	BOOL    canresize;
	int		restorelength;
	int     hidenlength;
	CRect	hitrect;
	CBitmap rectbmpleft,rectbmpright,rectbmpup,rectbmpdown;
	CDC		rectmemdc;
	CPoint curpt;			//当前鼠标的位置
	BOOL	pPainted;
	BOOL    bleftdown;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSPLITTERWND_H__56998EE9_A70F_48ED_9DFA_71B632FC8D96__INCLUDED_)
