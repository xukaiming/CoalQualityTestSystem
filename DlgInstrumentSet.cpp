// DlgInstrumentSet.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "DlgInstrumentSet.h"


// CDlgInstrumentSet dialog

IMPLEMENT_DYNAMIC(CDlgInstrumentSet, CDialog)

CDlgInstrumentSet::CDlgInstrumentSet(CWnd* pParent) 
{
	Rdb = NULL;
}

CDlgInstrumentSet::CDlgInstrumentSet(UINT nIDTemplate,CWnd*  pParentWnd = NULL)//��Ҫ��� 
:CDialog(nIDTemplate,pParentWnd)
{

}

CDlgInstrumentSet::~CDlgInstrumentSet()
{
}

void CDlgInstrumentSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInstrumentSet, CDialog)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_DEVICE_ADD, OnBnClickedDeviceAdd)
	ON_BN_CLICKED(IDC_DEVICE_DEL, OnBnClickedDeviceDel)
	ON_BN_CLICKED(IDC_DEVICE_MODIFY, OnBnClickedDeviceModify)
END_MESSAGE_MAP()


// CDlgInstrumentSet message handlers
void CDlgInstrumentSet::OnBnClickedDeviceAdd()
{
	try
	{ 		
		Rdb->pRsDev->AddNew();
		
		GetParamFromDlg();
		Rdb->UpdateParam2DB();	
		Rdb->pRsDev->Update();	
		//pRsDev->MoveNext();
		((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);
		((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,Rdb->pRsDev->RecordCount);
	}
	catch (_com_error &e) {
		Rdb->pRsDev->CancelUpdate();
		dump_com_error(e);
	}	
}

void CDlgInstrumentSet::OnBnClickedDeviceDel()
{
	if(
		(theApp.UserInfo.bLogon() == TRUE)&&
		(IDOK == AfxMessageBox(_T("���豸��Ҫ��ɾ��\n\n��ȷ����?"),MB_OKCANCEL))
		)

	{
		try
		{ 
			Rdb->pRsDev->Delete(adAffectCurrent);
			Rdb->pRsDev->Update(); 
			((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);
			((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,Rdb->pRsDev->RecordCount);
			//UDN_DELTAPOS 
			MessageBox(_T("���豸��ɾ��"),_T("��ʾ"),MB_ICONINFORMATION+MB_OK);
		}
		catch (_com_error &e) {
			Rdb->pRsDev->CancelUpdate();
			dump_com_error(e);
		}	
	}
}

void CDlgInstrumentSet::OnBnClickedDeviceModify()
{
	//if(!theApp.UserInfo.bLogon)
	//	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_MENU_USERLOGIN);
	if(
		(theApp.UserInfo.bLogon() == TRUE)&&
		(IDOK == AfxMessageBox(_T("�޸��豸���������ϵͳ��һϵ�����⣬\n\n������רҵ������Ա�޸ģ�\n\n���϶�Ҫ�޸���"),MB_OKCANCEL))
		)

	{	
		try
		{ 
			GetParamFromDlg(); 
			Rdb->UpdateParam2DB();
			Rdb->pRsDev->Update();
			((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);

		}
		catch (_com_error &e) {
			Rdb->pRsDev->CancelUpdate();
			dump_com_error(e);
		}
	}
}
 

void CDlgInstrumentSet::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString s;

	if (nSBCode == SB_ENDSCROLL) {
		return; // Reject spurious messages
	}  
	if(pScrollBar->GetDlgCtrlID()==IDC_DEVICE_SPIN)
	{
		try{ 
			Rdb->pRsDev->Move( nPos-1,(long)adBookmarkFirst);  
			Rdb->UpdateParamFromDB();	
			UpdateWinText();
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}	
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
 

void CDlgInstrumentSet::UpdateWinText(void)
{ 
}

void CDlgInstrumentSet::GetParamFromDlg(void)
{

}


BOOL CDlgInstrumentSet::OnInitDialog()
{
	CSpinButtonCtrl *pSpinCtl;
	CDialog::OnInitDialog();
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_ATTRIB),FALSE); 
	if((pSpinCtl = ((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN)))!=NULL)
		pSpinCtl ->SetRange(1,2);
	else
	{
		AfxMessageBox(_T("û��CSpinButtonCtrl�ؼ�"));
		return TRUE;
	} 
	BOOL bResult = TRUE;	 
	try
	{
		SQL();
		MoveDBPtr();
		//Rdb->UpdateParamFromDB();
		UpdateWinText();
	}
	catch (_com_error &e) {
		dump_com_error(e);
	}

	return TRUE;   
}

void CDlgInstrumentSet::SQL(void)
{ 
	HRESULT hr		= S_OK;
	try
	{ 
		Rdb->LoadParamFromDB(GetCurrParentSeleString()); 
		if(Rdb->pRsDev->RecordCount>0)
		{  
			((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,Rdb->pRsDev->RecordCount);			
		} 
	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
		dump_com_error(e); 
	} 	
}

void CDlgInstrumentSet::MoveDBPtr(void)
{
	try
	{  
		int i  =0 ;
		Rdb->LoadParamFromDB(GetCurrentSeleString(),GetCurrParentSeleString()); 
		i = Rdb->pRsDev->AbsolutePosition;
		SetDlgItemInt(IDC_DEVICE_ROW_NO ,i)  ; 
	}
	catch (_com_error &e) {
		dump_com_error(e);
	}
}

CString CDlgInstrumentSet::GetCurrentSeleString(void)
{
	CTreeCtrl  * pTreeCtrl = &((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_wndDeviceView.m_wndClassView;
	HTREEITEM    hItem     = pTreeCtrl->GetSelectedItem();
	return pTreeCtrl->GetItemText(hItem);
}

CString CDlgInstrumentSet::GetCurrParentSeleString()
{
	CMainFrame * pFrame				= (CMainFrame *)AfxGetApp()->m_pMainWnd;
	COfficeTreeCtrl * pTreeCtrl		= (COfficeTreeCtrl *)&pFrame->m_wndDeviceView.m_wndClassView;
	HTREEITEM    hItem				= pTreeCtrl->GetSelectedItem();
	hItem = pTreeCtrl->GetParentItem(hItem);
	return pTreeCtrl->GetItemText(hItem);
}

