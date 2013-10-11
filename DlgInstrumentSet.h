#pragma once 
#include "MainFrm.h"
#include "rdb.h"
// CDlgInstrumentSet dialog

class CDlgInstrumentSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgInstrumentSet)

public:
	CDlgInstrumentSet(CWnd* pParent = NULL);   // standard constructor
	explicit CDlgInstrumentSet(UINT nIDTemplate, CWnd* pParentWnd );//需要这个  
	virtual ~CDlgInstrumentSet();

// Dialog Data 
protected: 
	CRDB *Rdb; 
	//_RecordsetPtr	pRsDev;
	UINT	m_ROWNO;
	CString strSQL;	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support 
	virtual void UpdateWinText(void)=0;
	virtual void GetParamFromDlg(void)=0;
	virtual void SQL(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedDeviceAdd();
	afx_msg void OnBnClickedDeviceDel();
	afx_msg void OnBnClickedDeviceModify();
	//	afx_msg void OnBnClickedCancel();
	// //RefreshRDB

	virtual BOOL OnInitDialog();
	
	void MoveDBPtr(void);
	CString GetCurrentSeleString(void);
	CString GetCurrParentSeleString(void);
};
