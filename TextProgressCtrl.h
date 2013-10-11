#if !defined(AFX_TEXTPROGRESSCTRL_H__E32758E8_E827_4838_BA4F_337862D8CB12__INCLUDED_)
#define AFX_TEXTPROGRESSCTRL_H__E32758E8_E827_4838_BA4F_337862D8CB12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl window

#ifndef _MEMDC 
#define _MEMDC 
//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//
// This class implements a memory Device Context

class CMemDC : public CDC
{
public:

    // constructor sets up the memory DC
    CMemDC(CDC* pDC) : CDC()
    {
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();

		BITMAP bmpInfo;

              
        if (m_bMemDC)    // Create a Memory DC
        {
            pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
			//CreateCompatibleDC(NULL);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_bitmap.GetBitmap(&bmpInfo);

            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else        // Make a copy of the relevent parts of the current DC for printing
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    
    // Destructor copies the contents of the mem DC to the original DC
    ~CMemDC()
    {
        if (m_bMemDC) 
        {    
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);

            //Swap back the original bitmap.
            SelectObject(m_pOldBitmap);
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.
            m_hDC = m_hAttribDC = NULL;
        }
    }

    // Allow usage as a pointer
    CMemDC* operator->() {return this;}
        
    // Allow usage as a pointer
    operator CMemDC*() {return this;}

private:
    CBitmap  m_bitmap;      // Offscreen bitmap
    CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
    CDC*     m_pDC;         // Saves CDC passed in constructor
    CRect    m_rect;        // Rectangle of drawing area.
    BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

#endif

class CTextProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CTextProgressCtrl();

// Attributes
public:

// Operations
public:
	void		SetForeColour(COLORREF col);
	void		SetBkColour(COLORREF col);
	void		SetTextForeColour(COLORREF col);
	void		SetTextBkColour(COLORREF col);
	COLORREF	GetForeColour();
	COLORREF	GetBkColour();
	COLORREF	GetTextForeColour();
	COLORREF	GetTextBkColour();

    void		SetShowText(BOOL bShow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextProgressCtrl();
    BOOL		m_bShowText  ;

	// Generated message map functions
protected: 

    CString		m_strText;
    int			m_nBarWidth;
	COLORREF	m_colFore,
				m_colBk;
	COLORREF	m_colTextFore,
				m_colTextBk; 
	//{{AFX_MSG(CTextProgressCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG 
	afx_msg LRESULT OnSetText(WPARAM nID,LPARAM szText ); 
	afx_msg LRESULT OnGetText(WPARAM nID,LPARAM szText );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPROGRESSCTRL_H__E32758E8_E827_4838_BA4F_337862D8CB12__INCLUDED_)
