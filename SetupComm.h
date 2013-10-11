#pragma once

#include "XListCtrl.h"
#include "SortCStringArray.h"

// CSetupComm dialog

class CSetupComm : public CDialog
{
	DECLARE_DYNAMIC(CSetupComm)

public:
	CStringArray m_PortArray;
	CStringArray m_PorttDescriptArray;
	CSetupComm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupComm();

// Dialog Data
	enum { IDD = IDD_DIALOG_COMM };
private:  
	CStringArray strCommType;
	CStringArray strCommSpeed;
	CStringArray strCommCheckBit;
	CStringArray strCommStopBit;
	CStringArray strCommBit;
	//////////////////////////////
	CFont m_listFont;
	///////////////////////////////////
	_RecordsetPtr	pRsDev;
	CString			strSQL; 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	unsigned char EnumCommPort(CStringArray &PortArray,CStringArray &PortDescriptArray);
	virtual BOOL OnInitDialog();
	CXListCtrl m_XlistCtrl;
	void FillHeader(void);
	void FillList(void);
	afx_msg void OnBnClickedCommAdd();
};
