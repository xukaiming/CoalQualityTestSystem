#pragma once


// CEditListCtrl

class CEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditListCtrl)

public:
	CEditListCtrl();
	virtual ~CEditListCtrl();
public:
	CEdit* GetEditCtrl() const { return m_pctrlEdit; } 
	void CommitEditCtrl(); 
protected:
	CEdit *m_pctrlEdit;
	CComboBox* m_pctrlComboBox; 
	CDateTimeCtrl *m_pDateTimePicker;
	int m_iRow; 
	int m_iCol; 
	int m_iEditRow; 
	int m_iEditCol; 
protected:
	DECLARE_MESSAGE_MAP() 
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult); 
	//afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnDestroy(); 
	afx_msg void OnEnKillFocus(); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
	afx_msg void OnSetFocus(CWnd* pOldWnd); 
	afx_msg void OnNMCustomDraw(NMHDR *pNMHDR, LRESULT *pResult); 
	afx_msg void OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult); 
	afx_msg void OnLvnBeginScroll(NMHDR *pNMHDR, LRESULT *pResult); 

public:
	//************************************
	// Method:    PreTranslateMessage
	// FullName:  CEditListCtrl::PreTranslateMessage
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: MSG * pMsg
	//************************************
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	
public: 
	
protected: 
	void ResetTopPosition(); 
	void ShowEditCtrl(void);
	void ShowComboBoxCtrl(void);
	void ShowDateTimePicker(void);
	void ResetBottomPosition(); 
	void VerifyScrollPos(); 


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); 
private:
	
	virtual void ShowCtrl(void);
	virtual BOOL ValidCheck(int nRow, int nCol, LPCTSTR lpszText);
};


