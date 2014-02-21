#ifndef _GYFXY_HOSTVIEW_H_
#define _GYFXY_HOSTVIEW_H_
#pragma once

#include "GyfxyHostDoc1.h"

#include "AutoSampNODlg.h"
#include "TextProgressCtrl.h"
#include "afxcmn.h"
#include "xlistctrl.h"// CGyfxyHostView_G5200 form view 
class CGyfxyHostView : public CFormView
{
	CGyfxyHostView();
	DECLARE_DYNCREATE(CGyfxyHostView)
public:
	explicit CGyfxyHostView(UINT nIDTemplate);           
protected:   
	virtual ~CGyfxyHostView();

public:
	enum { IDD = IDD_GFYHOSTVIEW_FORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
	
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	CGyfxyHostDoc  * GetDocument() ;
	CGyfxyHostCtrl * pGyfxyHostCtrl;
	CGyfxyRDB_BaseRdb		*pRdb;
	CGyfxyImageSlave *pImg;
	CGyfxyRDB_BaseRdb::ALLSample::S_Sample *pSample ; 
	CGyfxyAutoSampNODlg pDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void InitUI();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnCalcData(WPARAM wp, LPARAM lP);
	afx_msg void OnBnClickedTestStart();
	afx_msg LRESULT OnImageDataChange(WPARAM sText, LPARAM lP);
	afx_msg LRESULT OnUpdateQuality(WPARAM cSamplePos, LPARAM lP);
	afx_msg LRESULT OnStartProgBar(WPARAM sText, LPARAM lP);
	 
	
private:

	int m_iTimer;
	int m_iMax;
	int m_iCount;
	CString	m_strProgBarText;
	CTextProgressCtrl m_ProgBar;
	CXListCtrl m_ListCtrl; 
	CFont m_listFont;
private:
	void StartProgBar(CString sText, int iMax);
	void EnableAllParmCtrl(bool bEnable);
	static BOOL   CALLBACK   EnumChildProc(HWND   hwndChild,LPARAM   lParam);
public:
	afx_msg void OnDestroy(); 
	afx_msg void OnBnClickedWeightPot();
	afx_msg void OnBnClickedWeightSample(); 
	afx_msg void OnBnClickedReset();
	afx_msg LRESULT OnCheckbox(WPARAM nItem, LPARAM nSubItem);
	afx_msg LRESULT OnEditEnd(WPARAM, LPARAM);

	void InitListHeadCtrl(void); 
	afx_msg void OnBnClickedButtonAutoNo();
	afx_msg void OnBnClickedButtonSampleUd();
	afx_msg void OnBnClickedButtonSampleDn(); 
	afx_msg void OnLvnColumnclickListResult(NMHDR *pNMHDR, LRESULT *pResult); 
	afx_msg void OnTimer(UINT_PTR nIDEvent); 
	afx_msg void OnBnClickedTestStop(); 
};
#endif

