#pragma once


#include "memdc.h"
#include "LiquidCrystalLabel.h"
#include "afxwin.h"
#include "Gyfxy1RDB.h"
#include "GyfxyHostDoc1.h"
// CGyfxyBlanceView form view

class CGyfxyBlanceView : public CFormView
{
	DECLARE_DYNCREATE(CGyfxyBlanceView)

protected:
	CGyfxyBlanceView();           // protected constructor used by dynamic creation
	virtual ~CGyfxyBlanceView();

public:
	enum { IDD = IDD_DLG_GYFXY_BALANCE };
	CGyfxyHostDoc_G5200  * GetDocument() ;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	CLiquidCrystalLabel m_PotWight;
	CLiquidCrystalLabel m_BalanceWeight;
	CLiquidCrystalLabel m_SampleWeight;
	CLiquidCrystalLabel Label1;
	CLiquidCrystalLabel label2;
	CLiquidCrystalLabel label3;
	CGyfxyRDB_G5200 *pGyfxyRdb;
	CGyfxyHostCtrl *pHostCtrl; 
	////////////////////////////////////  
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public: 
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


