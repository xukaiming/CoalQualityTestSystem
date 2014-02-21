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
		
		m_szName			= VariantToStr(pRsDev->Fields->GetItem("名称")->Value);;				//设备名称
		m_szType 			= VariantToStr(pRsDev->Fields->GetItem("型号")->Value);	//设备类型
		m_bID				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("标识"))->Value),NULL,10  );;		//ID号
		m_sComPort			= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("串行口号"))->Value),NULL,10);;;				//串口号	
		m_lScanPriod_mS		= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("扫描周期(mS)"))->Value),NULL,10);     //扫描周期;
		
		attrib.m_sTimePut2weigh			= pRsDev->Fields->GetItem(_T("放样到称样时间(0_01S)"))->Value.lVal;//;	SHORT
		attrib.m_sTimePut2HighTemp		= pRsDev->Fields->GetItem(_T("放样到高温时间(0_01S)"))->Value.lVal;//SHORT
		attrib.m_sPulsePerRound1Spit	= pRsDev->Fields->GetItem(_T("样盘一圈脉冲数"))->Value.iVal;//;		SHORT
		attrib.m_btDriverSpitCnt		= pRsDev->Fields->GetItem(_T("驱动器细分数"))->Value.bVal;;//;		BYTE
		attrib.m_btMaxSampleCnt			= pRsDev->Fields->GetItem(_T("样盘的样位个数"))->Value.bVal;;//	BYTE
		attrib.m_fBalanceMaxErr			= pRsDev->Fields->GetItem(_T("称样判值(g)"))->Value.fltVal;;//;FLOAT

		attrib.m_fVolSampleMax			= pRsDev->Fields->GetItem(_T("挥发分样品重量上限(g)"))->Value.fltVal;;//;FLOAT
		attrib.m_fVolSampleMin			= pRsDev->Fields->GetItem(_T("挥发分样品重量下限(g)"))->Value.fltVal;;//;FLOAT
		attrib.m_fWaterAshSampleMax		= pRsDev->Fields->GetItem(_T("水分灰分样品重量上限(g)"))->Value.fltVal;;//;FLOAT
		attrib.m_fWaterAshSampleMin		= pRsDev->Fields->GetItem(_T("水分灰分样品重量下限(g)"))->Value.fltVal;;//;FLOAT
		//SHORT
		attrib.m_sLowStoveTempSpeed		= pRsDev->Fields->GetItem(_T("低温炉升温速率"))->Value.iVal;;//
		attrib.m_sHighStoveTempSpeed	= pRsDev->Fields->GetItem(_T("高温炉升温速率"))->Value.iVal;;//;
		attrib.m_sQuick_WaterTestTime	= pRsDev->Fields->GetItem(_T("快速水分干燥时间(分)"))->Value.iVal;;//;
		attrib.m_sQuick_VolTestTime		= pRsDev->Fields->GetItem(_T("快速挥发分干燥时间(分)"))->Value.iVal;;//;
		attrib.m_sQuick_AshTestTime		= pRsDev->Fields->GetItem(_T("快速灰分干燥时间(分)"))->Value.iVal;;//;
		attrib.m_sGB_WaterTestTime		= pRsDev->Fields->GetItem(_T("国标水分干燥时间(分)"))->Value.iVal;;//;
		attrib.m_sGB_VolTestTime		= pRsDev->Fields->GetItem(_T("国标挥发分干燥时间(分)"))->Value.iVal;;//;
		attrib.m_sGB_AshTestTime		= pRsDev->Fields->GetItem(_T("国标灰分干燥时间(分)"))->Value.iVal;;//;
		attrib.m_sQuick_WaterTestTemp	= pRsDev->Fields->GetItem(_T("快速水分试验温度(℃)"))->Value.iVal;;//;
		attrib.m_sQuick_VolTestTemp		= pRsDev->Fields->GetItem(_T("快速挥发分试验温度(℃)"))->Value.iVal;;//;
		attrib.m_sQuick_AshTestTemp		= pRsDev->Fields->GetItem(_T("快速灰分试验温度(℃)"))->Value.iVal;;//;
		attrib.m_sGB_WaterTestTemp		= pRsDev->Fields->GetItem(_T("国标水分试验温度(℃)"))->Value.iVal;;//;
		attrib.m_sGB_VolTestTemp		= pRsDev->Fields->GetItem(_T("国标挥发分试验温度(℃)"))->Value.iVal;;//;
		attrib.m_sGB_AshTestTemp		= pRsDev->Fields->GetItem(_T("国标灰分试验温度(℃)"))->Value.iVal;;
		attrib.m_sWaterCoolTime			= pRsDev->Fields->GetItem(_T("水分炉坩埚冷却时间"))->Value.iVal;;;		//水分炉坩埚冷却时间
		attrib.m_sHighTempCoolTime		= pRsDev->Fields->GetItem(_T("高温炉坩埚冷却时间"))->Value.iVal;;;	//高温测试冷却时间
		//
		attrib.m_sBeepTime				= pRsDev->Fields->GetItem(_T("蜂鸣时间(mS)"))->Value.iVal;;
		//
		attrib.m_bFix					= pRsDev->Fields->GetItem(_T("修正"))->Value.boolVal;;//VARIANT_BOOL
		attrib.m_szalgorithm			= VariantToStr(pRsDev->Fields->GetItem(_T("算法"))->Value);;//CString
		attrib.m_bAutoPrint				= pRsDev->Fields->GetItem(_T("自动打印"))->Value.boolVal;;//VARIANT_BOOL
		attrib.m_szTestSequence			= VariantToStr(pRsDev->Fields->GetItem(_T("试验顺序"))->Value);;     //测试顺序CString
		attrib.m_fCrucibleCoeff1		= pRsDev->Fields->GetItem(_T("坩埚系数1"))->Value.fltVal;;//坩埚系数1;
		attrib.m_fCrucibleCoeff2		= pRsDev->Fields->GetItem(_T("坩埚系数2"))->Value.fltVal;;
		attrib.m_fCrucibleCoeff3		= pRsDev->Fields->GetItem(_T("坩埚系数3"))->Value.fltVal;;

		attrib.m_szSystemType			= VariantToStr(pRsDev->Fields->GetItem("设备类型")->Value);;
		//attrib.m_bdisperse				= pRsDev->Fields->GetItem(_T("disperse"))->Value.boolVal;;
	}
	else
	{
		bResult = FALSE;
		::MessageBox(theApp.m_pMainWnd->m_hWnd,_T("无法读取设备属性，初始化失败！"),_T("警告"),MB_OK+MB_ICONERROR);
	}
	return TRUE;
}

void CGyfxyRDB_BaseRdb::InitRDB(CGyfxyImageSlave* ImageSlave)
{
	enum enum_IED_type{G_NONE = 0,G5000,G5200,G5500}; 
	CGyfxyImageSlave::SDownParam *pImg = &ImageSlave->DownloadWorkParam;
	memset(pImg,0,sizeof(CGyfxyImageSlave::SDownParam));
	pImg->m_sTimePut2weigh		= attrib.m_sTimePut2weigh;		//放样到称样时间(0_1S);
	pImg->m_sTimePut2HighTemp	= attrib.m_sTimePut2HighTemp;	//放样到高温时间(0_1S);

	pImg->m_sPulsePerRound1Spit	= attrib.m_sPulsePerRound1Spit;	//样盘一圈脉冲数;
	pImg->m_btDriverSpitCnt		= attrib.m_btDriverSpitCnt;		//驱动器细分数;
	pImg->m_btMaxSampleCnt		= attrib.m_btMaxSampleCnt;		//样位个数

	pImg->m_fBalanceMaxErr		= attrib.m_fBalanceMaxErr*10000L;		//称样判值(0.1mg);	

	pImg->m_fVolSampleMax			= attrib.m_fVolSampleMax*10000L; 
	pImg->m_fVolSampleMin			= attrib.m_fVolSampleMax*10000L; 
	pImg->m_fWaterAshSampleMax		= attrib.m_fWaterAshSampleMax*10000L; 
	pImg->m_fWaterAshSampleMin		= attrib.m_fWaterAshSampleMin*10000L; 

//	pImg->m_sLowStoveTempSpeed	= attrib.m_sLowStoveTempSpeed;	//低温炉升温速率
//	pImg->m_sHighStoveTempSpeed	= attrib.m_sHighStoveTempSpeed;	//高温炉升温速率;
/*
	pImg->m_sQuick_WaterTestTime= attrib.m_sQuick_WaterTestTime*60;//快速水分干燥时间(分);
	pImg->m_sQuick_VolTestTime	= attrib.m_sQuick_VolTestTime*60;	//快速挥发份干燥时间(分)
	pImg->m_sQuick_AshTestTime	= attrib.m_sQuick_AshTestTime*60;	//快速灰分干燥时间(分);

	pImg->m_sGB_WaterTestTime	= attrib.m_sGB_WaterTestTime*60;	//国标水分干燥时间(分);
	pImg->m_sGB_VolTestTime		= attrib.m_sGB_VolTestTime*60;		//国标挥发分干燥时间(分);
	pImg->m_sGB_AshTestTime		= attrib.m_sGB_AshTestTime*60;		//国标灰分干燥时间(分);	
	//pImg->m_szTestSequence		= 
*/
	if(attrib.m_szalgorithm!=_T("国标"))
	{
		pImg->m_sWaterTestTime		= attrib.m_sGB_WaterTestTime*60;	//国标水分干燥时间(分);
		pImg->m_sVolTestTime		= attrib.m_sGB_VolTestTime*60;		//国标挥发分干燥时间(分);
		pImg->m_sAshTestTime		= attrib.m_sGB_AshTestTime*60;		//国标灰分干燥时间(分);			pImg->m_sWaterTestTemp	= TemptoAD(attrib.m_sQuick_WaterTestTemp);//快速水分试验温度(℃);
		
		pImg->m_sVolTestTemp	= TemptoAD(attrib.m_sQuick_VolTestTemp);	//快速挥发分试验温度(℃);
		pImg->m_sAshTestTemp	= TemptoAD(attrib.m_sQuick_AshTestTemp);	//快速灰分试验温度(℃);
	}
	else
	{
		pImg->m_sWaterTestTime	= attrib.m_sQuick_WaterTestTime*60;//快速水分干燥时间(分);
		pImg->m_sVolTestTime	= attrib.m_sQuick_VolTestTime*60;	//快速挥发份干燥时间(分)
		pImg->m_sAshTestTime	= attrib.m_sQuick_AshTestTime*60;	//快速灰分干燥时间(分);

		pImg->m_sWaterTestTemp	= TemptoAD(attrib.m_sGB_WaterTestTemp);	//国标水分试验温度(℃);
		pImg->m_sVolTestTemp	= TemptoAD(attrib.m_sGB_VolTestTemp);		//国标挥发分试验温度(℃);
		pImg->m_sAshTestTemp	= TemptoAD(attrib.m_sGB_AshTestTemp);		//国标灰分试验温度(℃);
	} 

	pImg->m_sWaterCoolTime		= attrib.m_sWaterCoolTime*60;;		//水分炉坩埚冷却时间
	pImg->m_sHighTempCoolTime	= attrib.m_sHighTempCoolTime*60;;	//高温炉坩埚冷却时间
	pImg->m_sBeepTime			= attrib.m_sBeepTime;			//蜂鸣时间
	if(attrib.m_szalgorithm==_T("国标"))
	{ 
		pImg->m_szalgorithm = CGyfxyImageSlave::SDownParam::M_GB;
	}
	else
	{
		pImg->m_szalgorithm = CGyfxyImageSlave::SDownParam::M_QK;
	}
	////////////////////////////////////////
	//设置工业分析仪仪器类型
	if(attrib.m_szSystemType==_T("G5000"))
		pImg->m_bIED_system_type = G5000;
	else if(attrib.m_szSystemType==_T("G5200"))
		pImg->m_bIED_system_type = G5200;
	else if(attrib.m_szSystemType==_T("G5500"))
		pImg->m_bIED_system_type = G5500;
	else  
		pImg->m_bIED_system_type = G_NONE;
	//pImg->m_szTestSequence[8]	= attrib.;     //测试顺序
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


int CGyfxyRDB_BaseRdb::ALLSample::QualityWater = 5;    //水分残重位置
int CGyfxyRDB_BaseRdb::ALLSample::ResultWater  = 2;		//水分计算结果位置	


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
		Sample[i].cSta =  3; //称样品 称坩埚
		*/
		if(i<2)
			Sample[i].cSampleName = _T("挥发分校准坩埚");
		else if(i<4)
			Sample[i].cSampleName = _T("水分/灰分校准坩埚");
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
				pSample[cSamplePos-1].cStabState&(1L<<i)? //不稳定的样品背景色变红
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
	ASSERT(!((EnableMask+DisableMask)^(EnableMask|DisableMask)));  //如果同时出现在 EnableMask/DisableMask  会报错了 
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
		Sample[SampleNO].cSta=0x03|           //需要测试项目,证明该位置有坩埚 样品
			(Sample[SampleNO].cItem&0x1?0x04:0)| //水分
											   //挥发钙盖子
			(Sample[SampleNO].cItem&0x2?0x10:0)| //挥发分
			(Sample[SampleNO].cItem&0x4?0x20:0);//灰分
		 
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
	SampleNO-=1;        //这里没有减一,造成指针越界,搞死我了
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
	memcpy(status.TempAdValue,pImageSlave->ADValue.ad,sizeof(status.TempAdValue));		//炉子温度AD值
	status.InstTempValue	= pImageSlave->ADValue.adInstTemp;			//环境温度
	for (int i=0;i<3;i++)
	{
		status.TempVoltage[i]	= ADToVol(status.TempAdValue[i])/1000;		//炉子温度电压			
		status.Temperature[i]	= VolToTemp(status.TempVoltage[i]);
		status.m_bWarmFlag[i]	= pImageSlave->fuzzy[i].WarmFlag;
		status.du[i]			= pImageSlave->fuzzy[i].du;

		unsigned short *pin		= &pImageSlave->InputStatus.cInputReg16;
		status.cCurrPos[i]		= (((*pin)>>(i*2+8))&0x3);					//输入状态  得到当前位置
		status.strCurrPos[i]	= status.cCurrPos[i]==0x03?_T("未知"):
									status.cCurrPos[i]==0x2?_T("到顶"):_T("到底");	
		unsigned short *pOut	= &pImageSlave->OutputCoil.Coil16;		//输出状态  得到运动状态和方向
		status.cMoving[i]		= (((*pOut)>>(i*2))&0x3);					//输入状态  得到当前位置
		status.strMov[i]		= status.cMoving[i]==0x00?_T("停止"):
									status.cMoving[i]==0x01?_T("上升"):_T("下降");

	} 
	status.CurrWeight	= ((double)pImageSlave->Balance.CurrWeight)/10000; 
	//////////////////////////////////////  	
	/////////////////////////////////////////////////	  
	/////////////////////////////////////////////////
	int     cTestState;
	/////////////////////////////////////////////////
	status.cSampleDiskCurrPos	= pImageSlave->MtrState.CurSamplePos;
	status.bSampleDiskMoving	= pImageSlave->MtrState.bRun;
	CString strSampleDiskCurrPos;    //样盘当前位置

	SampleArray.RefreshSampleData(pImageSlave);
	///
	//BOOL m_bStartTest;
	//BOOL m_bReseted;   

}



void CGyfxyRDB_BaseRdb::ALLSample::BuildAutoNO()
{ 
	COleDateTime  dtEnd;									//查询结束日期
	COleDateTime  dtStart=COleDateTime::GetCurrentTime( );  //得到今天的日期
	CString strStartDate,strEndDate,AutoNo;   //开始日期,结束日期,自动编号
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
		//查询已有 记录 		
		strSQL.Format(
			_T("select 试验编号 from 工业分析仪数据 where 试验编号 like '%%%s%%' and 试验日期  between  #%s# and #%s# and 设备名称='%s' order by 试验编号"), 
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

	//TODO:循环添加样品编号
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
	/*SELECT 样品编号
	FROM 测硫仪实验数据
		GROUP BY 样品编号
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
	//为0 将该台设备的临时计算结果全部恢复为默认设置  添加空记录
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

void CGyfxyRDB_BaseRdb::ALLSample::ClearTempTab(void) //设备名称
{
	//SetInstName(m_strInstName);
	try
	{ 
		if(pRsTempDB->State == adStateOpen) 
			pRsTempDB->Close(); 
		strTempDBSQL.Format(_T("delete * from 工分临时数据表 where 设备名称='%s'"),this->m_szName) ;  //清除数据表中数据
		pRsTempDB->Open(strTempDBSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(), 
			adOpenStatic,
			adLockOptimistic,   
			-1);  
		try
		{ 
			if(pRsTempDB->State == adStateOpen)
				pRsTempDB->Close();
			//添加空白记录 
			strTempDBSQL.Format(_T("select * from 工分临时数据表 where 设备名称='%s'"),this->m_szName) ;  //清除数据表中数据
			pRsTempDB->CursorLocation = adUseClientBatch;   //adUseServer 只支持一个更新
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
	//TODO:貌似有种更好的办法,可以连数据库表的属性一起复制
	strTempDBSQL = _T("select *  into 工分临时数据表 from 工业分析仪数据 where 1=2") ; 
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
	Sample[cSamplePos].m_strTestAutoNO	= VariantToStr(pRsTempDB->Fields->GetItem(_T("试验编号"))->Value);
	Sample[cSamplePos].cSampleName		= VariantToStr(pRsTempDB->Fields->GetItem(_T("样品编号"))->Value);
	Sample[cSamplePos].m_strInstName	= VariantToStr(pRsTempDB->Fields->GetItem(_T("设备名称"))->Value);
	Sample[cSamplePos].m_TestType		= VariantToStr(pRsTempDB->Fields->GetItem(_T("方法"))->Value);

	Sample[cSamplePos].m_SampleWeight	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("试验日期"))->Value),NULL);
	Sample[cSamplePos].SampleNO			= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("样位"))->Value),NULL,10);
	Sample[cSamplePos].m_fMad			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("水分"))->Value),NULL); 
	Sample[cSamplePos].m_fVad			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("挥发分"))->Value),NULL);
	Sample[cSamplePos].m_fAad			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("灰分"))->Value),NULL);
	Sample[cSamplePos].m_bFix			= V_BOOL(&pRsTempDB->Fields->GetItem(_T("修正"))->Value)?TRUE:FALSE;
	Sample[cSamplePos].m_sD				= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("D"))->Value),NULL,10);
	Sample[cSamplePos].m_fCO2			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("CO2"))->Value),NULL);
	Sample[cSamplePos].m_fBrezeCO2		= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("焦渣CO2"))->Value),NULL); 

	Sample[cSamplePos].UQuality.Quality[0]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("坩埚质量"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[1]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("样品质量"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[2]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("水分质量"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[3]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("挥发份前质量"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[4]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("挥发分质量"))->Value),NULL);
	Sample[cSamplePos].UQuality.Quality[5]	= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("灰分质量 "))->Value),NULL);
	Sample[cSamplePos].m_fMineral			= _tcstod(VariantToStr(pRsTempDB->Fields->GetItem(_T("矿物质"))->Value),NULL);
	Sample[cSamplePos].cSta					= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("测试项目"))->Value),NULL,10);
	Sample[cSamplePos].cCSta				= _tcstol(VariantToStr(pRsTempDB->Fields->GetItem(_T("已完成项目"))->Value),NULL,10);  
}

void CGyfxyRDB_BaseRdb::ALLSample::UpdateTempTabField(int cSamplePos)
{ 
	cSamplePos = cSamplePos-1;
	pRsTempDB->PutCollect("试验编号",_variant_t(Sample[cSamplePos].m_strTestAutoNO));		
	pRsTempDB->PutCollect("样品编号",_variant_t(Sample[cSamplePos].cSampleName));
	pRsTempDB->PutCollect("设备名称",_variant_t(Sample[cSamplePos].m_strInstName));
	pRsTempDB->PutCollect("方法",_variant_t(Sample[cSamplePos].m_TestType)); 
	pRsTempDB->PutCollect("样品质量",_variant_t(Sample[cSamplePos].m_SampleWeight));
	pRsTempDB->PutCollect("试验日期",_variant_t(Sample[cSamplePos].m_dtTestDateTime));
	
	pRsTempDB->PutCollect("水分",_variant_t(Sample[cSamplePos].m_fMad));
	pRsTempDB->PutCollect("挥发分",_variant_t(Sample[cSamplePos].m_fVad));
	pRsTempDB->PutCollect("灰分",_variant_t(Sample[cSamplePos].m_fAad)); 
	pRsTempDB->PutCollect("修正",_variant_t(Sample[cSamplePos].m_bFix));
	pRsTempDB->PutCollect("D",_variant_t(Sample[cSamplePos].m_sD));
	pRsTempDB->PutCollect("CO2",_variant_t(Sample[cSamplePos].m_fCO2));
	pRsTempDB->PutCollect("焦渣CO2",_variant_t(Sample[cSamplePos].m_fBrezeCO2)); 
	pRsTempDB->PutCollect("坩埚质量",_variant_t(Sample[cSamplePos].UQuality.Quality[0]));
	pRsTempDB->PutCollect("样品质量",_variant_t(Sample[cSamplePos].UQuality.Quality[1]));
	pRsTempDB->PutCollect("水分质量",_variant_t(Sample[cSamplePos].UQuality.Quality[2]));
	pRsTempDB->PutCollect("挥发份前质量",_variant_t(Sample[cSamplePos].UQuality.Quality[3]));
	pRsTempDB->PutCollect("挥发分质量",_variant_t(Sample[cSamplePos].UQuality.Quality[4]));
	pRsTempDB->PutCollect("灰分质量",_variant_t(Sample[cSamplePos].UQuality.Quality[5])); 
	pRsTempDB->PutCollect("矿物质",_variant_t(Sample[cSamplePos].m_fMineral)); 
	pRsTempDB->PutCollect("测试项目",_variant_t(Sample[cSamplePos].cSta)); 
	pRsTempDB->PutCollect("已完成项目",_variant_t(Sample[cSamplePos].cCSta)); 
	pRsTempDB->PutCollect("样位",_variant_t(Sample[cSamplePos].SampleNO));  //批量更新样位出错
}



void CGyfxyRDB_BaseRdb::ALLSample::AppendResult2TempTab(int cSamplePos)
{ 
	int Recordcnt;
	try
	{
		if(pRsTempDB->State == adStateOpen)
			pRsTempDB->Close();
		//添加空白记录 
		strTempDBSQL.Format(_T("select * from 工分临时数据表 where 设备名称='%s' and 样位 = %d"),m_szName,cSamplePos) ;  //清除数据表中数据
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
			TRACE(_T("该样位无数据!\n")); 
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
		strTempDBSQL.Format(_T("select * from 工分临时数据表 where 设备名称='%s' ORDER BY 样位 DESC"),this->m_szName) ;  //清除数据表中数据
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
	
	strTempDBSQL.Format(_T("INSERT into 工业分析仪数据\
							select * from 工分临时数据表\
							where 设备名称='%s' and  测试项目>0"),m_szName);

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
		strTempDBSQL.Format(_T("delete from 工分临时数据表\
							   where 设备名称='%s'"),m_szName);
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
		strSQL = _T("select * from 工分临时数据表 where 1=2") ; 
		try  
		{  
			pRsTestNO.CreateInstance("ADODB.Recordset");	
			pRsTestNO->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenKeyset,
				adLockBatchOptimistic,      //批量更新必须设定锁定类型和游标类型
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
	S_Sample calSample(*this); //复制一份以免修正是修改原始记录
	double m_dWaterWeight = 0;
	double m_dVolWeightWithCover  = 0 ;
	double m_dVolWeigth = 0 ;
	//TODO: 添加质量校准
	/* 
	///////////////////////////////////////////////////////////////
	double  lM0;      //试验前样品质量(unit=0.1mg),含坩埚质量。
	double  lM1;      //水分试验后样品质量(unit=0.1mg),含坩埚质量。
	/////////////////////////////////////////////////////////////////
	double  lMPotSample2;//挥发份实验前加盖重量
	///////////////////////////////////////////////////////////////
	double  lM2;      //挥发分试验后样品质量(unit=0.1mg),含坩埚质量。
	double  lM3;      //灰分试验后样品质量(unit=0.1mg),含坩埚质量。
	*/
	if(cCSta&(1<<S_WEIGHT_SAMPLE))
	{ 
		m_SampleWeight = calSample.UQuality.SQuality.lM0 - calSample.UQuality.SQuality.lMPot;  //样品质量 
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	if(cCSta&(1<<S_TEST_WATER))	
	{ 
		m_dWaterWeight  = calSample.UQuality.SQuality.lM0 - calSample.UQuality.SQuality.lM1;  //水分减少质量 
		m_fMad = m_dWaterWeight/m_SampleWeight*100 ;		//水分百分比
	} 
	else
		m_fMad = 0;
	////////////////////////////////////////////////////////////////////////////////////////////
	if(cCSta&(1<<S_TEST_VOL))	
	{ 
			
		if(cCSta&(1<<S_WEIGHT_COVER))
		{//如果另外加盖,.........	
			m_dVolWeightWithCover  = calSample.UQuality.SQuality.lMPotSample2 - calSample.UQuality.SQuality.lM2 ;  //
			m_fVad = m_dVolWeightWithCover /m_SampleWeight*100 ;
		}
		else
		{	// 如果是不另外加盖,直接减掉带坩埚样品重量 		
			m_dVolWeigth  = calSample.UQuality.SQuality.lM0 - calSample.UQuality.SQuality.lM2   ;  //挥发分减少质量
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
		double m_dAshWeigth  =  calSample.UQuality.SQuality.lM3 -  calSample.UQuality.SQuality.lMPot ;  //样品质量
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

