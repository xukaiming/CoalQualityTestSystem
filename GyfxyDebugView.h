#pragma once

#include "GyfxyHostDoc.h"
#include "GyfxyRDB.h"	// Added by ClassView
#include "GyfxyHostCtrl.h"
#include "Picture.h"
// CGyfxyDebugView form view

class CGyfxyDebugView : public CFormView
{
	DECLARE_DYNCREATE(CGyfxyDebugView)

protected:
	CGyfxyDebugView();           // protected constructor used by dynamic creation
	virtual ~CGyfxyDebugView();

public:
	enum { IDD = IDD_DIALOG_GYFXY_DEBUG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CGyfxyRDB_G5200 *pGyfxyRdb;
	CGyfxyHostCtrl *pHostCtrl;
	CFont	*pHexFont; 
	long	DestTemp[3];  //×ó,ÓÒ,Ë®
	enum STOVE_CNT{WATER_STOVE=0,LEFT_STOVE,RIGHT_STOVE,};
public:
	CPicture m_bLeftStoveTop;
	CPicture m_bLeftStoveBot;
	CPicture m_bRightStoveTop;
	CPicture m_bRightStoveBot;
	CPicture m_bWaterStoveTop;
	CPicture m_bWaterStoveBot;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonLeftOxygen();	
	afx_msg void OnBnClickedButtonLeftUp();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	void InitParam(void);
	afx_msg void OnBnClickedButtonTare();
	afx_msg void OnBnClickedButtonRightOxygen();
	afx_msg void OnBnClickedButtonWaterStoveUp();
	afx_msg void OnBnClickedButtonLeftDn();
	afx_msg void OnBnClickedButtonRightUp();
	afx_msg void OnBnClickedButtonRightDn();
	afx_msg void OnBnClickedButtonWaterStoveDn();
	afx_msg void OnBnClickedButtonLeftFan();
	afx_msg void OnBnClickedButtonRightFan();
	afx_msg void OnBnClickedButtonWaterStove(); 
	afx_msg void OnBnClickedButtonSampleRound(); 
	afx_msg void OnBnClickedButtonSamplenoMove();
	afx_msg void OnBnClickedButtonLeftWarm();
	afx_msg void OnBnClickedButtonRightWarm();
	afx_msg void OnBnClickedButtonWaterStoveWarm();
	afx_msg void OnBnClickedButtonSetid();
};


