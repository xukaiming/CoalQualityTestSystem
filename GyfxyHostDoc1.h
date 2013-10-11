#pragma once

#include "GyfxyHostDoc.h"
// CGyfxyHostDoc_G5200 document

class CGyfxyHostDoc_G5200 : public CGyfxyHostDoc
{
	DECLARE_DYNCREATE(CGyfxyHostDoc_G5200)

public:
	CGyfxyHostDoc_G5200();
	virtual ~CGyfxyHostDoc_G5200();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
