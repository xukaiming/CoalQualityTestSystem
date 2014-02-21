#if !defined(AFX_IEDDOCUMENT_H__1091F163_0DC1_4F4E_ACC1_A958083620C6__INCLUDED_)
#define AFX_IEDDOCUMENT_H__1091F163_0DC1_4F4E_ACC1_A958083620C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IEDDocument.h : header file
//
#include "RDB.h"
/////////////////////////////////////////////////////////////////////////////
// CIEDDocument document

class CIEDDocument : public CDocument
{
protected:
	CIEDDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIEDDocument)

// Attributes
public:
	CString sDeviceName;
	CRDB *pRDB;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEDDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIEDDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CIEDDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString GetCurrentSeleString(CString &strParent);
	virtual BOOL GetHostParam(CString sDeviceName,CString sDeviceType);
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEDDOCUMENT_H__1091F163_0DC1_4F4E_ACC1_A958083620C6__INCLUDED_)
