// LryHostDoc.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "LryHostDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLryHostDoc

IMPLEMENT_DYNCREATE(CLryHostDoc, CDocument)

CLryHostDoc::CLryHostDoc()
{

}

BOOL CLryHostDoc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
		return FALSE;
	sDeviceName = GetCurrentSeleString();
	if(!GetHostParam(&LryBindRdb,&LryRdb,sDeviceName))
	{		
		::MessageBox(NULL,_T("读取数据库失败,设备初始化失败"),_T("错误"),MB_OK+MB_ICONERROR);
		return FALSE;
	}
	else
	{
		SetTitle(LryRdb.m_szName);
		SetModifiedFlag(TRUE);  //关闭时弹出关闭对话  
		return TRUE;
	}
}

CLryHostDoc::~CLryHostDoc()
{
	
}


BEGIN_MESSAGE_MAP(CLryHostDoc, CDocument)
	//{{AFX_MSG_MAP(CLryHostDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLryHostDoc diagnostics

#ifdef _DEBUG
void CLryHostDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLryHostDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLryHostDoc serialization

void CLryHostDoc::Serialize(CArchive& ar)
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
// CLryHostDoc commands

BOOL CLryHostDoc::GetHostParam(LRYRBindDB *pBindRDB, LRYRDB * pLryRdb,CString sDeviceName)
{
	HRESULT hr= S_OK;
	BOOL bResult = TRUE;
	CString strSQL;		 
	try
	{
		_RecordsetPtr	pRsDev;
		pRsDev.CreateInstance("ADODB.Recordset");
		IADORecordBindingPtr LryRdb(pRsDev);  
		
		strSQL.Format(_T("select * from 量热仪 where 名称= '%s'"),sDeviceName); 
		hr = pRsDev->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);	
		TESTHR(LryRdb->BindToRecordset(pBindRDB));
		if(pRsDev->RecordCount>0)
		{  //COleVariant
	
			pRsDev->MoveFirst();  
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
			pLryRdb->m_sEndtJiaoBanTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("末期搅拌时间(分)"))->Value),NULL,10)*60;	//末期时间
			pLryRdb->m_sFillWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("内桶进水时间(S)"))->Value),NULL,10  );;	//进水时间
			pLryRdb->m_sFireTime						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("点火时间(S)"))->Value),NULL,10  );    //点火时间
			pLryRdb->m_sHotWireHeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("点火丝热值"))->Value),NULL,10  );;//点火丝热值
			pLryRdb->m_bID								= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("标识"))->Value),NULL,10  );;
			pLryRdb->m_sOutWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("内桶放水时间(S)"))->Value),NULL,10  );; ;//排水时间
			pLryRdb->m_sSenserPowerONTime				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("探头加电时间(S)"))->Value),NULL,10  );;//探头上电时间
			pLryRdb->m_sStartJiaoBanTime1				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("初期搅拌时间1(分)"))->Value),NULL,10  )*60;; //预缴拌时间1
			pLryRdb->m_sStartJiaoBanTime2				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("初期搅拌时间2(分)"))->Value),NULL,10  )*60;;//初期搅拌
			pLryRdb->m_sWaterHeight						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("内桶水质量"))->Value),NULL,10  );;
			pLryRdb->m_szAlgorithm						= VariantToStr(pRsDev->Fields->GetItem("算法")->Value);
			pLryRdb->m_szName							= VariantToStr(pRsDev->Fields->GetItem("名称")->Value);
			pLryRdb->m_szType							= VariantToStr(pRsDev->Fields->GetItem("型号")->Value);
			pLryRdb->m_HeatCapacity						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A氧弹热容量")->Value),NULL,10 );
			pLryRdb->m_AOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A氧弹热容量")->Value),NULL,10 );//A氧弹热容量
			pLryRdb->m_BOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("B氧弹热容量")->Value),NULL,10 );//B氧弹热容量		
		}
		else
		{
			::MessageBox(theApp.m_pMainWnd->m_hWnd,_T("无法读取设备属性，初始化失败！"),_T("警告"),MB_OK+MB_ICONERROR);
			bResult =  FALSE;
		}
		if(pRsDev->State==adStateOpen)
			pRsDev->Close();
	}
	catch(_com_error &e)
	{
		e.ErrorMessage();
		dump_com_error(e);
		bResult =  FALSE;
	}  
	for( int i = 0;i<sizeof(LryRdb.fCurve)/sizeof(double);i++)
		LryRdb.fCurve[i]=-1;
	return bResult;
}

CString CLryHostDoc::GetCurrentSeleString()
{
	CMainFrame * pFrame    = (CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CTreeCtrl  * pTreeCtrl = &((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_wndDeviceView.m_wndClassView; 
	HTREEITEM    hItem     = pTreeCtrl->GetSelectedItem();
	return pTreeCtrl->GetItemText(hItem);
}

BOOL CLryHostDoc::SaveModified() 
{
	
	CString strIEDName;
	strIEDName.Format(_T("确定停止%s实验并退出吗？"),LryRdb.m_szName);
	if((!IsModified()||
		(IDOK==MessageBox(AfxGetMainWnd()->m_hWnd,strIEDName,_T("提示"),MB_OKCANCEL|MB_ICONQUESTION|MB_TASKMODAL))))
	{ 
		 
		return TRUE;
	}
	else
		return FALSE;
}

 


