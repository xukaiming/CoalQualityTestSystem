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
		::MessageBox(NULL,_T("��ȡ���ݿ�ʧ��,�豸��ʼ��ʧ��"),_T("����"),MB_OK+MB_ICONERROR);
		return FALSE;
	}
	else
	{
		SetTitle(LryRdb.m_szName);
		SetModifiedFlag(TRUE);  //�ر�ʱ�����رնԻ�  
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
		
		strSQL.Format(_T("select * from ������ where ����= '%s'"),sDeviceName); 
		hr = pRsDev->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);	
		TESTHR(LryRdb->BindToRecordset(pBindRDB));
		if(pRsDev->RecordCount>0)
		{  //COleVariant
	
			pRsDev->MoveFirst();  
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
			pLryRdb->m_sEndtJiaoBanTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("ĩ�ڽ���ʱ��(��)"))->Value),NULL,10)*60;	//ĩ��ʱ��
			pLryRdb->m_sFillWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��Ͱ��ˮʱ��(S)"))->Value),NULL,10  );;	//��ˮʱ��
			pLryRdb->m_sFireTime						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���ʱ��(S)"))->Value),NULL,10  );    //���ʱ��
			pLryRdb->m_sHotWireHeatValue				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���˿��ֵ"))->Value),NULL,10  );;//���˿��ֵ
			pLryRdb->m_bID								= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��ʶ"))->Value),NULL,10  );;
			pLryRdb->m_sOutWaterTime					= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��Ͱ��ˮʱ��(S)"))->Value),NULL,10  );; ;//��ˮʱ��
			pLryRdb->m_sSenserPowerONTime				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("̽ͷ�ӵ�ʱ��(S)"))->Value),NULL,10  );;//̽ͷ�ϵ�ʱ��
			pLryRdb->m_sStartJiaoBanTime1				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���ڽ���ʱ��1(��)"))->Value),NULL,10  )*60;; //Ԥ�ɰ�ʱ��1
			pLryRdb->m_sStartJiaoBanTime2				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���ڽ���ʱ��2(��)"))->Value),NULL,10  )*60;;//���ڽ���
			pLryRdb->m_sWaterHeight						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��Ͱˮ����"))->Value),NULL,10  );;
			pLryRdb->m_szAlgorithm						= VariantToStr(pRsDev->Fields->GetItem("�㷨")->Value);
			pLryRdb->m_szName							= VariantToStr(pRsDev->Fields->GetItem("����")->Value);
			pLryRdb->m_szType							= VariantToStr(pRsDev->Fields->GetItem("�ͺ�")->Value);
			pLryRdb->m_HeatCapacity						= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A����������")->Value),NULL,10 );
			pLryRdb->m_AOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("A����������")->Value),NULL,10 );//A����������
			pLryRdb->m_BOxygenHeatCapacity 				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem("B����������")->Value),NULL,10 );//B����������		
		}
		else
		{
			::MessageBox(theApp.m_pMainWnd->m_hWnd,_T("�޷���ȡ�豸���ԣ���ʼ��ʧ�ܣ�"),_T("����"),MB_OK+MB_ICONERROR);
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
	strIEDName.Format(_T("ȷ��ֹͣ%sʵ�鲢�˳���"),LryRdb.m_szName);
	if((!IsModified()||
		(IDOK==MessageBox(AfxGetMainWnd()->m_hWnd,strIEDName,_T("��ʾ"),MB_OKCANCEL|MB_ICONQUESTION|MB_TASKMODAL))))
	{ 
		 
		return TRUE;
	}
	else
		return FALSE;
}

 


