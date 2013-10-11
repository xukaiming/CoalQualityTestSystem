// XSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "XSplitterWnd.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXSplitterWnd

CXSplitterWnd::CXSplitterWnd()
{
	mHideMode=NULLMODE;
	restorelength=DEFAULTSIZE;
	m_hidenstate=FALSE;
	canresize=TRUE;
	hidenlength=DEFAULTHIDESIZE;
	hitrect.SetRect(0,0,50,50);

	pPainted=FALSE;
	bleftdown=FALSE;
		
//-----------------------------------//

	m_cxSplitter = m_cySplitter = 8;
	m_cxBorderShare = m_cyBorderShare = 0;
	m_cxSplitterGap = m_cySplitterGap = 9;
	m_cxBorder = m_cyBorder = 1;


	
}

CXSplitterWnd::~CXSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CXSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CXSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
	
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXSplitterWnd message handlers
void CXSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	if((nType != splitBorder) || (pDC == NULL))
	{
		CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		return;
	}
	if(mHideMode){
		pDC->Draw3dRect(rectArg, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
		DrawMovingRect(pDC);
	}	
}

/*
void CXSplitterWnd::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap);
	LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);

	// adjust the panes (and optionally scroll bars)

	// give the hint for the maximum number of HWNDs
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);

	// size of scrollbars
	int cx = rectClient.right - rectInside.right;
	int cy = rectClient.bottom - rectInside.bottom;

	// reposition size box
	if (m_bHasHScroll && m_bHasVScroll)
	{
		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
		ASSERT(pScrollBar != NULL);

		// fix style if necessary
		BOOL bSizingParent = (GetSizingParent() != NULL);
		// modifyStyle returns TRUE if style changes
		if (pScrollBar->ModifyStyle(SBS_SIZEGRIP|SBS_SIZEBOX,
				bSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX))
			pScrollBar->Invalidate();
		pScrollBar->EnableWindow(bSizingParent);

		// reposition the size box
		DeferClientPos(&layout, pScrollBar,
			rectInside.right,
			rectInside.bottom, cx, cy, TRUE);
	}

	// reposition scroll bars
	if (m_bHasHScroll)
	{
		int cxSplitterBox = m_cxSplitter;// split box bigger
		int x = rectClient.left;
		int y = rectInside.bottom;
		for (int col = 0; col < m_nCols; col++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
			ASSERT(pScrollBar != NULL);
			int cx = m_pColInfo[col].nCurSize;
			if (col == 0 && m_nCols < m_nMaxCols)
				x += cxSplitterBox, cx -= cxSplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			x += cx + m_cxSplitterGap;
		}
	}

	if (m_bHasVScroll)
	{
		int cySplitterBox = m_cySplitter;// split box bigger
		int x = rectInside.right;
		int y = rectClient.top;
		for (int row = 0; row < m_nRows; row++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
			ASSERT(pScrollBar != NULL);
			int cy = m_pRowInfo[row].nCurSize;
			if (row == 0 && m_nRows < m_nMaxRows)
				y += cySplitterBox, cy -= cySplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			y += cy + m_cySplitterGap;
		}
	}

	//BLOCK: Reposition all the panes
	{
		int x = rectClient.left;
		for (int col = 0; col < m_nCols; col++)
		{
			int cx = m_pColInfo[col].nCurSize;
			int y = rectClient.top;
			for (int row = 0; row < m_nRows; row++)
			{
				int cy = m_pRowInfo[row].nCurSize;
				CWnd* pWnd = GetPane(row, col);
				DeferClientPos(&layout, pWnd, x, y, cx, cy, FALSE);
				y += cy + m_cySplitterGap;
			}
			x += cx + m_cxSplitterGap;
		}
	}

	// move and resize all the windows at once!
	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE(_T(("Warning: DeferWindowPos failed - low system resources.\n");

	// invalidate all the splitter bars (with NULL pDC)
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
	//CSplitterWnd::RecalcLayout();
}
*/

void CXSplitterWnd::OnInvertTracker(const CRect &rect)
{
	CSplitterWnd::OnInvertTracker(rect);
}

BOOL CXSplitterWnd::DoKeyboardSplit()
{
	return	CSplitterWnd::DoKeyboardSplit();
}

void CXSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	bleftdown = true;	
	if(canresize && !IsCursorInValidRect(point))
		CSplitterWnd::OnLButtonDown(nFlags, point);
	else{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

void CXSplitterWnd::OnMouseMove(UINT nFlags, CPoint point) 
{	

	SetTimer(1,10,NULL);	
	if(canresize && !IsCursorInValidRect(point))
		CSplitterWnd::OnMouseMove(nFlags, point);
	else
		CWnd::OnMouseMove(nFlags, point);
}

int CXSplitterWnd::GetHideMode()
{
	return mHideMode;
}

void CXSplitterWnd::SetHideMode(int newmode)
{
	if(newmode<=HIDEMODE_BOTTOMEDGE && newmode>=NULLMODE)
		mHideMode=newmode;
}

BOOL CXSplitterWnd::CreateStatic(CWnd *pParentWnd, int nRows, int nCols,int HideMode, DWORD dwStyle,UINT nID)
{
	if(HideMode<=HIDEMODE_BOTTOMEDGE && HideMode>=NULLMODE)
	{
		mHideMode=HideMode;
		m_hidenstate=FALSE;
	}
	return CSplitterWnd::CreateStatic(pParentWnd,nRows,nCols,dwStyle,nID);
	
}

BOOL CXSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	
	if(canresize && !IsCurCursorInRect())
		return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
	else	
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CXSplitterWnd::IsHiden()
{
	return m_hidenstate;
}

void CXSplitterWnd::DoHide()
{
	if(m_hidenstate==TRUE)
		return;
	
	m_hidenstate=TRUE;
	CRect rect;
	GetClientRect(&rect);
	switch(mHideMode) {
	case HIDEMODE_LEFTEDGE:
		GetColumnInfo(0,restorelength,hidenlength);
		SetColumnInfo(0,hidenlength,hidenlength);
		break;
	case HIDEMODE_RIGHTEDGE:
		GetColumnInfo(0,restorelength,hidenlength);
		SetColumnInfo(0,rect.Width(),hidenlength);		
		break;
	case HIDEMODE_TOPEDGE:
		GetRowInfo(0,restorelength,hidenlength);
		SetRowInfo(0,hidenlength,hidenlength);
		break;
	case HIDEMODE_BOTTOMEDGE:
		GetRowInfo(0,restorelength,hidenlength);
		SetRowInfo(0,rect.Height(),hidenlength);
		break;
	default:
		break;
	}
	RecalcLayout();
}

void CXSplitterWnd::DoRestore()
{
	if(m_hidenstate==FALSE)
		return;
	m_hidenstate=FALSE;
	CRect rect;
	GetClientRect(&rect);
	switch(mHideMode) {
	case HIDEMODE_LEFTEDGE:
	case HIDEMODE_RIGHTEDGE:
		SetColumnInfo(0,restorelength,hidenlength);		
		break;
	case HIDEMODE_TOPEDGE:
	case HIDEMODE_BOTTOMEDGE:
		SetRowInfo(0,restorelength,hidenlength);
		break;
	default:
		break;
	}
	RecalcLayout();
	
}

void CXSplitterWnd::Toggle()
{
	if(mHideMode==NULLMODE)
		return;
	if(!m_hidenstate)
		DoHide();
	else
		DoRestore();
}

BOOL CXSplitterWnd::IsCanResize()
{
	return canresize;
}

void CXSplitterWnd::EnableResize(BOOL canresize1)
{
	canresize=canresize1;
}

void CXSplitterWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	bleftdown = false;	
	if(canresize && !IsCursorInValidRect(point))
		CSplitterWnd::OnLButtonUp(nFlags, point);
	else{	
		Toggle();
		CWnd::OnLButtonUp(nFlags, point);
	}	
}

BOOL CXSplitterWnd::IsInHitTest(CPoint pt)
{
	if(mHideMode==NULLMODE)
		return FALSE;
	CRect rect;
	GetClientRect(&rect);
	if(mHideMode==1 || mHideMode==2)
	{
		if(pt.y>=(rect.Height()/2-25) && pt.y<=(rect.Height()/2+25))
			return	TRUE;
		else
			return FALSE;
	}
	else
	{
		if(pt.y>=(rect.Width()/2-25) && pt.y<=(rect.Width()/2+25))
			return	TRUE;
		else
			return FALSE;		
	}
}
/* Original Method Implementation:
*
* AFX_STATIC void AFXAPI _AfxLayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
*	int nMax, int nSize, int nSizeSplitter)
*
* In WINSPLIT.CPP
*
* Credit for this adaptation goes to Kris.
*/
// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy
void CXSplitterWnd::LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
	int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	CSplitterWnd::CRowColInfo* pInfo;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			(pInfo-1)->nCurSize += nSize + 2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - 2))
					pInfo->nCurSize -= (nSizeSplitter - 2);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

/* Original Method Implementation:
*
* AFX_STATIC void AFXAPI _AfxDeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
*	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
*
* In WINSPLIT.CPP
*
* Credit for this adaptation goes to Kris.
*/
// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
void CXSplitterWnd::DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		BOOL bNeedBorder = (cx <= 1 || cy <= 1);
		pWnd->ModifyStyle(bNeedBorder ? 0 : 1, bNeedBorder ? 1 : 0);
	}
	CRect rect(x, y, x+cx, y+cy);

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) || pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	{
		rect.InflateRect(1, 1); // for proper draw CFlatSplitterWnd in view
//		rect.InflateRect(afxData.cxBorder2, afxData.cyBorder2);
	}

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
		AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
}

BOOL CXSplitterWnd::OnEraseBkgnd(CDC* pDC) 
{	
	return CSplitterWnd::OnEraseBkgnd(pDC);
}

BOOL CXSplitterWnd::IsCursorInValidRect(CPoint pt)
{
	if(mHideMode==NULLMODE)
		return FALSE;
	CRect rect;
	GetValidRect(&rect);
	return rect.PtInRect(pt);
}

void CXSplitterWnd::GetValidRect(CRect *rect)
{
	CRect rect1;
	int leftortop,crmin;
	GetWindowRect(&rect1);
	if(mHideMode==1 || mHideMode==2)
	{
		GetColumnInfo(0,leftortop,crmin);
		rect->SetRect(leftortop+2,rect1.Height()/2-25,leftortop+10,rect1.Height()/2+25);
	}
	else
	{
		GetRowInfo(0,leftortop,crmin);
		rect->SetRect(rect1.Width()/2-25,leftortop+2,rect1.Width()/2+25,leftortop+10);
	}
	CString str;
	str.Format(_T("%d %d"),rect->Width(),rect->Height());
	//AfxMessageBox(str);
}

BOOL CXSplitterWnd::IsCurCursorInRect()
{
	GetCursorPos(&curpt);
	ScreenToClient(&curpt);
	return IsCursorInValidRect(curpt);
}


void CXSplitterWnd::DrawMovingRect(CDC *pDC)
{
	int index=(int)mHideMode-1 + (int)m_hidenstate*4;
	switch(index) {
	case 0:
	case 5:
		rectmemdc.SelectObject(&rectbmpleft);
		break;
	case 1:
	case 4:
		rectmemdc.SelectObject(&rectbmpright);
		break;
	case 2:
	case 7:
		rectmemdc.SelectObject(&rectbmpup);
		break;
	case 3:
	case 6:
		rectmemdc.SelectObject(&rectbmpdown);
		break;
	default:
		rectmemdc.SelectObject(&rectbmpleft);
		break;
	}
	GetValidRect(&hitrect);
	if(mHideMode==2 || mHideMode==1)
		pDC->BitBlt(hitrect.left,hitrect.top,hitrect.Width(),hitrect.Height(),&rectmemdc,0+8*(int)pPainted,0,SRCCOPY);
	else if(mHideMode==3 || mHideMode==4)
		pDC->BitBlt(hitrect.left,hitrect.top,hitrect.Width(),hitrect.Height(),&rectmemdc,0+50*(int)pPainted,0,SRCCOPY);
				
}

BOOL CXSplitterWnd::Set4Bitmaps(UINT left, UINT right, UINT down, UINT up)
{
	if(!(rectbmpleft.LoadBitmap(left) && rectbmpright.LoadBitmap(right)
	&& rectbmpup.LoadBitmap(up)		&& rectbmpdown.LoadBitmap(down)) )
	return FALSE;
	return rectmemdc.CreateCompatibleDC(GetDC());
}

void CXSplitterWnd::OnTimer(UINT nIDEvent) 
{

	if (bleftdown)	
	{		
		KillTimer (1);
		if (pPainted)		
			InvalidateRect(&hitrect,FALSE);
		pPainted = FALSE;		
		return;	
	}
	
	if (!IsCurCursorInRect())	
	{		
		KillTimer (1);
		
		if (pPainted)			
			InvalidateRect(&hitrect,FALSE);
		pPainted = FALSE;
		return;	
	}
	else
	{
		if (!pPainted)
		{
			pPainted = TRUE;
			InvalidateRect(&hitrect,FALSE);
		}
	}
	
	CSplitterWnd::OnTimer(nIDEvent);
}
void CXSplitterWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonUp(nFlags, point);
	SwitchOppositeDirection();

}

void CXSplitterWnd::SwitchOppositeDirection()
{
	switch(mHideMode) {
	case 1:
	case 3:
		mHideMode++;
		break;
	case 2:
	case 4:
		mHideMode--;
	}	
	InvalidateRect(&hitrect,FALSE);
}

void CXSplitterWnd::OnSize(UINT nType, int cx, int cy)
{
	CSplitterWnd::OnSize(nType, cx, cy);
}