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
//const int		GAIN_TEMP =	(252*500UL); //第二版测硫仪的放大增益 
const int		GAIN_TEMP =	(240*500UL); //第一版测硫仪的温度放大增益 
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
 
	//设备状态
	enum MOTORPOS {POS_UNKNOWN = 0,POS_PUT_SAMPLE,POS_MIDDLE_1,POS_MIDDLE_2,POS_HIGH_TEMP }; ;
	enum{RESET_INIT=1,RESET_POS,RESET_SAMPLE_POS,RESET_END};
	class CStatus
	{
		public:
			long   TempAdValue;		//炉子温度AD值
			double TempVoltage;		//炉子温度电压			
			double Temperature;		//炉子温度
			//////////////////////////////////////
			long   SensorAdValue;	//小电极AD
			double SensorVoltage;   //小电极电压			
			//////////////////////////////////////
			char   DJSwitch;		//电解开关
			long   DJFrequency;		//电解电流频率
			double DJCurrent;		//电解电流	
			LONG   DJCoulomb;		//电流库仑
			double SQuality;		//硫含量(mg)

			long   DJPoolAdValue;	//电解池电压	
			double DJPoolVoltage;	//电解池电压			
			/////////////////////////////////////////////////			
			int     cCurrPos;	
			BOOL	bMoving;
			CString strCurrPos;				//样杆当前位置
			/////////////////////////////////////////////////
			int     cTestState;
			/////////////////////////////////////////////////
			int     cSampleDiskCurrPos;	
			BOOL	bSampleDiskMoving;
			CString strSampleDiskCurrPos;    //样盘当前位置
			///
			BOOL m_bStartTest;
			BOOL m_bReseted;
	}status;
	////设备属性
	class CAttrib
	{
		public:
		//CString			m_szName;				//设备名称
		//CString			m_szType;				//设备类型
		//BYTE			m_bID;					//ID号
		//SHORT			m_sComPort;				//串口号	
		//LONG			m_lPriod;				//扫描周期 

		SHORT			m_sPreDecomposeTim1;	//m_s预分解1时间(S); 
		SHORT			m_sPreDecomposePos1;	//预分解1位置(S); 
		SHORT			m_sPreDecomposeTim2; 
		SHORT			m_sPreDecomposePos2;
		SHORT			m_sDecomposePos;		//分解位置
		SHORT			m_sEndDecomposDelay;	//分解延时
		//////////////////////////////////////////////////////////////
		SHORT           m_sPos1TimeOutTime;     //样位1超时时间
		SHORT           m_sPosNTimeOutTime;		//样位N超时时间 
		BYTE			m_MaxSampleCnt;			//最大样位数目
		BOOL			m_bAutoCly;				//自动测硫仪
		USHORT			sMaxTestTime;			//最大测试时间
		//////////////////////////////////////////////////////////////
		SHORT			m_sWarmUpSpeed;			//升温速度
		SHORT			m_sDestTemp;			//目标温度
		DOUBLE			m_dCoff;				//温度系数
		CString			m_szSampleType;		    //样品类型 油/煤
		FLOAT			m_fStartDJVol;			//开始电解电压
		FLOAT			m_fEndDJVol;			//结束电解电压
		double			m_sDJCoff;				//电解系数	
		SHORT			m_sBeepTime;			//蜂鸣时间
		BOOL			m_bFix;					//修正
		BOOL			m_bAutoPrint;			//自动打印
		DOUBLE			m_slope;				//斜率
		DOUBLE			m_intercept;			//截距
	}attrib;
	class Result:public IEDResult
	{
		
	public:  
		BOOL			m_bReadResult;
		BYTE			m_pos;					//样位信息
		int				m_ResultNO;				//在列表中第几行
		CString			m_szSampleNO;			//样品编号	
		COleDateTime	m_PickSampleDate;		//取样日期
		DOUBLE			m_dStad;				//分析基硫
		DOUBLE			m_dStd;					//干基硫 //何学化 
		DOUBLE			m_dSampleWeight;		//样品重量
		DOUBLE			m_dWater;				//分析水
		FLOAT			m_fCoulomb_ByDA;		//电量 DA计算
		FLOAT			m_fCoulomb_ByVF;		//电量 DA计算
		COleDateTime	m_dtAnalysisTime;		//实验时间
		//CString			m_szName;				//设备名称
		BOOL			m_bFix;					//修正
		COleDateTime	m_dtTestDate;			//测试时刻
		CString			m_szTestAutoNO;			//自动编号
		LONG			m_lPingXingNO;			//平行样编号

		double			m_sDJCoff;				//电解系数	 //修正电容容量不对引起的频率漂移
		DOUBLE			m_slope;				//斜率
		DOUBLE			m_intercept;			//截距

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

	class CCoff     //作废,未做分段校准
	{
		struct CoffTab
		{ 
		SHORT			m_sCodeNO;				//序号
		CString			m_szName;				//设备名称
		
		FLOAT			m_fSd;					//硫标值
		FLOAT			m_fCoff;				//系数
		}coffTab[MAX_COFF_COUNT];
		int coffCount;							//系数数据个数
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
	unsigned long gDest;  			    //目标温度的AD值
	unsigned long CTEMP;				//开始控制温度	
	long Error[2];						//偏差
	long dErr ;							//偏差变化率	   //   
	
	long SumErrLimit 	; 	//加热初值			 	
	long PreviewOut;          
	long du;                           
	unsigned long  iDestTemp;	//目标温度值      
	
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
        USHORT  cRightToMiddle1;    //放样位到预分解1
        USHORT  cRightToMiddle2;    //放样位到预分解2
        USHORT  cRightToLeft;       //放样位到高温位   //样车运行时间                                   
        USHORT  cTimeDecompose1;    //分解时间1    

        USHORT  cTimeDecompose2;    //分解时间2
        USHORT  cTimeDelay;         //结束延时
		USHORT  sMaxTestTime;		//最大实验时间
		UCHAR   bAutoCly;			//自动测硫仪
		UCHAR	bReserve;			//保留

		ULONG  sReserve[10];			//保留
        //
        ULONG    lDestTempADValue;  //目标温度AD值 	
		ULONG    lDStartDJADValue;
		ULONG    lDEndDJADValue;    
		//////////////////////////////////////////
		USHORT  cTimeoutSamplePosN;    //样盘移动一位的超时时间
		USHORT  cTimeoutSamplePos1;    //样盘移动到第一位的超时时间	
		
    }DownloadWorkParam;	
	//16 byte
	/////////////////////////////////////
	union _OutputCoil			  //输出寄存器
	{
		struct 	
		{
			UINT  DC_SAMPLE_DIR	:1; //样车方向	DC
			UINT  DC_SAMPLE_RUN	:1;	//样车运行
			UINT  DJ_DJ			:1; //电解控制接通
			UINT  DC_SPEAKER	:1; //蜂鸣器
			UINT  DC_SAMPLE_POS_DIR	:1; //样位旋转方向
			UINT  DC_SAMPLE_POS_RUN	:1; //样位旋转运行
			UINT  DC_JB         	:1; //搅拌	 	
			UINT  DC_PUMB			:1; //气泵
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
		UCHAR		DJSwitch;			   //电解开关
		USHORT		DJVALUE;			   //电解输出值DA	
		ULONG		lDJSum;					//DJ开环值求和
		UCHAR		lDJReserve[1];		   //电解保留
        ULONG		cPreFreq;          //过去的电流积分值         //32BIT
        ULONG		cCurrFreq;         //当前的电流积分值         //32BIT
    }Freq;//8 byte
    ///////////////////////////////////////////////////////////////
    struct CMotorCtrl
    {
		UCHAR   cPos;                               //样杆位置
		SCHAR   cSampleMoving;                      //样品电机正在移动
		UCHAR   cSampleMotorDir;                    //样车运行方向
		SCHAR   cMotorDlyTimer;                       //延时启动定时器
		UINT   MotorSampleRunTime;                 //样车运行时间
		UINT   MotorSampleTimeOutCnt;              //到位超时时间
    }MotorCtrl;//8 byte
    //////////////////////////////////////////////////////////////
    AI_CONTROL Ai_CTRL;   //32Byte
	struct StepMotor
	{
		UCHAR   cDestPos;							//样盘目标位置
		UCHAR   cSamplePos;                         //样盘当前位置		 
		SCHAR   cSamplePosMoving;                   //样盘电机正在移动
		UCHAR   cSamplePosMotorDir;                 //样盘运行方向

		SCHAR   cMotorDlyTimer;                     //延时启动定时器
		UCHAR 	cSampleDiskMoveCnt;					//需要移动的孔位个数
		USHORT  iTimeout;             				//样盘运行时间	        
	}PosCtrl;	 
	/////////////////////////////////////////////////// 
	UCHAR	cID;                					//IED站号  
	UCHAR	cTestState;
	UCHAR   cReserve;
	//UCHAR   bAutoCly;								//自动测硫仪
	UCHAR   cResetState;
	UINT	cStateTime;
	UINT	cTestTimer;							   //总共的测量时间		
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
