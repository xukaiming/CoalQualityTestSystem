#pragma once

#include "rmreportengine.h"	// Added by ClassView
#include "ReportSele.h"
#include "DBQryDocument.h" 
// CDBQryView view

class CDBQryView : public  CListView//CXTListView//
{
	DECLARE_DYNCREATE(CDBQryView)
	// Attributes
public:
	CReportSele	ReportSeleDlg;
	CDBQryDocument * GetDocument();
protected:
	CDBQryView();           // protected constructor used by dynamic creation
	virtual ~CDBQryView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CRMReportEngine pReportdlg;
	CStringArray ColNameArray;
	CXTHeaderCtrl   m_header;
	LOGFONT   lf; 
	CFont	font;
 
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateMenuState1(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void FillList();
	virtual void FillHeader();
	virtual void FillData();
	virtual void FileFilter();
	virtual void ParsarReport();
	virtual CString GetItemText(int nItem, CString strColName);
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual CString DBVariantToStr(_variant_t var,CString ParamName);
public:
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPrintReportSetting();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

#ifndef _DEBUG  // debug version in LRYCTRLView.cpp
inline CDBQryDocument* CDBQryView::GetDocument()
{ return (CDBQryDocument*) m_pDocument; }
#endif
