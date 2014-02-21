// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LRYCTRL.h"

#include "MainFrm.h"
#include "UserLogon.h"
#include "UserInfoDlg.h"

#include "SetupComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CXTPMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_USERLOGIN, OnMenuUserlogin)
	ON_COMMAND(ID_MENU_USERLOGOUT, OnMenuUserlogout)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_QUERYENDSESSION()
	ON_COMMAND(ID_SETUP_USERINFO, OnSetupUserinfo)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE() 
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
	ON_MESSAGE(WM_COMM_RXFLAG_DETECTED,OnReceive)   //串口接收消息
	ON_MESSAGE(WM_COMM_TXEMPTY_DETECTED,OnTxEmpty)   //串口接收消息
	//ON_COMMAND_EX(ID_VIEW_DBBAR, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_DBBAR,OnUpdateControlBarMenu)
	//ON_COMMAND_EX(CG_ID_VIEW_WORKSPACE, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(CG_ID_VIEW_WORKSPACE,OnUpdateControlBarMenu)
	ON_COMMAND_EX(CG_ID_VIEW_WORKSPACE, OnShowView)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_WORKSPACE,OnUpdateControlBarMenu)

	ON_UPDATE_COMMAND_UI_RANGE(ID_DATETIME_START,ID_AVERAGE,OnUpdatDBDateTime)//ID_BOMB_NO
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(ID_32815, &CMainFrame::OnRegOCX)
	ON_COMMAND(ID_SETUP_COMM, &CMainFrame::OnSetupComm)
END_MESSAGE_MAP()
 
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
 
	ID_INDICATOR_COMPRESSDB,		 
	ID_INDICATOR_DATETIMER,

	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_nTheme = xtpThemeNativeWinXP;
	//m_nTheme = xtpThemeVisualStudio2008;
	CXTPPaintManager::SetTheme((XTPPaintTheme)m_nTheme);
	InitComm();       //通信口初始化
	if (!InitCommandBars())
		return -1;	
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE(_T("Failed to create command bars object.\n"));
		return -1;      // fail to create
	}
	pCommandBars->GetImageManager()->SetIcons(IDR_MAINFRAME);
	CXTPMenuBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE(_T("Failed to create menu bar.\n"));
		return -1;      // fail to create
	}
		//主工具条
	CXTPToolBar* m_wndToolBar = (CXTPToolBar*) pCommandBars->Add(_T("工具栏(&T)"), xtpBarTop);
	if (!m_wndToolBar || !m_wndToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE(_T("Failed to create toolbar\n"));
		return -1;
	} 
	
	//CXTPToolBar* 
	m_wndDBBar = (CXTPToolBar*) pCommandBars->Add(_T("查询工具栏"), xtpBarTop);
	if (!m_wndDBBar || !m_wndDBBar->LoadToolBar(IDR_DB))
	{
		TRACE(_T("Failed to create toolbar\n"));
		return -1;
	}   
	m_wndComboBoxDevice = (CXTPControlComboBox*)m_wndDBBar->GetControls()->Add(xtpControlComboBox, ID_DEVICE); 
	m_wndComboBoxDevice->SetDropDownListStyle(FALSE);
	
	LoadDevice(_T("测硫仪"));
	LoadDevice(_T("量热仪"));	
	m_wndComboBoxDevice->InsertString(0,_T("全部")); 
	m_wndComboBoxDevice->SetCurSel(0); 
	m_wndComboBoxBombNO = (CXTPControlComboBox*)m_wndDBBar->GetControls()
		->Add(xtpControlComboBox, ID_BOMB_NO); 
	m_wndComboBoxBombNO->SetDropDownListStyle(FALSE);
	m_wndComboBoxBombNO->InsertString(0,_T("全部"));	
	m_wndComboBoxBombNO->InsertString(0,_T("B氧弹"));	
	m_wndComboBoxBombNO->InsertString(0,_T("A氧弹"));
	m_wndComboBoxBombNO->SetCurSel(0); 

	m_wndComboBoxAvr = (CXTPControlComboBox*)m_wndDBBar->GetControls()
		->Add(xtpControlComboBox, ID_AVERAGE); 
	m_wndComboBoxAvr->SetDropDownListStyle(FALSE);
	m_wndComboBoxAvr->InsertString(0,_T("平均记录"));	
	m_wndComboBoxAvr->InsertString(0,_T("原始记录"));	 
	m_wndComboBoxAvr->SetCurSel(0); 

 	//底部状态条
 	if (!m_wndStatusBar.Create(this) ||
 		!m_wndStatusBar.SetIndicators(indicators,
 		  sizeof(indicators)/sizeof(UINT)))
 	{
 		TRACE(_T("Failed to create status bar\n"));
 		return -1;      // fail to create
 	}
	int iDB = m_wndStatusBar.CommandToIndex(ID_INDICATOR_COMPRESSDB); 
	int iDtime = m_wndStatusBar.CommandToIndex(ID_INDICATOR_DATETIMER);
 	m_wndStatusBar.SetPaneInfo(iDB,ID_INDICATOR_COMPRESSDB,SBPS_DISABLED ,200);	
 	m_wndStatusBar.SetPaneInfo(iDtime,ID_INDICATOR_DATETIMER,SBPS_NORMAL,200);
////////////////////////////////////////////////////////////////////////////////////////
	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE; 
	
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
	pCommandBars->GetToolTipContext()->SetStyle(xtpToolTipOffice);
	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);
////////////////////////////////////////////////////////////////////////////////////////
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.CreatePane(CG_ID_VIEW_WORKSPACE, CRect(0, 0, 230, 140), xtpPaneDockLeft);
	m_paneManager.SetDefaultPaneOptions(xtpPaneHasMenuButton);
	m_paneManager.EnableKeyboardNavigate();
	m_paneManager.SetTheme(xtpPaneThemeWinNative);
	//m_wndTaskPanel.SetTheme(xtpTaskPanelThemeToolboxWhidbey);
////////////////////////////////////////////////////////////////////////////////////////////////
	
	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();	
	pImageManager->InternalAddRef();
	m_paneManager.SetImageManager(pImageManager);	
	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetShowDockingContextStickers(TRUE);
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.SetDefaultPaneOptions(xtpPaneHasMenuButton);	
	
	/*
	VERIFY(m_MTIClientWnd.Attach(this, TRUE));
	m_MTIClientWnd.EnableToolTips();	
	m_MTIClientWnd.GetPaintManager()->m_bShowIcons = FALSE;
	m_MTIClientWnd.SetNewTabPosition(xtpWorkspaceNewTabLeftMost);
	m_MTIClientWnd.SetFlags(xtpWorkspaceHideArrowsAlways | xtpWorkspaceShowActiveFiles);
	*/
	m_iTimerID = 		SetTimer(0,1000,NULL);		//100 mS
	
	LoadCommandBars(_T("煤质综合分析系统-CommandBars"));
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	static LPCTSTR className = NULL;
	if (className==NULL) {
	// One-time class registration
	// The only purpose is to make the class name something
	// meaningful instead of "Afx:0x4d:27:32:hup1hup:hike!"
	//
	WNDCLASS wndcls;
	::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls);
	wndcls.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	wndcls.lpszClassName = _T("MAIN_HOST");
	VERIFY(AfxRegisterClass(&wndcls));
	className = _T("MAIN_HOST");
	}
	cs.lpszClass = className;
 // Load window position from profile 
	return TRUE;
	

}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::OnMenuUserlogin() 
{
	theApp.UserInfo.bLogon();
}

void CMainFrame::OnMenuUserlogout() 
{
	theApp.UserInfo.Logout();
}

void CMainFrame::InitComm()
{ 
	DWORD dwCommEvents;
	CString strCommPort;
	_RecordsetPtr Rs;


	Rs.CreateInstance("ADODB.Recordset");
	try
	{
		CString strSQL;
		strSQL.Format(_T("select * from 通信口"));
		Rs->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockReadOnly, -1);


		
		m_ComPortCount = Rs->GetRecordCount();
		if(m_ComPortCount>0)
		{
			Rs->MoveFirst();
			m_pComPort = new CComPort[m_ComPortCount];
			pModbusMaster = new CModBusServer[m_ComPortCount];
			int i = 0;
			while(!Rs->adoEOF)
			{
				
				COleVariant  nID		= Rs->Fields->GetItem("口号")->Value;
				COleVariant  nPortType	= Rs->Fields->GetItem("方式")->Value;
				COleVariant  nBaud		= Rs->Fields->GetItem("速率")->Value;			
				COleVariant  strVerify	= Rs->Fields->GetItem("校验位")->Value;
				COleVariant  nByte		= Rs->Fields->GetItem("字节位")->Value;
				COleVariant  fStopBit	= Rs->Fields->GetItem("停止位")->Value;

				if(CString(nPortType.bstrVal)=="RS485")  
					dwCommEvents = EV_RXCHAR|EV_BREAK;//|EV_TXEMPTY ;
				else
					dwCommEvents = EV_RXCHAR;
				CString StrVerify = (LPCSTR)_bstr_t(strVerify);
			 
				if(m_pComPort[i].InitPort(this,nID.iVal,nBaud.iVal,StrVerify[0],nByte.iVal,fStopBit.fltVal,dwCommEvents))
				{
					//m_pComPort[i].StartMonitoring(); 
					pModbusMaster[i].SetPort(&m_pComPort[i]);					
				}
				else
				{
					TRACE(_T("COM%d Init Error! \n"),m_pComPort[i].m_nPortNr);
				} 
				Rs->MoveNext();
				i++;
			}
		}
		if(Rs->State==adStateOpen)
		{
			Rs->Close();
		}
	}
	catch(_com_error &e)
	{
		TRACE(_T("Error get from com port"));
		dump_com_error(e);
		return ;
	} 	
}

void CMainFrame::OnDestroy() 
{
	CXTPMDIFrameWnd::OnDestroy();
	delete []pModbusMaster;
	delete []m_pComPort;

	// TODO: Add your message handler code here 
}

LRESULT CMainFrame::OnReceive( WPARAM length,LPARAM nPort )
{
	/*
	for(int i = 0;i<m_ComPortCount;i++)
	{
		if(m_pComPort[i].m_nPortNr==nPort)		
		{
			m_pComPort[i].Read(Buffer,length);
 			break;
		}
	}
	*/
	return 0;
}

LRESULT CMainFrame::OnTxEmpty( WPARAM length,LPARAM nPort )
{
	for(int i = 0;i<m_ComPortCount;i++)
	{
		if(m_pComPort[i].m_nPortNr==nPort)		
		{ 
			;
			break;
		}
	}
	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXTPMDIFrameWnd::OnTimer(nIDEvent);
}



BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CXTPMDIFrameWnd::OnQueryEndSession())
		return FALSE;
	if(IDYES==AfxMessageBox(_T("确认继续关闭分析系统并注销Windows？"),MB_YESNO+MB_ICONQUESTION))		
		return TRUE;
	else
		return FALSE;
}

void CMainFrame::OnUpdatDBDateTime(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
	//pCmdUI->Enable(TRUE);
}


 

void CMainFrame::OnSetupUserinfo() 
{
	CUserInfoDlg dlg;
	if(theApp.UserInfo.bLogon())
	{
		if(dlg.DoModal()==IDOK)
		{
			if((dlg.m_ReNewPwd==dlg.m_NewPwd)&&(dlg.m_ReNewPwd!=_T("")))
			{
				dlg.SaveToDB();	
			}
			else
			{
				AfxMessageBox(_T("密码不匹配 ！"),MB_ICONEXCLAMATION);
			}
		}
	}
	
}


 

 void CMainFrame::LoadDevice(CString strDev)
 {
 	//量热仪 strDev
 	try
 	{
 		_variant_t str;
 		CString strSQL;
 		_RecordsetPtr pRecordSet;
 		strSQL.Format(_T("select * from %s"),strDev);
 		pRecordSet.CreateInstance("ADODB.Recordset");
 		pRecordSet->Open(
 			strSQL.AllocSysString(),
 			theApp.pConn.GetInterfacePtr(),
 			adOpenStatic,
 			adLockOptimistic,
 			-1);
 		if(pRecordSet->GetRecordCount()>0)
 			pRecordSet->MoveLast(); 
 		while(!pRecordSet->BOF)
 		{		
 			str = pRecordSet->GetCollect((_bstr_t)("名称"));
 			m_wndComboBoxDevice->InsertString(0,VariantToStr(str));				
 			pRecordSet->MovePrevious();
 		}	
 		m_wndComboBoxDevice->SetCurSel(0); 
 	}  
 	catch(_com_error &e)
 	{
 		dump_com_error(e);
 	} 
 
 }

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveCommandBars(_T("煤质综合分析系统-CommandBars"));
	CXTPMDIFrameWnd::OnClose();
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{ 
	if (lpCreateControl->nID == ID_DATETIME_START)
	{ 
		if (!m_wndDateTimeBoxStart.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE |/* WS_BORDER |*/ DTS_SHORTDATEFORMAT,
			CRect(0, 0, 100, 22), this, ID_DATETIME_START))
		{
			return FALSE;
		} 
		CXTPControlCustom* pControlMonthCal = CXTPControlCustom::CreateControlCustom(&m_wndDateTimeBoxStart);
		lpCreateControl->pControl = pControlMonthCal;
		return TRUE;
	}
	if (lpCreateControl->nID == ID_DATETIME_END)
	{ 
		if (!m_wndDateTimeBoxEnd.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE |/* WS_BORDER |*/ DTS_SHORTDATEFORMAT,
			CRect(0, 0, 100, 22), this, ID_DATETIME_END))
		{
			return FALSE;
		} 
		CXTPControlCustom* pControlMonthCal = CXTPControlCustom::CreateControlCustom(&m_wndDateTimeBoxEnd);
		//pControlMonthCal->SetFlags(xtpFlagManualUpdate); 
		lpCreateControl->pControl = pControlMonthCal;
		return TRUE;
	}
	return FALSE;

}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		
		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{ 
				
			case CG_ID_VIEW_WORKSPACE:
				
				if (!m_wndDeviceView.GetSafeHwnd())
				{
					m_wndDeviceView.Create(_T("STATIC"), NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, 0);
				}
				pPane->Attach(&m_wndDeviceView);				
				break;		
 	
			}
		}
		return 1;
	}
	
	if (wParam == XTP_DPN_CONTEXTMENU)
	{
		XTP_DOCKINGPANE_CLICK* pClick = (XTP_DOCKINGPANE_CLICK*)lParam;
		
		CXTPDockingPane* pPopupPane = pClick->pPane;
		if (!pPopupPane) 
			return FALSE;
		
		ShowDockingPanePopupMenu(pPopupPane, pClick->pt, pClick->rcExclude);
		
		return TRUE;
	}
	
	return 0;
}

void CMainFrame::ShowDockingPanePopupMenu(CXTPDockingPane *pPopupPane, CPoint pt, LPRECT lprcExclude)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
	CMenu* pPopup = menu.GetSubMenu(0);
	
	
	if (pPopupPane->IsHidden())
	{
		pPopup->CheckMenuItem(ID_POPUP_AUTOHIDE, MF_BYCOMMAND|MF_CHECKED);
		pPopup->EnableMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_DISABLED);
		pPopup->EnableMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_DISABLED);
	}
	else if (pPopupPane->GetOptions() & xtpPaneNoDockable)
	{
		pPopup->CheckMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_CHECKED);
	}
	else
	{
		pPopup->CheckMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_CHECKED);
	}
	
	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = lprcExclude ? *lprcExclude : CRect(0, 0, 0, 0);
	
	int nCommand = GetCommandBars()->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, pt.x, pt.y, NULL, &tpm);
	
	switch (nCommand)
	{
	case ID_POPUP_HIDE:
		pPopupPane->Close();
		break;		
		
	case ID_POPUP_AUTOHIDE:
		m_paneManager.ToggleAutoHide(pPopupPane);
		break;
		
	case ID_POPUP_DOCKABLE:
		if (pPopupPane->GetOptions() & xtpPaneNoDockable)
		{
			pPopupPane->SetOptions(pPopupPane->GetOptions() & ~xtpPaneNoDockable);
		}
		break;
		
	case ID_POPUP_FLOATING:
		if ((pPopupPane->GetOptions() & xtpPaneNoDockable) == 0)
		{
			if (!pPopupPane->IsFloating())
				m_paneManager.ToggleDocking(pPopupPane);
			
			pPopupPane->SetOptions(pPopupPane->GetOptions() | xtpPaneNoDockable);
		}
		
		break;
	}
}

BOOL CMainFrame::OnShowView(UINT nID)
{
	CXTPDockingPane *pPane = m_paneManager.FindPane(nID);
	//m_paneManager.ShowPane(nID,!pPane->IsHidden());
	m_paneManager.ShowPane(nID,!pPane->IsHidden());
	return 0;

}

void CMainFrame::OnUpdateControlBarMenu(CCmdUI *pCmdUI)
{
	CXTPDockingPane *pPane = m_paneManager.FindPane(pCmdUI->m_nID);
	pCmdUI->SetCheck(!pPane->IsHidden());		
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{	
	// TODO: Add your message handler code here
	static HWND hFocus = 0;
	
	if (nState == WA_INACTIVE)
	{
		hFocus = m_paneManager.GetActivePane() != NULL ? ::GetFocus() : NULL;
	}
	CXTPMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);	
	if (nState != WA_INACTIVE && hFocus)
	{
		::SetFocus(hFocus);		
		hFocus = NULL;		
	}	
}
 

void CMainFrame::OnRegOCX()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	int bResult = 0;
	//导入注册表
	STARTUPINFO lpStartupInfo;
	PROCESS_INFORMATION lpProcessInformation;
	TCHAR winPathBuf[MAX_PATH];
	TCHAR currPathBuf[MAX_PATH];

	lpStartupInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&lpStartupInfo,sizeof(STARTUPINFO));
	ZeroMemory(&lpProcessInformation,sizeof(PROCESS_INFORMATION));
	GetWindowsDirectory(winPathBuf,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,currPathBuf);
	lstrcat(winPathBuf,_T("\\regedit.exe /i /s mschrt20.reg"));

	bResult = CreateProcess(NULL,winPathBuf, NULL, NULL, FALSE, 0, NULL, currPathBuf, &lpStartupInfo, &lpProcessInformation);		
	ProcessErrorMessage(_T("RegOCX"));
	// Wait until child process exits.
	WaitForSingleObject( lpProcessInformation.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( lpProcessInformation.hProcess );
	CloseHandle( lpProcessInformation.hThread );

	if(bResult =RegActivex(_T("RMREPORT.OCX"))!=0)
	{
		MessageBox(_T("RMREPORT.OCX,无法注册，\n请检查文件是否损毁"),_T("警告!"),MB_OK+MB_ICONWARNING);
	}

	if(bResult =RegActivex(_T("MSChrt20.OCX"))!=0)
	{
		MessageBox(_T("MSChrt20.OCX,无法注册，\n请检查文件是否损毁"),_T("警告!"),MB_OK+MB_ICONWARNING);
	}
	MessageBox(_T("注册完成!\n"),_T("提示"),MB_OK+MB_ICONINFORMATION);
}

void CMainFrame::OnSetupComm()
{
	CSetupComm dlg;
	dlg.DoModal();
}
