#pragma once
#include "DBQryDocument.h"
// CDBQrySDocument document

class CDBQrySDocument : public CDBQryDocument
{
	DECLARE_DYNCREATE(CDBQrySDocument)

public:
	CDBQrySDocument();
	virtual ~CDBQrySDocument();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void SQL(void);
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
