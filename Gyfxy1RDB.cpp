// GyfxyRDB.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "Gyfxy1RDB.h"
// CGyfxyRDB_BaseRdb
IMPLEMENT_DYNCREATE(CGyfxyRDB_BaseRdb, CGyfxyRDB)
CGyfxyRDB_BaseRdb::CGyfxyRDB_BaseRdb()
{  
}

CGyfxyRDB_BaseRdb::~CGyfxyRDB_BaseRdb()
{
}


// CGyfxyRDB_BaseRdb member functions

BOOL CGyfxyRDB_BaseRdb::UpdateParamFromDB(void)
{
	BOOL bResult  = TRUE;
	if(pRsDev->RecordCount>0)
	{  //COleVariant 
		
		m_szName			= VariantToStr(pRsDev->Fields->GetItem("����")->Value);;				//�豸����
		m_szType 			= VariantToStr(pRsDev->Fields->GetItem("�ͺ�")->Value);	//�豸����
		m_bID				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("��ʶ"))->Value),NULL,10  );;		//ID��
		m_sComPort			= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("���пں�"))->Value),NULL,10);;;				//���ں�	
		m_lScanPriod_mS		= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("ɨ������(mS)"))->Value),NULL,10);     //ɨ������;
		
		attrib.m_sTimePut2weigh			= pRsDev->Fields->GetItem(_T("����������ʱ��(0_01S)"))->Value.lVal;//;	SHORT
		attrib.m_sTimePut2HighTemp		= pRsDev->Fields->GetItem(_T("����������ʱ��(0_01S)"))->Value.lVal;//SHORT
		attrib.m_sPulsePerRound1Spit	= pRsDev->Fields->GetItem(_T("����һȦ������"))->Value.iVal;//;		SHORT
		attrib.m_btDriverSpitCnt		= pRsDev->Fields->GetItem(_T("������ϸ����"))->Value.bVal;;//;		BYTE
		attrib.m_btMaxSampleCnt			= pRsDev->Fields->GetItem(_T("���̵���λ����"))->Value.bVal;;//	BYTE
		attrib.m_fBalanceMaxErr			= pRsDev->Fields->GetItem(_T("������ֵ(g)"))->Value.fltVal;;//;FLOAT

		attrib.m_fVolSampleMax			= pRsDev->Fields->GetItem(_T("�ӷ�����Ʒ��������(g)"))->Value.fltVal;;//;FLOAT
		attrib.m_fVolSampleMin			= pRsDev->Fields->GetItem(_T("�ӷ�����Ʒ��������(g)"))->Value.fltVal;;//;FLOAT
		attrib.m_fWaterAshSampleMax		= pRsDev->Fields->GetItem(_T("ˮ�ֻҷ���Ʒ��������(g)"))->Value.fltVal;;//;FLOAT
		attrib.m_fWaterAshSampleMin		= pRsDev->Fields->GetItem(_T("ˮ�ֻҷ���Ʒ��������(g)"))->Value.fltVal;;//;FLOAT
		//SHORT
		attrib.m_sLowStoveTempSpeed		= pRsDev->Fields->GetItem(_T("����¯��������"))->Value.iVal;;//
		attrib.m_sHighStoveTempSpeed	= pRsDev->Fields->GetItem(_T("����¯��������"))->Value.iVal;;//;
		attrib.m_sQuick_WaterTestTime	= pRsDev->Fields->GetItem(_T("����ˮ�ָ���ʱ��(��)"))->Value.iVal;;//;
		attrib.m_sQuick_VolTestTime		= pRsDev->Fields->GetItem(_T("���ٻӷ��ָ���ʱ��(��)"))->Value.iVal;;//;
		attrib.m_sQuick_AshTestTime		= pRsDev->Fields->GetItem(_T("���ٻҷָ���ʱ��(��)"))->Value.iVal;;//;
		attrib.m_sGB_WaterTestTime		= pRsDev->Fields->GetItem(_T("����ˮ�ָ���ʱ��(��)"))->Value.iVal;;//;
		attrib.m_sGB_VolTestTime		= pRsDev->Fields->GetItem(_T("����ӷ��ָ���ʱ��(��)"))->Value.iVal;;//;
		attrib.m_sGB_AshTestTime		= pRsDev->Fields->GetItem(_T("����ҷָ���ʱ��(��)"))->Value.iVal;;//;
		attrib.m_sQuick_WaterTestTemp	= pRsDev->Fields->GetItem(_T("����ˮ�������¶�(��)"))->Value.iVal;;//;
		attrib.m_sQuick_VolTestTemp		= pRsDev->Fields->GetItem(_T("���ٻӷ��������¶�(��)"))->Value.iVal;;//;
		attrib.m_sQuick_AshTestTemp		= pRsDev->Fields->GetItem(_T("���ٻҷ������¶�(��)"))->Value.iVal;;//;
		attrib.m_sGB_WaterTestTemp		= pRsDev->Fields->GetItem(_T("����ˮ�������¶�(��)"))->Value.iVal;;//;
		attrib.m_sGB_VolTestTemp		= pRsDev->Fields->GetItem(_T("����ӷ��������¶�(��)"))->Value.iVal;;//;
		attrib.m_sGB_AshTestTemp		= pRsDev->Fields->GetItem(_T("����ҷ������¶�(��)"))->Value.iVal;;
		attrib.m_sWaterCoolTime			= pRsDev->Fields->GetItem(_T("ˮ��¯������ȴʱ��"))->Value.iVal;;;		//ˮ��¯������ȴʱ��
		attrib.m_sHighTempCoolTime		= pRsDev->Fields->GetItem(_T("����¯������ȴʱ��"))->Value.iVal;;;	//���²�����ȴʱ��
		//
		attrib.m_sBeepTime				= pRsDev->Fields->GetItem(_T("����ʱ��(mS)"))->Value.iVal;;
		//
		attrib.m_bFix					= pRsDev->Fields->GetItem(_T("����"))->Value.boolVal;;//VARIANT_BOOL
		attrib.m_szalgorithm			= VariantToStr(pRsDev->Fields->GetItem(_T("�㷨"))->Value);;//CString
		attrib.m_bAutoPrint				= pRsDev->Fields->GetItem(_T("�Զ���ӡ"))->Value.boolVal;;//VARIANT_BOOL
		attrib.m_szTestSequence			= VariantToStr(pRsDev->Fields->GetItem(_T("����˳��"))->Value);;     //����˳��CString
		attrib.m_fCrucibleCoeff1		= pRsDev->Fields->GetItem(_T("����ϵ��1"))->Value.fltVal;;//����ϵ��1;
		attrib.m_fCrucibleCoeff2		= pRsDev->Fields->GetItem(_T("����ϵ��2"))->Value.fltVal;;
		attrib.m_fCrucibleCoeff3		= pRsDev->Fields->GetItem(_T("����ϵ��3"))->Value.fltVal;;

		attrib.m_szSystemType			= VariantToStr(pRsDev->Fields->GetItem("�豸����")->Value);;
		//attrib.m_bdisperse				= pRsDev->Fields->GetItem(_T("disperse"))->Value.boolVal;;
	}
	else
	{
		bResult = FALSE;
		::MessageBox(theApp.m_pMainWnd->m_hWnd,_T("�޷���ȡ�豸���ԣ���ʼ��ʧ�ܣ�"),_T("����"),MB_OK+MB_ICONERROR);
	}
	return TRUE;
}

void CGyfxyRDB_BaseRdb::InitRDB(CGyfxyImageSlave* ImageSlave)
{
	enum enum_IED_type{G_NONE = 0,G5000,G5200,G5500}; 
	CGyfxyImageSlave::SDownParam *pImg = &ImageSlave->DownloadWorkParam;
	memset(pImg,0,sizeof(CGyfxyImageSlave::SDownParam));
	pImg->m_sTimePut2weigh		= attrib.m_sTimePut2weigh;		//����������ʱ��(0_1S);
	pImg->m_sTimePut2HighTemp	= attrib.m_sTimePut2HighTemp;	//����������ʱ��(0_1S);

	pImg->m_sPulsePerRound1Spit	= attrib.m_sPulsePerRound1Spit;	//����һȦ������;
	pImg->m_btDriverSpitCnt		= attrib.m_btDriverSpitCnt;		//������ϸ����;
	pImg->m_btMaxSampleCnt		= attrib.m_btMaxSampleCnt;		//��λ����

	pImg->m_fBalanceMaxErr		= attrib.m_fBalanceMaxErr*10000L;		//������ֵ(0.1mg);	

	pImg->m_fVolSampleMax			= attrib.m_fVolSampleMax*10000L; 
	pImg->m_fVolSampleMin			= attrib.m_fVolSampleMax*10000L; 
	pImg->m_fWaterAshSampleMax		= attrib.m_fWaterAshSampleMax*10000L; 
	pImg->m_fWaterAshSampleMin		= attrib.m_fWaterAshSampleMin*10000L; 

//	pImg->m_sLowStoveTempSpeed	= attrib.m_sLowStoveTempSpeed;	//����¯��������
//	pImg->m_sHighStoveTempSpeed	= attrib.m_sHighStoveTempSpeed;	//����¯��������;
/*
	pImg->m_sQuick_WaterTestTime= attrib.m_sQuick_WaterTestTime*60;//����ˮ�ָ���ʱ��(��);
	pImg->m_sQuick_VolTestTime	= attrib.m_sQuick_VolTestTime*60;	//���ٻӷ��ݸ���ʱ��(��)
	pImg->m_sQuick_AshTestTime	= attrib.m_sQuick_AshTestTime*60;	//���ٻҷָ���ʱ��(��);

	pImg->m_sGB_WaterTestTime	= attrib.m_sGB_WaterTestTime*60;	//����ˮ�ָ���ʱ��(��);
	pImg->m_sGB_VolTestTime		= attrib.m_sGB_VolTestTime*60;		//����ӷ��ָ���ʱ��(��);
	pImg->m_sGB_AshTestTime		= attrib.m_sGB_AshTestTime*60;		//����ҷָ���ʱ��(��);	
	//pImg->m_szTestSequence		= 
*/
	if(attrib.m_szalgorithm!=_T("����"))
	{
		pImg->m_sWaterTestTime		= attrib.m_sGB_WaterTestTime*60;	//����ˮ�ָ���ʱ��(��);
		pImg->m_sVolTestTime		= attrib.m_sGB_VolTestTime*60;		//����ӷ��ָ���ʱ��(��);
		pImg->m_sAshTestTime		= attrib.m_sGB_AshTestTime*60;		//����ҷָ���ʱ��(��);			pImg->m_sWaterTestTemp	= TemptoAD(attrib.m_sQuick_WaterTestTemp);//����ˮ�������¶�(��);
		
		pImg->m_sVolTestTemp	= TemptoAD(attrib.m_sQuick_VolTestTemp);	//���ٻӷ��������¶�(��);
		pImg->m_sAshTestTemp	= TemptoAD(attrib.m_sQuick_AshTestTemp);	//���ٻҷ������¶�(��);
	}
	else
	{
		pImg->m_sWaterTestTime	= attrib.m_sQuick_WaterTestTime*60;//����ˮ�ָ���ʱ��(��);
		pImg->m_sVolTestTime	= attrib.m_sQuick_VolTestTime*60;	//���ٻӷ��ݸ���ʱ��(��)
		pImg->m_sAshTestTime	= attrib.m_sQuick_AshTestTime*60;	//���ٻҷָ���ʱ��(��);

		pImg->m_sWaterTestTemp	= TemptoAD(attrib.m_sGB_WaterTestTemp);	//����ˮ�������¶�(��);
		pImg->m_sVolTestTemp	= TemptoAD(attrib.m_sGB_VolTestTemp);		//����ӷ��������¶�(��);
		pImg->m_sAshTestTemp	= TemptoAD(attrib.m_sGB_AshTestTemp);		//����ҷ������¶�(��);
	} 

	pImg->m_sWaterCoolTime		= attrib.m_sWaterCoolTime*60;;		//ˮ��¯������ȴʱ��
	pImg->m_sHighTempCoolTime	= attrib.m_sHighTempCoolTime*60;;	//����¯������ȴʱ��
	pImg->m_sBeepTime			= attrib.m_sBeepTime;			//����ʱ��
	if(attrib.m_szalgorithm==_T("����"))
	{ 
		pImg->m_szalgorithm = CGyfxyImageSlave::SDownParam::M_GB;
	}
	else
	{
		pImg->m_szalgorithm = CGyfxyImageSlave::SDownParam::M_QK;
	}
	////////////////////////////////////////
	//���ù�ҵ��������������
	if(attrib.m_szSystemType==_T("G5000"))
		pImg->m_bIED_system_type = G5000;
	else if(attrib.m_szSystemType==_T("G5200"))
		pImg->m_bIED_system_type = G5200;
	else if(attrib.m_szSystemType==_T("G5500"))
		pImg->m_bIED_system_type = G5500;
	else  
		pImg->m_bIED_system_type = G_NONE;
	//pImg->m_szTestSequence[8]	= attrib.;     //����˳��
	/*
#ifdef UNICODE
	int iLen		= attrib.m_szTestSequence.GetLength();
	iLen = iLen*2+1;
	char *pchar = new char[iLen];
	memset(pchar,0,iLen);
	//pchar
	w2c(pchar,attrib.m_szTestSequence,iLen);

	strcpy(pImg->m_szTestSequence,pchar);
	delete pchar;
#else
	strcpy(pImg->m_szTestSequence,(LPCTSTR)attrib.m_szTestSequence);
#endif
	*/ 
}


int CGyfxyRDB_BaseRdb::ALLSample::QualityWater = 5;    //ˮ�ֲ���λ��
int CGyfxyRDB_BaseRdb::ALLSample::ResultWater  = 2;		//ˮ�ּ�����λ��	


void CGyfxyRDB_BaseRdb::ALLSample::SetDefaultSample(CXListCtrl &m_pListCtrl,int iRowCnt )
{
	iSampleCnt = iRowCnt;
	for(int i = 0;i<iRowCnt;i++)
	{
		/*
		if((i/2)%2!=0)
			Sample[i].cItem = TEST_WATER|TEST_ASH;
		else
			Sample[i].cItem = TEST_VOL;
			
		//////////////////////////////////////////////////////////////////////////////
		Sample[i].cSta =  3; //����Ʒ ������
		*/
		if(i<2)
			Sample[i].cSampleName = _T("�ӷ���У׼����");
		else if(i<4)
			Sample[i].cSampleName = _T("ˮ��/�ҷ�У׼����");
		else
			Sample[i].cSampleName.Format(_T("%d#"),i/2+1);
		Sample[i].SampleNO	= i+1;  
		OnUpdateQuality(i+1,&m_pListCtrl );
	}
}

void CGyfxyRDB_BaseRdb::ALLSample::GetSampleDataFromDlg(CXListCtrl &m_ListCtrl,CGyfxyImageSlave *pImg)
{

	for (int i=0;i<iSampleCnt;i++)
	{
		for (int j=0;j<3;j++)
		{
			GetSampleDataFromDlg(i, j+ResultWater,m_ListCtrl,pImg);
		}
	}
}



void CGyfxyRDB_BaseRdb::ALLSample::GetSampleDataFromDlg(int nItem, int nSubItem,CXListCtrl &m_ListCtrl,CGyfxyImageSlave *pImg)
{
	int EnableMask=0,  DisableMask=0;
	int WorkType;
	int  bChecked = m_ListCtrl.GetItemCheckedState(nItem, nSubItem);
	WorkType = 1L<<(nSubItem-ResultWater);
	if(bChecked)
		EnableMask  = WorkType;
	else
		DisableMask = WorkType;
	RefreshSampleData(pImg,nItem+1,EnableMask,DisableMask);
}



void CGyfxyRDB_BaseRdb::ALLSample::OnUpdateQuality(int cSamplePos,CXListCtrl *m_pListCtrl )
{
	CString str; 
	int i=cSamplePos-1;	
	S_Sample *pSample = Sample;
	if(cSamplePos>0)
	{ 
		if (pSample[cSamplePos-1].cItem&
			CGyfxyRDB_BaseRdb::ALLSample::TEST_WATER)
		{
			m_pListCtrl->SetCheckbox(i, ResultWater, 1); 
			str.Format(_T("%3.3f%%"),pSample[cSamplePos-1].m_fMad);
			m_pListCtrl->SetItemText(i, ResultWater,str );  
		}
		else
		{
			m_pListCtrl->SetCheckbox(i, ResultWater, 0);  
			m_pListCtrl->SetItemText(i, ResultWater,_T("0.000%")); 
		}
		////////////////////////////////////////////////////////////////////
		if (pSample[cSamplePos-1].cItem&
			CGyfxyRDB_BaseRdb::ALLSample::TEST_VOL)
		{
			m_pListCtrl->SetCheckbox(i, ResultWater+1, 1); 
			str.Format(_T("%3.3f%%"),pSample[cSamplePos-1].m_fVad);
			m_pListCtrl->SetItemText(i, ResultWater+1,str); 
		}
		else
		{
			m_pListCtrl->SetCheckbox(i, ResultWater+1, 0); 
			m_pListCtrl->SetItemText(i, ResultWater+1,_T("0.000%")); 
		}
		////////////////////////////////////////////////////////////////////
		if (pSample[cSamplePos-1].cItem&
			CGyfxyRDB_BaseRdb::ALLSample::TEST_ASH)
		{
			m_pListCtrl->SetCheckbox(i, ResultWater+2, 1);		 
			str.Format(_T("%3.3f%%"),pSample[cSamplePos-1].m_fAad);
			m_pListCtrl->SetItemText(i, ResultWater+2,str); 
		}
		else
		{
			m_pListCtrl->SetCheckbox(i, ResultWater+2, 0);  
			m_pListCtrl->SetItemText(i, ResultWater+2,_T("0.000%")); 
		} 
		if (((pSample[cSamplePos-1].cItem&
			(TEST_VOL|TEST_ASH|TEST_WATER))==0)
			  ||(cSamplePos<5))
		{ 
			m_pListCtrl->SetEnabled(i, FALSE); 
		}
		else
		{
			m_pListCtrl->SetEnabled(i, TRUE);   
			m_pListCtrl->EnsureVisible(cSamplePos-1,TRUE);
			m_pListCtrl->SetItemState(cSamplePos-1,LVIS_SELECTED, LVIS_SELECTED);
		} 
		//////////////////////////////////////////////////////////////////////////////
		m_pListCtrl->SetItemText(i,0,pSample[cSamplePos-1].cSampleName);
		//////////////////////////////////////////////////////////////////////////////
		for (int i=0;i<sizeof(CGyfxyImageSlave::YP_DATA::UQUALITY)/sizeof(long);i++)
		{ 
			str = _T("0.0000");
			if (i>0) 
			{ 
				if (pSample[cSamplePos-1].UQuality.Quality[i]!=0.0f)
				{
					str.Format(_T("%.4f"),pSample[cSamplePos-1].UQuality.Quality[i]-pSample[cSamplePos-1].UQuality.Quality[0]); 
				}
			}
			else
			{ 
				str.Format(_T("%.4f"),(pSample[cSamplePos-1].UQuality.Quality[i]));
			}
			//m_pListCtrl->SetItemText(cSamplePos-1,QualityWater+i,str); 

			m_pListCtrl->SetItemText(cSamplePos-1,QualityWater+i,str,RGB(0,0,0), 
				pSample[cSamplePos-1].cStabState&(1L<<i)? //���ȶ�����Ʒ����ɫ���
				RGB(255,0,0):RGB(255,255,255));  
		}   
	} 
	else
	{ 
		for (int j= 1;j<=iSampleCnt;j++)
		{
			OnUpdateQuality(j,m_pListCtrl);
		} 
	}
}


void CGyfxyRDB_BaseRdb::ALLSample::RefreshSampleData(OUT CGyfxyImageSlave *pImageSlave,
											 IN int SampleNO,
											 IN int EnableMask,
											 IN int DisableMask)
{ 
	ASSERT(SampleNO>0);
	int WorkType= Sample[SampleNO-1].cItem  ;
	ASSERT(!((EnableMask+DisableMask)^(EnableMask|DisableMask)));  //���ͬʱ������ EnableMask/DisableMask  �ᱨ���� 
	WorkType		|= EnableMask;
	WorkType		&= ~DisableMask;
	RefreshSampleData(pImageSlave,SampleNO,WorkType );
}



void CGyfxyRDB_BaseRdb::ALLSample::RefreshSampleData(OUT CGyfxyImageSlave *pImageSlave,IN int SampleNO,IN int WorkType )
{
	ASSERT(SampleNO>0);
	SampleNO-=1;   
	CGyfxyImageSlave::YP_DATA *SamplePtr = pImageSlave->Sample;
	Sample[SampleNO].cItem = WorkType;
	if(Sample[SampleNO].cItem>0)
	{
		Sample[SampleNO].cSta=0x03|           //��Ҫ������Ŀ,֤����λ�������� ��Ʒ
			(Sample[SampleNO].cItem&0x1?0x04:0)| //ˮ��
											   //�ӷ��Ƹ���
			(Sample[SampleNO].cItem&0x2?0x10:0)| //�ӷ���
			(Sample[SampleNO].cItem&0x4?0x20:0);//�ҷ�
		 
	}
	else
		Sample[SampleNO].cSta=0;
	//////////////////////////////////////////////////////////
	SamplePtr[SampleNO].cSta	= Sample[SampleNO].cSta    ;
	SamplePtr[SampleNO].cCSta	= Sample[SampleNO].cCSta   ;
	SamplePtr[SampleNO].cItem	= Sample[SampleNO].cItem   ;
	SamplePtr[SampleNO].cStabState	= Sample[SampleNO].cStabState  ; 
}

void CGyfxyRDB_BaseRdb::ALLSample::RefreshSampleData(IN CGyfxyImageSlave *pImageSlave)
{
	CGyfxyImageSlave::YP_DATA *SamplePtr = pImageSlave->Sample;
	if(iSampleCnt>MAX_SAMPLE_CNT)
		iSampleCnt = MAX_SAMPLE_CNT;
	for (int i=1;i<=iSampleCnt;i++)
	{
		RefreshSampleData(pImageSlave,  i);
	} 
}

void CGyfxyRDB_BaseRdb::ALLSample::RefreshSampleData(IN CGyfxyImageSlave *pImageSlave,IN int SampleNO)
{
	ASSERT(SampleNO>0);
	SampleNO-=1;        //����û�м�һ,���ָ��Խ��,��������
	CGyfxyImageSlave::YP_DATA *SamplePtr = pImageSlave->Sample;
	Sample[SampleNO].cSta  = SamplePtr[SampleNO].cSta;
	Sample[SampleNO].cCSta = SamplePtr[SampleNO].cCSta;
	Sample[SampleNO].cItem = SamplePtr[SampleNO].cItem;

	Sample[SampleNO].cStabState = SamplePtr[SampleNO].cStabState;
	int StructSize = sizeof(CGyfxyImageSlave::YP_DATA::UQUALITY)/sizeof(long);
	for (int j=0;j<StructSize;j++)
	{
		Sample[SampleNO].UQuality.Quality[j]=SamplePtr[SampleNO].UQuality.Quality[j]/10000.0f;
	}
	//Sample[SampleNO].UQuality.Quality[1]-=Sample[SampleNO].UQuality.Quality[0];
	StructSize = sizeof(Sample)/sizeof(S_Sample);
	for (int j=0;j<StructSize;j++)
	{
		for (int i =0;i<sizeof(S_Sample::UQUALITY)/sizeof(double);i++)
		{
			if(Sample[j].UQuality.Quality[i]<0.005f)
				Sample[j].UQuality.Quality[i] = 0;
		}
	}
}

void CGyfxyRDB_BaseRdb::RefreshRDBTab(CGyfxyImageSlave *pImageSlave)
{
	memcpy(status.TempAdValue,pImageSlave->ADValue.ad,sizeof(status.TempAdValue));		//¯���¶�ADֵ
	status.InstTempValue	= pImageSlave->ADValue.adInstTemp;			//�����¶�
	for (int i=0;i<3;i++)
	{
		status.TempVoltage[i]	= ADToVol(status.TempAdValue[i])/1000;		//¯���¶ȵ�ѹ			
		status.Temperature[i]	= VolToTemp(status.TempVoltage[i]);
		status.m_bWarmFlag[i]	= pImageSlave->fuzzy[i].WarmFlag;
		status.du[i]			= pImageSlave->fuzzy[i].du;

		unsigned short *pin		= &pImageSlave->InputStatus.cInputReg16;
		status.cCurrPos[i]		= (((*pin)>>(i*2+8))&0x3);					//����״̬  �õ���ǰλ��
		status.strCurrPos[i]	= status.cCurrPos[i]==0x03?_T("δ֪"):
									status.cCurrPos[i]==0x2?_T("����"):_T("����");	
		unsigned short *pOut	= &pImageSlave->OutputCoil.Coil16;		//���״̬  �õ��˶�״̬�ͷ���
		status.cMoving[i]		= (((*pOut)>>(i*2))&0x3);					//����״̬  �õ���ǰλ��
		status.strMov[i]		= status.cMoving[i]==0x00?_T("ֹͣ"):
									status.cMoving[i]==0x01?_T("����"):_T("�½�");

	} 
	status.CurrWeight	= ((double)pImageSlave->Balance.CurrWeight)/10000; 
	//////////////////////////////////////  	
	/////////////////////////////////////////////////	  
	/////////////////////////////////////////////////
	int     cTestState;
	/////////////////////////////////////////////////
	status.cSampleDiskCurrPos	= pImageSlave->MtrState.CurSamplePos;
	status.bSampleDiskMoving	= pImageSlave->MtrState.bRun;
	CString strSampleDiskCurrPos;    //���̵�ǰλ��

	SampleArray.RefreshSampleData(pImageSlave);
	///
	//BOOL m_bStartTest;
	//BOOL m_bReseted;   

}



void CGyfxyRDB_BaseRdb::ALLSample::BuildAutoNO()
{ 
	COleDateTime  dtEnd;									//��ѯ��������
	COleDateTime  dtStart=COleDateTime::GetCurrentTime( );  //�õ����������
	CString strStartDate,strEndDate,AutoNo;   //��ʼ����,��������,�Զ����
	CString sName = m_szName.Left(m_szName.Find(_T('#'))+1);
	sName+=_T("MAV");
	//
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	

	//dtStart.SetDateTime(2002, 3, 15, 1, 0, 0);  
	dtEnd   = MakeDays(1)+dtStart;
	strStartDate = dtStart.Format(_T("%Y%m%d"));
	{
		//1AE2002031504
		//��ѯ���� ��¼ 		
		strSQL.Format(
			_T("select ������ from ��ҵ���������� where ������ like '%%%s%%' and ��������  between  #%s# and #%s# and �豸����='%s' order by ������"), 
			strStartDate,
			dtStart.Format(_T("%Y-%m-%d")),
			dtEnd.Format(_T("%Y-%m-%d")),
			m_szName
			);	 
		try
		{
			int length;
			pRsTestNO->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);	
			if(!pRsTestNO->adoEOF)
				pRsTestNO->MoveLast();		  
			CString strCnt;
			strCnt.Format(_T("%03d"),RecordCount+1);
			AutoNo = sName+strStartDate+ strCnt;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
	}
	if(pRsTestNO->State == adStateOpen)
		pRsTestNO->Close();

	//TODO:ѭ�������Ʒ���
	//m_szTestAutoNO = AutoNo;
}

CGyfxyRDB_BaseRdb::ALLSample::ALLSample()
{ 
	try
	{  
		pRsTempDB.CreateInstance("ADODB.Recordset"); 
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	/*SELECT ��Ʒ���
	FROM ������ʵ������
		GROUP BY ��Ʒ���
		*/
}


void CGyfxyRDB_BaseRdb::ALLSample::SetInstName(CString strInstName) 
{
	IEDResult::SetInstName(strInstName);
	for(int i=0;i<iSampleCnt;i++)
		Sample[i].m_strInstName = strInstName;
}


void CGyfxyRDB_BaseRdb::ALLSample::UpdateResult2TempTab(int cSamplePos)
{
	//Ϊ0 ����̨�豸����ʱ������ȫ���ָ�ΪĬ������  ��ӿռ�¼
	if(cSamplePos>0)
	{
		AppendResult2TempTab(cSamplePos);
	} 
	else
	{
		ClearTempTab() ;
	}
	//
}

void CGyfxyRDB_BaseRdb::ALLSample::ClearTempTab(void) //�豸����
{
	//SetInstName(m_strInstName);
	try
	{ 
		if(pRsTempDB->State == adStateOpen) 
			pRsTempDB->Close(); 
		strTempDBSQL.Format(_T("delete * from ������ʱ���ݱ� where �豸����='%s'"),this->m_szName) ;  //������ݱ�������
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(), 
			adOpenStatic,
			adLockOptimistic,   
			-1);  
		try
		{ 
			if(pRsTempDB->State == adStateOpen)
				pRsTempDB->Close();
			//��ӿհ׼�¼ 
			strTempDBSQL.Format(_T("select * from ������ʱ���ݱ� where �豸����='%s'"),this->m_szName) ;  //������ݱ�������
			pRsTempDB->CursorLocation = adUseClientBatch;   //adUseServer ֻ֧��һ������
			pRsTempDB->Open(strTempDBSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockBatchOptimistic, 
				adCmdText);   

			if(iSampleCnt>MAX_SAMPLE_CNT)
				iSampleCnt = MAX_SAMPLE_CNT;
			for (int i=1;i<=iSampleCnt;i++)
			{ 
				pRsTempDB->AddNew();
				UpdateTempTabField(i);    
				
			}  
			pRsTempDB->UpdateBatch(adAffectAll);   
			if(pRsTempDB->State == adStateOpen)
				pRsTempDB->Close(); 
		} 
		catch(_com_error &e)
		{
			dump_com_error(e);
			pRsTempDB->CancelBatch(adAffectAll);
		}

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void CGyfxyRDB_BaseRdb::ALLSample::CreateTempTab(void)
{ 
	//
	//TODO:ò�����ָ��õİ취,���������ݿ�������һ����
	strTempDBSQL = _T("select *  into ������ʱ���ݱ� from ��ҵ���������� where 1=2") ; 
	try
	{ 
		if(pRsTempDB->State == adStateOpen) 
			pRsTempDB->Close(); 
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close();

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void CGyfxyRDB_BaseRdb::ALLSample::LoadSampleFromTempTab(int cSamplePos)
{
	cSamplePos = cSamplePos-1;
	Sample[cSamplePos].m_strTestAutoNO	= VariantToStr(pRsTempDB->Fields->GetItem(_T("������"))->Value);
	Sample[cSamplePos].cSampleName		= VariantToStr(pRsTempDB->Fields->GetItem(_T("��Ʒ���"))->Value);
	Sample[cSamplePos].m_strInstName	= VariantToStr(pRsTempDB->Fields->GetItem(_T("�豸����"))->Value);
	Sample[cSamplePos].m_TestType		= VariantToStr(pRsTempDB->Fields->GetItem(_T("����"))->Value);

	Sample[cSamplePos].m_SampleWeight	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("��������"))->Value),NULL);
	Sample[cSamplePos].SampleNO			= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("��λ"))->Value),NULL,10);
	Sample[cSamplePos].m_fMad			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("ˮ��"))->Value),NULL); 
	Sample[cSamplePos].m_fVad			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("�ӷ���"))->Value),NULL);
	Sample[cSamplePos].m_fAad			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("�ҷ�"))->Value),NULL);
	Sample[cSamplePos].m_bFix			= V_BOOL(&pRsTempDB->Fields->GetItem(_T("����"))->Value)?TRUE:FALSE;
	Sample[cSamplePos].m_sD				= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("D"))->Value),NULL,10);
	Sample[cSamplePos].m_fCO2			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("CO2"))->Value),NULL);
	Sample[cSamplePos].m_fBrezeCO2		= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("����CO2"))->Value),NULL); 

	Sample[cSamplePos].UQuality.Quality[0]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("��������"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[1]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("��Ʒ����"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[2]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("ˮ������"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[3]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("�ӷ���ǰ����"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[4]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("�ӷ�������"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[5]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("�ҷ����� "))->Value),NULL);
	Sample[cSamplePos].m_fMineral			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("������"))->Value),NULL);
	Sample[cSamplePos].cSta					= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("������Ŀ"))->Value),NULL,10);
	Sample[cSamplePos].cCSta				= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("�������Ŀ"))->Value),NULL,10);  
}

void CGyfxyRDB_BaseRdb::ALLSample::UpdateTempTabField(int cSamplePos)
{ 
	cSamplePos = cSamplePos-1;
	pRsTempDB->PutCollect("������",_variant_t(Sample[cSamplePos].m_strTestAutoNO));		
	pRsTempDB->PutCollect("��Ʒ���",_variant_t(Sample[cSamplePos].cSampleName));
	pRsTempDB->PutCollect("�豸����",_variant_t(Sample[cSamplePos].m_strInstName));
	pRsTempDB->PutCollect("����",_variant_t(Sample[cSamplePos].m_TestType)); 
	pRsTempDB->PutCollect("��Ʒ����",_variant_t(Sample[cSamplePos].m_SampleWeight));
	pRsTempDB->PutCollect("��������",_variant_t(Sample[cSamplePos].m_dtTestDateTime));
	
	pRsTempDB->PutCollect("ˮ��",_variant_t(Sample[cSamplePos].m_fMad));
	pRsTempDB->PutCollect("�ӷ���",_variant_t(Sample[cSamplePos].m_fVad));
	pRsTempDB->PutCollect("�ҷ�",_variant_t(Sample[cSamplePos].m_fAad)); 
	pRsTempDB->PutCollect("����",_variant_t(Sample[cSamplePos].m_bFix));
	pRsTempDB->PutCollect("D",_variant_t(Sample[cSamplePos].m_sD));
	pRsTempDB->PutCollect("CO2",_variant_t(Sample[cSamplePos].m_fCO2));
	pRsTempDB->PutCollect("����CO2",_variant_t(Sample[cSamplePos].m_fBrezeCO2)); 
	pRsTempDB->PutCollect("��������",_variant_t(Sample[cSamplePos].UQuality.Quality[0]));
	pRsTempDB->PutCollect("��Ʒ����",_variant_t(Sample[cSamplePos].UQuality.Quality[1]));
	pRsTempDB->PutCollect("ˮ������",_variant_t(Sample[cSamplePos].UQuality.Quality[2]));
	pRsTempDB->PutCollect("�ӷ���ǰ����",_variant_t(Sample[cSamplePos].UQuality.Quality[3]));
	pRsTempDB->PutCollect("�ӷ�������",_variant_t(Sample[cSamplePos].UQuality.Quality[4]));
	pRsTempDB->PutCollect("�ҷ�����",_variant_t(Sample[cSamplePos].UQuality.Quality[5])); 
	pRsTempDB->PutCollect("������",_variant_t(Sample[cSamplePos].m_fMineral)); 
	pRsTempDB->PutCollect("������Ŀ",_variant_t(Sample[cSamplePos].cSta)); 
	pRsTempDB->PutCollect("�������Ŀ",_variant_t(Sample[cSamplePos].cCSta)); 
	pRsTempDB->PutCollect("��λ",_variant_t(Sample[cSamplePos].SampleNO));  //����������λ����
}



void CGyfxyRDB_BaseRdb::ALLSample::AppendResult2TempTab(int cSamplePos)
{ 
	int Recordcnt;
	try
	{
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close();
		//��ӿհ׼�¼ 
		strTempDBSQL.Format(_T("select * from ������ʱ���ݱ� where �豸����='%s' and ��λ = %d"),m_szName,cSamplePos) ;  //������ݱ�������
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,   
			-1);
		Recordcnt = pRsTempDB->RecordCount;
		if(Recordcnt>0)
		{
			pRsTempDB->MoveFirst();
			UpdateTempTabField(cSamplePos);
			pRsTempDB->Update();
		}
		else
		{
			TRACE(_T("����λ������!\n")); 
		}
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	} 
}

void CGyfxyRDB_BaseRdb::ALLSample::LoadResultFromTempTab(int cSamplePos)
{ 
	try
	{ 
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close(); 
		strTempDBSQL.Format(_T("select * from ������ʱ���ݱ� where �豸����='%s' ORDER BY ��λ DESC"),this->m_szName) ;  //������ݱ�������
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,   
			-1);    

		if(!pRsTempDB->adoEOF)
			pRsTempDB->MoveLast();
		int i = 0;

		if(iSampleCnt>MAX_SAMPLE_CNT)
			iSampleCnt = MAX_SAMPLE_CNT;
		for (int i=1;i<=iSampleCnt;i++)
		while((!pRsTempDB->BOF)&&(i++<iSampleCnt))
		{		 
			LoadSampleFromTempTab(i);
			pRsTempDB->MovePrevious();
		}   
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close(); 

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void CGyfxyRDB_BaseRdb::ALLSample::SaveResult2DB(void)
{
	// 
	
	strTempDBSQL.Format(_T("INSERT into ��ҵ����������\
							select * from ������ʱ���ݱ�\
							where �豸����='%s' and  ������Ŀ>0"),m_szName);

	try
	{ 
		if(pRsTempDB->State == adStateOpen) 
			pRsTempDB->Close(); 
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close();
		strTempDBSQL.Format(_T("delete from ������ʱ���ݱ�\
							   where �豸����='%s'"),m_szName);
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close();

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	
	//ASSERT(FALSE); 
	//UpdateBatch
}

void CGyfxyRDB_BaseRdb::ALLSample::UpdateResultToDB(void)
{
	ASSERT(FALSE); 
}

void CGyfxyRDB_BaseRdb::ALLSample::CalResult(CImageSlave *pClyImageSlave)
{
	ASSERT(FALSE); 
}

void CGyfxyRDB_BaseRdb::ALLSample::BatchUpdateDB2TempTab(CString m_strInstName)
{ 
	_RecordsetPtr pRsTestNO;
	CString strSQL;
	{	
		strSQL = _T("select * from ������ʱ���ݱ� where 1=2") ; 
		try  
		{  
			pRsTestNO.CreateInstance("ADODB.Recordset");	
			pRsTestNO->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenKeyset,
				adLockBatchOptimistic,      //�������±����趨�������ͺ��α�����
				-1); 
			//DO something
			if(pRsTestNO->State == adStateOpen)
				pRsTestNO->Close();
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}     
	}
}
		/*
		Dim strMessage As String

		' Assign connection string to variable.
		strCnn = "Provider=sqloledb;" & _
		"Data Source=srv;Initial Catalog=pubs;User Id=sa;Password=; "

		Set rstTitles = New ADODB.Recordset
		rstTitles.CursorType = adOpenKeyset
		rstTitles.LockType = adLockBatchOptimistic
		rstTitles.Open "titles", strCnn, , , adCmdTable

		rstTitles.MoveFirst

		' Loop through recordset, and prompt user for 
		' change of type for a specified title.
		Do Until rstTitles.EOF
		If Trim(rstTitles!Type) = "psychology" Then
		strTitle = rstTitles!Title
		strMessage = "Title: " & strTitle & vbCr & _
		"Change type to self help?"

		If MsgBox(strMessage, vbYesNo) = vbYes Then
		rstTitles!Type = "self_help"
		End If
		End If

		rstTitles.MoveNext
		Loop

		' Ask if the user wants to commit to all the 
		' changes made earlier.
		If MsgBox("Save all changes?", vbYesNo) = vbYes Then
		rstTitles.UpdateBatch
		Else
		rstTitles.CancelBatch
		End If

		' Print current data in recordset.
		rstTitles.Requery
		rstTitles.MoveFirst
		Do While Not rstTitles.EOF
		Debug.Print rstTitles!Title & " - " & rstTitles!Type
		rstTitles.MoveNext
		Loop

		' Restore original values because this is a demonstration.
		rstTitles.MoveFirst
		Do Until rstTitles.EOF
		If Trim(rstTitles!Type) = "self_help" Then
		rstTitles!Type = "psychology"
		End If
		rstTitles.MoveNext
		Loop
		rstTitles.UpdateBatch

		rstTitles.Close

		End Sub
		*/ 

void CGyfxyRDB_BaseRdb::ALLSample::S_Sample::CalResultMAV()
{	
	S_Sample calSample(*this); //����һ�������������޸�ԭʼ��¼
	double m_dWaterWeight = 0;
	double m_dVolWeightWithCover  = 0 ;
	double m_dVolWeigth = 0 ;
	//TODO: �������У׼
	/* 
	///////////////////////////////////////////////////////////////
	double  lM0;      //����ǰ��Ʒ����(unit=0.1mg),������������
	double  lM1;      //ˮ���������Ʒ����(unit=0.1mg),������������
	/////////////////////////////////////////////////////////////////
	double  lMPotSample2;//�ӷ���ʵ��ǰ�Ӹ�����
	///////////////////////////////////////////////////////////////
	double  lM2;      //�ӷ����������Ʒ����(unit=0.1mg),������������
	double  lM3;      //�ҷ��������Ʒ����(unit=0.1mg),������������
	*/
	if(cCSta&(1<<S_WEIGHT_SAMPLE))
	{ 
		m_SampleWeight = calSample.UQuality.SQuality.lM0 - calSample.UQuality.SQuality.lMPot;  //��Ʒ���� 
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	if(cCSta&(1<<S_TEST_WATER))	
	{ 
		m_dWaterWeight  = calSample.UQuality.SQuality.lM0 - calSample.UQuality.SQuality.lM1;  //ˮ�ּ������� 
		m_fMad = m_dWaterWeight/m_SampleWeight*100 ;		//ˮ�ְٷֱ�
	} 
	else
		m_fMad = 0;
	////////////////////////////////////////////////////////////////////////////////////////////
	if(cCSta&(1<<S_TEST_VOL))	
	{ 
			
		if(cCSta&(1<<S_WEIGHT_COVER))
		{//�������Ӹ�,.........	
			m_dVolWeightWithCover  = calSample.UQuality.SQuality.lMPotSample2 - calSample.UQuality.SQuality.lM2 ;  //
			m_fVad = m_dVolWeightWithCover /m_SampleWeight*100 ;
		}
		else
		{	// ����ǲ�����Ӹ�,ֱ�Ӽ�����������Ʒ���� 		
			m_dVolWeigth  = calSample.UQuality.SQuality.lM0 - calSample.UQuality.SQuality.lM2   ;  //�ӷ��ּ�������
			m_fVad = m_dVolWeigth /m_SampleWeight*100 - m_fMad;
		} 
		
	}
	else
	{
		m_fVad = 0;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	if(cCSta&(1<<S_TEST_ASH))	
	{ 
		double m_dAshWeigth  =  calSample.UQuality.SQuality.lM3 -  calSample.UQuality.SQuality.lMPot ;  //��Ʒ����
		m_fAad = m_dAshWeigth /m_SampleWeight*100 ;
	}
	else
	{
		m_fAad = 0;
	}
}

void CGyfxyRDB_BaseRdb::ALLSample::S_Sample::LoadResultFromDB(CString strAutoNO)
{

}

void CGyfxyRDB_BaseRdb::ALLSample::CalcAllResult()
{

}

void CGyfxyRDB_BaseRdb::ALLSample::CalcResult(int cSamplePos)
{
	if(cSamplePos>0)
		Sample[cSamplePos-1].CalResultMAV();  
}

void CGyfxyRDB_BaseRdb::ALLSample::LoadResultFromDB(CString strAutoNO)
{
	ASSERT(FALSE); 
}

void CGyfxyRDB_BaseRdb::ALLSample::DelResultFromDB(CString strAutoNO)
{
	ASSERT(FALSE); 
}

