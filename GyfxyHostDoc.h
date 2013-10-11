#pragma once

// CGyfxyHostDoc document
#include "IEDDocument.h"
#include "gyfxyrdb.h"
#include "GyfxyHostCtrl.h"
#include "GyfxyDebugView.h"
#include "ChildFrm.h "

class CGyfxyHostDoc : public CIEDDocument
{
	DECLARE_DYNCREATE(CGyfxyHostDoc)

public:
	CGyfxyHostDoc();
	virtual ~CGyfxyHostDoc();
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
	// //virtual GetHostParam(CString sDeviceName,CString sDeviceType);
	virtual BOOL GetHostParam(CString sDeviceName , CString sDeviceType);
	CGyfxyRDB_G5200 GyfxyRDB;
	CGyfxyHostCtrl *pHostCtrl;
	CFrameWnd *pDebugWnd  ;
	CFrameWnd *pBalanceWnd;
	afx_msg void OnDebug();
	afx_msg void OnBalance();
};
