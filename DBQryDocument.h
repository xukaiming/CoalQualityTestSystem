#pragma once

// CDBQryDocument document

class CDBQryDocument : public CDocument
{
	DECLARE_DYNCREATE(CDBQryDocument)

public:
	CDBQryDocument();
	virtual ~CDBQryDocument();
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
	//响应数据移动指令
	afx_msg void OnCursorMove(UINT nID);
	afx_msg void OnUpdatCursorButton(CCmdUI* pCmdUI);
	/////////////////////////////////////////////////
	afx_msg void OnRecordRefresh();
	afx_msg void OnDeleteRecord();
	/////////////////////////////////////////////////
	afx_msg void OnDeviceIDChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBTimerChange(UINT id, NMHDR* pNotifyStruct,  LRESULT* pResult );
	afx_msg void OnUpdatDBDateTime(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
protected:
	COleDateTime StartTime;
	COleDateTime EndTime;
	CString strSQL;
	VARIANT_BOOL m_bBOF;
	VARIANT_BOOL m_bADOEOF;
public:
	_RecordsetPtr Rs;
	_RecordsetPtr RsPrintData;
	_RecordsetPtr RsTitle;	
	HRESULT MoveNext();
	HRESULT MovePrevious();
	HRESULT MoveLast();
	HRESULT MoveFirst();
	HRESULT Move(ADO_LONGPTR NumRecords, const _variant_t &Start);
public: 
	virtual void SQL(void);
	virtual void OnCloseDocument();
public: 
	class CRowObj:public CObject
	{
	public: LONG RowNO;
	};
	CRowObj pObj;
};
