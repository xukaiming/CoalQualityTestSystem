// LRYCTRLView.cpp : implementation of the CLRYCTRLView class
//

#include "stdafx.h"
#include "LRYCTRL.h"

#include "LRYCTRLDoc.h"
#include "LRYCTRLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLView

IMPLEMENT_DYNCREATE(CLRYCTRLView, CView)

BEGIN_MESSAGE_MAP(CLRYCTRLView, CView)
	//{{AFX_MSG_MAP(CLRYCTRLView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLView construction/destruction

CLRYCTRLView::CLRYCTRLView()
{
	// TODO: add construction code here

}

CLRYCTRLView::~CLRYCTRLView()
{
}

BOOL CLRYCTRLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLView drawing

void CLRYCTRLView::OnDraw(CDC* pDC)
{
	CLRYCTRLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLView diagnostics

#ifdef _DEBUG
void CLRYCTRLView::AssertValid() const
{
	CView::AssertValid();
}

void CLRYCTRLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLRYCTRLDoc* CLRYCTRLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLRYCTRLDoc)));
	return (CLRYCTRLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLView message handlers
