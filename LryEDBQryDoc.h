#if !defined(AFX_LRYEDBQRYDOC_H__3BBC3E47_25F5_4051_93A5_08FEFDDDA78A__INCLUDED_)
#define AFX_LRYEDBQRYDOC_H__3BBC3E47_25F5_4051_93A5_08FEFDDDA78A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LryEDBQryDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryDoc document

class CLryEDBQryDoc : public CDocument
{
protected:
	CLryEDBQryDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLryEDBQryDoc)

// Attributes
public:
	class CRowObj:public CObject
	{
		public: LONG RowNO;
	};
	CRowObj pObj;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLryEDBQryDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	HRESULT MoveNext();
	HRESULT MovePrevious();
	HRESULT MoveLast();
	HRESULT MoveFirst();
	HRESULT Move(ADO_LONGPTR NumRecords, const _variant_t &Start);
	void OnCursorMove(UINT nID);
	_RecordsetPtr Rs;
	_RecordsetPtr RsPrintData;
	_RecordsetPtr RsTitle;
	virtual ~CLryEDBQryDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnDeviceIDChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBTimerChange(UINT id, NMHDR* pNotifyStruct,  LRESULT* pResult );
	afx_msg void OnUpdatDBDateTime(CCmdUI* pCmdUI); 
	afx_msg void OnRecordRefresh();
	afx_msg void OnUpdatCursorButton(CCmdUI* pCmdUI);	
	afx_msg void OnDeleteRecord();
	//{{AFX_MSG(CLryEDBQryDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SQL();
	COleDateTime StartTime;
	COleDateTime EndTime;
	CString strSQL;
	VARIANT_BOOL m_bBOF;
	VARIANT_BOOL m_bADOEOF;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYEDBQRYDOC_H__3BBC3E47_25F5_4051_93A5_08FEFDDDA78A__INCLUDED_)
