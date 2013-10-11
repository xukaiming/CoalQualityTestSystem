// DeivceViewPane.h: interface for the CDeivceViewPane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEIVCEVIEWPANE_H__BE8587F8_B7B6_422D_AFF9_2DDD041489AC__INCLUDED_)
#define AFX_DEIVCEVIEWPANE_H__BE8587F8_B7B6_422D_AFF9_2DDD041489AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_PARAMETE_CHANGED  WM_USER+201

class CDeivceViewPane : public CWnd  
{
public:
	CDeivceViewPane();
	virtual ~CDeivceViewPane();
public:
	_RecordsetPtr Rs;
// Attributes
public:
	void AddDev(const HTREEITEM &ParenthtItem, const CString &str);
	COfficeTreeCtrl m_wndClassView; 
	
	CImageList  m_ImageList;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) ;
	afx_msg void OnCorrFactUI(CCmdUI* pCmdUI);
	afx_msg LRESULT OnParmChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()
private:
	void LoadDevice();
};

#endif // !defined(AFX_DEIVCEVIEWPANE_H__BE8587F8_B7B6_422D_AFF9_2DDD041489AC__INCLUDED_)
