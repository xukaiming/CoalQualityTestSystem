#pragma once


// CClyCoffDlg dialog

class CClyCoffDlg : public CDialog
{
	DECLARE_DYNAMIC(CClyCoffDlg)

public:
	CClyCoffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClyCoffDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CLY_COFF_ATTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	_RecordsetPtr	pRsDev;
	CString			strSQL;
	CStringArray	ColNameArray;
public:
	void SQL(void);
	void AddList(void);
private:
	CXTListCtrl ListCtrl;
public:
	virtual BOOL OnInitDialog();
	void AddListHeader(void);
	afx_msg void OnNMCustomdrawListCoff(NMHDR *pNMHDR, LRESULT *pResult);
};
