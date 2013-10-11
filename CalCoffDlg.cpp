// CalCoffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "CalCoffDlg.h" 
#include "MainFrm.h"
// CCalCoffDlg dialog

IMPLEMENT_DYNAMIC(CCalCoffDlg, CDialog)

CCalCoffDlg::CCalCoffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalCoffDlg::IDD, pParent)
{

	pStd		= NULL;
	pTrueStd	= NULL;
	pCalStd		= NULL;
}

CCalCoffDlg::~CCalCoffDlg()
{
	delete []pStd;
	delete []pTrueStd;
	delete []pCalStd;
}

void CCalCoffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TAB, m_Listctrl);
}


BEGIN_MESSAGE_MAP(CCalCoffDlg, CDialog)
	ON_BN_CLICKED(IDC_CAL_COFF, &CCalCoffDlg::OnBnClickedCalCoff)
	ON_BN_CLICKED(IDC_SAVE_COFF, &CCalCoffDlg::OnBnClickedSaveCoff)
END_MESSAGE_MAP()


// CCalCoffDlg message handlers

BOOL CCalCoffDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	DWORD ExStyle = m_Listctrl.GetExtendedStyle() ;
	m_Listctrl.SetExtendedStyle(ExStyle| LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//
	m_Listctrl.EnableToolTips(TRUE); 
	/////////////////////////////////////////////////////////////////////// 
	m_Listctrl.InsertColumn(0,_T("У����ɻ���"),LVCFMT_CENTER,100 ,65);
	m_Listctrl.InsertColumn(0,_T("��׼�ɻ���"),LVCFMT_CENTER,80 ,35);
	m_Listctrl.InsertColumn(0,_T("ʵ��ɻ���"),LVCFMT_CENTER,80 ,35);
	m_Listctrl.InsertColumn(0,_T("��Ʒ���"),LVCFMT_CENTER,80 ,35);
	m_Listctrl.InsertColumn(0,_T("���"),LVCFMT_CENTER,40 ,45);
	////////////////////////////////////////////////////////////////////////////
	int iSelectCount	= 0;
	int   nIndex ;
	int		i			= 0;
	iSelectCount		= pParentListCtrl->GetSelectedCount();
	pStd				= new double[iSelectCount];            //��ʵ���std
	pTrueStd			= new double[iSelectCount];				//��ú��Std
	pCalStd				= new double[iSelectCount];	
	m_Listctrl.SetItemCount(iSelectCount);
	POSITION   pos		= pParentListCtrl->GetFirstSelectedItemPosition(); 
	CString str;
	while   (pos) 
	{ 
		
		nIndex =   pParentListCtrl->GetNextSelectedItem(pos); 
		str.Format(_T("%d"),i+1);
		m_Listctrl.InsertItem(i,str); 
		str		= pParentListView->GetItemText(nIndex,CString(_T("��Ʒ���")));
		m_Listctrl.SetItemText(i,1,str);
		str		= pParentListView->GetItemText(nIndex,CString(_T("ȫ��Std")));
		pStd[i]	= _tcstod(str,NULL);
		m_Listctrl.SetItemText(i,2,str);
		m_Listctrl.SetEdit(i, 3);
		//int GetItemText( int nItem, int nSubItem, LPTSTR lpszText, int nLen ) const;
		//�����ȡ����Ҫ���е����֣���ΪnIndex����Ȼ��ӵ�CStriing�м��ɡ� 
		i++;
	} 

 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCalCoffDlg::OnBnClickedCalCoff()
{
	//��Ʒ��� 
	int iCount = m_Listctrl.GetItemCount();
	CString str;
	for(int i=0;i<iCount;i++)
	{
		str		= m_Listctrl.GetItemText(i,3);
		pTrueStd[i]	= _tcstod(str,NULL);
		
	}
	if(iCount>1)
	{
		GetALinearRegressionEquation(pStd,pTrueStd,iCount,m_slope,m_intercept,m_R );//б�� �ؾ�
		str.Format(_T("б��:%f"),m_slope);
		GetDlgItem(IDC_STATIC_SLOPE)->SetWindowText(str);
		str.Format(_T("�ؾ�:%f"),m_intercept);
		GetDlgItem(IDC_STATIC_INTERCEPT)->SetWindowText(str);
		str.Format(_T("R=%f"),m_R);
		GetDlgItem(IDC_STATIC_R)->SetWindowText(str);
		SetDlgItemText(IDC_STATIC_RESULT,m_R>0.98?_T("�ϸ�"):_T("���ϸ�"));
		GetDlgItem(IDC_SAVE_COFF)->EnableWindow(m_R>0.98?TRUE:FALSE);
		LinearRegressionEquation(pStd,iCount,m_slope,m_intercept,pCalStd);
		for(int i=0;i<iCount;i++)
		{
			str.Format(_T("%f"),pCalStd[i]);
			m_Listctrl.SetItemText(i,4,str);  
		}
	}
	else
	{
		MessageBox(_T("��¼�����������"),_T("��ʾ"),MB_OK+MB_ICONWARNING);
	}
}

void CCalCoffDlg::OnBnClickedSaveCoff()
{ 
	_RecordsetPtr Rs;
	HRESULT hr;
	CString strSQL ;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strDev = _T("1#������");
	int index = pFrame->m_wndComboBoxDevice->GetCurSel();
	pFrame->m_wndComboBoxDevice->GetLBText( index, strDev);
	////////////////////////////////////////////
	Rs.CreateInstance("ADODB.Recordset");
	
	strSQL.Format(_T("SELECT �ؾ�A, б��B FROM ������ WHERE (���� = '%s')"),strDev);
		

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
		Rs->PutCollect("�ؾ�A",m_intercept);//,
		Rs->PutCollect("б��B",m_slope);
		Rs->Update();
	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
		MessageBox(_T("�뽫��ѯ�豸�޸�Ϊ\n��Ҫ�޸�ϵ��������"),_T("����"),MB_ICONWARNING+MB_OK);

		//dump_com_error(e);
	} 
}
