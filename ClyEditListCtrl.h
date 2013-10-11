#include "EditListCtrl.h"
#pragma once


#include "XListCtrl.h "
#include "CLyHostDoc.h"
// CClyEditListCtrl

class CClyEditListCtrl : public  CEditListCtrl//CXListCtrl// CEditListCtrl
{
	DECLARE_DYNAMIC(CClyEditListCtrl)

public:
	CClyEditListCtrl();
	virtual ~CClyEditListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
private: 

	LOGFONT   lf; 
	CFont	*font;
public:
	// //ÃÓ≥‰±ÍÃ‚¿∏
	void FillHeaderCtrl(CClyHostDoc *pDoc);
	virtual BOOL ValidCheck(int nRow, int nCol, LPCTSTR lpszText);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
//	virtual void PreSubclassWindow();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetFontHeight(int iHeight);
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};


