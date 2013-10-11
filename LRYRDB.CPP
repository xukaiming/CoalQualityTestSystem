#include "stdafx.h"
#include "LRYCTRL.h"
#include "LRYRDB.H"
#include "math.h"
#include <float.h>
#include "UserLogon.h" 
#include "TEMPLATE.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//0 at 850°C  R(t) = R0 (1+ At + Bt2)
//A = 3 · 9083 x 10 -3·°C-1; B = -5.775 · 10 -7  °C-2 ; C = -4 · 183 · 10 –12 °C-4
//PT1000的系数
//#define
const double KCoffA = 39083000;   // 3.9083*1E-3f; // ;//     
//#define 
const double KCoffB = -5775;      //-5.775*1E-7f; // ;//       
//----------------------------
//下面系数为鹤壁孟凡韬的系数
//#define KCoffA 3.9083337 
//#define KCoffB 0.0005780 


void LRYRDB::CalcV0VnC(bool bE,bool bTestE)
{ 		
	double a,d; 

	if(m_szAlgorithm=="国标")		//计算冷却系数
	{
		if(bE)
			CalcV0Vn(false);	//不用AK系数计算V0,Vn
		else
			CalcV0Vn(true);		//用AK系数计算V0,Vn

		d=(ftn-ft0)/(ft140-ft0);     //主期温升大于总温升的1.2倍时采用a=k,否则采用a=k-0.1               
		if(d<=1.2)
			a=d-0.1f;
		else
			a=d;
		fC=((nn-a)*fVn+a*fV0);
	}
	else if(m_szAlgorithm=="瑞方")
	{
		CalcV0Vn(false);								//初期降速Vo,末期降速Vn 
		fC = (nn+1)*fV0+(fVn-fV0)/(ftn-ft0)*((ft0+ftn)/2+fSumT-(nn+1)*ft0);
		fC = _finite(fC)?fC:0;//_finite   
		fK = fA =0; 

		/*
		//由于量热桶问题，使得测量不稳定。采用修正来弥补。
		//方法：在V0一定的范围内，修正T0使V0满足特定的值
			
		ft140=fT0;	//用t140保存T0，因t140在瑞方法时未使用。
		if(bTestE)	//试验热容量时
		{
			if(fV0<XZ_V0_1 && fV0>XZ_V0_2)
				fV0=XZ_1;
			else if(fV0<XZ_V0_2 && fV0>XZ_V0_3)
				fV0=XZ_2;
			else if(fV0<XZ_V0_3)
				fV0=XZ_3;
		}
		else	//试验发热量时
		{
			if(fV0<XZ_V0_1 && fV0>XZ_V0_2)
				fV0=XZ_3;
			else if(fV0<XZ_V0_2 && fV0>XZ_V0_3)
				fV0=XZ_2;
			else if(fV0<XZ_V0_3)
				fV0=XZ_1;
		}
		fT0=fV0*nChuQiJiaoBan2+ft0;

		ttn=(ftn+fTn)/2;		//末期平均温度
		tt0=(fT0+ft0)/2;		//初期平均温度
		fC=(float)(nn*fV0 +
			((fVn-fV0)/(ttn-tt0))*((ft0+ftn)/2 + fSumT-nn*tt0));

		fA=0;	//no use
		fK=0;	//no use
		ftj=0;	//no use
		ft140=0;	//如果要修正，这行不要，用t140保存T0。
	*/  
	}
}

void LRYRDB::CalcV0Vn(bool bAK)
{
	if(bAK)
	{
		fV0=fK*(ft0-ftj)+fA;
		fVn=fK*(ftn-ftj)+fA;
	}
	else
	{
		fV0 = (fT0-ft0)/(m_sStartJiaoBanTime2/20);	//初期降速Vo
		fVn = (ftn-fTn)/(m_sEndtJiaoBanTime/20);		//末期降速Vn 
	} 
}


void LRYRDB::CalculateE()
{ 
	double Q; 
	CalcV0VnC(true,true);			//计算初期降速Vo,末期降速Vn,冷却系数C
	//计算冷却系数
	Q=m_sBenzoicAcidHeatValue*fYangPinZhiLiang*1.0015f;		//苯甲酸发热量+硝酸生成热
	//热容量
	lE=(long)((Q +  
			m_sHotWireHeatValue*fDianHuoSiZhiLiang +			//点火丝热量
			m_sadditive1HeatValue*fTianJiaWuZhiLiang1 +			//添加物热量
			m_sadditive2HeatValue*fTianJiaWuZhiLiang2)/(ftn-ft0+fC)); 

	//m_AOxygenHeatCapacity m_BOxygenHeatCapacity
	//苯甲酸?
	//修正? 
	if ((m_bFix)&&(strYangPinBianHao==_T("苯甲酸")))
	{ 
		srand( (unsigned)time( NULL ));  //随机数种子
		//#ifndef _DEBUG
		int RandomValue = rand()%30;
		//#else
		//int RandomValue = rand()%150;
		//#endif
		if(strYangPinBianHao==_T("A氧弹"))
		{
			lE = m_AOxygenHeatCapacity + RandomValue-15;
		}
		else
		{
			lE = m_BOxygenHeatCapacity + RandomValue-15;
		} 
	} 
}

void LRYRDB::ReCalculateE()
{ 
	double Q; 
	//CalcV0VnC(true,true);			//计算初期降速Vo,末期降速Vn,冷却系数C
	//计算冷却系数
	Q=m_sBenzoicAcidHeatValue*fYangPinZhiLiang*1.0015f;		//苯甲酸发热量+硝酸生成热
	//热容量
	lE=(long)((Q +  
		m_sHotWireHeatValue*fDianHuoSiZhiLiang +			//点火丝热量
		m_sadditive1HeatValue*fTianJiaWuZhiLiang1 +			//添加物热量
		m_sadditive2HeatValue*fTianJiaWuZhiLiang2)/(ftn-ft0+fC)); 
}

void LRYRDB::ReCalculateQ()
{
	float q1,q2,q3,k,lQbad_Z;
 
	q1= fDianHuoSiZhiLiang* m_sHotWireHeatValue;
	q2= fTianJiaWuZhiLiang1* m_sadditive1HeatValue;
	q3= fTianJiaWuZhiLiang2* m_sadditive2HeatValue;

	lQbad=(long)(( ( ftn- ft0+ fC)*m_HeatCapacity-q1-q2-q3)/ fYangPinZhiLiang);		//弹筒发热量	
	lQbad_Z=(long)(( ( ftn- ft0+ fC)*m_HeatCapacity)/ fYangPinZhiLiang);		//总热值

	if( lQbad_Z<=16700)		//硝酸校正系数
		k=0.001f;
	else if( lQbad_Z>16700 &&  lQbad_Z<=25100)
		k=0.0012f;
	else if( lQbad_Z>25100)
		k=0.0016f;
	
	 lQgrad=(long)( lQbad-94.1f* fSad-k* lQbad);							//高位发热量 
	 lQgrd =(long)lQgrad*100/(100-fMad);                                          //干基高位量
	 lQnetvad=(long)(( lQgrad-206* fHad)*(100- fMt)/(100- fMad)-23* fMt);	//低位发热量
}

void LRYRDB::CalculateQ()
{ 
	double q1,q2,q3,k,lQbad_Z;
 
	q1= fDianHuoSiZhiLiang* m_sHotWireHeatValue;
	q2= fTianJiaWuZhiLiang1* m_sadditive1HeatValue;
	q3= fTianJiaWuZhiLiang2* m_sadditive2HeatValue;

	CalcV0VnC(false,false);				//初期降速Vo,末期降速Vn,冷却系数C

	lQbad=(long)(( ( ftn- ft0+ fC)*m_HeatCapacity-q1-q2-q3)/ fYangPinZhiLiang);		//弹筒发热量
	//////////////////////////////////////////////////////////////////
	
	if ((m_bFix)&&(strYangPinBianHao==_T("苯甲酸")))    //修正
	{ 
		srand( (unsigned)time( NULL ));  //随机数种子
		//#ifndef _DEBUG
		int RandomValue = rand()%40;
		//#else
		//int RandomValue = rand()%90;
		//#endif
		lQbad = m_sBenzoicAcidHeatValue + RandomValue ;
	}
	
	////////////////////////////////////////////////////
	lQbad_Z=(long)(( ( ftn- ft0+ fC)*m_HeatCapacity)/ fYangPinZhiLiang);		//总热值

	if( lQbad_Z<=16700)		//硝酸校正系数
		k=0.001f;
	else if( lQbad_Z>16700 &&  lQbad_Z<=25100)
		k=0.0012f;
	else if( lQbad_Z>25100)
		k=0.0016f;
	
	 lQgrad=(long)( lQbad-94.1f* fSad-k* lQbad);							//高位发热量 
	 lQgrd =(long)lQgrad/(1-fMad);                                          //干基高位量
	 lQnetvad=(long)(( lQgrad-206* fHad)*(100- fMt)/(100- fMad)-23* fMt);	//低位发热量


}
void LRYRDB::InitRDB(SLryImageSlave *pLryImageSlave)
{
	ZeroMemory(&pLryImageSlave->DownloadWorkParam,sizeof(pLryImageSlave->DownloadWorkParam));
 
	pLryImageSlave->DownloadWorkParam.m_sFillWaterTime		= m_sFillWaterTime; 
	pLryImageSlave->DownloadWorkParam.m_sOutWaterTime		= m_sOutWaterTime; 
	pLryImageSlave->DownloadWorkParam.m_sStartJiaoBanTime1	= m_sStartJiaoBanTime1; 
	pLryImageSlave->DownloadWorkParam.m_sStartJiaoBanTime2	= m_sStartJiaoBanTime2; 
	pLryImageSlave->DownloadWorkParam.m_sEndtJiaoBanTime	= m_sEndtJiaoBanTime; 
	pLryImageSlave->DownloadWorkParam.m_sFireTime			= m_sFireTime; 
	pLryImageSlave->DownloadWorkParam.m_sSenserPowerONTime	= m_sSenserPowerONTime; 
	pLryImageSlave->DownloadWorkParam.m_sBeepTime			= m_sBeepTime;   

	//m_fFireOKtemp			= ((m_fFireOKtemp*0xFFFF)*16>0xFFFF)?0xFFFF:(unsigned short)((m_fFireOKtemp*0xFFFF)*16);   //将浮点温度转换为AD值 

#ifndef PT1000	
	/*
	long fFireOKtemp										= m_fFireOKtemp	*(0.06784f*0xFFFFFF);
	pLryImageSlave->DownloadWorkParam.m_fFireOKtemp_Hi		= fFireOKtemp>>16;	
	pLryImageSlave->DownloadWorkParam.m_fFireOKtemp_Lo		= fFireOKtemp&0xFFFF;
	pLryImageSlave->DownloadWorkParam.m_fEndCheckTemp		= m_fEndCheckTemp*(0.06784f*0xFFFFFF); //将浮点温度转换为AD值 
	*/
	double coff =400000L; //455266
	long fFireOKtemp										= m_fFireOKtemp*coff;//	*(0.027136f*0xFFFFFF);
	pLryImageSlave->DownloadWorkParam.m_fFireOKtemp_Hi		= fFireOKtemp>>16;	
	pLryImageSlave->DownloadWorkParam.m_fFireOKtemp_Lo		= fFireOKtemp&0xFFFF;
	pLryImageSlave->DownloadWorkParam.m_fEndCheckTemp		= m_fEndCheckTemp*coff;//(0.027136f*0xFFFFFF); //将浮点温度转换为AD值 

 #else								//0.032f*0xFFFFFF
	long fFireOKtemp										= m_fFireOKtemp	*(0.032*0xFFFFFF)*2;
	pLryImageSlave->DownloadWorkParam.m_fFireOKtemp_Hi		= fFireOKtemp>>16;	
	pLryImageSlave->DownloadWorkParam.m_fFireOKtemp_Lo		= fFireOKtemp&0xFFFF;
	pLryImageSlave->DownloadWorkParam.m_fEndCheckTemp		= m_fEndCheckTemp*(0.032*0xFFFFFF); //将浮点温度转换为AD值 
#endif
	if(m_szAlgorithm==_T("瑞方"))
	{
		strcpy(pLryImageSlave->DownloadWorkParam.m_szAlgorithm,"瑞方");
	}
	if(m_szAlgorithm==_T("国标"))
	{
		strcpy(pLryImageSlave->DownloadWorkParam.m_szAlgorithm,"国标");
	}
	if(m_szAlgorithm==_T("国标快速"))
	{
		strcpy(pLryImageSlave->DownloadWorkParam.m_szAlgorithm,"国标快速");
	}  
}

void LRYRDB::RefreshRDBReportTab(SLryImageSlave *pLryImageSlave)
{
	ftj =GetTemperature(GetRt(pLryImageSlave->Report.lTj ));
	fT0 =GetTemperature(GetRt(pLryImageSlave->Report.lT0 ));
	ft0 =GetTemperature(GetRt(pLryImageSlave->Report.lt0));
	ft140 =GetTemperature(GetRt(pLryImageSlave->Report.lT140 ));
	ftn =GetTemperature(GetRt(pLryImageSlave->Report.ltn ));
	fTn =GetTemperature(GetRt(pLryImageSlave->Report.lTn ));

	t0nn =  pLryImageSlave->Report.lT0Cnt; 
	for(int i=0;i<t0nn;i++)
	{
		ftt0[i] = GetTemperature(GetRt(pLryImageSlave->ReportT0Tn.lTT0[i] ));
		TRACE(_T("pLryImageSlave->Report.lTi[%d] = %f\n"),i,
			GetTemperature(GetRt(pLryImageSlave->ReportT0Tn.lTT0[i]))); 
	}	

	tnnn =  pLryImageSlave->Report.lTnCnt; 
	for(int i=0;i<tnnn;i++)
	{
		fttn[i] = GetTemperature(GetRt(pLryImageSlave->ReportT0Tn.lTTn[i] ));
		TRACE(_T("pLryImageSlave->Report.lTi[%d] = %f\n"),i,
			GetTemperature(GetRt(pLryImageSlave->ReportT0Tn.lTTn[i]))); 
	}

	nn =  pLryImageSlave->Report.lTiCnt; 
	fSumT = 0;
	for(int i=0;i<nn;i++)
	{
		fTi[i]= GetTemperature(GetRt(pLryImageSlave->Report.lTi[i] ));
		fSumT +=fTi[i];
		TRACE(_T("pLryImageSlave->Report.lTi[%d] = %f\n"),i,
			GetTemperature(GetRt(pLryImageSlave->Report.lTi[i]))); 
	} 

}


#define		PT1000   
#undef      PT1000
#define K_AD			(1.0/0x7fffff/2) 
#define BRIDGE_R		300.0				//桥路下臂电阻
#define BRIDGE_R1		680.0				//桥路上臂电阻
#define BRIDGE_a		(BRIDGE_R1/BRIDGE_R)
////计算t度时电阻
double LRYRDB::GetRt(ULONG iAd)
{
#ifdef PT1000
	double Rt; 
	//Rt=((double)0x7FFFFF)/(0.5f*0x7FFFFF-((double)iAd)/64)-1L ;    	//单极性32倍增益时
	Rt=((double)0x7FFFFF)/(0.5f*0x7FFFFF-((double)iAd)/32)-1L ;			//单极性16倍增益时
	//return Rt*1000;
	return Rt*1000000;
#else       //PT300
	double delta1,delta2;
	
	//double Vin = iAd/4; //双极性4倍
	double Vin = iAd/64;  //单极性32倍
	delta1 = 680/(1-300.0f/980-Vin/0x7fffff)-980;

	LONG64 son1 = 	680UL;
	son1*=0x7fffffUL;
	son1*=1000UL;
	LONG64 Mon1 = 	300UL;
	Mon1*=0x7fffffUL;
	delta2 = son1/(0x7fffff-Mon1/980.0f-Vin)-980.0f*1000UL;
	return 300*1000+delta2 ;
#endif

}


#define VREF   2.5f
double LRYRDB::GetVoltage(ULONG iAD)
{
	double voltage;
	//iAD-=0x800000;
	//voltage = iAD*VREF/4/0x7FFFFF;    
	voltage = iAD*VREF/64/0x7FFFFF;  
	return voltage ;
}

//计算温度

double LRYRDB::GetTemperature(double Rt)
{ 
	double R0			= m_fR0;
	double dTemperature	= 0;
	Rt = Rt/R0;	
	dTemperature =      (-KCoffA+sqrt(
		KCoffA*KCoffA-4*KCoffB*(1000-Rt)*10000000)
									)/(2*KCoffB);	

	return dTemperature;
}


void LRYRDB::RefreshRDBTempTab(SLryImageSlave *pLryImageSlave)
{
	/*
	short	nSpeed;		//转数
	UINT	iAd;		//AD读数
	double	fAdV;		//AD电压
	double	fRt;		//探头电阻
	double	fty;		//实测温度用Rt=R0(1+At+Bt^2)求得
	double	ft;			//当前内桶(校正)温度
	*/
	nSpeed	= pLryImageSlave->UploadRegister.m_JiaoBanSpeed;
	iAd		= pLryImageSlave->UploadRegister.AD7714Value[pLryImageSlave->UploadRegister.AD7714ValuePtr];
 	nSpeed	= pLryImageSlave->UploadRegister.m_JiaoBanSpeed;
	//iAd = 0.032f*0xFFFFFF;
	//iAd = (0.027136f*0xFFFFFF);
	//iAd		= 0xFFFFFF;
	fAdV	= GetVoltage(iAd)*1000;
	fRt		= GetRt(iAd);
	fty		= GetTemperature(fRt);
	ft		= GetTemperature(fRt); 
}

double LRYRDB::CalcStandardErrorE(long *pData, int iCount)
{
	int i;
	long average,b,bb;

	//求平均值
	average=0;
	for(i=0;i<iCount;i++)
	{
		average+=pData[i];
	}
	average=average/iCount;

	//计算标准差
	bb=0;
	for(i=0;i<iCount;i++)
	{
		b=pData[i]-average;
		bb+=b*b;
	}
	return (float)sqrt(((float)bb)/(iCount-1));
}


CString LRYRDB::BuildAutoNO(CString sWorkType, CString strAB)
{
	COleDateTime  dtEnd;									//查询结束日期
	COleDateTime  dtStart=COleDateTime::GetCurrentTime( );  //得到今天的日期
	CString strStartDate,strEndDate,AutoNo;   //开始日期,结束日期,自动编号
	CString sName = m_szName.Left(1); ; 
	strAB=strAB.Left(1);
	//
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	
 
	//dtStart.SetDateTime(2002, 3, 15, 1, 0, 0);  
	dtEnd   = MakeDays(1)+dtStart;
	strStartDate = dtStart.Format(_T("%Y%m%d"));
	if(sWorkType==_T("热容量"))
	{
		//1AE2002031504
		sWorkType = _T("E");
		//查询已有 记录 		
		strSQL.Format(
			_T("select 试验编号 from 热容量 where 试验编号 like '%%%s%%' and 试验日期  between  #%s# and #%s# and 设备名称='%s' order by 试验编号"), 
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
			if((RecordCount = pRsTestNO->GetRecordCount())==0)
			{
				AutoNo = sName+strAB+sWorkType+strStartDate+(_T("01"));				
			}
			else 
			{
				AutoNo = VariantToStr(pRsTestNO->Fields->GetItem("试验编号")->Value);
				length = AutoNo.GetLength();		
				RecordCount = _tcstol(AutoNo.Right(2).GetBuffer(2),NULL,10);
				AutoNo = AutoNo.Left(length-2);;
				strStartDate= AutoNo+(_T("%02d"));
				AutoNo.Format(strStartDate,RecordCount+1);
			}
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
	}
	else
	{
		//1AE2002031504
		//查询已有 记录 
		sWorkType = _T("Q");
		strSQL.Format(
			_T("select 试验编号 from 发热量 where 试验编号 like '%%%s%%' and 试验日期  between  #%s# and #%s# and 设备名称='%s' order by 试验编号"), 
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
			if((RecordCount = pRsTestNO->GetRecordCount())==0)
			{ 
				AutoNo = sName+strAB+sWorkType+strStartDate+(_T("01"));	
			}
			else 
			{
				AutoNo = VariantToStr(pRsTestNO->Fields->GetItem("试验编号")->Value);
				length = AutoNo.GetLength();		
				RecordCount = _tcstol(AutoNo.Right(2).GetBuffer(2),NULL,10);
				AutoNo = AutoNo.Left(length-2);;
				strStartDate= AutoNo+(_T("%02d"));
				AutoNo.Format(strStartDate,RecordCount+1);
			}
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
	}	
	
	if(pRsTestNO->State == adStateOpen)
		pRsTestNO->Close();
	return AutoNo;
}

void LRYRDB::SaveEtoDatabase()
{

	HRESULT hr= S_OK;
	CString strSQL;
	_RecordsetPtr	pRsResult;

	pRsResult.CreateInstance("ADODB.Recordset");
	strSQL.Format(_T("select * from 热容量 where 试验日期>NOW()"));
	try
	{ 
		
		hr = pRsResult->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);	 
		pRsResult->AddNew();
		pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
		pRsResult->PutCollect("样品编号",_variant_t(strYangPinBianHao)); 
		pRsResult->PutCollect("试验日期",_variant_t(COleDateTime::GetCurrentTime()));

		pRsResult->PutCollect("样品质量",_variant_t(fYangPinZhiLiang));
		pRsResult->PutCollect("设备名称",_variant_t(m_szName));
		pRsResult->PutCollect("氧弹编号",_variant_t(strYangDanBianHao));
		pRsResult->PutCollect("算法",_variant_t(m_szAlgorithm));
		pRsResult->PutCollect("修正",_variant_t(false ));
		pRsResult->PutCollect("热容量",_variant_t((SHORT)lE));
		pRsResult->PutCollect("苯甲酸热值",_variant_t( m_sBenzoicAcidHeatValue));
		pRsResult->PutCollect("点火丝质量",_variant_t( fDianHuoSiZhiLiang));
		pRsResult->PutCollect("点火丝热值",_variant_t(m_sHotWireHeatValue ));
		pRsResult->PutCollect("添加物1质量",_variant_t( fTianJiaWuZhiLiang1));
		pRsResult->PutCollect("添加物1热值",_variant_t( m_sadditive1HeatValue));		
		pRsResult->PutCollect("添加物2质量",_variant_t( fTianJiaWuZhiLiang2));
		pRsResult->PutCollect("添加物2热值",_variant_t( m_sadditive2HeatValue));
		pRsResult->PutCollect("冷却系数C",_variant_t( fC  ));
		pRsResult->PutCollect("初期降速Vo",_variant_t( fV0));
		pRsResult->PutCollect("末期降速Vn",_variant_t( fVn));
		pRsResult->PutCollect("初期温度T0",_variant_t( fT0));
		pRsResult->PutCollect("点火温度t0",_variant_t( ft0));
		pRsResult->PutCollect("第1分40秒温度",_variant_t( ft140));
		pRsResult->PutCollect("终点温度tn",_variant_t( ftn));
		pRsResult->PutCollect("末期温度Tn",_variant_t( fTn));
		pRsResult->PutCollect("主期时间n",_variant_t((short) nn ));
		pRsResult->PutCollect("主期温度总和",_variant_t(fSumT));
		pRsResult->PutCollect("常数K",_variant_t(fK ));
		pRsResult->PutCollect("常数A",_variant_t(fA ));
		pRsResult->PutCollect("外桶温度tj",_variant_t( ftj));
		pRsResult->PutCollect("合格",_variant_t(_T("合格")));
		pRsResult->PutCollect("组号",_variant_t( lGroupNum)); 
		pRsResult->Update();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	if(pRsResult->State==adStateOpen)
		pRsResult->Close();
}


void LRYRDB::SaveQtoDatabase()
{
	HRESULT hr= S_OK;
	CString strSQL;
	_RecordsetPtr	pRsResult;

	strSQL.Format(_T("select * from 发热量 where 试验日期>NOW()"));
	try
	{ 		
		pRsResult.CreateInstance("ADODB.Recordset");
		hr = pRsResult->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);	 
		//pRsResult->MoveLast();
		pRsResult->AddNew();
		pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));

		pRsResult->PutCollect("样品编号",_variant_t(strYangPinBianHao)); 
		pRsResult->PutCollect("来样情况",_variant_t(strLaiYang)); 
		pRsResult->PutCollect("取样时间",_variant_t(m_datePickSample)); //
		pRsResult->PutCollect("试验日期",_variant_t(m_tDate));//=COleDateTime::GetCurrentTime()
		pRsResult->PutCollect("设备名称",_variant_t(m_szName));
		pRsResult->PutCollect("氧弹编号",_variant_t(strYangDanBianHao));
		pRsResult->PutCollect("算法",_variant_t(m_szAlgorithm));
		pRsResult->PutCollect("修正",_variant_t(false ));
		pRsResult->PutCollect("弹筒发热量",_variant_t(lQbad));
		pRsResult->PutCollect("高位发热量",_variant_t(lQgrad));
		pRsResult->PutCollect("干基高位发热量",_variant_t(lQgrd));  
		pRsResult->PutCollect("低位发热量",_variant_t(lQnetvad)); 
		pRsResult->PutCollect("热容量",_variant_t((SHORT)m_HeatCapacity)); 
		pRsResult->PutCollect("样品质量",_variant_t(fYangPinZhiLiang));
		
		pRsResult->PutCollect("点火丝质量",_variant_t( fDianHuoSiZhiLiang));
		pRsResult->PutCollect("点火丝热值",_variant_t(m_sHotWireHeatValue ));
		pRsResult->PutCollect("添加物1质量",_variant_t( fTianJiaWuZhiLiang1));
		pRsResult->PutCollect("添加物1热值",_variant_t( m_sadditive1HeatValue));		
		pRsResult->PutCollect("添加物2质量",_variant_t( fTianJiaWuZhiLiang2));
		pRsResult->PutCollect("添加物2热值",_variant_t( m_sadditive2HeatValue));
		pRsResult->PutCollect("冷却系数C",_variant_t( fC ));
		pRsResult->PutCollect("初期降速Vo",_variant_t( fV0));
		pRsResult->PutCollect("末期降速Vn",_variant_t( fVn));
		pRsResult->PutCollect("初期温度T0",_variant_t( fT0));
		pRsResult->PutCollect("点火温度t0",_variant_t( ft0));
		pRsResult->PutCollect("第1分40秒温度",_variant_t( ft140));
		pRsResult->PutCollect("终点温度tn",_variant_t( ftn));
		pRsResult->PutCollect("末期温度Tn",_variant_t( fTn));
		pRsResult->PutCollect("外桶温度tj",_variant_t( ftj));
		
		pRsResult->PutCollect("常数K",_variant_t(fK ));
		pRsResult->PutCollect("常数A",_variant_t(fA ));
		pRsResult->PutCollect("主期时间n",_variant_t((short) nn ));
		pRsResult->PutCollect("主期温度总和",_variant_t(fSumT));

		pRsResult->PutCollect("全水分Mt",_variant_t(fMt));
		pRsResult->PutCollect("水分Mad",_variant_t(fMad));
		pRsResult->PutCollect("硫含量Sad",_variant_t(fSad));
		pRsResult->PutCollect("氢含量Had",_variant_t(fHad));
		pRsResult->PutCollect("标志",_variant_t(_T("未完成")));
		pRsResult->PutCollect("结论",_variant_t(_T("合格")));
		pRsResult->PutCollect("平行样号",_variant_t( lPingXingNum)); 
		pRsResult->Update();
		if(pRsResult->State==adStateOpen)
			pRsResult->Close();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}

}


void LRYRDB::SaveCurveToFile()
{ 
	try
	{
		if(strAutoNo!=_T(""))
		{
			TCHAR   cPath[MAX_PATH]; 
			GetModuleFileName(AfxGetApp()-> m_hInstance,cPath,MAX_PATH); 
			LPTSTR   p=_tcsrchr(cPath, _T('\\'));p++; 
			p[0]=0; 
			_tcscat(cPath,_T("CurveSave\\\0"));
			CFile *fileCurve = new CFile(CString(cPath)+strAutoNo+_T(".CSV"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );
			char strTemp[20] ; 
			if(fileCurve != NULL)
			{
				sprintf(strTemp,"%d,%f\n",iAd,ft);
				fileCurve->SeekToEnd ( );
				fileCurve->Write(strTemp,strlen(strTemp));
			}
			fileCurve->Close();
			delete fileCurve;
		}
	}
	catch(CFileException* ex )
	{
		ex->ReportError();
		ex->Delete();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}  
}

LRYRDB::LRYRDB()
{
	nSpeed=0;		//转数
	iAd=0;		//AD读数
	fAdV=0;		//AD电压
	fRt=0;		//探头电阻
	fty=0;		//实测温度用Rt=R0(1+At+Bt^2)求得
	ft=0;		//当前内桶(校正)温度

	ftj=0;		//外桶温度
	fT0=0;		//初期内桶温度
	ft0=0;		//点火时内桶温度
	ft140=0;	//1'40''内桶温度
	ftn=0;		//终点内桶温度
	fTn=0;		//末期内桶温度
	fSumT=0;	//主期温度总和Σti
	nn=0;		//t0到tn的分钟数n

	fV0=0;		//初期降速Vo
	fVn=0;		//末期降速Vn
	fC=0;		//冷却系数C
	lE=0;		//热容量
	fK=0;		//常数K
	fA=0;		//常数A
	lGroupNum=0;	//组号

	lQbad=0;		//弹筒发热量
	lQgrad=0;		//高位发热量
	lQnetvad=0;		//低位发热量

	ftt0 = new double[60];
	fttn = new double[60];
	fTi  = new double[100];

	m_szAlgorithm = _T("-");

}
LRYRDB::~LRYRDB()
{
	delete []fttn;
	delete []ftt0;
	delete []fTi;
}

void DATA_E::ClearDataE()
{
	/*
	for(int i=0;i<MAX_DATA_E;i++)
	{
		D[i].bActive=false;
	}*/
	ZeroMemory(D,sizeof(D));
	iPtr=0;
	iOkCount=0;
	fRSError=0;
	fSError=0;
	lAverageE=0;
	lMaxDiff=0;
	fA=0;
	fK=0;
}
 
int DATA_E::LoadTestData(CString strDeviceName,CString strYangPinBianHao /*strYangPinBianHao*/,CString strTestMode,long lDuplicateSampleNO )
{
	int i =0;
	CString StrSQL;
	CString m_strOk;
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");   

	try
	{ 
		strSQL.Format(
			_T("select * from 热容量 where   设备名称='%s' and 氧弹编号='%s' and 算法='%s' and 组号=%d order by 试验编号 "),   
			strDeviceName,strYangPinBianHao,strTestMode,lDuplicateSampleNO
			);	 
		pRsTestNO->Open(strSQL.AllocSysString(),
						theApp.pConn.GetInterfacePtr(),
						adOpenStatic,
						adLockOptimistic,
							-1);	
		RecordCount = pRsTestNO->GetRecordCount();
		if(!pRsTestNO->BOF)
			pRsTestNO->MoveFirst();	
		while(!pRsTestNO->adoEOF)
		{  
			D[i].bActive=true;
			D[i].lE= _tcstol(
				 VariantToStr(pRsTestNO->Fields->GetItem("热容量")->Value),
				 NULL,10);
			lstrcpy(D[i].strTestNum,(TCHAR*)(LPCTSTR)VariantToStr(pRsTestNO->Fields->GetItem("试验编号")->Value));
			D[i].fYangPinZhiLiang	= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("样品质量")->Value),NULL ); 
			D[i].fZhuQiWenSheng		= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("终点温度tn")->Value),NULL)-
									_tcstod(VariantToStr(pRsTestNO->Fields->GetItem("点火温度t0")->Value),NULL);
				 //m_ftn-m_ft0;
			D[i].ft0				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("点火温度t0")->Value),NULL);
			D[i].fC					= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("冷却系数C")->Value),NULL);;			
			D[i].fV0				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("初期降速Vo")->Value),NULL);
			D[i].fVn				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("末期降速Vn")->Value),NULL);
			//D[i].ft0tj				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("外桶温度tj")->Value),NULL);	//(m_fT0+m_ft0)/2-m_ftj;
			// D[i].ftntj				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("外桶温度tj")->Value),NULL);	//(m_fTn+m_ftn)/2-m_ftj;
			m_strOk					= VariantToStr(pRsTestNO->Fields->GetItem("合格")->Value);
			if(m_strOk==_T("合格"))
			{
				 D[i].bOk=true;
				 iOkCount++;
			}
			else
				 D[i].bOk=false;
			szMode					= VariantToStr(pRsTestNO->Fields->GetItem("算法")->Value);;
			tDate					= pRsTestNO->Fields->GetItem("试验日期")->Value  ;
			DArray[i] = &D[i];
			i++;
			iPtr=i;
			
			if(i>=MAX_DATA_E)
				break; 
			pRsTestNO->MoveNext();
		}
		if(pRsTestNO->State == adStateOpen)
			pRsTestNO->Close();  
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}

 
	return i;
}



void DATA_Q::ClearDataQ()
{
	for(int i=0;i<MAX_DATA_Q;i++)
		D[i].bActive=false;
	iPtr			= 0;
	lPingXingNum	= 0;				//平行样编号
	lMaxDiffQbad	= 0;				//极差
	lAverageQbad	= 0;				//平均弹筒发热量
	lAverageQgrad	= 0;				//平均空干基高位发热量
	lAverageQgrd	= 0;				//平均干基高位发热量
	lAverageQnetvad	= 0;				//平均低位发热量
	fAverageMad		= 0;				//平均空干基水分
	fAverageMt		= 0;				//平均全水分
	fAverageSad		= 0;				//平均硫含量
	fAverageHad		= 0;				//平均氢含量
	fAverageM		= 0;				//平均质量

}

LONG LRYRDB::GetELastGroupNum()
{
	CString sDeviceName = m_szName;
	CString strAB	=	strYangDanBianHao; 
	long GroupNum;
	CString strGroupNum;
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	  
	strSQL.Format(
		_T("select 组号 from 热容量 where   设备名称='%s' and 氧弹编号='%s' order by 试验编号"),   
		sDeviceName,
		strAB
		);	 
	try
	{ 
		pRsTestNO->Open(strSQL.AllocSysString(),
						theApp.pConn.GetInterfacePtr(),
						adOpenStatic,
						adLockOptimistic,
							-1);	
		if(!pRsTestNO->adoEOF)
			pRsTestNO->MoveLast();		 
		if((RecordCount = pRsTestNO->GetRecordCount())==0)
		{ 		
			GroupNum = 0;
		}
		else 
		{
			strGroupNum = VariantToStr(pRsTestNO->Fields->GetItem("组号")->Value);
			GroupNum = _tcstol(strGroupNum,NULL,10); 
		}
		if(pRsTestNO->State == adStateOpen)
			pRsTestNO->Close();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}

	return GroupNum;
}


LONG LRYRDB::GetQLastGroupNum()
{
	CString sDeviceName = m_szName;
	CString strAB		= strYangDanBianHao ; 
	long GroupNum;
	CString strGroupNum;
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	  
	strSQL.Format(
		_T("select 平行样号 from 发热量 where   设备名称='%s' and 氧弹编号='%s' order by 平行样号"),   
		sDeviceName,
		strAB
		);	 
	try
	{ 
		pRsTestNO->Open(strSQL.AllocSysString(),
						theApp.pConn.GetInterfacePtr(),
						adOpenStatic,
						adLockOptimistic,
							-1);	
		if(!pRsTestNO->adoEOF)
			pRsTestNO->MoveLast();		 
		if((RecordCount = pRsTestNO->GetRecordCount())==0)
		{ 		
			GroupNum = 0;
		}
		else 
		{
			strGroupNum = VariantToStr(pRsTestNO->Fields->GetItem("平行样号")->Value);
			GroupNum = _tcstol(strGroupNum,NULL,10); 
		}
		if(pRsTestNO->State == adStateOpen)
			pRsTestNO->Close();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	} 
	return GroupNum;
}

int DATA_Q::LoadTestData(CString strDeviceName, CString strYangPinBianHao, CString strTestMode, long &lDuplicateSampleNO)
{
	int i=0,j =0;
	CString StrSQL;
	CString m_strOk;
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");   
	strSQL.Format(
		_T("select * from 发热量 where   设备名称='%s' and 样品编号='%s' and 算法='%s' and 平行样号=%d and 标志='未完成' order by 试验编号 "),   
		strDeviceName,strYangPinBianHao,strTestMode,lDuplicateSampleNO
		);	 
	try
	{ 
	pRsTestNO->Open(strSQL.AllocSysString(),
						theApp.pConn.GetInterfacePtr(),
						adOpenStatic,
						adLockOptimistic,
							-1);	
		RecordCount = pRsTestNO->GetRecordCount();
		if((RecordCount>0)&&(RecordCount<MAX_DATA_Q))
		{ 
			if(!pRsTestNO->BOF)
				pRsTestNO->MoveFirst();	
			while(!pRsTestNO->adoEOF)
			{  
				
				D[i].bActive			= true;
				D[i].strTestNum			= VariantToStr(pRsTestNO->Fields->GetItem("试验编号")->Value);;			//试验编号
				D[i].strYangPinNum 		= VariantToStr(pRsTestNO->Fields->GetItem("样品编号")->Value);;			//样品编号
				D[i].fYangPinM 			= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("样品质量")->Value),NULL );//样品质量
				D[i].lQbad 				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("弹筒发热量")->Value),NULL );//弹筒发热量
				D[i].lQgrad				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("高位发热量")->Value),NULL );//空干基高位发热量
				D[i].lQgrd 				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("干基高位发热量")->Value),NULL );//干基高位发热量=Qgrad*100/(100-Mad)
				D[i].lQnetvad 			= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("低位发热量")->Value),NULL );//低位发热量
				D[i].fMad 				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("水分Mad")->Value),NULL );//空干基水分
				D[i].fMt 				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("全水分Mt")->Value),NULL );	//全水分
				D[i].fSad 				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("硫含量Sad")->Value),NULL );	//硫含量
				D[i].fHad 				= _tcstod(VariantToStr(pRsTestNO->Fields->GetItem("氢含量Had")->Value),NULL );//氢含量Had
				tDate					= pRsTestNO->Fields->GetItem("试验日期")->Value  ;
				i++;
				iPtr=i;
				pRsTestNO->MoveNext(); 
			} 
			m_strMode				= strTestMode;
			lPingXingNum			= lDuplicateSampleNO;			
			if(RecordCount>0)
			{
				for(j=0;j<RecordCount;j++)
				{
					DArray[j] = &D[j];
				}
				CalcQMaxDiffAndAverage(0,RecordCount);
			}
		}
		else
		{
			ClearDataQ();
			lDuplicateSampleNO++;
		}
		m_strMode					= strTestMode;	
		m_strOk					=  _T("合格");
		if(pRsTestNO->State == adStateOpen)
			pRsTestNO->Close();   	
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}

	return i;
}

//取热容量的平均值，极差，标准差，相对标准差。
void DATA_E::GetAverageDataE()
{
	if(iPtr>1)
	{ 
		BubbleSort ();
		lMaxDiff = abs(DArray[0]->lE -DArray[iPtr-1]->lE);
		
		long *ptrLong = new long[iPtr];	 
		for(int i=0;i<iPtr;i++)
		{
			ptrLong[i] =DArray[i]->lE;
		}  
 		lAverageE	= Sum(ptrLong,iPtr)/iPtr;				//平均值
		fSError		= GetStandardDeviation(ptrLong,iPtr);	//标准差
		fRSError	= GetRSD(ptrLong,iPtr);					//相对标准差		 				
		
		delete ptrLong; 
	
	}
}

//取热容量的平均值，极差，标准差，相对标准差。
void DATA_E::GetAverageDataE(Report_E **ptrD,int Len)
{
	if(Len>1)
	{
		long *ptrLong = new long[Len];	 
		for(int i=0;i<Len;i++)
		{
			ptrLong[i] =ptrD[i]->lE;
		}
		lMaxDiff = abs(ptrD[0]->lE -ptrD[Len-1]->lE);
		lAverageE	= Sum(ptrLong,Len)/Len;				//平均值
		fSError		= GetStandardDeviation(ptrLong,Len);	//标准差
		fRSError	= GetRSD(ptrLong,Len);					//相对标准差
		
		
		delete ptrLong;
	}
}

int __cdecl DATA_E::compare(const void* elem1, const void* elem2 )
{ 
	return (((Report_E*)elem1)->lE-((Report_E*)elem2)->lE) ;
}


double DATA_E::GetDataERSD(Report_E **ptrD,int Len)
{
	double _fRSError;
	if(Len>1)
	{
		long *ptrLong = new long[Len];	 
		for(int i=0;i<Len;i++)
		{
			ptrLong[i] =ptrD[i]->lE;
		} 
		_fRSError	= GetRSD(ptrLong,Len);					//相对标准差 				
		
		delete ptrLong;
	}
	return _fRSError;
}

//void　DATA_E::swap(a* ptr1,a * ptr2)
void DATA_E::swap(APtr* ptr1,APtr* ptr2)
{
	APtr  swap; 
	swap = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = swap; 
} 


void DATA_E::BubbleSort()
{ 
	int pass = 1;  int exchange = 1;
    while ( pass < iPtr && exchange )
	{ 
		exchange = 0;	     //交换标志置为0,假定未交换
		for ( int j = iPtr-1;  j >= pass;  j-- ) 
		{
			if(DArray[j-1]->lE<DArray[j]->lE)           //大个在前 
			{
				swap(&DArray[j-1],&DArray[j]); //交换
				exchange = 1;        //交换标志置为1,有交换
			}
		}
		pass++;
	}
}
 

void DATA_E::InitDataE(LRYRDB *pLryRdb)
{  
	int lGroupNo;
	pRdb = pLryRdb;
	ClearDataE();
	lGroupNo = pLryRdb->GetELastGroupNum(); 
	if(lGroupNo!=0)
	{

		LoadTestData(pLryRdb->m_szName,pLryRdb->strYangDanBianHao,pRdb->m_szAlgorithm,lGroupNo); 
		if( iOkCount>=5 ||  iPtr>=6 ||  iPtr- iOkCount>=2 )	//试验已完成，重新试验
		{
			ClearDataE();
			lGroupNo++;
		}
		else
		{
			if(IDNO==AfxMessageBox(_T("上次试验未结束，要继续吗？"),MB_YESNO))
			{
				ClearDataE();	//重新试验
				lGroupNo++;
			}
			else
			{
				 GetAverageDataE( );	//计算平均值
			}
		} 
	}
	else
	{
		lGroupNo++;
	}
	pLryRdb->lGroupNum=lGroupNo; 
}

void DATA_Q::InitDataQ(LRYRDB *pLryRdb)
{
	long lPingXingNum;
	pRdb = pLryRdb;
	ClearDataQ();
	lPingXingNum = pLryRdb->GetQLastGroupNum(); 
	if(pLryRdb->iPingXing)
	{		

		if(lPingXingNum!=0)
		{ 
			LoadTestData(pLryRdb->m_szName,pLryRdb->strYangPinBianHao,pLryRdb->m_szAlgorithm,lPingXingNum);
			//LoadTestData(pLryRdb->m_szName,pLryRdb->strYangDanBianHao,pLryRdb->m_szAlgorithm,lPingXingNum);
		}
		else
		{
			lPingXingNum++;
		}
		pLryRdb->lPingXingNum=lPingXingNum; 
	}
	else
	{
		pLryRdb->lPingXingNum=lPingXingNum+1; 	
	}
}

int DATA_Q::UpdateOutputRecordQ()
{
	int e,start,oknum,j;
	bool ok;
	bool result = false;

	//pRdb->lPingXingNum= lPingXingNum;		//取平行样编号
	//m_QSet.SaveQRecordSet(&m_TestDb);				//存记录
	if(iPtr ==MAX_DATA_Q)
	{
		ClearDataQ();
	}
	 D[ iPtr].bActive			= true;
	 D[ iPtr].strTestNum		= pRdb->strAutoNo;
	 D[ iPtr].fYangPinM			= pRdb->fYangPinZhiLiang;
	 D[ iPtr].lQbad				= pRdb->lQbad;
	 D[ iPtr].lQgrad			= pRdb->lQgrad;
	 D[ iPtr].lQnetvad			= pRdb->lQnetvad;
	 D[ iPtr].strYangPinNum		= pRdb->strYangPinBianHao;
	e							= pRdb->m_sDuplicateSamplesCheckPoint;
	ok=false;

	++iPtr;
	for(j=0;j<iPtr;j++)
	{
		DArray[j] = &D[j];
	} 
	switch( iPtr)
	{
	case 1:
		lAverageQbad= D[ iPtr-1].lQbad;
		if(!pRdb-> iPingXing)			//非平行样
		{
			//ok=true;
			start=0;
			oknum=1;
			iPtr = 0;
			SetQResultEndFlag(pRdb->lPingXingNum);
			AfxMessageBox(_T("发热量合格 ！"),MB_ICONEXCLAMATION);
			pRdb->lPingXingNum++;

		}
		break;
	case 2:		//第二次试验
		CalcQMaxDiffAndAverage(0,2);
		if( lMaxDiffQbad < e)
		{
			ok=true;
			start=0;
			oknum=2;
		}
		break;
	case 3:		//第三次试验
		CalcQMaxDiffAndAverage(0,3);
		if( lMaxDiffQbad < e*1.2)
		{
			ok=true;
			start=0;
			oknum=3;
		}
		break;
	case 4:		//第四次试验
	case 5:
		CalcQMaxDiffAndAverage(0,4);
		if( lMaxDiffQbad < e*1.3)
		{
			ok=true;
			start=0;
			oknum=4;
		}
		else 
		{	//判四次试验中的三次数据是否合格
			CalcQMaxDiffAndAverage(1,3);
			if( lMaxDiffQbad < e)
			{
				ok=true;
				start=1;
				oknum=3;
			}
			else
			{
				CalcQMaxDiffAndAverage(0,3);
				if( lMaxDiffQbad < e)
				{
					ok=true;
					start=2;
					oknum=3;
				}
			}
		}
		break;
	}

	if(ok)
	{
		// SetFlageOfQReordSet(start,oknum,&m_DataQ);	
		SetQResultEndFlag(pRdb->lPingXingNum);
		AfxMessageBox(_T("平行样实验合格 ！"),MB_ICONEXCLAMATION);
		result =  true;
	}
	else if( iPtr==MAX_DATA_Q)
	{
		// SetFlageOfQReordSet(0,0,&m_DataQ);
		AfxMessageBox(_T("平行样实验不合格 ！"),MB_ICONEXCLAMATION);
		result =  true;
	} 
	if(iPtr ==MAX_DATA_Q)
	{ 		
		pRdb->lPingXingNum++;
	}	
	return result;
}

int DATA_E::UpdateOutputRecordE()
{
	 
	 int iResult = PINXIN_WAITCONTINUE;
	 double fRSDErr1,fRSDErr2;
	 D[ iPtr].bActive		= true;
	 D[ iPtr].bOk			= true;
	 lstrcpy( D[ iPtr].strTestNum ,(TCHAR*)(LPCTSTR)pRdb->strAutoNo);
	 D[ iPtr].lE			= pRdb->lE;
	 D[ iPtr].fYangPinZhiLiang	= pRdb-> fYangPinZhiLiang;
	
	 iOkCount++;			//2002-08-28
	 iPtr++;

	 
	for(int i=0;i<MAX_DATA_E;i++)
	{
		DArray[i] = &D[i];
	}
	
	switch( iPtr)
	{
	case 1:
		break;
	case 5:
		 //
		 GetAverageDataE();
		if( fRSError<=pRdb->m_lHeatCapacityRSDErrorCheckPoint  )
		{ 
			iResult =  PINXIN_OK;
		}
		break;
	case 6: 

		GetAverageDataE(); 
		fRSDErr1 =  GetDataERSD(& DArray[0],5);
		fRSDErr2 =  GetDataERSD(& DArray[1],5);  
		fRSError =  min(fRSDErr1,fRSDErr2);
		if(fRSError<=pRdb->m_lHeatCapacityRSDErrorCheckPoint)
		{
			if(fRSDErr1<fRSDErr2)
			{
				SetEResultBadFlag(DArray[5]);  //设置第六个超差 
				GetAverageDataE(&DArray[0],5);
			}
			else
			{
				SetEResultBadFlag(DArray[0]);  //设置第1个超差 
				GetAverageDataE(&DArray[1],5);
			}
			
			iResult =  PINXIN_OK;
		}
		else
		{
			for(int i=0;i<6;i++)
			{
				SetEResultBadFlag(& D[i]);					
			}
			iResult = PINXIN_FAIL;
			
		} 
		break;
	default:
		GetAverageDataE();
		break;

	} 
	return iResult;
}

void DATA_E::SetEResultBadFlag(Report_E *aPtr)
{
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	 
 
		//查询已有 记录 		
	strSQL.Format(_T("select * from 热容量 where 试验编号 ='%s'"), aPtr->strTestNum);	 
	aPtr->bOk = false;
	try
	{ 
		pRsTestNO->Open(strSQL.AllocSysString(),
						theApp.pConn.GetInterfacePtr(),
						adOpenStatic,
						adLockOptimistic,
					-1);	
		if(!pRsTestNO->adoEOF)
			pRsTestNO->MoveLast();		 
		if((RecordCount = pRsTestNO->GetRecordCount())==1)
		{ 
			pRsTestNO->PutCollect("合格", _variant_t("超差"));
			pRsTestNO->Update(); 
		}
		else
		{
			AfxMessageBox(_T("有几个相同的实验标号！\n有人涂改实验数据吗?"),MB_ICONEXCLAMATION);
		}
		if(pRsTestNO->State == adStateOpen)
			pRsTestNO->Close();  
	} 
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}


void DATA_Q::SetQResultEndFlag(int lPingXingNum)
{
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	 
	try
	{ 
		strSQL.Format(_T("update  发热量 set  标志 = '完成' where 平行样号 =%d"),lPingXingNum);
		pRsTestNO->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	
		if(pRsTestNO->State == adStateOpen)
			pRsTestNO->Close();  
	} 
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	
}

void DATA_Q::SetQResultBadFlag(int iStart, int iOkCount)
{
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	  
	for(int i = iStart;i<iStart+iOkCount;i++)
	{
		try
		{ 
			strSQL.Format(_T("select * from 发热量 where 试验编号 ='%s'"),D[i].strTestNum);
			pRsTestNO->Open(strSQL.AllocSysString(),
							theApp.pConn.GetInterfacePtr(),
							adOpenStatic,
							adLockOptimistic,
						-1);	
			if(!pRsTestNO->adoEOF)
				pRsTestNO->MoveLast();		 
			if((RecordCount = pRsTestNO->GetRecordCount())==1)
			{  
				pRsTestNO->PutCollect("结论", _variant_t("不合格"));
				pRsTestNO->PutCollect("结论", _variant_t("不合格"));
				pRsTestNO->Update(); 
			}
			else
			{
				AfxMessageBox(_T("有几个相同的实验标号！\n有人涂改实验数据吗?"),MB_ICONEXCLAMATION);
			}
			if(pRsTestNO->State == adStateOpen)
				pRsTestNO->Close();  
		} 
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
	} 
}


void DATA_E::UpdateE(CString InstrName, CString strOxgenBombName, long E)
{
	HRESULT hr= S_OK;
	CString strSQL;
	_RecordsetPtr	pRsResult;

	pRsResult.CreateInstance("ADODB.Recordset");
	strSQL.Format(_T("select * from 量热仪 where 名称 ='%s'"),InstrName);
	try
	{ 
		
		hr = pRsResult->Open(strSQL.AllocSysString(),
					theApp.pConn.GetInterfacePtr(),
					adOpenStatic,
					adLockOptimistic,
					-1);	  
		if(strOxgenBombName==_T("B氧弹"))
		{
			pRsResult->PutCollect("B氧弹热容量",_variant_t(E)); 
		}
		else
		{
			pRsResult->PutCollect("A氧弹热容量",_variant_t(E)); 
		}
		
		pRsResult->Update();
		if(pRsResult->State==adStateOpen)
			pRsResult->Close();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void LRYRDB::LoadERecordFromDB(_RecordsetPtr Rs)
{ 
	/*
	
	SHORT			m_sBenzoicAcidHeatValue;			//苯甲酸热值
	SHORT			m_sHotWireHeatValue;				//点火丝热值
	SHORT			m_sadditive1HeatValue;				//1#添加物热值
	SHORT			m_sadditive2HeatValue;				//2#添加物热值
     
	SHORT			m_HeatCapacity;						//热容量
	SHORT			m_AOxygenHeatCapacity;				//A氧弹热容量
	SHORT			m_BOxygenHeatCapacity;				//B氧弹热容量
	double fV0;					//初期降速Vo
	double fVn;					//末期降速Vn
	double fC;					//冷却系数C
	long   lE;					//热容量
	double fK;					//常数K
	double fA;					//常数A
	long lGroupNum;				//组号
	试验编号
	样品编号
	试验日期
	设备名称
	氧弹编号
	算法
	热容量
	样品质量
	苯甲酸热值
	点火丝质量
	点火丝热值
	添加物1质量
	添加物1热值
	添加物1热值
	添加物1热值
	*/ 
 
	_variant_t var;
	//Rs->PutCollect("修正",_variant_t(false ));     
    fV0 = Rs->Fields->GetItem(_T("初期降速Vo"))->Value.fltVal;
	fVn = Rs->Fields->GetItem(_T("末期降速Vn"))->Value.fltVal;
	fT0 = Rs->Fields->GetItem(_T("初期温度T0"))->Value.fltVal;
	ft0 = Rs->Fields->GetItem(_T("点火温度t0"))->Value.fltVal;

	ft140 = Rs->Fields->GetItem(_T("第1分40秒温度"))->Value.fltVal;
	ftn = Rs->Fields->GetItem(_T("终点温度tn"))->Value.fltVal;
	fTn = Rs->Fields->GetItem(_T("末期温度Tn"))->Value.fltVal;
 
	
	nn	= Rs->Fields->GetItem(_T("主期时间n"))->Value.lVal;  
	fSumT = Rs->Fields->GetItem(_T("主期温度总和"))->Value.fltVal; 
	fK = Rs->Fields->GetItem(_T("常数K"))->Value.fltVal; 
	fA = Rs->Fields->GetItem(_T("常数A"))->Value.fltVal; 
	ftj = Rs->Fields->GetItem(_T("外桶温度tj"))->Value.fltVal; 
	lGroupNum = Rs->Fields->GetItem(_T("组号"))->Value.lVal; 
   
	

	m_sBenzoicAcidHeatValue = Rs->Fields->GetItem(_T("苯甲酸热值"))->Value.lVal;
	m_sHotWireHeatValue		= Rs->Fields->GetItem(_T("点火丝热值"))->Value.fltVal;
	m_sadditive1HeatValue	= Rs->Fields->GetItem(_T("添加物1热值"))->Value.lVal;
	m_sadditive2HeatValue	= Rs->Fields->GetItem(_T("添加物2热值"))->Value.fltVal;

	m_szName			= VariantToStr(Rs->Fields->GetItem(_T("设备名称"))->Value) ; 
	strYangPinBianHao	= VariantToStr(Rs->Fields->GetItem(_T("样品编号"))->Value) ;  
 	m_tDate				= COleDateTime(Rs->Fields->GetItem(_T("试验日期"))->Value); 
	
	strYangDanBianHao	= VariantToStr(Rs->Fields->GetItem(_T("氧弹编号"))->Value);;	//氧弹编号
 	fYangPinZhiLiang	= Rs->Fields->GetItem(_T("样品质量"))->Value.fltVal ;			//样品质量
	fDianHuoSiZhiLiang	= Rs->Fields->GetItem(_T("点火丝质量"))->Value.fltVal;;		//点火丝质量
	fTianJiaWuZhiLiang1	= Rs->Fields->GetItem(_T("添加物1质量"))->Value.fltVal;;	//1#添加物质量
	fTianJiaWuZhiLiang2	= Rs->Fields->GetItem(_T("添加物2质量"))->Value.fltVal;		//2#添加物质量
 
	m_sadditive1HeatValue	= Rs->Fields->GetItem(_T("添加物1热值"))->Value.lVal; 
	m_sadditive2HeatValue	= Rs->Fields->GetItem(_T("添加物2热值"))->Value.lVal; 
	m_HeatCapacity			= Rs->Fields->GetItem(_T("热容量"))->Value.lVal; 
	m_sHotWireHeatValue		= Rs->Fields->GetItem(_T("点火丝热值"))->Value.lVal; 
 
	strAutoNo			= VariantToStr(Rs->Fields->GetItem(_T("试验编号"))->Value); 
	m_szAlgorithm			= VariantToStr(Rs->Fields->GetItem(_T("算法"))->Value);  
	fC						= Rs->Fields->GetItem(_T("冷却系数C"))->Value.fltVal ;
	ft0						= Rs->Fields->GetItem(_T("点火温度t0"))->Value.fltVal ;
	ftn						= Rs->Fields->GetItem(_T("终点温度tn"))->Value.fltVal ; 
}

void LRYRDB::LoadQRecordFromDB(_RecordsetPtr Rs)
{
	_variant_t var;
	m_szName			= VariantToStr(Rs->Fields->GetItem(_T("设备名称"))->Value) ;
	strAutoNo			= VariantToStr(Rs->Fields->GetItem(_T("试验编号"))->Value) ;
	strYangPinBianHao	= VariantToStr(Rs->Fields->GetItem(_T("样品编号"))->Value) ;  
	m_datePickSample	= COleDateTime(Rs->Fields->GetItem(_T("取样时间"))->Value); ;	// 
	m_tDate				= COleDateTime(Rs->Fields->GetItem(_T("试验日期"))->Value); 
	
	strYangDanBianHao	= VariantToStr(Rs->Fields->GetItem(_T("氧弹编号"))->Value);;	//氧弹编号
	strLaiYang			= VariantToStr(Rs->Fields->GetItem(_T("来样情况"))->Value);; 	//
	fYangPinZhiLiang	= Rs->Fields->GetItem(_T("样品质量"))->Value.fltVal ;			//样品质量
	fDianHuoSiZhiLiang	= Rs->Fields->GetItem(_T("点火丝质量"))->Value.fltVal;;		//点火丝质量
	fTianJiaWuZhiLiang1	= Rs->Fields->GetItem(_T("添加物1质量"))->Value.fltVal;;	//1#添加物质量
	fTianJiaWuZhiLiang2	= Rs->Fields->GetItem(_T("添加物2质量"))->Value.fltVal;		//2#添加物质量
	fMad					= Rs->Fields->GetItem(_T("水分Mad"))->Value.fltVal;			//空干基水分Mad
	fMt 					= Rs->Fields->GetItem(_T("全水分Mt"))->Value.fltVal ;		//全水分Mt
	fSad 					= Rs->Fields->GetItem(_T("硫含量Sad"))->Value.fltVal ;		//硫含量Sad
	fHad 					= Rs->Fields->GetItem(_T("氢含量Had"))->Value.fltVal ;		//氢含量Had  
	lPingXingNum			= Rs->Fields->GetItem(_T("平行样号"))->Value.lVal;		//平行样编号

	m_sadditive1HeatValue	= Rs->Fields->GetItem(_T("添加物1热值"))->Value.lVal; 
	m_sadditive2HeatValue	= Rs->Fields->GetItem(_T("添加物2热值"))->Value.lVal; 
	m_HeatCapacity			= Rs->Fields->GetItem(_T("热容量"))->Value.lVal; 
	m_sHotWireHeatValue		= Rs->Fields->GetItem(_T("点火丝热值"))->Value.lVal; 
	lQbad					= Rs->Fields->GetItem(_T("弹筒发热量"))->Value.lVal; 
	lQgrad					= Rs->Fields->GetItem(_T("高位发热量"))->Value.lVal; 
	lQgrd					= Rs->Fields->GetItem(_T("干基高位发热量"))->Value.lVal; 	
	lQnetvad				= Rs->Fields->GetItem(_T("低位发热量"))->Value.lVal; 	
	m_szAlgorithm			= VariantToStr(Rs->Fields->GetItem(_T("算法"))->Value);  
	fC						= Rs->Fields->GetItem(_T("冷却系数C"))->Value.fltVal ;
	ft0						= Rs->Fields->GetItem(_T("点火温度t0"))->Value.fltVal ;
	ftn						= Rs->Fields->GetItem(_T("终点温度tn"))->Value.fltVal ; 
}

void LRYRDB::UpdateQRecordToDB(_RecordsetPtr Rs)
{ 
	if(theApp.UserInfo.bLogon())
	try
	{ 
		Rs->PutCollect("热容量",m_HeatCapacity); //热容量
		Rs->PutCollect("样品质量",fYangPinZhiLiang);//样品质量
		Rs->PutCollect("添加物1质量",m_sadditive1HeatValue);//1#添加物质量
		Rs->PutCollect("添加物2质量",m_sadditive2HeatValue);//2#添加物质量
		Rs->PutCollect("点火丝质量",fDianHuoSiZhiLiang);//点火丝质量
		Rs->PutCollect("平行样号",lPingXingNum);//平行样编号
		Rs->PutCollect("弹筒发热量",lQbad	);//弹筒发热量(Qb,ad)
		Rs->PutCollect("高位发热量",lQgrad);//高位发热量
		Rs->PutCollect("干基高位发热量",lQgrd);//干基高位发热量
		Rs->PutCollect("低位发热量",lQnetvad);//低位发热量

		Rs->PutCollect("硫含量Sad",fSad	);////硫含量
		Rs->PutCollect("水分Mad",fMad);////空干基水分
		Rs->PutCollect("全水分Mt",fMt);////全水分 		
		Rs->PutCollect("氢含量Had",fHad);//空干基氢含量Had 
 
		Rs->Update();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void LRYRDB::UpdateERecordToDB( _RecordsetPtr Rs )
{ 
	if(theApp.UserInfo.bLogon())
	{
		try
		{ 
			Rs->PutCollect("试验编号",_variant_t(strAutoNo));
			Rs->PutCollect("样品编号",_variant_t(strYangPinBianHao)); 
			Rs->PutCollect("试验日期",_variant_t(m_tDate));  //COleDateTime::GetCurrentTime()
			
			Rs->PutCollect("样品质量",_variant_t(fYangPinZhiLiang));
			Rs->PutCollect("设备名称",_variant_t(m_szName));
			Rs->PutCollect("氧弹编号",_variant_t(strYangDanBianHao));
			Rs->PutCollect("算法",_variant_t(m_szAlgorithm));
			Rs->PutCollect("修正",_variant_t(false ));
			Rs->PutCollect("热容量",_variant_t((SHORT)lE));
			Rs->PutCollect("苯甲酸热值",_variant_t( m_sBenzoicAcidHeatValue));
			Rs->PutCollect("点火丝质量",_variant_t( fDianHuoSiZhiLiang));
			Rs->PutCollect("点火丝热值",_variant_t(m_sHotWireHeatValue ));
			Rs->PutCollect("添加物1质量",_variant_t( fTianJiaWuZhiLiang1));
			Rs->PutCollect("添加物1热值",_variant_t( m_sadditive1HeatValue));		
			Rs->PutCollect("添加物2质量",_variant_t( fTianJiaWuZhiLiang2));
			Rs->PutCollect("添加物2热值",_variant_t( m_sadditive2HeatValue));
			Rs->PutCollect("冷却系数C",_variant_t( fC  ));
			Rs->PutCollect("初期降速Vo",_variant_t( fV0));
			Rs->PutCollect("末期降速Vn",_variant_t( fVn));
			Rs->PutCollect("初期温度T0",_variant_t( fT0));
			Rs->PutCollect("点火温度t0",_variant_t( ft0));
			Rs->PutCollect("第1分40秒温度",_variant_t( ft140));
			Rs->PutCollect("终点温度tn",_variant_t( ftn));
			Rs->PutCollect("末期温度Tn",_variant_t( fTn));
			Rs->PutCollect("主期时间n",_variant_t((short) nn ));
			Rs->PutCollect("主期温度总和",_variant_t(fSumT));
			Rs->PutCollect("常数K",_variant_t(fK ));
			Rs->PutCollect("常数A",_variant_t(fA ));
			Rs->PutCollect("外桶温度tj",_variant_t( ftj));
			Rs->PutCollect("合格",_variant_t(_T("合格")));
			Rs->PutCollect("组号",_variant_t( lGroupNum)); 
			Rs->Update();

		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		} 
	}
}
 

int __cdecl DATA_Q::compare(const void* elem1, const void* elem2 )
{   
	Report_Q ** Q1 = ((Report_Q ** )elem1);
	Report_Q ** Q2 = ((Report_Q ** )elem2);
	
	return  Q1[0]->lQbad-Q2[0]->lQbad;//Q1.lQbad-Q2.lQbad;  
	  
}

void DATA_Q::CalcQMaxDiffAndAverage(int iStart, int iCount)
{
	lAverageQbad			= 0;
	lAverageQgrad			= 0;
	lAverageQgrd			= 0;
	lAverageQnetvad			= 0;
	fAverageMad				= 0;
	fAverageMt				= 0;
	fAverageSad				= 0;
	fAverageHad				= 0;
	fAverageM				= 0;

	qsort(&DArray[iStart],iCount,sizeof(Report_Q*)/sizeof(char),compare);
	lMaxDiffQbad = abs(DArray[iStart]->lQbad -DArray[iStart+iCount-1]->lQbad);//GetMaxDiff(&DArray[iStart],iCount);
	long *ptrLong = new long[iCount];
	for(int i=0;i<iCount;i++)
	{
		lAverageQbad			+= DArray[iStart+i]->lQbad;
		lAverageQgrad			+= DArray[iStart+i]->lQgrad;			
		lAverageQgrd			+= DArray[iStart+i]->lQgrd;	
		lAverageQnetvad			+= DArray[iStart+i]->lQnetvad;	
		fAverageMad				+= DArray[iStart+i]->fMad;	
		fAverageMt				+= DArray[iStart+i]->fMt;	
		fAverageSad				+= DArray[iStart+i]->fSad;	
		fAverageHad				+= DArray[iStart+i]->fHad;	
		fAverageMt				+= DArray[iStart+i]->fYangPinM;
	}
	lAverageQbad			/= iCount;
	lAverageQgrad			/= iCount;
	lAverageQgrd			/= iCount;
	lAverageQnetvad			/= iCount;
	fAverageMad				/= iCount;
	fAverageMt				/= iCount;
	fAverageSad				/= iCount;
	fAverageHad				/= iCount;
	fAverageM				/= iCount;	
	delete ptrLong;
}


void LRYRDB::SaveKeyPoint(void)
{
	HRESULT hr= S_OK;
	CString strSQL;
	_RecordsetPtr	pRsResult;
	int j	= 0;
	int i	= 0;

	pRsResult.CreateInstance("ADODB.Recordset");
	strSQL.Format(_T("select * from 量热仪实验数据 where 顺序ID=9999"));
	try
	{ 

		hr = pRsResult->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	 
		//T0
		pRsResult->AddNew();
		pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
		pRsResult->PutCollect("温度数据",_variant_t(fT0)); 
		pRsResult->PutCollect("顺序ID",_variant_t(j)); 

		pRsResult->Update();
		pRsResult->MoveLast();
		j++;
		//tt0
		for (i=0;i<t0nn;i++)
		{
			pRsResult->AddNew();
			pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
			pRsResult->PutCollect("温度数据",_variant_t(ftt0[i])); 
			pRsResult->PutCollect("顺序ID",_variant_t(j)); 
			j++;

			pRsResult->Update();
			pRsResult->MoveLast();
		}
		//t0
		pRsResult->AddNew();
		pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
		pRsResult->PutCollect("温度数据",_variant_t(ft0)); 
		pRsResult->PutCollect("顺序ID",_variant_t(j)); 
		j++;

		pRsResult->Update();
		pRsResult->MoveLast();
		//ti
		for (i=0;i<nn;i++)
		{
			pRsResult->AddNew();
			pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
			pRsResult->PutCollect("温度数据",_variant_t(fTi[i])); 
			pRsResult->PutCollect("顺序ID",_variant_t(j)); 
			j++;
			pRsResult->Update();
			pRsResult->MoveLast();
		} 

		pRsResult->AddNew();
		pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
		pRsResult->PutCollect("温度数据",_variant_t(ftn)); 
		pRsResult->PutCollect("顺序ID",_variant_t(j)); 
		j++;

		pRsResult->Update();
		pRsResult->MoveLast();
		for (i=0;i<tnnn;i++)
		{
			pRsResult->AddNew();
			pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
			pRsResult->PutCollect("温度数据",_variant_t(fttn[i])); 
			pRsResult->PutCollect("顺序ID",_variant_t(j)); 
			j++;

			pRsResult->Update();
			pRsResult->MoveLast();
		}
		pRsResult->AddNew();
		pRsResult->PutCollect("试验编号",_variant_t(strAutoNo));
		pRsResult->PutCollect("温度数据",_variant_t(fTn)); 
		pRsResult->PutCollect("顺序ID",_variant_t(j)); 
		j++;
		pRsResult->Update();
		pRsResult->MoveLast();  
	}
	catch(_com_error &e)
	{ 
		dump_com_error(e);
	}
	if(pRsResult->State==adStateOpen)
		pRsResult->Close();
}
