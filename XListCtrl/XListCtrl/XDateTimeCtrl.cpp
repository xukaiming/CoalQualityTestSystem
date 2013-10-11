// XDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XDateTimeCtrl.h"


// CXDateTimeCtrl
UINT NEAR WM_XDATETIMECTRL_VK_RETURN = ::RegisterWindowMessage(_T("WM_XDATETIMECTRL_VK_RETURN"));
UINT NEAR WM_XDATETIMECTRL_VK_ESCAPE = ::RegisterWindowMessage(_T("WM_XDATETIMECTRL_VK_ESCAPE"));
UINT NEAR WM_XDATETIMECTRL_KEYDOWN   = ::RegisterWindowMessage(_T("WM_XDATETIMECTRL_KEYDOWN"));
UINT NEAR WM_XDATETIMECTRL_LBUTTONUP = ::RegisterWindowMessage(_T("WM_XDATETIMECTRL_LBUTTONUP"));

UINT WM_XDATETIMECTRL_COMPLETE  = ::RegisterWindowMessage(_T("WM_XDATETIMECTRL_COMPLETE"));
IMPLEMENT_DYNAMIC(CXDateTimeCtrl, CDateTimeCtrl)

CXDateTimeCtrl::CXDateTimeCtrl()
{

}

CXDateTimeCtrl::~CXDateTimeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXDateTimeCtrl, CDateTimeCtrl)
END_MESSAGE_MAP()



// CXDateTimeCtrl message handlers

void CXDateTimeCtrl::SendRegisteredMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = m_pParent;
	if (pWnd)
		pWnd->SendMessage(nMsg, wParam, lParam);
}

void CXDateTimeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	XLISTCTRL_TRACE(_T("in CXDateTimeCtrl::OnKillFocus\n"));
	CDateTimeCtrl::OnKillFocus(pNewWnd);
	if (pNewWnd != GetDropWnd())
	{
		SendRegisteredMessage(WM_XDATETIMECTRL_COMPLETE, 0, 0);
	}
}

void CXDateTimeCtrl::OnEscapeKey()
{
	XLISTCTRL_TRACE(_T("in CXDateTimeCtrl::OnEscapeKey\n"));

	SendRegisteredMessage(WM_XDATETIMECTRL_VK_ESCAPE, 0, 0);
}

void CXDateTimeCtrl::OnComboComplete()
{
	XLISTCTRL_TRACE(_T("in CXDateTimeCtrl::OnComboComplete\n"));

	SendRegisteredMessage(WM_XDATETIMECTRL_COMPLETE, 0, 0);
}
