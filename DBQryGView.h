#pragma once
#include "DBQryView.h"


// CDBQryGView form view

class CDBQryGView : public CDBQryView
{
	DECLARE_DYNCREATE(CDBQryGView)

protected:
	CDBQryGView();           // protected constructor used by dynamic creation
	virtual ~CDBQryGView();

public:
	//enum { IDD = IDD_DBQRYGVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


