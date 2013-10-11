// GyfxyHostDoc1.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyHostDoc1.h"


// CGyfxyHostDoc_G5200

IMPLEMENT_DYNCREATE(CGyfxyHostDoc_G5200, CGyfxyHostDoc)

CGyfxyHostDoc_G5200::CGyfxyHostDoc_G5200()
{
}

BOOL CGyfxyHostDoc_G5200::OnNewDocument()
{
	if (!CGyfxyHostDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CGyfxyHostDoc_G5200::~CGyfxyHostDoc_G5200()
{
}


BEGIN_MESSAGE_MAP(CGyfxyHostDoc_G5200, CGyfxyHostDoc)
END_MESSAGE_MAP()


// CGyfxyHostDoc_G5200 diagnostics

#ifdef _DEBUG
void CGyfxyHostDoc_G5200::AssertValid() const
{
	CGyfxyHostDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CGyfxyHostDoc_G5200::Dump(CDumpContext& dc) const
{
	CGyfxyHostDoc::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CGyfxyHostDoc_G5200 serialization

void CGyfxyHostDoc_G5200::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CGyfxyHostDoc_G5200 commands
