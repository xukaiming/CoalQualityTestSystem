#pragma once
#include "afxcmn.h"
#include "DBQrySView.h"
#include "XListCtrl.h"
// CCalCoffDlg dialog

class CCalCoffDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalCoffDlg)

public:
	CCalCoffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalCoffDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CLY_COFF_CAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CXListCtrl m_Listctrl;
	CListCtrl  *pParentListCtrl;
	CDBQrySView *pParentListView; 
	afx_msg void OnBnClickedCalCoff();
private:
	double *pStd;
	double *pTrueStd;
	double *pCalStd;
	double m_slope,m_intercept,m_R;
public:
	afx_msg void OnBnClickedSaveCoff();
};
