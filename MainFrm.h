// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__AFEFF8B7_E696_4E61_B954_1B3CF6B2853A__INCLUDED_)
#define AFX_MAINFRM_H__AFEFF8B7_E696_4E61_B954_1B3CF6B2853A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comport.h"
#include "ModBusServer.h"
#include "DeivceViewPane.h" 
class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	void InitComm();
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	CComPort *m_pComPort;
	CModBusServer *pModbusMaster;
	int m_ComPortCount;
	int m_nTheme;
// Implementation
public:  
	void ShowDockingPanePopupMenu(CXTPDockingPane* pPopupPane, CPoint pt, LPRECT lprcExclude);
	CDateTimeCtrl m_wndDateTimeBoxStart;
	CDateTimeCtrl m_wndDateTimeBoxEnd; 
	CXTPControlComboBox *m_wndComboBoxDevice; //设备组合框控件 CXTPComboBox 
	CXTPControlComboBox *m_wndComboBoxBombNO;
	CXTPControlComboBox *m_wndComboBoxAvr;

	CXTPDockingPaneManager m_paneManager;
	CXTPTabClientWnd m_MTIClientWnd;	
	CDeivceViewPane m_wndDeviceView;
	CXTPToolBar *m_wndDBBar;
	CXTPStatusBar m_wndStatusBar;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members


	

// Generated message map functions
protected:
	//CCoolMenu			m_menu; 
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuUserlogin();
	afx_msg void OnMenuUserlogout();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnSetupUserinfo();
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	//}}AFX_MSG
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl); // Called by the XTK to create the controls on the CommandBars
	afx_msg void OnUpdatDBDateTime(CCmdUI* pCmdUI); 
	afx_msg void OnUpdateControlBarMenu(CCmdUI *pCmdUI);
	afx_msg LRESULT OnReceive( WPARAM length,LPARAM nPort );
	afx_msg LRESULT OnTxEmpty( WPARAM length,LPARAM nPort );
	afx_msg BOOL OnShowView(UINT nID);
	DECLARE_MESSAGE_MAP()
private:
	void LoadDevice(CString strDev);
	int m_iTimerID;
public:
	afx_msg void OnRegOCX();
	afx_msg void OnSetupComm();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__AFEFF8B7_E696_4E61_B954_1B3CF6B2853A__INCLUDED_)
