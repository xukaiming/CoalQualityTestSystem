// GyfxyDlgDeviceSetup.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyDlgDeviceSetup.h"


// CGyfxyDlgDeviceSetup dialog

IMPLEMENT_DYNAMIC(CGyfxyDlgDeviceSetup, CDlgInstrumentSet)

CGyfxyDlgDeviceSetup::CGyfxyDlgDeviceSetup(CWnd* pParent /*=NULL*/)
	: CDlgInstrumentSet(CGyfxyDlgDeviceSetup::IDD, pParent)
{ 
	Rdb = new CGyfxyRDB_G5200();
	m_bInitListCtl = FALSE;
}

CGyfxyDlgDeviceSetup::~CGyfxyDlgDeviceSetup()
{ 
	delete Rdb;
}

void CGyfxyDlgDeviceSetup::DoDataExchange(CDataExchange* pDX)
{
	CDlgInstrumentSet::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ATTR, mListAttrib);
}


BEGIN_MESSAGE_MAP(CGyfxyDlgDeviceSetup, CDlgInstrumentSet)
END_MESSAGE_MAP()


// CGyfxyDlgDeviceSetup message handlers

void CGyfxyDlgDeviceSetup::UpdateWinText(void)
{

	if (!m_bInitListCtl)
	{
		 InitList(); 
		 m_bInitListCtl = true;
	}
	CString colName;
	CString strColValue;
	COleVariant colValue;
	int rowCnt = mListAttrib.GetItemCount();
	for (int i=0;i<rowCnt;i++)
	{ 
		colName = mListAttrib.GetItemText(i,0);
		colValue= Rdb->pRsDev->GetCollect((_bstr_t)colName); 
		if(colValue.vt==VT_R4)
			strColValue.Format(_T("%0.4f"),V_R4(&colValue));
		else if(colValue.vt==VT_BOOL) 
		{
			strColValue=_T("");  
			int bValue = V_BOOL(&colValue);
			bValue=bValue==0?0:1;
			mListAttrib.SetItemCheckedState(i,1,bValue);
		}
		else
			strColValue = VariantToStr(colValue);
		mListAttrib.SetItemText(i,1,strColValue);
	} 
	colName  =_T("名称");
	colValue= Rdb->pRsDev->GetCollect((_bstr_t)colName); 
	SetDlgItemText(IDC_DEVICE_NAME,CString(colValue)); 
	//
	colName  =_T("型号");
	colValue= Rdb->pRsDev->GetCollect((_bstr_t)colName);
	SetDlgItemText(IDC_DEVICE_TYPE,CString(colValue));
}

void CGyfxyDlgDeviceSetup::GetParamFromDlg(void)
{  
	CGyfxyRDB_G5200 *pRdb = (CGyfxyRDB_G5200*)Rdb;
	GetDlgItem(IDC_DEVICE_NAME) ->GetWindowText(pRdb->m_szName);
	GetDlgItem(IDC_DEVICE_TYPE)->GetWindowText(pRdb->m_szType);
	///////////////////////////////////////////////////////////////// 
	CString colName;
	CString strColValue;
	COleVariant colValue;
	int iCHeckBox;
	int rowCnt = mListAttrib.GetItemCount();
	try
	{	
		for (int i=0;i<rowCnt;i++)
		{  
			iCHeckBox = mListAttrib.GetCheckbox(i,1);//=-1不存在
			colName = mListAttrib.GetItemText(i,0);
			strColValue	=mListAttrib.GetItemText(i,1);
			colValue=(iCHeckBox==-1)?_variant_t(strColValue):VARIANT_BOOL(iCHeckBox); 
			Rdb->pRsDev->PutCollect((_bstr_t)colName,colValue);  
		}  
		///////////////////////////////////////////////////////////////////
		colName = _T("型号");
		Rdb->pRsDev->PutCollect((_bstr_t)colName,_variant_t(pRdb->m_szType));  
		colName = _T("名称");	
		Rdb->pRsDev->PutCollect((_bstr_t)colName,_variant_t(pRdb->m_szName)) ; 
	}
	catch (_com_error & e)
	{
		dump_com_error(e);	
	} 

}

BOOL CGyfxyDlgDeviceSetup::OnInitDialog()
{
	CDlgInstrumentSet::OnInitDialog();


	mListAttrib.SetExtendedStyle(mListAttrib.GetExtendedStyle()
		|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP
		|LVS_EX_GRIDLINES|LVS_EX_BORDERSELECT );//
	 
	CFont *pFont = mListAttrib.GetFont();
	::ZeroMemory(&lf,   sizeof(lf)); 
	pFont->GetLogFont(&lf);
	lf.lfHeight   =   -16;  
	font.CreateFontIndirect(&lf);   
	mListAttrib.SetFont(&font); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGyfxyDlgDeviceSetup::InitList(void)
{ 
	GetAllFieldName(Rdb->pRsDev,&ColNameArray);

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_DEVICE_TYPE);
	pCombo->AddString(_T("TF_GYFXY2012"));
	pCombo->AddString(_T("TF_GYFXY2013"));

	CRect rect;
	mListAttrib.GetClientRect(rect); 
	int iRollWidth = GetSystemMetrics(SM_CYVSCROLL );
	mListAttrib.InsertColumn(0,_T("属性"),LVCFMT_CENTER,rect.Width()*(1-0.618f) ,0);  
	mListAttrib.InsertColumn(0,_T("参数"),LVCFMT_CENTER,rect.Width()*0.618f-iRollWidth ,0);  
 
	for(int i=ColNameArray.GetSize()-1;i>=0;i--)
	{
		//删除 名称   型号
		if((ColNameArray[i]==_T("名称"))
			||(ColNameArray[i]==_T("型号"))
			||(ColNameArray[i]==_T("实验顺序")) 
			)
			continue; 

		mListAttrib.InsertItem(0,ColNameArray[i]);
		if((ColNameArray[i]==_T("修正"))
			||(ColNameArray[i]==_T("自动打印")) 
			||(ColNameArray[i]==_T("快速称样")) 
			)
		{
			mListAttrib.SetCheckbox(0,1,0); 
		}
		else if((ColNameArray[i]==_T("算法")))
		{

			strAlgorithm.RemoveAll();
			strAlgorithm.Add(_T("快速"));
			strAlgorithm.Add(_T("国标"));
			mListAttrib.SetComboBox(0, 1, 
				TRUE,		// enable combobox
				&strAlgorithm,		// pointer to persistent CStringArray
				2,			// size of dropdown
				0,			// initial combo selection
				TRUE);		// sort CStringArray
		}
		else
		{
			mListAttrib.SetEdit(0,1);
		}
	}
}
