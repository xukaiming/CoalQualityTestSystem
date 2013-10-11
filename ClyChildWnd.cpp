// ClyChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "ClyChildWnd.h"
#include "ClyDebugView.h"
#include "ClyHostView.h"
 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClyChildWnd

IMPLEMENT_DYNCREATE(CClyChildWnd, CMDIChildWnd)

CClyChildWnd::CClyChildWnd()
{
}

CClyChildWnd::~CClyChildWnd()
{
}


BEGIN_MESSAGE_MAP(CClyChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CClyChildWnd)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
	ON_WM_GETMINMAXINFO() 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClyChildWnd message handlers

BOOL CClyChildWnd::PreCreateWindow(CREATESTRUCT& cs) 
{ 
	if(!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;
//	cs.style &=  ~WS_HSCROLL;
//	cs.style &=  ~WS_VSCROLL;
	return TRUE;
}

BOOL CClyChildWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{ 
	/*
	m_bMainSplitterReady = FALSE;
	if(!m_wndSplitter.CreateStatic(this,1,2,HIDEMODE_RIGHTEDGE))
		return FALSE;
	m_wndSplitter.ModifyStyle(WS_VSCROLL,0);     
	if(!m_wndSplitter.Set4Bitmaps(IDB_BMPLEFT,IDB_BMPRIGHT,IDB_BMPDOWN,IDB_BMPUP))
		return FALSE;
	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CClyHostView),CSize(900,900),pContext)||
		!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CClyDebugView),CSize(940,900),pContext))		
		return FALSE;
	
	m_bMainSplitterReady = TRUE;
	*/
	/*
	CRect rect;
	CView * pView	= (CView *)m_wndSplitter.GetPane(0,1); 
	pView->GetDlgItem(IDC_EDIT_STATE)->GetWindowRect(&rect); 
	m_wndSplitter.SetColumnInfo(1,rect.Width()+::GetSystemMetrics(SM_CXBORDER)*2 ,0);//
	m_wndSplitter.RecalcLayout();
	*/
	//return TRUE;	
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CClyChildWnd::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);	
	CRect rect;
	int iBdWidth =0;	
	/*
	if(m_bMainSplitterReady&& m_wndSplitter.GetSafeHwnd())
	{

		CView * pView	= (CView *)m_wndSplitter.GetPane(0,1); 
		iBdWidth=::GetSystemMetrics(SM_CXBORDER);
		pView->GetDlgItem(IDC_EDIT_STATE)->GetClientRect(&rect); 
		m_wndSplitter.SetColumnInfo(0,cx-rect.Width(),cx-rect.Width());//	
		m_wndSplitter.SetColumnInfo(1,rect.Width()+iBdWidth*2,0);
		m_wndSplitter.RecalcLayout();
	}
*/
	//GetWindowRectºÍGetClientRect 
	
}

void CClyChildWnd::OnSysCommand(UINT nID, LPARAM lParam)
{

	if(nID==SC_CLOSE)
	{
		SendMessage(WM_COMMAND,ID_FILE_CLOSE,0);
	}
	else
	{
		CMDIChildWnd::OnSysCommand(nID, lParam);
	}
	
}
void CClyChildWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{ 

	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 500;
	lpMMI->ptMinTrackSize.y = 550;

}
