#pragma once
#define CListView CXTListView

//CCustomMenuHeaderCtrl
class CCustomMenuHeaderCtrl : public CXTHeaderCtrl
{
	typedef struct  
	{
		int m_iSort;
		BOOL bCheck;
		CString m_strColName;
	}HeadMenuName;
	CArray <HeadMenuName> MenuArray;
	DECLARE_DYNCREATE(CCustomMenuHeaderCtrl)
public:
	CCustomMenuHeaderCtrl();           // protected constructor used by dynamic creation
	virtual ~CCustomMenuHeaderCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void UpdateHeaderMenu(void);
};

// CDBListView view

class CDBListView : public CListView
{
	DECLARE_DYNCREATE(CDBListView)

protected:
	CDBListView();           // protected constructor used by dynamic creation
	virtual ~CDBListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CCustomMenuHeaderCtrl m_HeaderCtrl;
	CMenu CColMenu;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void UpdateHeaderMenu(void);
};


