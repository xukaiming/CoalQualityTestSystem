#ifndef _LRYRDB_H_
#define _LRYRDB_H_
/********************************************************************
	created:	2011/11/01
	created:	1:11:2011   15:34
	filename: 	D:\ͨ��\������\����\��λ��\LRYCTRL\LRYRDB.H
	file path:	D:\ͨ��\������\����\��λ��\LRYCTRL
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

	//CString			m_szName ;			//�豸����
	//CString			m_szType ;			//�豸�ͺ�	
	//SHORT			m_bID;				//��ʶ��
	//SHORT			m_sCommID;			//���ں�
	//LONG			m_lScanPriod_mS;	//ɨ������
	FLOAT			m_fR0;				//0�Ȳ�������ֵ
	SHORT			m_sWaterHeight;			

	SHORT			m_sFillWaterTime;		//��ˮʱ��
	SHORT			m_sOutWaterTime;		//��ˮʱ��
	SHORT			m_sStartJiaoBanTime1;	//���ڽ���ʱ��1
	SHORT			m_sStartJiaoBanTime2;	//���ڽ���ʱ��2
	SHORT			m_sEndtJiaoBanTime;     //ĩ�ڽ���ʱ��
	SHORT			m_sFireTime;            //���ʱ��
	SHORT			m_sSenserPowerONTime; 
	SHORT			m_sBeepTime;			//����ʱ��
	FLOAT			m_fFireOKtemp;			//���ɹ��²�
	FLOAT			m_fEndCheckTemp;		//ĩ���е�

	CString			m_szAlgorithm ;			//�㷨

	FLOAT			m_fCoffa; 
	FLOAT			m_fCoffb; 
	FLOAT			m_fCoffc; 
	FLOAT			m_fCoffd; 
	FLOAT			m_fCoffe; 
	FLOAT			m_fCofff; 
	FLOAT			m_fCoffg; 
	FLOAT			m_fCoffh; 

	SHORT			m_sBenzoicAcidHeatValue;			//��������ֵ
	SHORT			m_sHotWireHeatValue;				//���˿��ֵ
	SHORT			m_sadditive1HeatValue;				//1#��������ֵ
	SHORT			m_sadditive2HeatValue;				//2#��������ֵ
	LONG			m_lHeatCapacityErrorCheckPoint;		//����������е�
	FLOAT			m_lHeatCapacityRSDErrorCheckPoint;  //��������Ա�׼���е�
	SHORT			m_sDuplicateSamplesCheckPoint;		//ƽ�����е�
	
	BOOL			m_bFix;								//����
	
	BOOL			m_bAutoPrint;						//�Զ���ӡ
	SHORT			m_HeatCapacity;						//������
	SHORT			m_AOxygenHeatCapacity;				//A����������
	SHORT			m_BOxygenHeatCapacity;				//B����������


	//test param from dlg
	CString strAutoNo;				//ʵ����
	CString strWorkType;			//ʵ������
	//CString strShiYanBianHao;		//�������
	COleDateTime m_datePickSample;	//ȡ��ʱ��
	COleDateTime m_tDate;			//ʵ������
	CString strYangPinBianHao;		//��Ʒ���
	CString strYangDanBianHao;		//�������
	CString strLaiYang;				//�������
	double	fYangPinZhiLiang;		//��Ʒ����
	double	fDianHuoSiZhiLiang;		//���˿����
	double	fTianJiaWuZhiLiang1;	//1#����������
	double	fTianJiaWuZhiLiang2;	//2#����������
	double	fMad;					//�ոɻ�ˮ��Mad
	double	fMt;					//ȫˮ��Mt
	double	fSad;					//����Sad
	double	fHad;					//�⺬��Had  
	int		iPingXing;				//1=��ƽ����
	long	lPingXingNum;			//ƽ�������

	//calculate


	LONG	nSpeed;		//ת��
	UINT	iAd;		//AD����
	double	fAdV;		//AD��ѹ
	double	fRt;		//̽ͷ����
	double	fty;		//ʵ���¶���Rt=R0(1+At+Bt^2)���
	double	ft;			//��ǰ��Ͱ(У��)�¶�

	double	ftj;		//��Ͱ�¶� 
	double	fT0;		//������Ͱ�¶�
	double  *ftt0;
	double	ft0;		//���ʱ��Ͱ�¶�
	double	ft140;		//1'40''��Ͱ�¶�
	double	ftn;		//�յ���Ͱ�¶�
	double  *fttn;
	double	fTn;		//ĩ����Ͱ�¶�
	double	fSumT;		//�����¶��ܺͦ�ti
	double  *fTi;
	int	nn;				//t0��tn�ķ�����n
	int t0nn;			//t0�ĸ���
	int tnnn;           //tn�ĸ���

	double fV0;					//���ڽ���Vo
	double fVn;					//ĩ�ڽ���Vn
	double fC;					//��ȴϵ��C
	long   lE;					//������
	double fK;					//����K
	double fA;					//����A
	long lGroupNum;				//���

	long lQbad;					//��Ͳ������
	long lQgrad;				//��λ������
	long lQgrd;					//�ɻ���λ������
	long lQnetvad;				//��λ������
	 
	double fCurve[MAX_CURVE];	//��������
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

//����������
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

	long  lAverageE;				//ƽ��������
	float fSError;					//��׼��
	float fRSError;					//��Ա�׼��
	long  lMaxDiff;					//����ֵ
	float fK;						//����K
	float fA;						//����A
	CString szMode;					//�㷨
	COleDateTime tDate;
	struct Report_E
	{ 
		bool	bActive;
		bool	bOk;
		long	lE;					//������
		double	fC;					//��ȴϵ��
		double	fYangPinZhiLiang;	//��Ʒ����
		double	fZhuQiWenSheng;		//��������
		double	ft0;				//����¶�
		double	fV0;				//���ڽ���
		double	fVn;				//ĩ�ڽ���  
		//CString strTestNum;		//������
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
			D[i].lE					= 0;					//������
			D[i].fC 				= 0;//��ȴϵ��
			D[i].fYangPinZhiLiang	= 0;//��Ʒ����
			D[i].fZhuQiWenSheng		= 0;//��������
			D[i].ft0				= 0;//����¶�
			D[i].fV0 				= 0;//���ڽ���
			D[i].fVn 				= 0;//ĩ�ڽ��� 
		}
		lAverageE	= 0;					//ƽ��������
		fSError		= 0;					//��׼��
		fRSError	= 0; 					//��Ա�׼��
		lMaxDiff	= 0; 					//����ֵ
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


//����������

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
	long lPingXingNum;				//ƽ�������
	long lMaxDiffQbad;				//����
	long lAverageQbad;				//ƽ����Ͳ������
	long lAverageQgrad;				//ƽ���ոɻ���λ������
	long lAverageQgrd;				//ƽ���ɻ���λ������ 
	long lAverageQnetvad;			//ƽ����λ������
	float fAverageMad;				//ƽ���ոɻ�ˮ��
	float fAverageMt;				//ƽ��ȫˮ��
	float fAverageSad;				//ƽ������
	float fAverageHad;				//ƽ���⺬��
	float fAverageM;				//ƽ������
	CString m_strMode;				//�㷨
	CString m_strOk;				//����
	COleDateTime tDate;
	struct  Report_Q
	{
		bool	bActive;
		CString strTestNum;			//������
		CString strYangPinNum;		//��Ʒ���
		float	fYangPinM;			//��Ʒ����
		long	lQbad;				//��Ͳ������
		long	lQgrad;				//�ոɻ���λ������
		long	lQgrd;				//�ɻ���λ������=Qgrad*100/(100-Mad)
		long	lQnetvad;			//��λ������
		float	fMad;				//�ոɻ�ˮ��
		float	fMt;				//ȫˮ��
		float	fSad;				//����
		float	fHad;				//�⺬��Had
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
 