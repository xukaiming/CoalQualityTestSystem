#if !defined(AFX_PAGECURVE_H__75313E59_1A2E_4988_AC49_D7ED6212A84A__INCLUDED_)
#define AFX_PAGECURVE_H__75313E59_1A2E_4988_AC49_D7ED6212A84A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageCurve.h : header file
//
#include "LryHostCtrl.h" 
/////////////////////////////////////////////////////////////////////////////
// CPageCurve dialog

class CPageCurve : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageCurve)

// Construction
public:
	void EnableUpdateCurve(BOOL bEnable,int iCurveXMax);
 
 	CLryHostCtrl * m_pCtrl;
	BOOL bShowCurve;
	///////////////////////////////////////
	//void DisCurve(float * pfCurve);
	void DisCurve( );
	CPageCurve();
	~CPageCurve();

// Dialog Data
	//{{AFX_DATA(CPageCurve)
	enum { IDD = IDD_PAGE_CURVE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageCurve)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageCurve)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	int m_iPriod;
	int m_iCurveXMax;
	int fCurvePtr;
	int	FlagDisp;
	LRESULT OnImageDataChange(WPARAM sText, LPARAM lP);
#ifdef _DEBUG
	CFile *fileCurve;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGECURVE_H__75313E59_1A2E_4988_AC49_D7ED6212A84A__INCLUDED_)
