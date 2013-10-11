#if !defined(AFX_LRYHOSTDOC_H__EC724248_FF1F_4E60_955A_F78289656011__INCLUDED_)
#define AFX_LRYHOSTDOC_H__EC724248_FF1F_4E60_955A_F78289656011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LryHostDoc.h : header file
//
 
#include "LRYRDB.H"	// Added by ClassView
#include "IEDDocument.h"
/////////////////////////////////////////////////////////////////////////////
// CLryHostDoc document 
class CLryHostDoc : public CIEDDocument
{
protected:
	CLryHostDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLryHostDoc)

// Attributes
public:
 
	//CString	sDeviceName;
	LRYRBindDB LryBindRdb ; //∞Û∂® Ù–‘
	LRYRDB     LryRdb;      //“«∆˜ Ù–‘
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLryHostDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetCurrentSeleString();
	BOOL GetHostParam(LRYRBindDB *pRDB,LRYRDB * pLryRdb,CString sDeviceName);
	virtual ~CLryHostDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLryHostDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYHOSTDOC_H__EC724248_FF1F_4E60_955A_F78289656011__INCLUDED_)
