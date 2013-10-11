#pragma once

#include "DBQryView.h"
// CDBQrySView view

class CDBQrySView : public CDBQryView
{
	DECLARE_DYNCREATE(CDBQrySView)

protected:
	CDBQrySView();           // protected constructor used by dynamic creation
	virtual ~CDBQrySView();
	virtual void FileFilter();
	virtual void ParsarReport();
public: 
	CString DBVariantToStr(_variant_t str,CString ParamName);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	void AverageReport(CReportSele * pDlg);
	void RiBao(CReportSele * pDlg);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/); 
	afx_msg void OnCalCoff();
	afx_msg void OnAnalyseRecal();
	afx_msg void OnUpdateMenuState1(CCmdUI* pCmdUI);
//	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};


