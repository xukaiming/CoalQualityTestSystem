// DBQryGDocument.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DBQryGDocument.h"
#include "MainFrm.h"

// CDBQryGDocument

IMPLEMENT_DYNCREATE(CDBQryGDocument, CDBQryDocument)

CDBQryGDocument::CDBQryGDocument()
{
}

BOOL CDBQryGDocument::OnNewDocument()
{
	if (!CDBQryDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDBQryGDocument::~CDBQryGDocument()
{
}


BEGIN_MESSAGE_MAP(CDBQryGDocument, CDBQryDocument)
END_MESSAGE_MAP()


// CDBQryGDocument diagnostics

#ifdef _DEBUG
void CDBQryGDocument::AssertValid() const
{
	CDBQryDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CDBQryGDocument::Dump(CDumpContext& dc) const
{
	CDBQryDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CDBQryGDocument serialization

void CDBQryGDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CDBQryGDocument commands

void CDBQryGDocument::SQL(void)
{
	HRESULT hr= S_OK;
	CString strParam1,strParam2;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strDev = _T("1#量热仪");
	int index = pFrame->m_wndComboBoxDevice->GetCurSel();
	pFrame->m_wndComboBoxDevice->GetLBText( index, strDev);

	CString strBombNO = _T("A氧弹");
	index = pFrame->m_wndComboBoxBombNO->GetCurSel();
	pFrame->m_wndComboBoxBombNO->GetLBText( index, strBombNO );

	pFrame->m_wndDateTimeBoxStart.GetTime(StartTime);
	pFrame->m_wndDateTimeBoxEnd.GetTime(EndTime); 
	EndTime =MakeDays(1)+EndTime;
	CString strStartTime,strEndTime;
	strStartTime.Format(_T("#%d-%d-%d#"),StartTime.GetYear(),StartTime.GetMonth(),StartTime.GetDay());
	strEndTime.Format(_T("#%d-%d-%d#"),EndTime.GetYear(),EndTime.GetMonth(),EndTime.GetDay());

	if(strDev!=_T("全部"))
		strParam2.Format(_T("and 设备名称='%s'"),strDev);
	else
		strParam2 = _T("");
	strSQL.Format(_T("select * from 工业分析仪数据 where 试验日期>=%s and 试验日期<=%s   %s order by 试验日期"),strStartTime,strEndTime,strParam2);

	try
	{
		if(Rs->State==adStateOpen)
			Rs->Close();
		//IADORecordBindingPtr pIRB(pRsIED);
		hr = Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	
		TESTHR(hr);
		//TESTHR(pIRB->BindToRecordset(&rsQryBind));
	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
	} 
}
