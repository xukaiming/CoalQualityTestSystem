// DBQryDocument.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DBQryDocument.h"


// CDBQryDocument

IMPLEMENT_DYNCREATE(CDBQryDocument, CDocument)

CDBQryDocument::CDBQryDocument()
{
}

BOOL CDBQryDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	Rs.CreateInstance("ADODB.Recordset");
	RsPrintData.CreateInstance("ADODB.Recordset");
	RsTitle.CreateInstance("ADODB.Recordset"); 
	SQL();
	return TRUE;
}

CDBQryDocument::~CDBQryDocument()
{
}


BEGIN_MESSAGE_MAP(CDBQryDocument, CDocument)
	//响应数据移动指令
	ON_COMMAND_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV, OnCursorMove)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV,OnUpdatCursorButton)
	//响应查询时间的变化

	
	//ON_NOTIFY_RANGE(DTN_CLOSEUP,ID_DATETIME_START,ID_DATETIME_END,OnDBTimerChange)
	ON_NOTIFY_RANGE(DTN_DATETIMECHANGE,ID_DATETIME_START,ID_DATETIME_END,OnDBTimerChange)   
	ON_UPDATE_COMMAND_UI_RANGE(ID_DATETIME_START,ID_BOMB_NO,OnUpdatDBDateTime)
	//响应设备改变
	ON_XTP_EXECUTE(ID_DEVICE,OnDeviceIDChange)  // CComboBox 	
	ON_XTP_EXECUTE(ID_BOMB_NO,OnDeviceIDChange)  // CComboBox	
	//{{AFX_MSG_MAP(CLryQDBQryDoc)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	ON_COMMAND(ID_DELETE_RECORD, OnDeleteRecord)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDBQryDocument diagnostics

#ifdef _DEBUG
void CDBQryDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CDBQryDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CDBQryDocument serialization

void CDBQryDocument::Serialize(CArchive& ar)
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


// CDBQryDocument commands

void CDBQryDocument::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class
	if(Rs->State==adStateOpen)
		Rs->Close();
	if(RsPrintData->State==adStateOpen)
		RsPrintData->Close();
	if(RsTitle->State==adStateOpen)
		RsTitle->Close(); 
	CDocument::OnCloseDocument();
}

void CDBQryDocument::SQL(void)
{
	ASSERT(FALSE);
}

HRESULT CDBQryDocument::MoveNext()
{
	HRESULT hr = S_OK ;
	try
	{
		if((!Rs->adoEOF))
			hr = Rs->MoveNext();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}			
	return hr;
}

HRESULT CDBQryDocument::MovePrevious()
{
	HRESULT hr = S_OK;
	try
	{
		if((!Rs->BOF))
			hr = Rs->MovePrevious();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}			
	return hr;
}

HRESULT CDBQryDocument::MoveLast()
{
	HRESULT hr= S_OK;
	try
	{
		hr = Rs->MoveLast(); //MoveLast 不设置adoEOF标志?
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}			
	return hr;
}

HRESULT CDBQryDocument::MoveFirst()
{
	HRESULT hr = S_OK;
	try
	{
		hr = Rs->MoveFirst(); //MoveFirst 不设置BOF标志
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}		
	return hr;
}

HRESULT CDBQryDocument::Move(ADO_LONGPTR NumRecords, const _variant_t &Start)
{
	HRESULT Result = Rs->Move(NumRecords,Start); 
	UpdateAllViews(NULL,1,NULL);
	return Result;
}

void CDBQryDocument::OnCursorMove(UINT nID)
{
	switch(nID)
	{
	case ID_RECORD_FIRST:		
		MoveFirst();
		break;
	case ID_RECORD_PREV:
		MovePrevious();
		break;
	case ID_RECORD_NEXT:
		MoveNext();
		break;
	case ID_RECORD_LAST:
		MoveLast();
		break;
	default: break;
	} 
	UpdateAllViews(	NULL,1);
}

void CDBQryDocument::OnUpdatCursorButton(CCmdUI* pCmdUI)
{

	BOOL bResult = TRUE;
	int id=0;
	LONG iCursor ;
	LONG iRecordCount ;
	 
	try
	{
		iCursor = Rs->AbsolutePosition;
		iRecordCount = Rs->RecordCount;
		switch(pCmdUI->m_nID)
		{
		case ID_RECORD_FIRST:
		case ID_RECORD_PREV:
			if( (iCursor==1)||(Rs->BOF))
			{
				m_bBOF = TRUE;
				bResult = FALSE;
			}	
			else
			{
				m_bBOF = FALSE;
			}
			break;
		case ID_RECORD_NEXT:
		case ID_RECORD_LAST:

			if((iCursor==iRecordCount)||(Rs->adoEOF))
			{
				m_bADOEOF	= TRUE;
				bResult		= FALSE;
			}
			else
			{
				m_bBOF = FALSE;
			}
			break;
		default: 
			break;
		}
	}
	catch(_com_error &e)
	{ 
		dump_com_error(e);
	} 
	pCmdUI->Enable(bResult);
}

void CDBQryDocument::OnRecordRefresh()
{
	SQL(); 
	UpdateAllViews(NULL); 	
}
void CDBQryDocument::OnDeleteRecord()
{
	HRESULT hr; 
	try
	{ 
		if(theApp.UserInfo.bLogon()&&
			(ID_OK==MessageBox(NULL,_T("确认删除记录吗?"),_T("警告!"),MB_OKCANCEL+MB_ICONWARNING+MB_TASKMODAL)))  
		{
			pObj.RowNO = Rs->AbsolutePosition-1;
			hr = Rs->Delete(adAffectCurrent); 			
			TESTHR(hr); 
			hr = Rs->Update();		
			TESTHR(hr);		 
			hr = MoveNext();
			TESTHR(hr); 
			//hr = MovePrevious();
			//TESTHR(hr); 
			UpdateAllViews(NULL,2,&pObj);	
		}
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	} 
}
/////////////////////////////////////////////////
void CDBQryDocument::OnDeviceIDChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_RECORD_REFRESH,0); 

}
void CDBQryDocument::OnDBTimerChange(UINT id, NMHDR* pNotifyStruct,  LRESULT* pResult )
{
	
	*pResult = 0;
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0);
}

void CDBQryDocument::OnUpdatDBDateTime(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}