// ClyRDB.h: interface for the CClyRDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLYRDB_H__0B1FCAAF_2544_4595_98C9_24A3FD99A1C1__INCLUDED_)
#define AFX_CLYRDB_H__0B1FCAAF_2544_4595_98C9_24A3FD99A1C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_COFF_COUNT 30

#include "RDB.h"
#include "tea\\Encpypt.h"
class  CClyImageSlave ;
/////////////////////////////////////// 
#define		OPEN_LOOP 
#undef		OPEN_LOOP
#define		_USE_DAC8560 
#undef		_USE_DAC8560
const int DJ_PERIOD		= 10L;  
const int DJ_REF_RES	= 5L;
#ifdef	OPEN_LOOP

	#ifdef _USE_DAC8560
		const INT64 DACVREF		= 2500L;
	#else
		const INT64 DACVREF		= 5000L;
	#endif
	

#endif

const INT64		VREF  = 5000L;
//const int		GAIN_TEMP =	(252*500UL); //�ڶ�������ǵķŴ����� 
const int		GAIN_TEMP =	(240*500UL); //��һ������ǵ��¶ȷŴ����� 
const int		GAIN_POL  = (9UL*32UL);
const double	GAIN_DJ	  = 0.1f;
class CClyRDB : public CRDB
{
public:
	CClyRDB();	
	virtual ~CClyRDB();
	DECLARE_DYNCREATE(CClyRDB)
public:

	virtual void InitRDB(CClyImageSlave *ImageSlave);	
	virtual void RefreshRDBTab(CClyImageSlave *pImageSlave);
	virtual BOOL UpdateParam2DB();
	virtual BOOL UpdateParamFromDB();
public:
	void	SaveCurve();
	double	GetVoltage(double Temperature); 
	double	TempADtoVol(long ADVAlue);
	double  GetTemperature(double voltage);

	long	TemptoAD(double Temp);	 
	long	TempVoltoAD(double voltage);
	long	PolVoltoAD(double voltage);
	double  PolADToVol(long volADValue);
	double  DJADToVol(long volADValue); 
 
	//�豸״̬
	enum MOTORPOS {POS_UNKNOWN = 0,POS_PUT_SAMPLE,POS_MIDDLE_1,POS_MIDDLE_2,POS_HIGH_TEMP }; ;
	enum{RESET_INIT=1,RESET_POS,RESET_SAMPLE_POS,RESET_END};
	class CStatus
	{
		public:
			long   TempAdValue;		//¯���¶�ADֵ
			double TempVoltage;		//¯���¶ȵ�ѹ			
			double Temperature;		//¯���¶�
			//////////////////////////////////////
			long   SensorAdValue;	//С�缫AD
			double SensorVoltage;   //С�缫��ѹ			
			//////////////////////////////////////
			char   DJSwitch;		//��⿪��
			long   DJFrequency;		//������Ƶ��
			double DJCurrent;		//������	
			LONG   DJCoulomb;		//��������
			double SQuality;		//����(mg)

			long   DJPoolAdValue;	//���ص�ѹ	
			double DJPoolVoltage;	//���ص�ѹ			
			/////////////////////////////////////////////////			
			int     cCurrPos;	
			BOOL	bMoving;
			CString strCurrPos;				//���˵�ǰλ��
			/////////////////////////////////////////////////
			int     cTestState;
			/////////////////////////////////////////////////
			int     cSampleDiskCurrPos;	
			BOOL	bSampleDiskMoving;
			CString strSampleDiskCurrPos;    //���̵�ǰλ��
			///
			BOOL m_bStartTest;
			BOOL m_bReseted;
	}status;
	////�豸����
	class CAttrib
	{
		public:
		//CString			m_szName;				//�豸����
		//CString			m_szType;				//�豸����
		//BYTE			m_bID;					//ID��
		//SHORT			m_sComPort;				//���ں�	
		//LONG			m_lPriod;				//ɨ������ 

		SHORT			m_sPreDecomposeTim1;	//m_sԤ�ֽ�1ʱ��(S); 
		SHORT			m_sPreDecomposePos1;	//Ԥ�ֽ�1λ��(S); 
		SHORT			m_sPreDecomposeTim2; 
		SHORT			m_sPreDecomposePos2;
		SHORT			m_sDecomposePos;		//�ֽ�λ��
		SHORT			m_sEndDecomposDelay;	//�ֽ���ʱ
		//////////////////////////////////////////////////////////////
		SHORT           m_sPos1TimeOutTime;     //��λ1��ʱʱ��
		SHORT           m_sPosNTimeOutTime;		//��λN��ʱʱ�� 
		BYTE			m_MaxSampleCnt;			//�����λ��Ŀ
		BOOL			m_bAutoCly;				//�Զ�������
		USHORT			sMaxTestTime;			//������ʱ��
		//////////////////////////////////////////////////////////////
		SHORT			m_sWarmUpSpeed;			//�����ٶ�
		SHORT			m_sDestTemp;			//Ŀ���¶�
		DOUBLE			m_dCoff;				//�¶�ϵ��
		CString			m_szSampleType;		    //��Ʒ���� ��/ú
		FLOAT			m_fStartDJVol;			//��ʼ����ѹ
		FLOAT			m_fEndDJVol;			//��������ѹ
		double			m_sDJCoff;				//���ϵ��	
		SHORT			m_sBeepTime;			//����ʱ��
		BOOL			m_bFix;					//����
		BOOL			m_bAutoPrint;			//�Զ���ӡ
		DOUBLE			m_slope;				//б��
		DOUBLE			m_intercept;			//�ؾ�
	}attrib;
	class Result:public IEDResult
	{
		
	public:  
		BOOL			m_bReadResult;
		BYTE			m_pos;					//��λ��Ϣ
		int				m_ResultNO;				//���б��еڼ���
		CString			m_szSampleNO;			//��Ʒ���	
		COleDateTime	m_PickSampleDate;		//ȡ������
		DOUBLE			m_dStad;				//��������
		DOUBLE			m_dStd;					//�ɻ��� //��ѧ�� 
		DOUBLE			m_dSampleWeight;		//��Ʒ����
		DOUBLE			m_dWater;				//����ˮ
		FLOAT			m_fCoulomb_ByDA;		//���� DA����
		FLOAT			m_fCoulomb_ByVF;		//���� DA����
		COleDateTime	m_dtAnalysisTime;		//ʵ��ʱ��
		//CString			m_szName;				//�豸����
		BOOL			m_bFix;					//����
		COleDateTime	m_dtTestDate;			//����ʱ��
		CString			m_szTestAutoNO;			//�Զ����
		LONG			m_lPingXingNO;			//ƽ�������

		double			m_sDJCoff;				//���ϵ��	 //���������������������Ƶ��Ư��
		DOUBLE			m_slope;				//б��
		DOUBLE			m_intercept;			//�ؾ�

		virtual void			BuildAutoNO();
		virtual void			SaveResult2DB();
		virtual void			UpdateResultToDB();
		virtual void			CalResult(CImageSlave *pClyImageSlave); 
		void					CalResultS(); 
		virtual void			LoadResultFromDB(CString strAutoNO);
		virtual void			DelResultFromDB(CString strAutoNO);
	public:
		Result(){m_bReadResult = FALSE;m_ResultNO=0;m_lPingXingNO=0;m_bFix=VARIANT_FALSE;m_sDJCoff=1.0f;m_dtAnalysisTime.SetTime(0,0,0);}
	}result;

	class CCoff     //����,δ���ֶ�У׼
	{
		struct CoffTab
		{ 
		SHORT			m_sCodeNO;				//���
		CString			m_szName;				//�豸����
		
		FLOAT			m_fSd;					//���ֵ
		FLOAT			m_fCoff;				//ϵ��
		}coffTab[MAX_COFF_COUNT];
		int coffCount;							//ϵ�����ݸ���
	public:
		BOOL UpdateCoff2DB();
		BOOL LoadCoffFromDB();
	}coff;
	////test param from dlg
private:
	static double		TempTable[]; 
	
};

typedef struct _AI_CONTROL
{
	unsigned long gDest;  			    //Ŀ���¶ȵ�ADֵ
	unsigned long CTEMP;				//��ʼ�����¶�	
	long Error[2];						//ƫ��
	long dErr ;							//ƫ��仯��	   //   
	
	long SumErrLimit 	; 	//���ȳ�ֵ			 	
	long PreviewOut;          
	long du;                           
	unsigned long  iDestTemp;	//Ŀ���¶�ֵ      
	
	long WarmFlag;
    long AI_Reserve; 
}AI_CONTROL;

typedef signed char SCHAR; 
class  CClyImageSlave  :public CImageSlave
{

public:
	~CClyImageSlave()
	{

	}
	CClyImageSlave()
	{ 
		memset(&DownloadWorkParam,0,(sizeof(DownloadWorkParam)
									+sizeof(OutputCoil)
									+sizeof(InputStatus)
									+sizeof(AdValue)
									+sizeof(Freq)
									+sizeof(MotorCtrl)
									+sizeof(Ai_CTRL))/sizeof(char));
		cTestTimer = 0; 
	} 
public:   
	////////////////////////////////////
	struct DownloadParam
	{                  
        USHORT  cRightToMiddle1;    //����λ��Ԥ�ֽ�1
        USHORT  cRightToMiddle2;    //����λ��Ԥ�ֽ�2
        USHORT  cRightToLeft;       //����λ������λ   //��������ʱ��                                   
        USHORT  cTimeDecompose1;    //�ֽ�ʱ��1    

        USHORT  cTimeDecompose2;    //�ֽ�ʱ��2
        USHORT  cTimeDelay;         //������ʱ
		USHORT  sMaxTestTime;		//���ʵ��ʱ��
		UCHAR   bAutoCly;			//�Զ�������
		UCHAR	bReserve;			//����

		ULONG  sReserve[10];			//����
        //
        ULONG    lDestTempADValue;  //Ŀ���¶�ADֵ 	
		ULONG    lDStartDJADValue;
		ULONG    lDEndDJADValue;    
		//////////////////////////////////////////
		USHORT  cTimeoutSamplePosN;    //�����ƶ�һλ�ĳ�ʱʱ��
		USHORT  cTimeoutSamplePos1;    //�����ƶ�����һλ�ĳ�ʱʱ��	
		
    }DownloadWorkParam;	
	//16 byte
	/////////////////////////////////////
	union _OutputCoil			  //����Ĵ���
	{
		struct 	
		{
			UINT  DC_SAMPLE_DIR	:1; //��������	DC
			UINT  DC_SAMPLE_RUN	:1;	//��������
			UINT  DJ_DJ			:1; //�����ƽ�ͨ
			UINT  DC_SPEAKER	:1; //������
			UINT  DC_SAMPLE_POS_DIR	:1; //��λ��ת����
			UINT  DC_SAMPLE_POS_RUN	:1; //��λ��ת����
			UINT  DC_JB         	:1; //����	 	
			UINT  DC_PUMB			:1; //����
			UINT  cReserve		    :24;
		};
		UCHAR  Coil8;          
        UINT   cCoil32;                  
	}OutputCoil;
    union _InputStatus
    { 
		struct
		{
			UINT    DC_MANUAL_KEY				:1;
			UINT    DC_RESERVE					:1;
			UINT    DC_POS_PUT_SAMPLE			:1;  
			UINT    DC_POS_HIGH_TEMP			:1;

			UINT    DC_SAMPLE_NON				:1;  
			UINT    DC_SAMPLE_NO1				:1;
			UINT    DC_InputRes1            :10;
		};
        UCHAR cInputReg8;
        unsigned short  cInputReg16;
        UINT  cInputReg32;
    }InputStatus;  
	//8 byte
	///////////////////////////////////////////////////

    struct AD
    {
		long lPreChlPol;
		long lChlPol;                                        //32BIT
		long lPreTemp;
        long lTemp;                                          //
		long lPreChlVDJ;
		long lVDJ;
		long reserve[2];
    }AdValue; 
	//8 byte
    ///////////////////////////////////////////////////////			  
    struct FREQ
    {
		UCHAR		DJSwitch;			   //��⿪��
		USHORT		DJVALUE;			   //������ֵDA	
		ULONG		lDJSum;					//DJ����ֵ���
		UCHAR		lDJReserve[1];		   //��Ᵽ��
        ULONG		cPreFreq;          //��ȥ�ĵ�������ֵ         //32BIT
        ULONG		cCurrFreq;         //��ǰ�ĵ�������ֵ         //32BIT
    }Freq;//8 byte
    ///////////////////////////////////////////////////////////////
    struct CMotorCtrl
    {
		UCHAR   cPos;                               //����λ��
		SCHAR   cSampleMoving;                      //��Ʒ��������ƶ�
		UCHAR   cSampleMotorDir;                    //�������з���
		SCHAR   cMotorDlyTimer;                       //��ʱ������ʱ��
		UINT   MotorSampleRunTime;                 //��������ʱ��
		UINT   MotorSampleTimeOutCnt;              //��λ��ʱʱ��
    }MotorCtrl;//8 byte
    //////////////////////////////////////////////////////////////
    AI_CONTROL Ai_CTRL;   //32Byte
	struct StepMotor
	{
		UCHAR   cDestPos;							//����Ŀ��λ��
		UCHAR   cSamplePos;                         //���̵�ǰλ��		 
		SCHAR   cSamplePosMoving;                   //���̵�������ƶ�
		UCHAR   cSamplePosMotorDir;                 //�������з���

		SCHAR   cMotorDlyTimer;                     //��ʱ������ʱ��
		UCHAR 	cSampleDiskMoveCnt;					//��Ҫ�ƶ��Ŀ�λ����
		USHORT  iTimeout;             				//��������ʱ��	        
	}PosCtrl;	 
	/////////////////////////////////////////////////// 
	UCHAR	cID;                					//IEDվ��  
	UCHAR	cTestState;
	UCHAR   cReserve;
	//UCHAR   bAutoCly;								//�Զ�������
	UCHAR   cResetState;
	UINT	cStateTime;
	UINT	cTestTimer;							   //�ܹ��Ĳ���ʱ��		
    char InstrumentName[18];    
	//24 byte
	/////////////////////////////
    struct sCmd
    {   	           		
        UCHAR   CmdType;
        UCHAR   CmdParam1;
        UCHAR   CmdParam2;   
        UCHAR   CmdParam3;     
		UCHAR   CmdReserve[4];   
    }Cmd;	
	Encpypt::SecruitInfo Info;
};
#endif // !defined(AFX_CLYRDB_H__0B1FCAAF_2544_4595_98C9_24A3FD99A1C1__INCLUDED_)
