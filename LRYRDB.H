#ifndef _LRYRDB_H_
#define _LRYRDB_H_
/********************************************************************
	created:	2011/11/01
	created:	1:11:2011   15:34
	filename: 	D:\通发\量热仪\程序\上位机\LRYCTRL\LRYRDB.H
	file path:	D:\通发\量热仪\程序\上位机\LRYCTRL
	file base:	LRYRDB
	file ext:	H
	author:		xukaiming
	
	purpose:	
*********************************************************************/
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// NOTE : In order to use this code against a different version of ADO, the appropriate
// ADO library needs to be used in the #import statement

#include "icrsint.h"

#include "oledb.h"
#include "lryimageslave.h"
#include "rdb.h"
// LRYRDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LRYRDB class
_COM_SMARTPTR_TYPEDEF(IADORecordBinding, __uuidof(IADORecordBinding));


class LRYRBindDB : public CADORecordBinding
{
BEGIN_ADO_BINDING(LRYRBindDB)
	ADO_VARIABLE_LENGTH_ENTRY2(  1, adVarWChar, m_szName, sizeof(m_szName), m_lNameStatus, FALSE)
	ADO_VARIABLE_LENGTH_ENTRY2(  2, adVarWChar, m_szType, sizeof(m_szType), m_lTypeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  3, adSmallInt, m_bID, m_lIDStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  4, adSmallInt, m_sCommID, m_lCommIDStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  5, adInteger, m_lScanPriod_mS, m_lScanPriod_mSStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  6, adSingle, m_fR0, m_lR0Status, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  7, adSmallInt, m_sWaterHeight, m_lWaterHeightStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  8, adSmallInt, m_sFillWaterTime, m_lFillWaterTimeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    (  9, adSmallInt, m_sOutWaterTime, m_lOutWaterTimeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 10, adSmallInt, m_sStartJiaoBanTime1, m_lStartJiaoBanTime1Status, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 11, adSmallInt, m_sStartJiaoBanTime2, m_lStartJiaoBanTime2Status, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 12, adSmallInt, m_sEndtJiaoBanTime, m_lEndtJiaoBanTimeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 13, adSmallInt, m_sFireTime, m_lFireTimeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 14, adSmallInt, m_sSenserPowerONTime, m_lSenserPowerONTimeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 15, adSmallInt, m_sBeepTime, m_lBeepTimeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 16, adSingle, m_fFireOKtemp, m_lFireOKtempStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 17, adSingle, m_fEndCheckTemp, m_lEndCheckTempStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 18, adSingle, m_fCoffa, m_lCoffaStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 19, adSingle, m_fCoffb, m_lCoffbStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 20, adSingle, m_fCoffc, m_lCoffcStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 21, adSingle, m_fCoffd, m_lCoffdStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 22, adSingle, m_fCoffe, m_lCoffeStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 23, adSingle, m_fCofff, m_lCofffStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 24, adSingle, m_fCoffg, m_lCoffgStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 25, adSingle, m_fCoffh, m_lCoffhStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 26, adSmallInt, m_sBenzoicAcidHeatValue, m_lBenzoicAcidHeatValueStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 27, adSmallInt, m_sHotWireHeatValue, m_lHotWireHeatValueStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 28, adSmallInt, m_sadditive1, m_ladditive1Status, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 29, adSmallInt, m_sadditive2, m_ladditive2Status, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 30, adInteger, m_lHeatCapacityErrorCheckPoint, m_lHeatCapacityErrorCheckPointStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 31, adSingle, m_lHeatCapacityRSDErrorCheckPoint, m_lHeatCapacityRSDErrorCheckPointStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 32, adSmallInt, m_sDuplicateSamplesCheckPoint, m_lDuplicateSamplesCheckPointStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 33, adBoolean, m_bFix, m_lFixStatus, FALSE)
	ADO_VARIABLE_LENGTH_ENTRY2( 34, adVarWChar, m_szAlgorithm, sizeof(m_szAlgorithm), m_lAlgorithmStatus, FALSE)
	ADO_FIXED_LENGTH_ENTRY    ( 35, adBoolean, m_bAutoPrint, m_lAutoPrintStatus, FALSE)
END_ADO_BINDING()

//Attributes
public:
	CHAR			m_szName[41];
	ULONG			m_lNameStatus;
	CHAR			m_szType[21];
	ULONG			m_lTypeStatus;
	SHORT			m_bID;
	ULONG			m_lIDStatus;
	SHORT			m_sCommID;
	ULONG			m_lCommIDStatus;
	LONG			m_lScanPriod_mS;
	ULONG			m_lScanPriod_mSStatus;
	FLOAT			m_fR0;
	ULONG			m_lR0Status;
	SHORT			m_sWaterHeight;
	ULONG			m_lWaterHeightStatus;
	SHORT			m_sFillWaterTime;
	ULONG			m_lFillWaterTimeStatus;
	SHORT			m_sOutWaterTime;
	ULONG			m_lOutWaterTimeStatus;
	SHORT			m_sStartJiaoBanTime1;
	ULONG			m_lStartJiaoBanTime1Status;
	SHORT			m_sStartJiaoBanTime2;
	ULONG			m_lStartJiaoBanTime2Status;
	SHORT			m_sEndtJiaoBanTime;
	ULONG			m_lEndtJiaoBanTimeStatus;
	SHORT			m_sFireTime;
	ULONG			m_lFireTimeStatus;
	SHORT			m_sSenserPowerONTime;
	ULONG			m_lSenserPowerONTimeStatus;
	SHORT			m_sBeepTime;
	ULONG			m_lBeepTimeStatus;
	FLOAT			m_fFireOKtemp;
	ULONG			m_lFireOKtempStatus;
	FLOAT			m_fEndCheckTemp;
	ULONG			m_lEndCheckTempStatus;
	FLOAT			m_fCoffa;
	ULONG			m_lCoffaStatus;
	FLOAT			m_fCoffb;
	ULONG			m_lCoffbStatus;
	FLOAT			m_fCoffc;
	ULONG			m_lCoffcStatus;
	FLOAT			m_fCoffd;
	ULONG			m_lCoffdStatus;
	FLOAT			m_fCoffe;
	ULONG			m_lCoffeStatus;
	FLOAT			m_fCofff;
	ULONG			m_lCofffStatus;
	FLOAT			m_fCoffg;
	ULONG			m_lCoffgStatus;
	FLOAT			m_fCoffh;
	ULONG			m_lCoffhStatus;
	SHORT			m_sBenzoicAcidHeatValue;
	ULONG			m_lBenzoicAcidHeatValueStatus;
	SHORT			m_sHotWireHeatValue;
	ULONG			m_lHotWireHeatValueStatus;
	SHORT			m_sadditive1;
	ULONG			m_ladditive1Status;
	SHORT			m_sadditive2;
	ULONG			m_ladditive2Status;
	LONG			m_lHeatCapacityErrorCheckPoint;	
	ULONG			m_lHeatCapacityErrorCheckPointStatus;
	FLOAT			m_lHeatCapacityRSDErrorCheckPoint;
	ULONG			m_lHeatCapacityRSDErrorCheckPointStatus;
	SHORT			m_sDuplicateSamplesCheckPoint;
	ULONG			m_lDuplicateSamplesCheckPointStatus;
	VARIANT_BOOL	m_bFix;
	ULONG			m_lFixStatus;
	CHAR			m_szAlgorithm[11];
	ULONG			m_lAlgorithmStatus;
	VARIANT_BOOL	m_bAutoPrint;
	ULONG			m_lAutoPrintStatus;
};


#define MAX_DATA_E		8
#define MAX_DATA_Q		5
#define MAX_CURVE		300

class LRYRDB :public CRDB
{ 
public:
	LRYRDB();
	~LRYRDB();
public:
	void UpdateQRecordToDB(_RecordsetPtr Rs);
	void LoadQRecordFromDB(_RecordsetPtr Rs);
	void LoadERecordFromDB(_RecordsetPtr Rs);
	void UpdateERecordToDB(_RecordsetPtr Rs);
	void ReCalculateQ();
	LONG GetELastGroupNum();
	LONG GetQLastGroupNum();
	void SaveCurveToFile();
	void SaveEtoDatabase();
	void SaveQtoDatabase();
	CString BuildAutoNO(CString sWorkType,CString strAB);
	double CalcStandardErrorE(long *pData, int iCount);

	//CString			m_szName ;			//设备名称
	//CString			m_szType ;			//设备型号	
	//SHORT			m_bID;				//标识符
	//SHORT			m_sCommID;			//串口号
	//LONG			m_lScanPriod_mS;	//扫描周期
	FLOAT			m_fR0;				//0度铂电阻阻值
	SHORT			m_sWaterHeight;			

	SHORT			m_sFillWaterTime;		//进水时间
	SHORT			m_sOutWaterTime;		//放水时间
	SHORT			m_sStartJiaoBanTime1;	//初期搅拌时间1
	SHORT			m_sStartJiaoBanTime2;	//初期搅拌时间2
	SHORT			m_sEndtJiaoBanTime;     //末期搅拌时间
	SHORT			m_sFireTime;            //点火时间
	SHORT			m_sSenserPowerONTime; 
	SHORT			m_sBeepTime;			//蜂鸣时间
	FLOAT			m_fFireOKtemp;			//点火成功温差
	FLOAT			m_fEndCheckTemp;		//末期判点

	CString			m_szAlgorithm ;			//算法

	FLOAT			m_fCoffa; 
	FLOAT			m_fCoffb; 
	FLOAT			m_fCoffc; 
	FLOAT			m_fCoffd; 
	FLOAT			m_fCoffe; 
	FLOAT			m_fCofff; 
	FLOAT			m_fCoffg; 
	FLOAT			m_fCoffh; 

	SHORT			m_sBenzoicAcidHeatValue;			//苯甲酸热值
	SHORT			m_sHotWireHeatValue;				//点火丝热值
	SHORT			m_sadditive1HeatValue;				//1#添加物热值
	SHORT			m_sadditive2HeatValue;				//2#添加物热值
	LONG			m_lHeatCapacityErrorCheckPoint;		//热容量误差判点
	FLOAT			m_lHeatCapacityRSDErrorCheckPoint;  //热容量相对标准差判点
	SHORT			m_sDuplicateSamplesCheckPoint;		//平行样判点
	
	BOOL			m_bFix;								//修正
	
	BOOL			m_bAutoPrint;						//自动打印
	SHORT			m_HeatCapacity;						//热容量
	SHORT			m_AOxygenHeatCapacity;				//A氧弹热容量
	SHORT			m_BOxygenHeatCapacity;				//B氧弹热容量


	//test param from dlg
	CString strAutoNo;				//实验编号
	CString strWorkType;			//实验类型
	//CString strShiYanBianHao;		//试样编号
	COleDateTime m_datePickSample;	//取样时间
	COleDateTime m_tDate;			//实验日期
	CString strYangPinBianHao;		//样品编号
	CString strYangDanBianHao;		//氧弹编号
	CString strLaiYang;				//来样情况
	double	fYangPinZhiLiang;		//样品质量
	double	fDianHuoSiZhiLiang;		//点火丝质量
	double	fTianJiaWuZhiLiang1;	//1#添加物质量
	double	fTianJiaWuZhiLiang2;	//2#添加物质量
	double	fMad;					//空干基水分Mad
	double	fMt;					//全水分Mt
	double	fSad;					//硫含量Sad
	double	fHad;					//氢含量Had  
	int		iPingXing;				//1=做平行样
	long	lPingXingNum;			//平行样编号

	//calculate


	LONG	nSpeed;		//转数
	UINT	iAd;		//AD读数
	double	fAdV;		//AD电压
	double	fRt;		//探头电阻
	double	fty;		//实测温度用Rt=R0(1+At+Bt^2)求得
	double	ft;			//当前内桶(校正)温度

	double	ftj;		//外桶温度 
	double	fT0;		//初期内桶温度
	double  *ftt0;
	double	ft0;		//点火时内桶温度
	double	ft140;		//1'40''内桶温度
	double	ftn;		//终点内桶温度
	double  *fttn;
	double	fTn;		//末期内桶温度
	double	fSumT;		//主期温度总和Σti
	double  *fTi;
	int	nn;				//t0到tn的分钟数n
	int t0nn;			//t0的个数
	int tnnn;           //tn的个数

	double fV0;					//初期降速Vo
	double fVn;					//末期降速Vn
	double fC;					//冷却系数C
	long   lE;					//热容量
	double fK;					//常数K
	double fA;					//常数A
	long lGroupNum;				//组号

	long lQbad;					//弹筒发热量
	long lQgrad;				//高位发热量
	long lQgrd;					//干基高位发热量
	long lQnetvad;				//低位发热量
	 
	double fCurve[MAX_CURVE];	//温升曲线
	void CalcV0Vn(bool bAK); 
	void CalcV0VnC(bool bE,bool bTestE);
	double GetTemperature(double Rt);
	void RefreshRDBReportTab(SLryImageSlave *pLryImageSlave);
	void InitRDB(SLryImageSlave *pLryImageSlave);
	double GetVoltage(ULONG iAD); 
	void RefreshRDBTempTab(SLryImageSlave *pLryImageSlave);
	void CalculateE(); 
	void ReCalculateE();
	void CalculateQ(); 
	double  GetRt(ULONG iAd); 
	
private:
	
public:
	void SaveKeyPoint(void);
};
// 

//热容量数据
class DATA_E
{
public:
	void UpdateE(CString InstrName, CString strOxgenBombName,long E);
	int UpdateOutputRecordE();
	void InitDataE(LRYRDB *pLryRdb);
	void BubbleSort (); 
	void GetAverageDataE(); 
	void ClearDataE(); 
	int  LoadTestData(CString strDeviceName,CString strYangDanBianHao,CString strTestMode,long lDuplicateSampleNO );
	int	 iPtr;
	int	 iOkCount;

	long  lAverageE;				//平均热容量
	float fSError;					//标准差
	float fRSError;					//相对标准差
	long  lMaxDiff;					//极差值
	float fK;						//常数K
	float fA;						//常数A
	CString szMode;					//算法
	COleDateTime tDate;
	struct Report_E
	{ 
		bool	bActive;
		bool	bOk;
		long	lE;					//热容量
		double	fC;					//冷却系数
		double	fYangPinZhiLiang;	//样品质量
		double	fZhuQiWenSheng;		//主期温生
		double	ft0;				//点火温度
		double	fV0;				//初期降速
		double	fVn;				//末期降速  
		//CString strTestNum;		//试验编号
		TCHAR	strTestNum[20];
	}D[MAX_DATA_E];
	Report_E* DArray[MAX_DATA_E];
	typedef Report_E* APtr;
	DATA_E()
	{
		
		for(int i=0;i<MAX_DATA_E;i++)
		{
			D[i].bActive			= false;
			D[i].bOk				= false;
			D[i].lE					= 0;					//热容量
			D[i].fC 				= 0;//冷却系数
			D[i].fYangPinZhiLiang	= 0;//样品质量
			D[i].fZhuQiWenSheng		= 0;//主期温生
			D[i].ft0				= 0;//点火温度
			D[i].fV0 				= 0;//初期降速
			D[i].fVn 				= 0;//末期降速 
		}
		lAverageE	= 0;					//平均热容量
		fSError		= 0;					//标准差
		fRSError	= 0; 					//相对标准差
		lMaxDiff	= 0; 					//极差值
		iPtr		= 0;
		
	};
	static int __cdecl compare(const void* elem1, const void* elem2 );
	void swap(APtr* ptr1,APtr* ptr2);
	double  GetDataERSD(Report_E **ptrD,int Len);
	void GetAverageDataE(Report_E **ptrD,int Len);
	void SetEResultBadFlag(Report_E *aPtr);
	~DATA_E(){};
private:
	LRYRDB *pRdb;
};


//发热量数据

enum PinXinState{PINXIN_WAITCONTINUE=0,PINXIN_OK,PINXIN_FAIL}	;
class DATA_Q
{

public:
	void SetQResultEndFlag(int  iPingXingNO);
	
	void SetQResultBadFlag(int iStart, int iOkCount);
	int UpdateOutputRecordQ();
	int LoadTestData(CString strDeviceName,CString strYangDanBianHao,CString strTestMode,long &lDuplicateSampleNO );
	void ClearDataQ();
	int iPtr;
	long lPingXingNum;				//平行样编号
	long lMaxDiffQbad;				//极差
	long lAverageQbad;				//平均弹筒发热量
	long lAverageQgrad;				//平均空干基高位发热量
	long lAverageQgrd;				//平均干基高位发热量 
	long lAverageQnetvad;			//平均低位发热量
	float fAverageMad;				//平均空干基水分
	float fAverageMt;				//平均全水分
	float fAverageSad;				//平均硫含量
	float fAverageHad;				//平均氢含量
	float fAverageM;				//平均质量
	CString m_strMode;				//算法
	CString m_strOk;				//结论
	COleDateTime tDate;
	struct  Report_Q
	{
		bool	bActive;
		CString strTestNum;			//试验编号
		CString strYangPinNum;		//样品编号
		float	fYangPinM;			//样品质量
		long	lQbad;				//弹筒发热量
		long	lQgrad;				//空干基高位发热量
		long	lQgrd;				//干基高位发热量=Qgrad*100/(100-Mad)
		long	lQnetvad;			//低位发热量
		float	fMad;				//空干基水分
		float	fMt;				//全水分
		float	fSad;				//硫含量
		float	fHad;				//氢含量Had
	}D[MAX_DATA_Q];
	Report_Q* DArray[MAX_DATA_Q];
	void  InitDataQ(LRYRDB *pLryRdb);
	static int __cdecl compare(const void* elem1, const void* elem2 );
	//typedef int (__cdecl *compare )(const void* elem1, const void* elem2 );
private:
	void CalcQMaxDiffAndAverage(int iStart, int iCount);
	LRYRDB *pRdb;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !_LRYRDB_H_
 