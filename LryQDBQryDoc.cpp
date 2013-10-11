// LryDBQryDoc.cpp : implementation file
//

#include "stdafx.h"

#include "LryQDBQryDoc.h"
#include "LRYCTRL.h"
#include "mainfrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryDoc

IMPLEMENT_DYNCREATE(CLryQDBQryDoc, CDocument)

CLryQDBQryDoc::CLryQDBQryDoc()
{
}

BOOL CLryQDBQryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	Rs.CreateInstance("ADODB.Recordset");
	RsPrintData.CreateInstance("ADODB.Recordset");
	RsTitle.CreateInstance("ADODB.Recordset");
	SQL();
	return TRUE;
}

CLryQDBQryDoc::~CLryQDBQryDoc()
{
}


BEGIN_MESSAGE_MAP(CLryQDBQryDoc, CDocument)
	//��Ӧ�����ƶ�ָ��
	ON_COMMAND_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV, OnCursorMove)
 	ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_FIRST,ID_RECORD_PREV,OnUpdatCursorButton)
	//��Ӧ��ѯʱ��ı仯
	
	ON_NOTIFY_RANGE(DTN_CLOSEUP,ID_DATETIME_START,ID_DATETIME_END,OnDBTimerChange)
	//ON_NOTIFY_RANGE(DTN_DATETIMECHANGE,ID_DATETIME_START,ID_DATETIME_END,OnDBTimerChange)   
	ON_UPDATE_COMMAND_UI_RANGE(ID_DATETIME_START,ID_AVERAGE,OnUpdatDBDateTime)//ID_BOMB_NO
	//��Ӧ�豸�ı�
	ON_XTP_EXECUTE(ID_DEVICE,OnDeviceIDChange)  // CComboBox 	
	ON_XTP_EXECUTE(ID_BOMB_NO,OnDeviceIDChange)  // CComboBox	
	ON_XTP_EXECUTE(ID_AVERAGE,OnDeviceIDChange)  // CComboBox	
	//{{AFX_MSG_MAP(CLryQDBQryDoc)
	ON_COMMAND(ID_RECORD_REFRESH, OnRecordRefresh)
	ON_COMMAND(ID_DELETE_RECORD, OnDeleteRecord) 
	ON_COMMAND(ID_AVERAGE_MENU, OnAverageMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryDoc diagnostics

#ifdef _DEBUG
void CLryQDBQryDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLryQDBQryDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLryQDBQryDoc serialization

void CLryQDBQryDoc::Serialize(CArchive& ar)
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
// CLryQDBQryDoc commands

HRESULT CLryQDBQryDoc::Move(ADO_LONGPTR NumRecords, const _variant_t &Start)
{
	HRESULT Result = Rs->Move(NumRecords,Start); 
	UpdateAllViews(NULL,1,NULL);
	return Result;
}

HRESULT CLryQDBQryDoc::MoveFirst()
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

HRESULT CLryQDBQryDoc::MoveLast()
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

HRESULT CLryQDBQryDoc::MovePrevious()
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

HRESULT CLryQDBQryDoc::MoveNext()
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

void CLryQDBQryDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(Rs->State==adStateOpen)
		Rs->Close();	
	CDocument::OnCloseDocument();	 
}

void CLryQDBQryDoc::SQL()
{
	HRESULT hr= S_OK;
	CString strParam1,strParam2;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strDev = _T("1#������       ");
	int index = pFrame->m_wndComboBoxDevice->GetCurSel();
	pFrame->m_wndComboBoxDevice->GetLBText( index, strDev );
	
	CString strBombNO = _T("A����");
	index = pFrame->m_wndComboBoxBombNO->GetCurSel();
	pFrame->m_wndComboBoxBombNO->GetLBText( index, strBombNO);

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

	CString strAvr ;
	index = pFrame->m_wndComboBoxAvr->GetCurSel();
	pFrame->m_wndComboBoxAvr->GetLBText( index, strAvr);
	if(strAvr==_T("ƽ����¼"))
	{
		strAvr.Format(_T("SELECT ��Ʒ���,Min(��������) AS ��������,AVG(��Ͳ������) AS ƽ����Ͳ������,\
						 AVG(��λ������) AS ƽ����λ������,AVG(�ɻ���λ������) AS ƽ���ɻ���λ������, \
						 AVG(��λ������) AS ƽ����λ������,�豸����,�������, ƽ������\
			FROM ������\
			WHERE ƽ������ IN \
			(%s)\
			GROUP BY ��Ʒ���, ƽ������,�豸����,�������\
			ORDER BY ƽ������"),strSQL);
			strSQL = strAvr;
			strSQL.Replace(_T("*"),_T("ƽ������")); 

	}

	try
	{
		if(Rs->State==adStateOpen)
			Rs->Close();
		Rs->Filter = _variant_t("");
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

void CLryQDBQryDoc::OnCursorMove(UINT nID)
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

 

void CLryQDBQryDoc::OnUpdatCursorButton(CCmdUI* pCmdUI)
{

	BOOL bResult = TRUE;
	int id=0;
	LONG iCursor ;
	LONG iRecordCount ;
	try
	{
		iRecordCount = Rs->RecordCount;
		iCursor = Rs->AbsolutePosition;
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

void CLryQDBQryDoc::OnUpdatDBDateTime(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CLryQDBQryDoc::OnDBTimerChange(UINT id, NMHDR *pNotifyStruct, LRESULT *pResult)
{
	*pResult = 0;
	HWND hwnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::SendMessage(hwnd,WM_COMMAND,ID_RECORD_REFRESH,0);
}

void CLryQDBQryDoc::OnRecordRefresh() 
{
	// TODO: Add your command handler code here
	SQL(); 
	UpdateAllViews(NULL); 		
}

 

void CLryQDBQryDoc::OnDeleteRecord() 
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

void CLryQDBQryDoc::OnDeviceIDChange(NMHDR* pNMHDR, LRESULT* pResult)
{ 
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_RECORD_REFRESH,0); 
}


void CLryQDBQryDoc::OnAverageMenu()
{
	CString strAvr ;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	int index = pFrame->m_wndComboBoxAvr->GetCurSel();
	pFrame->m_wndComboBoxAvr->GetLBText( index, strAvr);  
	if (strAvr==_T("ƽ����¼"))
	{ 
		pFrame->m_wndComboBoxAvr->SetCurSel(0);

	} 
	else
	{
		pFrame->m_wndComboBoxAvr->SetCurSel(1); 
	}
	try
	{
		_variant_t filter = Rs->GetCollect("ƽ������");
		CString strFilter;
		strFilter.Format(_T("ƽ������=%s"),VariantToStr(filter));
		pFrame->m_wndComboBoxAvr->OnExecute(); 
		Rs->Filter = _variant_t(strFilter);
		UpdateAllViews(NULL);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}


