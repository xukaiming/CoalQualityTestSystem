// DBQryGView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DBQryGView.h"


// CDBQryGView

IMPLEMENT_DYNCREATE(CDBQryGView, CDBQryView)

CDBQryGView::CDBQryGView()
	: CDBQryView( )
{

}

CDBQryGView::~CDBQryGView()
{
}

void CDBQryGView::DoDataExchange(CDataExchange* pDX)
{
	CDBQryView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDBQryGView, CDBQryView)
END_MESSAGE_MAP()


// CDBQryGView diagnostics

#ifdef _DEBUG
void CDBQryGView::AssertValid() const
{
	CDBQryView::AssertValid();
}

#ifndef _WIN32_WCE
void CDBQryGView::Dump(CDumpContext& dc) const
{
	CDBQryView::Dump(dc);
}
#endif
#endif //_DEBUG

//
// CDBQryGView message handlers
