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
	pLryRdb->m_bAutoPrint		= VariantToStr(pRsDev->Fields->GetItem(_T("自动打印"))->Value)==_T("是")?TRUE:FALSE;
	pLryRdb->m_bFix				= VariantToStr(pRsDev->Fields->GetItem(_T("修正"))->Value)==_T("是")?TRUE:FALSE;	
	pLryRdb->m_fCoffa			= pRsDev->Fields->GetItem(_T("系数a"))->Value.fltVal ;	
	pLryRdb->m_fCoffb			= pRsDev->Fields->GetItem(_T("系数b"))->Value.fltVal ;
	pLryRdb->m_fCoffc			= pRsDev->Fields->GetItem(_T("系数c"))->Value.fltVal ;
	pLryRdb->m_fCoffd			= pRsDev->Fields->GetItem(_T("系数d"))->Value.fltVal ;
	pLryRdb->m_fCoffe			= pRsDev->Fields->GetItem(_T("系数e"))->Value.fltVal ;
	pLryRdb->m_fCofff			= pRsDev->Fields->GetItem(_T("系数f"))->Value.fltVal ;
	pLryRdb->m_fCoffg			= pRsDev->Fields->GetItem(_T("系数g"))->Value.fltVal ;
	pLryRdb->m_fCoffh			= pRsDev->Fields->GetItem(_T("系数h"))->Value.fltVal ;
	pLryRdb->m_fEndCheckTemp					= pRsDev->Fields->GetItem(_T("终点判定温差"))->Value.fltVal;//终点判断温差
	pLryRdb->m_fFireOKtemp						= pRsDev->Fields->GetItem(_T("点火成功温差"))->Value.fltVal; //点火成功判断
	pLryRdb->m_fR0								= pRsDev->Fields->GetItem(_T("R0"))->Value;
	pLryRdb->m_lHeatCapacityErrorCheckPoint		= _tcstol( VariantToStr(pRsDev->Fields->GetItem(_T("热容量极差判点"))->Value),NULL,10  );//热容量判断点
	pLryRdb->m_lHeatCapacityRSDErrorCheckPoint	= pRsDev->Fields->GetItem(_T("热容量相对标准差"))->Value.fltVal;							//热容量相对标准差判断点
	pLryRdb->m_lScanPriod_mS					= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("扫描周期(mS)"))->Value),NULL,10);     //扫描周期
	pLryRdb->m_sadditive1HeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("1#添加物热值"))->Value),NULL,10);	 //1号添加物热值
	pLryRdb->m_sadditive2HeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("2#添加物热值"))->Value),NULL,10);     //2号添加物热值
	pLryRdb->m_sBeepTime						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("蜂鸣时间(mS)"))->Value),NULL,10);
	pLryRdb->m_sBenzoicAcidHeatValue			= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("苯甲酸热值"))->Value),NULL,10);;		//苯甲酸热值
	pLryRdb->m_sComPort							= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("串行口号"))->Value),NULL,10);;
	pLryRdb->m_sDuplicateSamplesCheckPoint		= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("平行样判点"))->Value),NULL,10);;		//平行样判点
	pLryRdb->m_sEndtJiaoBanTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("末期搅拌时间(分)"))->Value),NULL,10);	//末期时间
	pLryRdb->m_sFillWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("内桶进水时间(S)"))->Value),NULL,10  );;	//进水时间
	pLryRdb->m_sFireTime						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("点火时间(S)"))->Value),NULL,10  );    //点火时间
	pLryRdb->m_sHotWireHeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("点火丝热值"))->Value),NULL,10  );;//点火丝热值
	pLryRdb->m_bID								= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("标识"))->Value),NULL,10  );;
	pLryRdb->m_sOutWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("内桶放水时间(S)"))->Value),NULL,10  );; ;//排水时间
	pLryRdb->m_sSenserPowerONTime				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("探头加电时间(S)"))->Value),NULL,10  );;//探头上电时间
	pLryRdb->m_sStartJiaoBanTime1				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("初期搅拌时间1(分)"))->Value),NULL,10  );; //预缴拌时间1
	pLryRdb->m_sStartJiaoBanTime2				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("初期搅拌时间2(分)"))->Value),NULL,10  );;//初期搅拌
	pLryRdb->m_sWaterHeight						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("内桶水质量"))->Value),NULL,10  );;
	pLryRdb->m_szAlgorithm						= VariantToStr(pRsDev->Fields->GetItem("算法")->Value);
	pLryRdb->m_szName							= VariantToStr(pRsDev->Fields->GetItem("名称")->Value);
	pLryRdb->m_szType							= VariantToStr(pRsDev->Fields->GetItem("型号")->Value);
	pLryRdb->m_HeatCapacity						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A氧弹热容量")->Value),NULL,10 );
	pLryRdb->m_AOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A氧弹热容量")->Value),NULL,10 );//A氧弹热容量
	pLryRdb->m_BOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("B氧弹热容量")->Value),NULL,10 );//B氧弹热容量		

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
	pRsDev->PutCollect("自动打印",_variant_t(pLryRdb->m_bAutoPrint?VARIANT_TRUE:VARIANT_FALSE));
	pRsDev->PutCollect("修正",_variant_t(pLryRdb->m_bFix?VARIANT_TRUE:VARIANT_FALSE));	
	/*
	pRsDev->PutCollect("系数a",_variant_t(pLryRdb->m_fCoffa );	
	pRsDev->PutCollect("系数b",_variant_t(pLryRdb->m_fCoffb) ;
	pRsDev->PutCollect("系数c",_variant_t(pLryRdb->m_fCoffc) ;
	pRsDev->PutCollect("系数d",_variant_t(pLryRdb->m_fCoffd) ;
	pRsDev->PutCollect("系数e",_variant_t(pLryRdb->m_fCoffe) ;
	pRsDev->PutCollect("系数f",_variant_t(pLryRdb->m_fCofff) ;
	pRsDev->PutCollect("系数g",_variant_t(pLryRdb->m_fCoffg) ;
	pRsDev->PutCollect("系数h",_variant_t(pLryRdb->m_fCoffh) ;
	*/
	pRsDev->PutCollect("终点判定温差",pLryRdb->m_fEndCheckTemp);//终点判断温差
	pRsDev->PutCollect("点火成功温差",pLryRdb->m_fFireOKtemp); //点火成功判断
	pRsDev->PutCollect("R0",pLryRdb->m_fR0); 
//	pRsDev->PutCollect("热容量极差判点",pLryRdb->m_lHeatCapacityErrorCheckPoint);//热容量判断点 
	pRsDev->PutCollect("热容量相对标准差",pLryRdb->m_lHeatCapacityRSDErrorCheckPoint);	 
	pRsDev->PutCollect("扫描周期(mS)",pLryRdb->m_lScanPriod_mS);     //扫描周期
	pRsDev->PutCollect("1#添加物热值",pLryRdb->m_sadditive1HeatValue);	 //1号添加物热值
	pRsDev->PutCollect("2#添加物热值",pLryRdb->m_sadditive2HeatValue);     //2号添加物热值
	pRsDev->PutCollect("蜂鸣时间(mS)",pLryRdb->m_sBeepTime);
	pRsDev->PutCollect("苯甲酸热值",pLryRdb->m_sBenzoicAcidHeatValue);;		//苯甲酸热值
	pRsDev->PutCollect("串行口号",pLryRdb->m_sComPort);;
	pRsDev->PutCollect("平行样判点",pLryRdb->m_sDuplicateSamplesCheckPoint);;		//平行样判点
	pRsDev->PutCollect("末期搅拌时间(分)",pLryRdb->m_sEndtJiaoBanTime	);	//末期时间
	pRsDev->PutCollect("内桶进水时间(S)",pLryRdb->m_sFillWaterTime );;	//进水时间
	pRsDev->PutCollect("点火时间(S)",pLryRdb->m_sFireTime  );    //点火时间
	pRsDev->PutCollect("点火丝热值",pLryRdb->m_sHotWireHeatValue  );;//点火丝热值
	pRsDev->PutCollect("标识",pLryRdb->m_bID  );;
	pRsDev->PutCollect("内桶放水时间(S)",pLryRdb->m_sOutWaterTime);; ;//排水时间
	pRsDev->PutCollect("探头加电时间(S)" ,pLryRdb->m_sSenserPowerONTime);;//探头上电时间
	pRsDev->PutCollect("初期搅拌时间1(分)",pLryRdb->m_sStartJiaoBanTime1 );; //预缴拌时间1
	pRsDev->PutCollect("初期搅拌时间2(分)",pLryRdb->m_sStartJiaoBanTime2);;//初期搅拌
	pRsDev->PutCollect("内桶水质量",pLryRdb->m_sWaterHeight);; 
	pRsDev->PutCollect("算法",_variant_t(pLryRdb->m_szAlgorithm!=""?pLryRdb->m_szAlgorithm:"瑞芳"));
	pRsDev->PutCollect("名称",_variant_t(pLryRdb->m_szName));
	pRsDev->PutCollect("型号",_variant_t(pLryRdb->m_szType));
	pRsDev->PutCollect("A氧弹热容量",pLryRdb->m_HeatCapacity	 );
	pRsDev->PutCollect("A氧弹热容量",_variant_t(pLryRdb->m_AOxygenHeatCapacity)  );//A氧弹热容量
	pRsDev->PutCollect("B氧弹热容量",_variant_t(pLryRdb->m_AOxygenHeatCapacity) );//B氧弹热容量		

}


void CDeviceSetupDlg::OnDeviceDel() 
{ 
	if(
		(theApp.UserInfo.bLogon() == TRUE)&&
		(IDOK == AfxMessageBox(_T("该设备将要被删除\n\n你确认吗?"),MB_OKCANCEL))
		)

	{
		try
		{ 
			pRsDev->Delete(adAffectCurrent);
			pRsDev->Update(); 
			((CMainFrame*)AfxGetMainWnd())->m_wndDeviceView.SendMessage(WM_PARAMETE_CHANGED,0,0);
			((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,pRsDev->RecordCount);
			//UDN_DELTAPOS 
			MessageBox(_T("改设备已删除"),_T("提示"),MB_ICONINFORMATION+MB_OK);
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
		(IDOK == AfxMessageBox(_T("修改设备参数会造成系统的一系列问题，\n\n必须由专业技术人员修改！\n\n您肯定要修改吗？"),MB_OKCANCEL))
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
			if(CString(pRsDev->Fields->GetItem("名称")->Value.bstrVal)==strDev)
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
