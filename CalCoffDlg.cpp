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
	m_Listctrl.InsertColumn(0,_T("校正后干基硫"),LVCFMT_CENTER,100 ,65);
	m_Listctrl.InsertColumn(0,_T("标准干基硫"),LVCFMT_CENTER,80 ,35);
	m_Listctrl.InsertColumn(0,_T("实测干基硫"),LVCFMT_CENTER,80 ,35);
	m_Listctrl.InsertColumn(0,_T("样品编号"),LVCFMT_CENTER,80 ,35);
	m_Listctrl.InsertColumn(0,_T("序号"),LVCFMT_CENTER,40 ,45);
	////////////////////////////////////////////////////////////////////////////
	int iSelectCount	= 0;
	int   nIndex ;
	int		i			= 0;
	iSelectCount		= pParentListCtrl->GetSelectedCount();
	pStd				= new double[iSelectCount];            //做实验的std
	pTrueStd			= new double[iSelectCount];				//标煤的Std
	pCalStd				= new double[iSelectCount];	
	m_Listctrl.SetItemCount(iSelectCount);
	POSITION   pos		= pParentListCtrl->GetFirstSelectedItemPosition(); 
	CString str;
	while   (pos) 
	{ 
		
		nIndex =   pParentListCtrl->GetNextSelectedItem(pos); 
		str.Format(_T("%d"),i+1);
		m_Listctrl.InsertItem(i,str); 
		str		= pParentListView->GetItemText(nIndex,CString(_T("样品编号")));
		m_Listctrl.SetItemText(i,1,str);
		str		= pParentListView->GetItemText(nIndex,CString(_T("全硫Std")));
		pStd[i]	= _tcstod(str,NULL);
		m_Listctrl.SetItemText(i,2,str);
		m_Listctrl.SetEdit(i, 3);
		//int GetItemText( int nItem, int nSubItem, LPTSTR lpszText, int nLen ) const;
		//这里获取你想要的列的文字（行为nIndex），然后加到CStriing中即可。 
		i++;
	} 

 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCalCoffDlg::OnBnClickedCalCoff()
{
	//样品编号 
	int iCount = m_Listctrl.GetItemCount();
	CString str;
	for(int i=0;i<iCount;i++)
	{
		str		= m_Listctrl.GetItemText(i,3);
		pTrueStd[i]	= _tcstod(str,NULL);
		
	}
	if(iCount>1)
	{
		GetALinearRegressionEquation(pStd,pTrueStd,iCount,m_slope,m_intercept,m_R );//斜率 截距
		str.Format(_T("斜率:%f"),m_slope);
		GetDlgItem(IDC_STATIC_SLOPE)->SetWindowText(str);
		str.Format(_T("截距:%f"),m_intercept);
		GetDlgItem(IDC_STATIC_INTERCEPT)->SetWindowText(str);
		str.Format(_T("R=%f"),m_R);
		GetDlgItem(IDC_STATIC_R)->SetWindowText(str);
		SetDlgItemText(IDC_STATIC_RESULT,m_R>0.98?_T("合格"):_T("不合格"));
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
		MessageBox(_T("记录必须大于两条"),_T("提示"),MB_OK+MB_ICONWARNING);
	}
}

void CCalCoffDlg::OnBnClickedSaveCoff()
{ 
	_RecordsetPtr Rs;
	HRESULT hr;
	CString strSQL ;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strDev = _T("1#量热仪");
	int index = pFrame->m_wndComboBoxDevice->GetCurSel();
	pFrame->m_wndComboBoxDevice->GetLBText( index, strDev);
	////////////////////////////////////////////
	Rs.CreateInstance("ADODB.Recordset");
	
	strSQL.Format(_T("SELECT 截距A, 斜率B FROM 测硫仪 WHERE (名称 = '%s')"),strDev);
		

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
		Rs->PutCollect("截距A",m_intercept);//,
		Rs->PutCollect("斜率B",m_slope);
		Rs->Update();
	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
		MessageBox(_T("请将查询设备修改为\n你要修改系数的仪器"),_T("警告"),MB_ICONWARNING+MB_OK);

		//dump_com_error(e);
	} 
}
