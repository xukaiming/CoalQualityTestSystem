#if !defined(AFX_CLYHOSTDOC_H__E58F2608_A1C5_4E34_8021_225AEEFCF350__INCLUDED_)
#define AFX_CLYHOSTDOC_H__E58F2608_A1C5_4E34_8021_225AEEFCF350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CLyHostDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCLyHostDoc document
#include "IEDDocument.h"
#include "ClyRDB.h" 
#include "CLYHostCtrl.h"



class CClyHostDoc : public CIEDDocument
{
protected:
	CClyHostDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CClyHostDoc)
// Attributes
public: 
	CClyRDB ClyRDB;
	CCLYHostCtrl *pHostCtrl;
	CFrameWnd *pDebugWnd  ;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLyHostDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClyHostDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCLyHostDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL GetHostParam(CString sDeviceName, CString sDeviceType);
	
public:
	afx_msg void OnDebug();
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLYHOSTDOC_H__E58F2608_A1C5_4E34_8021_225AEEFCF350__INCLUDED_)
