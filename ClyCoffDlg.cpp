// ClyCoffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "ClyCoffDlg.h"


// CClyCoffDlg dialog

IMPLEMENT_DYNAMIC(CClyCoffDlg, CDialog)

CClyCoffDlg::CClyCoffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClyCoffDlg::IDD, pParent)
{

}

CClyCoffDlg::~CClyCoffDlg()
{
}

void CClyCoffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COFF, ListCtrl);
}


BEGIN_MESSAGE_MAP(CClyCoffDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_COFF, &CClyCoffDlg::OnNMCustomdrawListCoff)
END_MESSAGE_MAP()


// CClyCoffDlg message handlers

void CClyCoffDlg::SQL(void)
{ 
	HRESULT hr= S_OK;
	//CString strSQL;
	try
	{ 
		if(pRsDev->State!=adStateClosed)
			pRsDev->Close(); 
		strSQL.Format(_T("select * from 测硫仪系数 where 设备名称='%s' \
			order by 序号"),GetCurrentSeleString()); 
		hr = pRsDev->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
		GetAllFieldName(pRsDev,&ColNameArray);
		/////////////////////////////////////////////////////////////
		AddListHeader();
		/////////////////////////////////////////////////////////////////
		AddList();
		//bResult = UpdateParamFromDB(); 
	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
		dump_com_error(e);
	}   
}

void CClyCoffDlg::AddList(void)
{
	_variant_t  str;
	CString strData;
	int i;
	if(!pRsDev->adoEOF)
		pRsDev->MoveLast();

	while(!pRsDev->BOF)
	{		
		for(i=0;i<pRsDev->Fields->Count;i++)
		{
			str = pRsDev->GetCollect((_bstr_t)ColNameArray.GetAt(i));
			strData = VariantToStr(str); 
			if(i==0)
				ListCtrl.InsertItem(0,strData);
			else
				ListCtrl.SetItemText(0,i,strData);
		}
		pRsDev->MovePrevious();
	}  	
}

BOOL CClyCoffDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	pRsDev.CreateInstance("ADODB.Recordset");  
	SQL(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CClyCoffDlg::AddListHeader(void)
{
	int iStrLen,i;
	ListCtrl.DeleteAllItems();
	//清除所有标题
	do
	{
		ListCtrl.DeleteColumn(0);
	}
	while(ListCtrl.GetHeaderCtrl()->GetItemCount()>0); 
	//写字段标题
	for(  i=ColNameArray.GetSize()-1;i>=0;i--)
	{
		CDC *pDC = GetDC();
		iStrLen	= pDC->GetTextExtent(ColNameArray.GetAt(i)).cx*2;				
		ListCtrl.InsertColumn(0,ColNameArray.GetAt(i),LVCFMT_CENTER,iStrLen ,10);
		ReleaseDC(pDC);	
	}
}

void CClyCoffDlg::OnNMCustomdrawListCoff(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR); 
	*pResult = 0;
}
