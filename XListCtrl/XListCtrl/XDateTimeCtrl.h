#ifndef _XDATETIMECTRL_H_
#define _XDATETIMECTRL_H_
#pragma once

#include "DropWnd.h"
// CXDateTimeCtrl

class CXDateTimeCtrl : public CDateTimeCtrl
{
	DECLARE_DYNAMIC(CXDateTimeCtrl)

public:
	CXDateTimeCtrl();
	virtual ~CXDateTimeCtrl();
public:
	CWnd * GetDropWnd() { return m_pDropWnd; }
private:
	CDropWnd* m_pDropWnd;
	CRect m_rcDropButton;
protected:
	CWnd * m_pParent;
	void SendRegisteredMessage(UINT nMsg, WPARAM wParam, LPARAM lParam);
protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEscapeKey();
	afx_msg void OnComboComplete();
	DECLARE_MESSAGE_MAP()
};
#endif

