#if !defined(AFX_PAGEPARAM_H__F8B11AB0_369E_4313_A611_8ADD4A242023__INCLUDED_)
#define AFX_PAGEPARAM_H__F8B11AB0_369E_4313_A611_8ADD4A242023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LryHostCtrl.h"
// PageParam.h : header file
// 
/////////////////////////////////////////////////////////////////////////////
// CPageParam dialog
#define WM_PARAMETE_CHANGED		WM_USER+201		//参数改变时，向父窗口发消息
class CPageParam : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageParam)

// Construction
public:
	void InitParamFromDB(LRYRDB *pRDB);
	CLryHostCtrl * m_pCtrl;
	void GetParamFromDlg(LRYRDB *pRDB );	
	void EnableAllParmCtrl(bool bEnable);
	static BOOL   CALLBACK EnumChildProc(HWND   hwndChild,LPARAM   lParam);
	CPageParam();
	~CPageParam();

// Dialog Data
	//{{AFX_DATA(CPageParam)
	enum { IDD = IDD_PAGE_PARAM };
	CButton	m_PingXing;
	CDateTimeCtrl	m_SampleDate;
	CComboBox	m_YangDanIdBox;
	CComboBox	m_TestType;
	CComboBox	m_YangPin;
	double	m_HeatWireQuality;
	double	m_SampeQuality;
	CString	m_AutoNO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageParam)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageParam)
	afx_msg void OnTestPingxingyang();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeTestYangpinnum();
	afx_msg void OnSelchangeTestType();
	afx_msg void OnSelchangeTestYangdanid(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//CString m_sYangDan;
	//CString m_sTest;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPARAM_H__F8B11AB0_369E_4313_A611_8ADD4A242023__INCLUDED_)
