// LryEDBQryDoc.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "LryEDBQryDoc.h"
#include "LRYCTRL.h"
#include "mainfrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryDoc

IMPLEMENT_DYNCREATE(CLryEDBQryDoc, CDocument)

CLryEDBQryDoc::CLryEDBQryDoc()
{
}

BOOL CLryEDBQryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE; 
	Rs.CreateInstance("ADODB.Recordset");
	RsPrintData.CreateInstance("ADODB.Recordset");
	RsTitle.CreateInstance("ADODB.Recordset");
	SQL();
	return TRUE;
}

CLryEDBQryDoc::~CLryEDBQryDoc()
{
}


BEGIN_MESSAGE_MAP(CLryEDBQryDoc, CDocument)
	//��Ӧ�����ƶ�ָ��
	ON_COMMAND_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV, OnCursorMove)
 	ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV,OnUpdatCursorButton)
	//��Ӧ��ѯʱ��ı仯
	
	ON_NOTIFY_RANGE(DTN_CLOSEUP,ID_DATETIME_START,ID_DATETIME_END,OnDBTimerChange)
	//ON_NOTIFY_RANGE(DTN_DATETIMECHANGE,ID_DATETIME_START,ID_DATETIME_END,OnDBTimerChange)   
	ON_UPDATE_COMMAND_UI_RANGE(ID_DATETIME_START,ID_AVERAGE,OnUpdatDBDateTime)
	//��Ӧ�豸�ı� 
	//ON_CBN_SELCHANGE(ID_DEVICE,OnDeviceIDChange)  // CComboBox 	

	ON_XTP_EXECUTE(ID_DEVICE,OnDeviceIDChange)  // CComboBox 	
	ON_XTP_EXECUTE(ID_BOMB_NO,OnDeviceIDChange)  // CComboBox	
	ON_XTP_EXECUTE(ID_AVERAGE,OnDeviceIDChange)  // CComboBox	
	//{{AFX_MSG_MAP(CLryEDBQryDoc)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	ON_COMMAND(ID_DELETE_RECORD, OnDeleteRecord)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryDoc diagnostics

#ifdef _DEBUG
void CLryEDBQryDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLryEDBQryDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryDoc serialization

void CLryEDBQryDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CLryEDBQryDoc commands

HRESULT CLryEDBQryDoc::Move(ADO_LONGPTR NumRecords, const _variant_t &Start)
{
	HRESULT  Result;
	try
	{
		Result = Rs->Move(NumRecords,Start); 
		TESTHR(Result);
		UpdateAllViews(NULL,1,NULL);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	return Result;
}

HRESULT CLryEDBQryDoc::MoveFirst()
{
	HRESULT hr = S_OK;
	try
	{
		hr = Rs->MoveFirst(); //MoveFirst ������BOF��־
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}		
	return hr;
}

HRESULT CLryEDBQryDoc::MoveLast()
{
	HRESULT hr= S_OK;
	try
	{
		hr = Rs->MoveLast(); //MoveLast ������adoEOF��־?
	}
	catch(_com_error &e)
	{
			dump_com_error(e);
	}			
	return hr;
}

HRESULT CLryEDBQryDoc::MovePrevious()
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

HRESULT CLryEDBQryDoc::MoveNext()
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

void CLryEDBQryDoc::OnCursorMove(UINT nID)
{
	try
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
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	UpdateAllViews(	NULL,1);
}


void CLryEDBQryDoc::OnRecordRefresh() 
{
	// TODO: Add your command handler code here
	try
	{
		SQL(); 
		UpdateAllViews(NULL); 
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
		
}

void CLryEDBQryDoc::SQL()
{
	HRESULT hr= S_OK;
	CString strParam1,strParam2;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strDev = _T("1#������");
	int index = pFrame->m_wndComboBoxDevice->GetCurSel();
	pFrame->m_wndComboBoxDevice->GetLBText( index, strDev);

	CString strBombNO = _T("A����");
	index = pFrame->m_wndComboBoxBombNO->GetCurSel();
	pFrame->m_wndComboBoxBombNO->GetLBText( index, strBombNO );

	pFrame->m_wndDateTimeBoxStart.GetTime(StartTime);
	pFrame->m_wndDateTimeBoxEnd.GetTime(EndTime); 
	EndTime =MakeDays(1)+EndTime;
	CString strStartTime,strEndTime;
	strStartTime.Format(_T("#%d-%d-%d#"),StartTime.GetYear(),StartTime.GetMonth(),StartTime.GetDay());
	strEndTime.Format(_T("#%d-%d-%d#"),EndTime.GetYear(),EndTime.GetMonth(),EndTime.GetDay());
	if(strBombNO!=_T("ȫ��"))
		strParam1.Format(_T("and �������='%s'"),strBombNO);
	else
		strParam1 = _T("");
	if(strDev!=_T("ȫ��"))
		strParam2.Format(_T("and �豸����='%s'"),strDev);
	else
		strParam2 = _T("");
	strSQL.Format(_T("select * from ������ where ��������>=%s and ��������<=%s  %s  %s"),strStartTime,strEndTime,strParam1,strParam2);
	
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

 

void CLryEDBQryDoc::OnDeleteRecord() 
{ 
	
	HRESULT hr; 
	try
	{ 
			if((theApp.UserInfo.bLogon()== TRUE)&&
			(ID_OK==MessageBox(NULL,_T("ȷ��ɾ����¼��?"),_T("����!"),MB_OKCANCEL+MB_ICONWARNING+MB_TASKMODAL)))
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

void CLryEDBQryDoc::OnDeviceIDChange(NMHDR* pNMHDR, LRESULT* pResult)
{ 
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0);
}


void CLryEDBQryDoc::OnDBTimerChange(UINT id, NMHDR *pNotifyStruct, LRESULT *pResult)
{
	*pResult = 0;
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0);
}


void CLryEDBQryDoc::OnUpdatCursorButton(CCmdUI* pCmdUI)
{

	BOOL bResult = TRUE;
	int id=0;
	LONG iCursor  = Rs->AbsolutePosition;
	LONG iRecordCount;
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

void CLryEDBQryDoc::OnUpdatDBDateTime(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
} 

 
