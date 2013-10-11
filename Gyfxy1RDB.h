#pragma once

// CGyfxyRDB_G5200 command target
//#include "RDB.h"
#include "GyfxyRDB.H"
#include "xlistctrl.h"
class  CGyfxyImageSlave ;

#define MAX_WEIGHT_BUF   8
#define MAX_SAMPLE_CNT   24
#define FUZZY_ARRAY_SIZE 3

enum SIDE{MIDDLE=0,LEFT,RIGHT};
class CGyfxyRDB_G5200  : public CGyfxyRDB
{
public:
	CGyfxyRDB_G5200();
	
	virtual ~CGyfxyRDB_G5200();
	DECLARE_DYNCREATE(CGyfxyRDB_G5200)
	BOOL UpdateParamFromDB(void);
public:
	////设备属性
	class CAttrib
	{
	public:
		//CString			m_szName;				//设备名称
		//CString			m_szType;				//设备类型
		//BYTE			m_bID;					//ID号
		//SHORT			m_sComPort;				//串口号	
		//LONG			m_lPriod;				//扫描周期 

		int				m_sTimePut2weigh;//放样到称样时间(0_1S);
		int				m_sTimePut2HighTemp;//放样到高温时间(0_1S);
		int				m_sPulsePerRound1Spit;//样盘一圈脉冲数;
		BYTE			m_btDriverSpitCnt;//驱动器细分数;
		BYTE			m_btMaxSampleCnt;//样位个数
		FLOAT			m_fBalanceMaxErr;//称样判值(mg);

		FLOAT			m_fVolSampleMax;//挥发分样重上限;
		FLOAT			m_fVolSampleMin;//样重下限;

		FLOAT			m_fWaterAshSampleMax;//水分灰分样重上限;
		FLOAT			m_fWaterAshSampleMin;//样重下限;

		SHORT			m_sLowStoveTempSpeed;//低温炉升温速率
		SHORT			m_sHighStoveTempSpeed;//高温炉升温速率;
		SHORT			m_sQuick_WaterTestTime;//快速水分干燥时间(分);
		SHORT			m_sQuick_VolTestTime;//分干燥时间(分);
		SHORT			m_sQuick_AshTestTime;//快速灰分干燥时间(分);
		SHORT			m_sGB_WaterTestTime;//国标水分干燥时间(分);
		SHORT			m_sGB_VolTestTime;//国标挥发分干燥时间(分);
		SHORT			m_sGB_AshTestTime;//国标灰分干燥时间(分);
		SHORT			m_sQuick_WaterTestTemp;//快速水分试验温度(℃);
		SHORT			m_sQuick_VolTestTemp;//快速挥发分试验温度(℃);
		SHORT			m_sQuick_AshTestTemp;//快速灰分试验温度(℃);
		SHORT			m_sGB_WaterTestTemp;//国标水分试验温度(℃);
		SHORT			m_sGB_VolTestTemp;//国标挥发分试验温度(℃);
		SHORT			m_sGB_AshTestTemp;
		SHORT			m_sWaterCoolTime;		//水分炉坩埚冷却时间
		SHORT			m_sHighTempCoolTime;	//高温炉坩埚冷却时间
		SHORT			m_sBeepTime;
		VARIANT_BOOL	m_bFix;
		CString			m_szalgorithm;
		VARIANT_BOOL	m_bAutoPrint;
		CString			m_szTestSequence;     //测试顺序
		FLOAT			m_fCrucibleCoeff1;//坩埚系数1;
		FLOAT			m_fCrucibleCoeff2;
		FLOAT			m_fCrucibleCoeff3;
		VARIANT_BOOL	m_bdisperse;

		CString			m_szSystemType; //G5000 G5200 G5500  

		//
		enum WEIGHT_SPEED_TYPE{WEIGHT_NORMAL=0,WEIGHT_QUICK};
		
	}attrib;
	struct CStatus
	{ 
	public: 
		long	TempAdValue[3];		//炉子温度AD值
		long	InstTempValue;		//环境温度
		double	TempVoltage[3];		//炉子温度电压			
		double	Temperature[3];		//炉子温度
		BOOL	m_bWarmFlag[3];
		SHORT	du[3];
		//////////////////////////////////////  	
		/////////////////////////////////////////////////			
		int     cCurrPos[3];			//当前位置
		
		CString strCurrPos[3];			//样杆当前位置
		int		cMoving[3];				
		CString strMov[3];				//移动状态
		/////////////////////////////////////////////////
		int     cTestState;
		/////////////////////////////////////////////////
		int     cSampleDiskCurrPos;	
		BOOL	bSampleDiskMoving;
		CString strSampleDiskCurrPos;    //样盘当前位置
		///
		//BOOL m_bStartTest;
		//BOOL m_bReseted;   
		double	CurrWeight;	
	public:
		enum StoveBarPos{POS_TOP=0,POS_UNKNOWN,POS_BOT};
		CStatus()
		{
			
			memset(TempAdValue,0,sizeof(TempAdValue));		//炉子温度AD值
			memset(TempVoltage,0,sizeof(TempVoltage));		////炉子温度电压		
			memset(Temperature,0,sizeof(Temperature));		//炉子温度 
			memset(du,0,sizeof(du));						//炉子温度 
			InstTempValue = 0;								//环境温度     
			/////////////////////////////////////////////////
			cTestState				= 0;
			/////////////////////////////////////////////////
			cSampleDiskCurrPos 		= 0;
			CurrWeight				= 0;	 
		}
	}status;
	class  ALLSample:public IEDResult
	{
		private:
			static int QualityWater; 
			static int ResultWater	;
			enum METAGE_TYPE{S_WEIGHT_POT=0,S_WEIGHT_SAMPLE,S_TEST_WATER,S_WEIGHT_COVER,S_TEST_VOL,S_TEST_ASH};
			
			
		public:
			enum TestType{TEST_WATER=1,TEST_VOL=2,TEST_ASH=4};
			class S_Sample 
			{ 
				///////////////////////////////////////////////////////////////
			public://数据库
				CString m_strTestAutoNO;		//自动编号 试验编号
				CString cSampleName;			//样品编号
				CString m_strInstName;			//设备名称
				CString	m_TestType;				//测试方法
				FLOAT   m_SampleWeight;			//样品质量
				DATE	m_dtTestDateTime;		//试验日期
				int		SampleNO;				//样位
				FLOAT	m_fMad;					//水分百分比
				FLOAT	m_fVad;					//挥发分 百分比
				FLOAT	m_fAad;					//灰分百分比
				SHORT	m_sD; 
				bool	m_bFix;					//修正
				FLOAT			m_fCO2;			//二氧化碳
				FLOAT			m_fBrezeCO2;    //焦渣二氧化碳
				///////////////////////////////////////////////////////////////
				union UQUALITY{
					struct{
						double  lMPot;    //坩埚质量(unit=0.1mg)
						///////////////////////////////////////////////////////////////
						double  lM0;      //试验前样品质量(unit=0.1mg),含坩埚质量。
						double  lM1;      //水分试验后样品质量(unit=0.1mg),含坩埚质量。
						/////////////////////////////////////////////////////////////////
						double  lMPotSample2;//挥发份实验前加盖重量
						///////////////////////////////////////////////////////////////
						double  lM2;      //挥发分试验后样品质量(unit=0.1mg),含坩埚质量。
						double  lM3;      //灰分试验后样品质量(unit=0.1mg),含坩埚质量。
						///////////////////////////////////////////////////////////////
					}SQuality;
					double Quality[6];
				}UQuality; 
				FLOAT			m_fMineral;//	矿物质; 
				///////////////////////////////////////////////////////////////
			public:
				UCHAR cSta;     // 
				UCHAR cCSta;    //
				//当前试验状态：0-称坩埚，01-称样，02-水分试验，03,加坩埚盖,04-挥发分试验，05-灰分试验，
				enum STATE {S_WEIGHT_POT=0,S_WEIGHT_SAMPLE,S_TEST_WATER,S_WEIGHT_COVER,S_TEST_VOL,S_TEST_ASH};

				UCHAR cItem;    //试验内容：b0=1:水分，b1=1:挥发分，b2=1:灰分
				UCHAR reserve; 
				
				S_Sample()
				{ 
					SetDefault();						
				}
				void SetDefault()
				{
					reserve		= 0;
					cItem		= 0;
					cCSta		= 0;
					cSta		= 0;

					m_SampleWeight		= 0;			//样品质量
					m_dtTestDateTime	= COleDateTime::GetCurrentTime();		//试验日期 
					m_fMad = 0;		//水分百分比
					m_fVad = 0;			//挥发分 百分比
					m_fAad = 0;			//灰分百分比
					for(int j=0;j<6;j++)
						UQuality.Quality[j]=0;
					SampleNO = 0;
					m_sD = 0;
					m_bFix = false;
					m_fCO2 = 0;
					m_fBrezeCO2 = 0;
					m_fMineral = 0; 
				}
				/////////////////////////////////////////////////////////////// 
				void	CalResultMAV(); 
				void	LoadResultFromDB(CString strAutoNO); 
			}Sample[MAX_SAMPLE_CNT]; 
			//TODO: 
			/*
			校准
			数据库	
			报表
			设备属性
			*/
			public: 
			virtual void BuildAutoNO();
			///////////////////////////////////////////////////////////////
			virtual void SaveResult2DB();		//批量保存到数据库中
			virtual void UpdateResultToDB();    //更新数据库...
			virtual void SetInstName(CString strInstName) ;
			//////////////////////////////////////////////////////////////
			CString strTempDBSQL;
			_RecordsetPtr pRsTempDB;
			void LoadSampleFromTempTab(int cSamplePos);
			void UpdateTempTabField(int cSamplePos);
			void AppendResult2TempTab(int cSamplePos);	//添加数据到临时数据库表格里
			void UpdateResult2TempTab(int cSamplePos);  //修改数据到临时数据库表格里
			void LoadResultFromTempTab(int cSamplePos);	//从临时数据库中载入
			void BatchUpdateDB2TempTab(CString m_strInstName);		//批量更新到临时数据库中
			void CreateTempTab(void);			//创建临时数据库,以备断点续作
			void ClearTempTab(void);
			
			ALLSample();
			///////////////////////////////////////////////////////////////
			virtual void CalResult(CImageSlave *pClyImageSlave); //批量计算结果 
			virtual void LoadResultFromDB(CString strAutoNO);    
			virtual void DelResultFromDB(CString strAutoNO); 
			////////////////////////////////////////////////////////////////
		
			////////////////////////////////////////////////////////////////
			void SetDefaultSample(CXListCtrl &m_pListCtrl);			
			void CalcAllResult();
			void CalcResult(int cSamplePos);
			//从内存镜像中更新列表
			void OnUpdateQuality(int cSamplePos,CXListCtrl *m_pListCtrl);
			///////////////////////////////从界面得到样品信息数据集并保存到内存镜像里
			void GetSampleDataFromDlg(CXListCtrl &m_ListCtrl,CGyfxyImageSlave *pImg);
			void GetSampleDataFromDlg(int nItem, int nSubItem, CXListCtrl &m_ListCtrl,CGyfxyImageSlave *pImg) ;
			///////////////////////////////更新数据
			void RefreshSampleData(OUT CGyfxyImageSlave *pImageSlave,IN int SampleNO,IN int EnableMask,IN int DisableMask);
			void RefreshSampleData(OUT CGyfxyImageSlave *pImageSlave,OUT int SampleNO,OUT int WorkType );
			void RefreshSampleData(IN CGyfxyImageSlave *pImageSlave);
			void RefreshSampleData(IN CGyfxyImageSlave *pImageSlave,IN int SampleNO);
			
			//////////////////////////////////////////////////////////////////////////
			
		}SampleArray;  
		/////////////////////////////////////////////////////////////// 
		virtual void InitRDB(CGyfxyImageSlave* ImageSlave); 
		virtual void RefreshRDBTab(CGyfxyImageSlave *pImageSlave);
		
		////////////////////////////////////////////////////////////////

};


#define INFO_LENTH 32
class  CGyfxyImageSlave  :public CImageSlave
{

public:
	~CGyfxyImageSlave()
	{
	
	}
	CGyfxyImageSlave()
	{
		memset(this,0,sizeof(CGyfxyImageSlave));
	} 
public:   
	struct SDownParam{
		ULONG			m_sTimePut2weigh;//放样到称样时间(0_1S);
		ULONG			m_sTimePut2HighTemp;//放样到高温时间(0_1S);
		SHORT			m_sPulsePerRound1Spit;//样盘一圈脉冲数;
		BYTE			m_btDriverSpitCnt;//驱动器细分数;
		BYTE			m_btMaxSampleCnt;//样位个数
		BYTE			m_ByteReserve[8];

		SHORT			m_fBalanceMaxErr;//称样判值(0.1mg);
		SHORT			m_fVolSampleMax;
		SHORT			m_fVolSampleMin;
		SHORT			m_fWaterAshSampleMax;
		SHORT			m_fWaterAshSampleMin;
		//温升速度不需要,所以注释掉
		//SHORT			m_sLowStoveTempSpeed;//低温炉升温速率
		//SHORT			m_sHighStoveTempSpeed;//高温炉升温速率;
		/*
		SHORT			m_sQuick_WaterTestTime;//快速水分干燥时间(分);
		SHORT			m_sQuick_VolTestTime;//分干燥时间(分);
		SHORT			m_sQuick_AshTestTime;//快速灰分干燥时间(分);
		
		SHORT			m_sGB_WaterTestTime;//国标水分干燥时间(分);
		SHORT			m_sGB_VolTestTime;//国标挥发分干燥时间(分);
		SHORT			m_sGB_AshTestTime;//国标灰分干燥时间(分);
		*/

		SHORT			m_sWaterTestTime;//水分干燥时间(S);
		SHORT			m_sVolTestTime;//挥发分干燥时间(S);
		SHORT			m_sAshTestTime;//灰分干燥时间(S);


		SHORT			m_sWaterCoolTime;		//水分炉坩埚冷却时间
		SHORT			m_sHighTempCoolTime;	//高温炉坩埚冷却时间

		SHORT			m_sBeepTime;	

		LONG			m_sWaterTestTemp;//快速水分试验温度(℃);
		LONG			m_sVolTestTemp;//快速挥发分试验温度(℃);
		LONG			m_sAshTestTemp;//快速灰分试验温度(℃);
		

		/*
		LONG			m_sQuick_WaterTestTemp;//快速水分试验温度(℃);
		LONG			m_sQuick_VolTestTemp;//快速挥发分试验温度(℃);
		LONG			m_sQuick_AshTestTemp;//快速灰分试验温度(℃);
		
		LONG			m_sGB_WaterTestTemp;//国标水分试验温度(℃);
		LONG			m_sGB_VolTestTemp;//国标挥发分试验温度(℃);
		LONG			m_sGB_AshTestTemp;
		*/
		

		enum M_TSQ{};
		SHORT			m_szTestSequence;		//测试顺序
		enum M_ALG{M_GB=0,M_QK	};
		SHORT			m_szalgorithm;			//测试方法

		char            m_bIED_system_type;
		char            m_cReserve[3];
		long            m_reServe[8]; 
	}DownloadWorkParam;
	////////////////////////////////////
	enum CoilPos{MID_MOTOR_UP=0,MID_MOTOR_DN,LEFT_UP,LEFT_DN,};
	union _OutputCoil			  //输出寄存器
	{
		struct 	
		{	
			UINT BALANCE_MOTOR_UP	:1;		//称样上升			0
			UINT BALANCE_MOTOR_DN	:1;		//称样下降			1
			UINT LEFT_MOTOR_UP		:1;		//左炉上升			2
			UINT LEFT_MOTOR_DN		:1;		//左炉下降			3
		
			UINT RIGHT_MOTOR_UP		:1;		//右炉上升			4
			UINT RIGHT_MOTOR_DN		:1;		//右炉下降			5

			UINT Reserve1			:2;		//本来留着准备交流电机控制样盘转动的	6

			UINT LEFT_OXYGEN		:1;		//左炉氧气开关		8
			UINT RIGHT_OXYGEN		:1;		//右炉氧气开关		9

			UINT WATER_STOVE_FAN	:1;		//水分炉子交流风扇	10
			UINT LEFT_STOVE_FAN		:1;		//左炉风扇			11
			UINT RIGHT_STOVE_FAN	:1;		//右炉风扇			12
	
			UINT SPK				:1;		//蜂鸣器			13
			UINT Reserve2			:2;		// 
			
		};
		UCHAR   coil8;
		USHORT  Coil16;          
		UINT   cCoil32;                  
	}OutputCoil;
	union  _InputStatus					        								   
	{
		struct
		{  		
			UINT    ReserveKey1				:3;	
			UINT	MANUAL_KEY				:1;  //手动控制 
			UINT    ReserveKey2				:4;	 // 

			UINT	RIGHT_STOVE_TOP			:1;  //左高位
			UINT	RIGHT_STOVE_BOT			:1;  //低位
			UINT	LEFT_STOVE_TOP			:1;  //左高位
			UINT	LEFT_STOVE_BOT			:1;  //低位
			UINT    WATER_STOVE_PUT			:1;  //放样区	
			UINT    WATER_STOVE_WEIGH		:1;  //称样区 
			UINT    SAMPLE_POS_1			:1;  //样位1
			UINT    SAMPLE_POS_N			:1;  // 

			UINT    ReserveKey3				:16;	
		};
		UCHAR   cInputReg8;
		USHORT  cInputReg16;
		UINT    cInputReg32;
	}InputStatus;  
	struct  
	{
		long	ad[3];
		long    adInstTemp;   //主板环境温度
	}ADValue;

	struct  CFuzzy
	{
			enum {ON = 0,OFF=!ON};
			long Reserve;	
		public:
			short WarmFlag;			//升温标志
			short PreviewOut;       //上次输出的值   
			short du;               //输出         
			unsigned short Kp;		//比例系数
			unsigned short Ki;		//衰减系数1
			unsigned short KI;		//衰减系数2
			struct PoutParam{ 
				char 	iTimerID;		//关断加热定时器
				char	iReserve;
				UINT32  *pPWMPort;		//加热控制端口		   
			}pParam;
			long gDest;  			//目标温度的AD值
			long *AdValue ;
			long CTEMP;				//开始控制温度 
			long Error[2];			//偏差
			long dErr ;				//偏差变化率	  
			long SumErrLimit; 		//加热初值	
	} fuzzy[FUZZY_ARRAY_SIZE];
	struct  
	{
		char	DeviceName[24]; 
		int		CurrWeight;		  
	}Balance;
	struct StepMotorState 
	{										
		bool bRun;							//运行
		bool bDir;							//方向
		int DestPulseCnt;					//到达目标位置脉冲数
		int DestSamplePos;					//目标工位

		int CurPulseCnt;					//当前位置脉冲书
		int CurSamplePos;					//当前工位 

		int PulsePerRound;					//每移动一圈需要的脉冲数
		int Reserve;						//复位输入端口
	}MtrState; 
	///////////////////////////////////////////////////////////
	enum REST_STATE{REST_INIT=0,REST_START,REST_SAMPLEDISH,
		REST_END}; //复位状态
	short ResetState; 
	short ErrorFlag;	//错误标志

		
	//enum TEST_STATE {T_INIT=0,T_WARM_WATER_STOVE,T_TEST_WATER,T_WEIGHT_WATER,T_WATER_END,
	//	T_WARM_HIGH,T_TEST_VOL	};	
	
	enum TEST_STATE {T_INIT=0,
		T_WARM_WATER,T_TEST_WATER,T_WATERCOOL,T_WEIGHT_WATER,T_WATER_END,
		T_WARM_HIGH1,T_TEST_VOL,T_BURN_VOL,T_WEIGHT_VOL,T_VOL_END,
		T_WARM_HIGH2,T_TEST_ASH,T_BURN_ASH,T_WEIGHT_ASH,T_ASH_END,	
		T_END};///测试状态
	long TestState;
	enum METAGE_STATE {M_INIT=0,M_POS,M_TARE,M_METAGE,M_SAVE,M_NEXTPOS,M_END};//称量状态		
	long MetageState;
	
	typedef struct ypd      //样品数据定义
	{
		///////////////////////////////////////////////////////////////
		UCHAR cSta;     //当前试验状态,00-无坩埚,01-等待(有坩埚),02-称坩埚,03-称样,04-水分,05-挥发分,06-灰分,07-试验完成.08-烧坩埚
		UCHAR cCSta;    //试验完成状态,b0-称坩埚,b1-称样,b2-水分,b3-挥发分,b4-灰分,b5-烧坩埚,b7-试验完成
		UCHAR cItem;    //试验内容：b0=1:水分，b1=1:挥发分，b2=1:灰分
		UCHAR reserve;
		union UQUALITY{
			struct{
			long  lMPot;    //坩埚质量(unit=0.1mg)
			///////////////////////////////////////////////////////////////
			long  lM0;      //试验前样品质量(unit=0.1mg),含坩埚质量。
			long  lM1;      //水分试验后样品质量(unit=0.1mg),含坩埚质量。
			/////////////////////////////////////////////////////////////////
			long  lMPotSample2;//挥发份实验前加盖重量
			///////////////////////////////////////////////////////////////
			long  lM2;      //挥发分试验后样品质量(unit=0.1mg),含坩埚质量。
			long  lM3;      //灰分试验后样品质量(unit=0.1mg),含坩埚质量。
			///////////////////////////////////////////////////////////////
			}SQuality;
			long Quality[6];
		}UQuality;
	}YP_DATA;
	YP_DATA Sample[MAX_SAMPLE_CNT] ;
	//////////////////////////////////////////////////////////
	struct
	{
		UCHAR   CmdType;
		UCHAR   CmdParam1;
		UCHAR   CmdParam2;   
		UCHAR   CmdParam3;   
		LONG	CmdReserve;   
	}Cmd;
	char InstrumentName[32];      //32BIT[]
	char ErrorMessage[INFO_LENTH];  //错误信息
	/////////////////////////////////////////////////////////////////   
	UCHAR   cID;                //IED站号
	UCHAR   ReserveID;

};

