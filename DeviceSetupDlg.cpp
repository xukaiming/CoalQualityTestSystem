// DeviceSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "DeviceSetupDlg.h"
#include "MainFrm.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceSetupDlg dialog


CDeviceSetupDlg::CDeviceSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceSetupDlg)
	m_RowNO = 1;
	//}}AFX_DATA_INIT
}


void CDeviceSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceSetupDlg)
	DDX_Text(pDX, IDC_DEVICE_ROW_NO, m_RowNO);
	DDV_MinMaxUInt(pDX, m_RowNO, 1, 254);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CDeviceSetupDlg)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_DEVICE_MODIFY, OnDeviceModify)
	ON_BN_CLICKED(IDC_DEVICE_ADD, OnDeviceAdd)
	ON_BN_CLICKED(IDC_DEVICE_DEL, OnDeviceDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceSetupDlg message handlers

BOOL CDeviceSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog(); 
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_ATTRIB),FALSE);
	//IDI_ICON_ATTRIB
	((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,2);
	 
	BOOL bResult = TRUE;	
	pRsDev.CreateInstance("ADODB.Recordset");  
	SQL();
	try
	{
		MoveDBPtr();
		RefreshRDB();
		UpdateWinText();
	}
	catch (_com_error &e) {
		dump_com_error(e);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeviceSetupDlg::RefreshRDB()
{
	LRYRDB *pLryRdb = &LryRdb ;
	pLryRdb->m_bAutoPrint		= VariantToStr(pRsDev->Fields->GetItem(_T("�Զ���ӡ"))->Value)==_T("��")?TRUE:FALSE;
	pLryRdb->m_bFix				= VariantToStr(pRsDev->Fields->GetItem(_T("����"))->Value)==_T("��")?TRUE:FALSE;	
	pLryRdb->m_fCoffa			= pRsDev->Fields->GetItem(_T("ϵ��a"))->Value.fltVal ;	
	pLryRdb->m_fCoffb			= pRsDev->Fields->GetItem(_T("ϵ��b"))->Value.fltVal ;
	pLryRdb->m_fCoffc			= pRsDev->Fields->GetItem(_T("ϵ��c"))->Value.fltVal ;
	pLryRdb->m_fCoffd			= pRsDev->Fields->GetItem(_T("ϵ��d"))->Value.fltVal ;
	pLryRdb->m_fCoffe			= pRsDev->Fields->GetItem(_T("ϵ��e"))->Value.fltVal ;
	pLryRdb->m_fCofff			= pRsDev->Fields->GetItem(_T("ϵ��f"))->Value.fltVal ;
	pLryRdb->m_fCoffg			= pRsDev->Fields->GetItem(_T("ϵ��g"))->Value.fltVal ;
	pLryRdb->m_fCoffh			= pRsDev->Fields->GetItem(_T("ϵ��h"))->Value.fltVal ;
	pLryRdb->m_fEndCheckTemp					= pRsDev->Fields->GetItem(_T("�յ��ж��²�"))->Value.fltVal;//�յ��ж��²�
	pLryRdb->m_fFireOKtemp						= pRsDev->Fields->GetItem(_T("���ɹ��²�"))->Value.fltVal; //���ɹ��ж�
	pLryRdb->m_fR0								= pRsDev->Fields->GetItem(_T("R0"))->Value;
	pLryRdb->m_lHeatCapacityErrorCheckPoint		= _tcstol( VariantToStr(pRsDev->Fields->GetItem(_T("�����������е�"))->Value),NULL,10  );//�������жϵ�
	pLryRdb->m_lHeatCapacityRSDErrorCheckPoint	= pRsDev->Fields->GetItem(_T("��������Ա�׼��"))->Value.fltVal;							//��������Ա�׼���жϵ�
	pLryRdb->m_lScanPriod_mS					= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("ɨ������(mS)"))->Value),NULL,10);     //ɨ������
	pLryRdb->m_sadditive1HeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("1#�������ֵ"))->Value),NULL,10);	 //1���������ֵ
	pLryRdb->m_sadditive2HeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("2#�������ֵ"))->Value),NULL,10);     //2���������ֵ
	pLryRdb->m_sBeepTime						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("����ʱ��(mS)"))->Value),NULL,10);
	pLryRdb->m_sBenzoicAcidHeatValue			= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��������ֵ"))->Value),NULL,10);;		//��������ֵ
	pLryRdb->m_sComPort							= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���пں�"))->Value),NULL,10);;
	pLryRdb->m_sDuplicateSamplesCheckPoint		= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("ƽ�����е�"))->Value),NULL,10);;		//ƽ�����е�
	pLryRdb->m_sEndtJiaoBanTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("ĩ�ڽ���ʱ��(��)"))->Value),NULL,10);	//ĩ��ʱ��
	pLryRdb->m_sFillWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��Ͱ��ˮʱ��(S)"))->Value),NULL,10  );;	//��ˮʱ��
	pLryRdb->m_sFireTime						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���ʱ��(S)"))->Value),NULL,10  );    //���ʱ��
	pLryRdb->m_sHotWireHeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���˿��ֵ"))->Value),NULL,10  );;//���˿��ֵ
	pLryRdb->m_bID								= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��ʶ"))->Value),NULL,10  );;
	pLryRdb->m_sOutWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��Ͱ��ˮʱ��(S)"))->Value),NULL,10  );; ;//��ˮʱ��
	pLryRdb->m_sSenserPowerONTime				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("̽ͷ�ӵ�ʱ��(S)"))->Value),NULL,10  );;//̽ͷ�ϵ�ʱ��
	pLryRdb->m_sStartJiaoBanTime1				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���ڽ���ʱ��1(��)"))->Value),NULL,10  );; //Ԥ�ɰ�ʱ��1
	pLryRdb->m_sStartJiaoBanTime2				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���ڽ���ʱ��2(��)"))->Value),NULL,10  );;//���ڽ���
	pLryRdb->m_sWaterHeight						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��Ͱˮ����"))->Value),NULL,10  );;
	pLryRdb->m_szAlgorithm						= VariantToStr(pRsDev->Fields->GetItem("�㷨")->Value);
	pLryRdb->m_szName							= VariantToStr(pRsDev->Fields->GetItem("����")->Value);
	pLryRdb->m_szType							= VariantToStr(pRsDev->Fields->GetItem("�ͺ�")->Value);
	pLryRdb->m_HeatCapacity						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A����������")->Value),NULL,10 );
	pLryRdb->m_AOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A����������")->Value),NULL,10 );//A����������
	pLryRdb->m_BOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("B����������")->Value),NULL,10 );//B����������		

}
 
void CDeviceSetupDlg::OnDestroy() 
{
	CDialog::OnDestroy(); 
	if(pRsDev->State==adStateOpen)
		pRsDev->Close();	
}

void CDeviceSetupDlg::UpdateWinText()
{ 
	CString str;
	 
	GetDlgItem(IDC_DEVICE_NAME)->SetWindowText(LryRdb.m_szName) ;
	GetDlgItem(IDC_DEVICE_TYPE)->SetWindowText(LryRdb.m_szType) ;   
	SetDlgItemInt(IDC_DEVICE_ID,LryRdb.m_bID); 
	SetDlgItemInt(IDC_DEVICE_COMM ,LryRdb.m_sComPort)  ; 
	SetDlgItemInt(IDC_DEVICE_PERIOD , LryRdb.m_lScanPriod_mS  );
	str.Format(_T("%5.3f"),LryRdb.m_fR0);
	GetDlgItem(IDC_DEVICE_R0 ) ->SetWindowText(str) ;
	SetDlgItemInt(IDC_DEVICE_ShuiZhiLiang,LryRdb.m_sWaterHeight);
	SetDlgItemInt(IDC_DEVICE_JinShui,LryRdb.m_sFillWaterTime);
	SetDlgItemInt(IDC_DEVICE_FangShui,LryRdb.m_sOutWaterTime);
	SetDlgItemInt(IDC_DEVICE_ChuQiJiaoBan,LryRdb.m_sStartJiaoBanTime1) ;  
	SetDlgItemInt(IDC_DEVICE_ChuQiJiaoBan2,LryRdb.m_sStartJiaoBanTime2) ; 
	SetDlgItemInt(IDC_DEVICE_MoQiJiaoBan,LryRdb.m_sEndtJiaoBanTime) ;  
    SetDlgItemInt(IDC_DEVICE_DianHuo ,LryRdb.m_sFireTime) ;
    SetDlgItemInt(IDC_DEVICE_JiaDian ,LryRdb.m_sSenserPowerONTime) ;
    SetDlgItemInt(IDC_DEVICE_FenMing , LryRdb.m_sBeepTime) ;

	str.Format(_T("%5.4f"),LryRdb.m_fFireOKtemp);
	GetDlgItem(IDC_DEVICE_DianHuoWenCha ) ->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fEndCheckTemp);
    GetDlgItem(IDC_DEVICE_ZhongDianWenCha) ->SetWindowText(str) ; 
	/*
	str.Format(_T("%5.4f"),LryRdb.m_fCoffa);
	GetDlgItem(IDC_DEVICE_KA)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffb);
	GetDlgItem(IDC_DEVICE_KB)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffc);
	GetDlgItem(IDC_DEVICE_KC)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffd);
	GetDlgItem(IDC_DEVICE_KD)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffe);
	GetDlgItem(IDC_DEVICE_KE)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCofff);
	GetDlgItem(IDC_DEVICE_KF)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffg);
	GetDlgItem(IDC_DEVICE_KG)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffh);
	GetDlgItem(IDC_DEVICE_KH)->SetWindowText(str) ; 	
	*/
	str.Format(_T("%d"),LryRdb.m_sHotWireHeatValue);
	GetDlgItem(IDC_DEVICE_DianHouSiReZhi)->SetWindowText(str) ;
    SetDlgItemInt(IDC_DEVICE_BengJiaSuanReZhi,LryRdb.m_sBenzoicAcidHeatValue) ;
    SetDlgItemInt(IDC_DEVICE_TianJiaWuReZhi1,LryRdb.m_sadditive1HeatValue) ;
    SetDlgItemInt(IDC_DEVICE_TianJiaWuReZhi2,LryRdb.m_sadditive2HeatValue) ;
    SetDlgItemInt(IDC_DEVICE_PingXingYanPanDian,LryRdb.m_sDuplicateSamplesCheckPoint) ; 
	str.Format(_T("%5.4f"),LryRdb.m_lHeatCapacityRSDErrorCheckPoint);
    GetDlgItem(IDC_DEVICE_EJiChaPanDian) ->SetWindowText(str) ;
    GetDlgItem(IDC_DEVICE_MODE ) ->SetWindowText(LryRdb.m_szAlgorithm) ;
    ((CButton*)GetDlgItem(IDC_DEVICE_AUTOPRINT)) ->SetCheck(LryRdb.m_bAutoPrint) ;
    ((CButton*)GetDlgItem(IDC_DEVICE_XiuZheng)) ->SetCheck(LryRdb.m_bFix) ;
 
}

void CDeviceSetupDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString s;
 
	if (nSBCode == SB_ENDSCROLL) {
		return; // Reject spurious messages
	}  
	if(pScrollBar->GetDlgCtrlID()==IDC_DEVICE_SPIN)
	{
		try{ 
			pRsDev->Move( nPos-1,(long)adBookmarkFirst);  
			RefreshRDB();	
			UpdateWinText();
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}	
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDeviceSetupDlg::GetParamFromDlg()
{
	CString str;
	GetDlgItem(IDC_DEVICE_NAME)->GetWindowText(LryRdb.m_szName) ;
	GetDlgItem(IDC_DEVICE_TYPE)->GetWindowText(LryRdb.m_szType) ;   
	LryRdb.m_bID			= GetDlgItemInt(IDC_DEVICE_ID); 
	LryRdb.m_sComPort		= GetDlgItemInt(IDC_DEVICE_COMM )  ; 
	LryRdb.m_lScanPriod_mS	= GetDlgItemInt(IDC_DEVICE_PERIOD );
 
	GetDlgItem(IDC_DEVICE_R0 ) ->GetWindowText(str) ;
	LryRdb.m_fR0			= _tcstod(str,NULL);
	LryRdb.m_sWaterHeight	= GetDlgItemInt(IDC_DEVICE_ShuiZhiLiang);
	LryRdb.m_sFillWaterTime	= GetDlgItemInt(IDC_DEVICE_JinShui );
	LryRdb.m_sOutWaterTime	= GetDlgItemInt(IDC_DEVICE_FangShui );
	LryRdb.m_sStartJiaoBanTime1 = GetDlgItemInt(IDC_DEVICE_ChuQiJiaoBan ) ;  
	LryRdb.m_sStartJiaoBanTime2 = GetDlgItemInt(IDC_DEVICE_ChuQiJiaoBan2 ) ; 
	LryRdb.m_sEndtJiaoBanTime	= GetDlgItemInt(IDC_DEVICE_MoQiJiaoBan ) ;  
    LryRdb.m_sFireTime			= GetDlgItemInt(IDC_DEVICE_DianHuo  ) ;
    LryRdb.m_sSenserPowerONTime = GetDlgItemInt(IDC_DEVICE_JiaDian ) ;
    LryRdb.m_sBeepTime			= GetDlgItemInt(IDC_DEVICE_FenMing   ) ;
 
	GetDlgItem(IDC_DEVICE_DianHuoWenCha ) ->GetWindowText(str) ;
	LryRdb.m_fFireOKtemp	= _tcstod(str,NULL); 
    GetDlgItem(IDC_DEVICE_ZhongDianWenCha) ->GetWindowText(str) ; 
	LryRdb.m_fEndCheckTemp	= _tcstod(str,NULL);
	/*
	str.Format(_T("%5.4f"),LryRdb.m_fCoffa);
	GetDlgItem(IDC_DEVICE_KA)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffb);
	GetDlgItem(IDC_DEVICE_KB)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffc);
	GetDlgItem(IDC_DEVICE_KC)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffd);
	GetDlgItem(IDC_DEVICE_KD)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffe);
	GetDlgItem(IDC_DEVICE_KE)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCofff);
	GetDlgItem(IDC_DEVICE_KF)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffg);
	GetDlgItem(IDC_DEVICE_KG)->SetWindowText(str) ;
	str.Format(_T("%5.4f"),LryRdb.m_fCoffh);
	GetDlgItem(IDC_DEVICE_KH)->SetWindowText(str) ; 
	*/ 
	GetDlgItem(IDC_DEVICE_DianHouSiReZhi)->GetWindowText(str) ;
	LryRdb.m_sHotWireHeatValue		= _tcstod(str,NULL);
    LryRdb.m_sBenzoicAcidHeatValue	= GetDlgItemInt(IDC_DEVICE_BengJiaSuanReZhi) ;
    LryRdb.m_sadditive1HeatValue	= GetDlgItemInt(IDC_DEVICE_TianJiaWuReZhi1) ;
    LryRdb.m_sadditive2HeatValue	= GetDlgItemInt(IDC_DEVICE_TianJiaWuReZhi2) ;
    LryRdb.m_sDuplicateSamplesCheckPoint = GetDlgItemInt(IDC_DEVICE_PingXingYanPanDian ) ;  
    GetDlgItem(IDC_DEVICE_EJiChaPanDian) ->GetWindowText(str) ;
	LryRdb.m_lHeatCapacityRSDErrorCheckPoint = _tcstod(str,NULL);
    GetDlgItem(IDC_DEVICE_MODE ) ->GetWindowText(LryRdb.m_szAlgorithm) ;
	
    LryRdb.m_bAutoPrint = ((CButton*)GetDlgItem(IDC_DEVICE_AUTOPRINT)) ->GetCheck() ;
    LryRdb.m_bFix		= ((CButton*)GetDlgItem(IDC_DEVICE_XiuZheng)) ->GetCheck() ; 
}

void CDeviceSetupDlg::UpdateDB()
{
	LRYRDB *pLryRdb = &LryRdb ;
	//((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,pRsDev->RecordCount);
	pRsDev->PutCollect("�Զ���ӡ",_variant_t(pLryRdb->m_bAutoPrint?VARIANT_TRUE:VARIANT_FALSE));
	pRsDev->PutCollect("����",_variant_t(pLryRdb->m_bFix?VARIANT_TRUE:VARIANT_FALSE));	
	/*
	pRsDev->PutCollect("ϵ��a",_variant_t(pLryRdb->m_fCoffa );	
	pRsDev->PutCollect("ϵ��b",_variant_t(pLryRdb->m_fCoffb) ;
	pRsDev->PutCollect("ϵ��c",_variant_t(pLryRdb->m_fCoffc) ;
	pRsDev->PutCollect("ϵ��d",_variant_t(pLryRdb->m_fCoffd) ;
	pRsDev->PutCollect("ϵ��e",_variant_t(pLryRdb->m_fCoffe) ;
	pRsDev->PutCollect("ϵ��f",_variant_t(pLryRdb->m_fCofff) ;
	pRsDev->PutCollect("ϵ��g",_variant_t(pLryRdb->m_fCoffg) ;
	pRsDev->PutCollect("ϵ��h",_variant_t(pLryRdb->m_fCoffh) ;
	*/
	pRsDev->PutCollect("�յ��ж��²�",pLryRdb->m_fEndCheckTemp);//�յ��ж��²�
	pRsDev->PutCollect("���ɹ��²�",pLryRdb->m_fFireOKtemp); //���ɹ��ж�
	pRsDev->PutCollect("R0",pLryRdb->m_fR0); 
//	pRsDev->PutCollect("�����������е�",pLryRdb->m_lHeatCapacityErrorCheckPoint);//�������жϵ� 
	pRsDev->PutCollect("��������Ա�׼��",pLryRdb->m_lHeatCapacityRSDErrorCheckPoint);	 
	pRsDev->PutCollect("ɨ������(mS)",pLryRdb->m_lScanPriod_mS);     //ɨ������
	pRsDev->PutCollect("1#�������ֵ",pLryRdb->m_sadditive1HeatValue);	 //1���������ֵ
	pRsDev->PutCollect("2#�������ֵ",pLryRdb->m_sadditive2HeatValue);     //2���������ֵ
	pRsDev->PutCollect("����ʱ��(mS)",pLryRdb->m_sBeepTime);
	pRsDev->PutCollect("��������ֵ",pLryRdb->m_sBenzoicAcidHeatValue);;		//��������ֵ
	pRsDev->PutCollect("���пں�",pLryRdb->m_sComPort);;
	pRsDev->PutCollect("ƽ�����е�",pLryRdb->m_sDuplicateSamplesCheckPoint);;		//ƽ�����е�
	pRsDev->PutCollect("ĩ�ڽ���ʱ��(��)",pLryRdb->m_sEndtJiaoBanTime	);	//ĩ��ʱ��
	pRsDev->PutCollect("��Ͱ��ˮʱ��(S)",pLryRdb->m_sFillWaterTime );;	//��ˮʱ��
	pRsDev->PutCollect("���ʱ��(S)",pLryRdb->m_sFireTime  );    //���ʱ��
	pRsDev->PutCollect("���˿��ֵ",pLryRdb->m_sHotWireHeatValue  );;//���˿��ֵ
	pRsDev->PutCollect("��ʶ",pLryRdb->m_bID  );;
	pRsDev->PutCollect("��Ͱ��ˮʱ��(S)",pLryRdb->m_sOutWaterTime);; ;//��ˮʱ��
	pRsDev->PutCollect("̽ͷ�ӵ�ʱ��(S)" ,pLryRdb->m_sSenserPowerONTime);;//̽ͷ�ϵ�ʱ��
	pRsDev->PutCollect("���ڽ���ʱ��1(��)",pLryRdb->m_sStartJiaoBanTime1 );; //Ԥ�ɰ�ʱ��1
	pRsDev->PutCollect("���ڽ���ʱ��2(��)",pLryRdb->m_sStartJiaoBanTime2);;//���ڽ���
	pRsDev->PutCollect("��Ͱˮ����",pLryRdb->m_sWaterHeight);; 
	pRsDev->PutCollect("�㷨",_variant_t(pLryRdb->m_szAlgorithm!=""?pLryRdb->m_szAlgorithm:"��"));
	pRsDev->PutCollect("����",_variant_t(pLryRdb->m_szName));
	pRsDev->PutCollect("�ͺ�",_variant_t(pLryRdb->m_szType));
	pRsDev->PutCollect("A����������",pLryRdb->m_HeatCapacity	 );
	pRsDev->PutCollect("A����������",_variant_t(pLryRdb->m_AOxygenHeatCapacity)  );//A����������
	pRsDev->PutCollect("B����������",_variant_t(pLryRdb->m_AOxygenHeatCapacity) );//B����������		

}


void CDeviceSetupDlg::OnDeviceDel() 
{ 
	if(
		(theApp.UserInfo.bLogon() == TRUE)&&
		(IDOK == AfxMessageBox(_T("���豸��Ҫ��ɾ��\n\n��ȷ����?"),MB_OKCANCEL))
		)

	{
		try
		{ 
			pRsDev->Delete(adAffectCurrent);
			pRsDev->Update(); 
			((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);
			((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,pRsDev->RecordCount);
			//UDN_DELTAPOS 
			MessageBox(_T("���豸��ɾ��"),_T("��ʾ"),MB_ICONINFORMATION+MB_OK);
		}
		catch (_com_error &e) {
			pRsDev->CancelUpdate();
			dump_com_error(e);
		}	
	}
}

void CDeviceSetupDlg::OnDeviceAdd() 
{
	try
	{ 		
		pRsDev->AddNew();
		GetParamFromDlg();
		UpdateDB();		
		pRsDev->Update();	
		//pRsDev->MoveNext();
		((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);
		((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,pRsDev->RecordCount);
	}
	catch (_com_error &e) {
		pRsDev->CancelUpdate();
		dump_com_error(e);
	}	
}

void CDeviceSetupDlg::OnDeviceModify() 
{
	//if(!theApp.UserInfo.bLogon())
	//	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_MENU_USERLOGIN); 
	if(
		(theApp.UserInfo.bLogon()== TRUE)&&
		(IDOK == AfxMessageBox(_T("�޸��豸���������ϵͳ��һϵ�����⣬\n\n������רҵ������Ա�޸ģ�\n\n���϶�Ҫ�޸���"),MB_OKCANCEL))
		)

	{	
		try
		{ 
			GetParamFromDlg();
			UpdateDB();	
			pRsDev->Update();
			((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);
 
		}
		catch (_com_error &e) {
			pRsDev->CancelUpdate();
			dump_com_error(e);
		}
	}
}

void CDeviceSetupDlg::SQL()
{
	HRESULT hr		= S_OK;
	try
	{ 
		strSQL.Format(_T("select * from %s"),GetCurrParentSeleString()); 
		hr = pRsDev->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);	 
		if(pRsDev->RecordCount>0)
		{  
			((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,pRsDev->RecordCount);
			pRsDev->MoveFirst();  

		} 

	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
		dump_com_error(e); 
	}  
}


CString CDeviceSetupDlg::GetCurrentSeleString()
{
	CMainFrame * pFrame				= (CMainFrame *)AfxGetApp()->m_pMainWnd;
	COfficeTreeCtrl * pTreeCtrl		= (COfficeTreeCtrl *)&pFrame->m_wndDeviceView.m_wndClassView;
	HTREEITEM    hItem				= pTreeCtrl->GetSelectedItem();
	return pTreeCtrl->GetItemText(hItem);
}

void CDeviceSetupDlg::MoveDBPtr()
{
	try
	{ 
		CString strDev = GetCurrentSeleString();
		pRsDev->MoveFirst();
		for (int i=0;i<pRsDev->RecordCount;i++) 
		{
			if(CString(pRsDev->Fields->GetItem("����")->Value.bstrVal)==strDev)
			{	
				SetDlgItemInt(IDC_DEVICE_ROW_NO ,i+1)  ; 	
				break;
			}
			pRsDev->MoveNext();			
			if(pRsDev->adoEOF==VARIANT_TRUE) 
			{
				pRsDev->MoveLast();
				break;
			}	
		}
	}
	catch (_com_error &e) {
		dump_com_error(e);
	}
}

CString CDeviceSetupDlg::GetCurrSeleParentString()
{ 
	CTreeCtrl  * pTreeCtrl = &((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_wndDeviceView.m_wndClassView;
	HTREEITEM    hItem     = pTreeCtrl->GetSelectedItem();
	hItem = pTreeCtrl->GetParentItem(hItem);
	return pTreeCtrl->GetItemText(hItem);
}
