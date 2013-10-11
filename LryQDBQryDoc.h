#if !defined(AFX_LRYDBQRYDOC_H__8F64A571_83EA_4FEE_BC94_86083E77F9FE__INCLUDED_)
#define AFX_LRYDBQRYDOC_H__8F64A571_83EA_4FEE_BC94_86083E77F9FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LryQDBQryDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryDoc document

class CLryQDBQryDoc : public CDocument
{
protected:
	CLryQDBQryDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLryQDBQryDoc)

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
	//{{AFX_VIRTUAL(CLryQDBQryDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	void SQL();
	HRESULT MoveNext();
	HRESULT MovePrevious();
	HRESULT MoveLast();
	HRESULT MoveFirst();
	HRESULT Move(ADO_LONGPTR NumRecords, const _variant_t &Start);
	_RecordsetPtr Rs;
	_RecordsetPtr RsPrintData;
	_RecordsetPtr RsTitle;
	virtual ~CLryQDBQryDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnDeviceIDChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBTimerChange(UINT id, NMHDR* pNotifyStruct,  LRESULT* pResult );
	afx_msg void OnUpdatDBDateTime(CCmdUI* pCmdUI);
	//响应数据移动指令
	afx_msg void OnCursorMove(UINT nID);
	afx_msg void OnUpdatCursorButton(CCmdUI* pCmdUI);
	//{{AFX_MSG(CLryQDBQryDoc)
	afx_msg void OnRecordRefresh();
	afx_msg void OnDeleteRecord();
	afx_msg void OnAverageMenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	COleDateTime StartTime;
	COleDateTime EndTime;
	CString strSQL;
	VARIANT_BOOL m_bBOF;
	VARIANT_BOOL m_bADOEOF;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYDBQRYDOC_H__8F64A571_83EA_4FEE_BC94_86083E77F9FE__INCLUDED_)
