#if !defined(AFX_PAGEOUT_H__28E675A5_25B4_498A_A18F_463CD5B27EC7__INCLUDED_)
#define AFX_PAGEOUT_H__28E675A5_25B4_498A_A18F_463CD5B27EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageOut.h : header file
//
#include "LryHostCtrl.h" 
/////////////////////////////////////////////////////////////////////////////
// CPageOut dialog

class CPageOut : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageOut)

// Construction
public:
	void DisEListView(DATA_E *pD);
	void DisQListView(DATA_Q *pQ);
	CLryHostCtrl * m_pCtrl;
	CPageOut();
	~CPageOut();

// Dialog Data
	//{{AFX_DATA(CPageOut)
	enum { IDD = IDD_PAGE_OUTINFO };
	CEdit	m_editView;
	CListCtrl	m_listView;
//	CSortListCtrl m_listView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FormatList(int iItem, CString * csText,int iCol);
	void BuildColumns(int nCols, int *nWidth, int *iCol);
	void AddExStyle(DWORD dwNewStyle);
private:
	CString m_sTest;
	long m_lMaxDiffQ;		//发热量极差
	long m_lAverageQ;		//平均发热量

	long m_lMaxDiffE;		//热容量极差
	float m_fSErrorE;		//热容量标准差
	float m_fRSErrorE;		//热容量相对标准差
	long m_lAverageE;		//平均热容量
private:
//	BOOL m_fAsc;                  //用来设置是升序排列还是降序排列
//	int m_nSortedCol;             //用来记录被点击的是哪一列

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEOUT_H__28E675A5_25B4_498A_A18F_463CD5B27EC7__INCLUDED_)
