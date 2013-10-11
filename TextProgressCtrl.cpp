// TextProgressCtrl.cpp : implementation file
//

#include "stdafx.h"

#include "TextProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

CTextProgressCtrl::CTextProgressCtrl()
{  

    m_bShowText		= TRUE;
    m_strText.Empty();
	m_colFore		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colBk			= ::GetSysColor(COLOR_BACKGROUND);
	//m_colTextFore	= ::GetSysColor(COLOR_WINDOWTEXT);
	m_colTextFore	= ::GetSysColor(RGB(10,10,0));
	m_colTextBk		= ::GetSysColor(COLOR_WINDOW);	
    m_nBarWidth = -1;
}

CTextProgressCtrl::~CTextProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CTextProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
    ON_MESSAGE(WM_GETTEXT, OnGetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers
 
LRESULT CTextProgressCtrl::OnGetText(WPARAM cchTextMax,LPARAM szText )
{	
 
	if (!_tcsncpy(((LPTSTR)szText), m_strText, cchTextMax))
        return 0;
    else 
        return min(cchTextMax, (UINT) m_strText.GetLength());
}

LRESULT CTextProgressCtrl::OnSetText(WPARAM nID,LPARAM szText ) 	
{ 
	LRESULT result = Default();	
    if ( (!szText && m_strText.GetLength()) ||
         (szText && (m_strText != (LPTSTR)szText))   )
    {
        m_strText = (LPTSTR)szText;
        Invalidate();
    }
	
    return result;
}

BOOL CTextProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{	
 	return TRUE;
}

void CTextProgressCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CProgressCtrl::OnSize(nType, cx, cy);
	
    m_nBarWidth	= -1;   // Force update if SetPos called
}

void CTextProgressCtrl::OnPaint() 
{
	CFont font;
	CFont *oldFont;

	int iMin,iMax;
	GetRange(iMin,iMax);
	if (iMin >= iMax) 
        return;

    CRect LeftRect, RightRect, ClientRect;
    GetClientRect(ClientRect);

    double Fraction = (double)(CProgressCtrl::GetPos() - iMin) / ((double)(iMax - iMin));

	CPaintDC PaintDC(this); // device context for painting
    CMemDC dc(&PaintDC);
	//CPaintDC dc(this);    // device context for painting (if not double buffering)

    LeftRect = RightRect = ClientRect;

    LeftRect.right = LeftRect.left + (int)((LeftRect.right - LeftRect.left)*Fraction);
    dc.FillSolidRect(LeftRect, m_colFore);

	//在此选择字体
	font.CreateFont(20, 0, 0, 0, 600, FALSE, FALSE, 0,
	                     ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                     DEFAULT_PITCH | FF_SWISS,NULL ); //_T("微软雅黑")
	oldFont = dc.SelectObject(&font);

    RightRect.left = LeftRect.right;
    dc.FillSolidRect(RightRect, m_colBk);

    if (m_bShowText)
    {
        CString str;
 //       if (m_strText.GetLength())
            str = m_strText;
 //       else
 //           str.Format("%d%%", (int)(Fraction*100.0));

        dc.SetBkMode(TRANSPARENT);

        CRgn rgn;
        rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextBk);

        dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        rgn.DeleteObject();
        rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextFore);

        dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
	dc.SelectObject(oldFont);		//restore the old font to the DC

}

void CTextProgressCtrl::SetForeColour(COLORREF col)
{
	m_colFore = col;
}

void CTextProgressCtrl::SetBkColour(COLORREF col)
{
	//m_colBk = col;
	m_colBk = SendMessage(PBM_SETBARCOLOR,0,(LPARAM)col);
}

void CTextProgressCtrl::SetTextForeColour(COLORREF col)
{
	m_colTextFore = col;
}

void CTextProgressCtrl::SetTextBkColour(COLORREF col)
{
	m_colTextBk = col;
}

COLORREF CTextProgressCtrl::GetForeColour()
{
	return m_colFore;
}

COLORREF CTextProgressCtrl::GetBkColour()
{
	return m_colBk;
}

COLORREF CTextProgressCtrl::GetTextForeColour()
{
	return m_colTextFore;
}

COLORREF CTextProgressCtrl::GetTextBkColour()
{
	return m_colTextBk;
}
/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

void CTextProgressCtrl::SetShowText(BOOL bShow)
{ 
    if (::IsWindow(m_hWnd) && m_bShowText != bShow)
        Invalidate();
    m_bShowText = bShow;
}


void CTextProgressCtrl::OnDestroy() 
{
	CProgressCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
