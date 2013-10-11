// ClyDlgDeviceSetup.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "ClyDlgDeviceSetup.h"
#include "XListCtrl.h"
#include "MainFrm.h" 
// CClyDlgDeviceSetup dialog

IMPLEMENT_DYNAMIC(CClyDlgDeviceSetup, CDlgInstrumentSet)

CClyDlgDeviceSetup::CClyDlgDeviceSetup(CWnd* pParent /*=NULL*/)
	: CDlgInstrumentSet(CClyDlgDeviceSetup::IDD, pParent)
{
	Rdb = new CClyRDB();
}

CClyDlgDeviceSetup::~CClyDlgDeviceSetup()
{
	delete Rdb;
}

void CClyDlgDeviceSetup::DoDataExchange(CDataExchange* pDX)
{
	CDlgInstrumentSet::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClyDlgDeviceSetup, CDlgInstrumentSet)  
END_MESSAGE_MAP()



void CClyDlgDeviceSetup::UpdateWinText(void)
{
	CString str;
	CClyRDB *pRdb = (CClyRDB*)Rdb;
	CClyRDB::CAttrib *pAtt = &pRdb->attrib;

	GetDlgItem(IDC_DEVICE_NAME) ->SetWindowText(pRdb->m_szName);
	GetDlgItem(IDC_DEVICE_TYPE)->SetWindowText(pRdb->m_szType);
	
	SetDlgItemInt(IDC_EDIT_ID,pRdb->m_bID); 
	SetDlgItemInt(IDC_DEVICE_PERIOD,pRdb->m_lScanPriod_mS);   
	SetDlgItemInt(IDC_DEVICE_COMM,pRdb->m_sComPort);

	SetDlgItemInt(IDC_EDIT_BEEPTIME,pAtt->m_sBeepTime);
	
 	CComboBox *pComBox = ((CComboBox*)GetDlgItem(IDC_COMBO_SAMPLE_TYPE));
	pComBox->SetCurSel(pComBox->FindString(-1,pAtt->m_szSampleType));
	SetDlgItemInt(IDC_EDIT_PREDECOMPOSE1,pAtt->m_sPreDecomposePos1);
	SetDlgItemInt(IDC_EDIT_PREDECOMPOSE2,pAtt->m_sPreDecomposePos2);
	SetDlgItemInt(IDC_EDIT_DECOMPOSE,pAtt->m_sDecomposePos);
	SetDlgItemInt(IDC_EDIT_PREDECOMPOSE1_TIME,pAtt->m_sPreDecomposeTim1);
	SetDlgItemInt(IDC_EDIT_PREDECOMPOSE2_TIME,pAtt->m_sPreDecomposeTim2);
	SetDlgItemInt(IDC_EDIT_DECOMPOSE_DELAY_TIME,pAtt->m_sEndDecomposDelay);
	//////////////////////////////////////////////////////////////////////
	SetDlgItemInt(IDC_EDIT_POS1_TIM,pAtt->m_sPos1TimeOutTime);
	SetDlgItemInt(IDC_EDIT_POSN_TIM,pAtt->m_sPosNTimeOutTime);
	((CButton*)GetDlgItem(IDC_CHECK_AUTO)) ->SetCheck(pAtt->m_bAutoCly) ; 
	SetDlgItemInt(IDC_EDIT_START_DJ,pAtt->m_fStartDJVol);
	SetDlgItemInt(IDC_EDIT_END_DJ,pAtt->m_fEndDJVol);
	str.Format(_T("%5.3f"),pAtt->m_sDJCoff);
	GetDlgItem(IDC_EDIT_DJ_COFF) ->SetWindowText(str); 
	///////////////////////////////////////////////////////////////////////
	SetDlgItemInt(IDC_EDIT_DEST_TEMP,pAtt->m_sDestTemp);
	str.Format(_T("%5.3f"),pAtt->m_dCoff);
	GetDlgItem(IDC_EDIT_TEMP_COFF) ->SetWindowText(str); 
		//LryRdb.m_szAlgorithm) ;
	((CButton*)GetDlgItem(IDC_CHECK_FIX)) ->SetCheck(pAtt->m_bFix) ;
	

	/* 
	FLOAT			m_fStartDJVol;			//开始电解电压
	FLOAT			m_fEndDJVol;			//结束电解电压 
	double			m_sDJCoff;				//电解系数	 
	BOOL			m_bFix;					//修正
	BOOL			m_bAutoPrint;			//自动打印
	*/
}

void CClyDlgDeviceSetup::GetParamFromDlg(void)
{
	CString str;
	CClyRDB *pRdb = (CClyRDB*)Rdb;
	CClyRDB::CAttrib *pAtt = &pRdb->attrib;

	GetDlgItem(IDC_DEVICE_NAME) ->GetWindowText(pRdb->m_szName);
	GetDlgItem(IDC_DEVICE_TYPE)->GetWindowText(pRdb->m_szType);

	pRdb->m_bID				= GetDlgItemInt(IDC_EDIT_ID); 
	pRdb->m_lScanPriod_mS	= GetDlgItemInt(IDC_DEVICE_PERIOD);   
	pRdb->m_sComPort		= GetDlgItemInt(IDC_DEVICE_COMM);

	pAtt->m_sBeepTime		= GetDlgItemInt(IDC_EDIT_BEEPTIME );
	
	CComboBox *pComBox = ((CComboBox*)GetDlgItem(IDC_COMBO_SAMPLE_TYPE));
	pComBox->GetWindowText(pAtt->m_szSampleType); 
	pAtt->m_sPreDecomposePos1	= GetDlgItemInt(IDC_EDIT_PREDECOMPOSE1 );
	pAtt->m_sPreDecomposePos2	= GetDlgItemInt(IDC_EDIT_PREDECOMPOSE2 );
	pAtt->m_sDecomposePos		= GetDlgItemInt(IDC_EDIT_DECOMPOSE );
	pAtt->m_sPreDecomposeTim1	= GetDlgItemInt(IDC_EDIT_PREDECOMPOSE1_TIME );
	pAtt->m_sPreDecomposeTim2	= GetDlgItemInt(IDC_EDIT_PREDECOMPOSE2_TIME );
	pAtt->m_sEndDecomposDelay	= GetDlgItemInt(IDC_EDIT_DECOMPOSE_DELAY_TIME );

	//////////////////////////////////////////////////////////////////////
	pAtt->m_sPos1TimeOutTime	= GetDlgItemInt(IDC_EDIT_POS1_TIM);
	pAtt->m_sPosNTimeOutTime	= GetDlgItemInt(IDC_EDIT_POSN_TIM );
	pAtt->m_bAutoCly			= ((CButton*)GetDlgItem(IDC_CHECK_AUTO)) ->GetCheck() ; 
	GetDlgItem(IDC_EDIT_START_DJ) ->GetWindowText(str);  
	pAtt->m_fStartDJVol			=  _tcstod(str,NULL);
	GetDlgItem(IDC_EDIT_END_DJ) ->GetWindowText(str);  
	pAtt->m_fEndDJVol			=  _tcstod(str,NULL);
	//pAtt->m_fStartDJVol		= GetDlgItemInt(IDC_EDIT_START_DJ );
	//pAtt->m_fEndDJVol			= GetDlgItemInt(IDC_EDIT_END_DJ );
	GetDlgItem(IDC_EDIT_DJ_COFF) ->GetWindowText(str);  
	pAtt->m_sDJCoff				=  _tcstod(str,NULL);
	///////////////////////////////////////////////////////////////////////
	pAtt->m_sDestTemp			= GetDlgItemInt(IDC_EDIT_DEST_TEMP );
	
	GetDlgItem(IDC_EDIT_TEMP_COFF) ->GetWindowText(str); 
	pAtt->m_dCoff				= _tcstod(str,NULL);
	
	pAtt->m_bFix				= ((CButton*)GetDlgItem(IDC_CHECK_FIX)) ->GetCheck() ;
	
} 

