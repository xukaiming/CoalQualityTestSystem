#pragma once
#include "DBQryDocument.h"
// CDBQryGDocument document

class CDBQryGDocument : public CDBQryDocument
{
	DECLARE_DYNCREATE(CDBQryGDocument)

public:
	CDBQryGDocument();
	virtual ~CDBQryGDocument();
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
public:
	virtual void SQL(void);
};
