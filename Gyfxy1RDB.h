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
	////�豸����
	class CAttrib
	{
	public:
		//CString			m_szName;				//�豸����
		//CString			m_szType;				//�豸����
		//BYTE			m_bID;					//ID��
		//SHORT			m_sComPort;				//���ں�	
		//LONG			m_lPriod;				//ɨ������ 

		int				m_sTimePut2weigh;//����������ʱ��(0_1S);
		int				m_sTimePut2HighTemp;//����������ʱ��(0_1S);
		int				m_sPulsePerRound1Spit;//����һȦ������;
		BYTE			m_btDriverSpitCnt;//������ϸ����;
		BYTE			m_btMaxSampleCnt;//��λ����
		FLOAT			m_fBalanceMaxErr;//������ֵ(mg);

		FLOAT			m_fVolSampleMax;//�ӷ�����������;
		FLOAT			m_fVolSampleMin;//��������;

		FLOAT			m_fWaterAshSampleMax;//ˮ�ֻҷ���������;
		FLOAT			m_fWaterAshSampleMin;//��������;

		SHORT			m_sLowStoveTempSpeed;//����¯��������
		SHORT			m_sHighStoveTempSpeed;//����¯��������;
		SHORT			m_sQuick_WaterTestTime;//����ˮ�ָ���ʱ��(��);
		SHORT			m_sQuick_VolTestTime;//�ָ���ʱ��(��);
		SHORT			m_sQuick_AshTestTime;//���ٻҷָ���ʱ��(��);
		SHORT			m_sGB_WaterTestTime;//����ˮ�ָ���ʱ��(��);
		SHORT			m_sGB_VolTestTime;//����ӷ��ָ���ʱ��(��);
		SHORT			m_sGB_AshTestTime;//����ҷָ���ʱ��(��);
		SHORT			m_sQuick_WaterTestTemp;//����ˮ�������¶�(��);
		SHORT			m_sQuick_VolTestTemp;//���ٻӷ��������¶�(��);
		SHORT			m_sQuick_AshTestTemp;//���ٻҷ������¶�(��);
		SHORT			m_sGB_WaterTestTemp;//����ˮ�������¶�(��);
		SHORT			m_sGB_VolTestTemp;//����ӷ��������¶�(��);
		SHORT			m_sGB_AshTestTemp;
		SHORT			m_sWaterCoolTime;		//ˮ��¯������ȴʱ��
		SHORT			m_sHighTempCoolTime;	//����¯������ȴʱ��
		SHORT			m_sBeepTime;
		VARIANT_BOOL	m_bFix;
		CString			m_szalgorithm;
		VARIANT_BOOL	m_bAutoPrint;
		CString			m_szTestSequence;     //����˳��
		FLOAT			m_fCrucibleCoeff1;//����ϵ��1;
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
		long	TempAdValue[3];		//¯���¶�ADֵ
		long	InstTempValue;		//�����¶�
		double	TempVoltage[3];		//¯���¶ȵ�ѹ			
		double	Temperature[3];		//¯���¶�
		BOOL	m_bWarmFlag[3];
		SHORT	du[3];
		//////////////////////////////////////  	
		/////////////////////////////////////////////////			
		int     cCurrPos[3];			//��ǰλ��
		
		CString strCurrPos[3];			//���˵�ǰλ��
		int		cMoving[3];				
		CString strMov[3];				//�ƶ�״̬
		/////////////////////////////////////////////////
		int     cTestState;
		/////////////////////////////////////////////////
		int     cSampleDiskCurrPos;	
		BOOL	bSampleDiskMoving;
		CString strSampleDiskCurrPos;    //���̵�ǰλ��
		///
		//BOOL m_bStartTest;
		//BOOL m_bReseted;   
		double	CurrWeight;	
	public:
		enum StoveBarPos{POS_TOP=0,POS_UNKNOWN,POS_BOT};
		CStatus()
		{
			
			memset(TempAdValue,0,sizeof(TempAdValue));		//¯���¶�ADֵ
			memset(TempVoltage,0,sizeof(TempVoltage));		////¯���¶ȵ�ѹ		
			memset(Temperature,0,sizeof(Temperature));		//¯���¶� 
			memset(du,0,sizeof(du));						//¯���¶� 
			InstTempValue = 0;								//�����¶�     
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
			public://���ݿ�
				CString m_strTestAutoNO;		//�Զ���� ������
				CString cSampleName;			//��Ʒ���
				CString m_strInstName;			//�豸����
				CString	m_TestType;				//���Է���
				FLOAT   m_SampleWeight;			//��Ʒ����
				DATE	m_dtTestDateTime;		//��������
				int		SampleNO;				//��λ
				FLOAT	m_fMad;					//ˮ�ְٷֱ�
				FLOAT	m_fVad;					//�ӷ��� �ٷֱ�
				FLOAT	m_fAad;					//�ҷְٷֱ�
				SHORT	m_sD; 
				bool	m_bFix;					//����
				FLOAT			m_fCO2;			//������̼
				FLOAT			m_fBrezeCO2;    //����������̼
				///////////////////////////////////////////////////////////////
				union UQUALITY{
					struct{
						double  lMPot;    //��������(unit=0.1mg)
						///////////////////////////////////////////////////////////////
						double  lM0;      //����ǰ��Ʒ����(unit=0.1mg),������������
						double  lM1;      //ˮ���������Ʒ����(unit=0.1mg),������������
						/////////////////////////////////////////////////////////////////
						double  lMPotSample2;//�ӷ���ʵ��ǰ�Ӹ�����
						///////////////////////////////////////////////////////////////
						double  lM2;      //�ӷ����������Ʒ����(unit=0.1mg),������������
						double  lM3;      //�ҷ��������Ʒ����(unit=0.1mg),������������
						///////////////////////////////////////////////////////////////
					}SQuality;
					double Quality[6];
				}UQuality; 
				FLOAT			m_fMineral;//	������; 
				///////////////////////////////////////////////////////////////
			public:
				UCHAR cSta;     // 
				UCHAR cCSta;    //
				//��ǰ����״̬��0-��������01-������02-ˮ�����飬03,��������,04-�ӷ������飬05-�ҷ����飬
				enum STATE {S_WEIGHT_POT=0,S_WEIGHT_SAMPLE,S_TEST_WATER,S_WEIGHT_COVER,S_TEST_VOL,S_TEST_ASH};

				UCHAR cItem;    //�������ݣ�b0=1:ˮ�֣�b1=1:�ӷ��֣�b2=1:�ҷ�
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

					m_SampleWeight		= 0;			//��Ʒ����
					m_dtTestDateTime	= COleDateTime::GetCurrentTime();		//�������� 
					m_fMad = 0;		//ˮ�ְٷֱ�
					m_fVad = 0;			//�ӷ��� �ٷֱ�
					m_fAad = 0;			//�ҷְٷֱ�
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
			У׼
			���ݿ�	
			����
			�豸����
			*/
			public: 
			virtual void BuildAutoNO();
			///////////////////////////////////////////////////////////////
			virtual void SaveResult2DB();		//�������浽���ݿ���
			virtual void UpdateResultToDB();    //�������ݿ�...
			virtual void SetInstName(CString strInstName) ;
			//////////////////////////////////////////////////////////////
			CString strTempDBSQL;
			_RecordsetPtr pRsTempDB;
			void LoadSampleFromTempTab(int cSamplePos);
			void UpdateTempTabField(int cSamplePos);
			void AppendResult2TempTab(int cSamplePos);	//������ݵ���ʱ���ݿ�����
			void UpdateResult2TempTab(int cSamplePos);  //�޸����ݵ���ʱ���ݿ�����
			void LoadResultFromTempTab(int cSamplePos);	//����ʱ���ݿ�������
			void BatchUpdateDB2TempTab(CString m_strInstName);		//�������µ���ʱ���ݿ���
			void CreateTempTab(void);			//������ʱ���ݿ�,�Ա��ϵ�����
			void ClearTempTab(void);
			
			ALLSample();
			///////////////////////////////////////////////////////////////
			virtual void CalResult(CImageSlave *pClyImageSlave); //���������� 
			virtual void LoadResultFromDB(CString strAutoNO);    
			virtual void DelResultFromDB(CString strAutoNO); 
			////////////////////////////////////////////////////////////////
		
			////////////////////////////////////////////////////////////////
			void SetDefaultSample(CXListCtrl &m_pListCtrl);			
			void CalcAllResult();
			void CalcResult(int cSamplePos);
			//���ڴ澵���и����б�
			void OnUpdateQuality(int cSamplePos,CXListCtrl *m_pListCtrl);
			///////////////////////////////�ӽ���õ���Ʒ��Ϣ���ݼ������浽�ڴ澵����
			void GetSampleDataFromDlg(CXListCtrl &m_ListCtrl,CGyfxyImageSlave *pImg);
			void GetSampleDataFromDlg(int nItem, int nSubItem, CXListCtrl &m_ListCtrl,CGyfxyImageSlave *pImg) ;
			///////////////////////////////��������
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
		ULONG			m_sTimePut2weigh;//����������ʱ��(0_1S);
		ULONG			m_sTimePut2HighTemp;//����������ʱ��(0_1S);
		SHORT			m_sPulsePerRound1Spit;//����һȦ������;
		BYTE			m_btDriverSpitCnt;//������ϸ����;
		BYTE			m_btMaxSampleCnt;//��λ����
		BYTE			m_ByteReserve[8];

		SHORT			m_fBalanceMaxErr;//������ֵ(0.1mg);
		SHORT			m_fVolSampleMax;
		SHORT			m_fVolSampleMin;
		SHORT			m_fWaterAshSampleMax;
		SHORT			m_fWaterAshSampleMin;
		//�����ٶȲ���Ҫ,����ע�͵�
		//SHORT			m_sLowStoveTempSpeed;//����¯��������
		//SHORT			m_sHighStoveTempSpeed;//����¯��������;
		/*
		SHORT			m_sQuick_WaterTestTime;//����ˮ�ָ���ʱ��(��);
		SHORT			m_sQuick_VolTestTime;//�ָ���ʱ��(��);
		SHORT			m_sQuick_AshTestTime;//���ٻҷָ���ʱ��(��);
		
		SHORT			m_sGB_WaterTestTime;//����ˮ�ָ���ʱ��(��);
		SHORT			m_sGB_VolTestTime;//����ӷ��ָ���ʱ��(��);
		SHORT			m_sGB_AshTestTime;//����ҷָ���ʱ��(��);
		*/

		SHORT			m_sWaterTestTime;//ˮ�ָ���ʱ��(S);
		SHORT			m_sVolTestTime;//�ӷ��ָ���ʱ��(S);
		SHORT			m_sAshTestTime;//�ҷָ���ʱ��(S);


		SHORT			m_sWaterCoolTime;		//ˮ��¯������ȴʱ��
		SHORT			m_sHighTempCoolTime;	//����¯������ȴʱ��

		SHORT			m_sBeepTime;	

		LONG			m_sWaterTestTemp;//����ˮ�������¶�(��);
		LONG			m_sVolTestTemp;//���ٻӷ��������¶�(��);
		LONG			m_sAshTestTemp;//���ٻҷ������¶�(��);
		

		/*
		LONG			m_sQuick_WaterTestTemp;//����ˮ�������¶�(��);
		LONG			m_sQuick_VolTestTemp;//���ٻӷ��������¶�(��);
		LONG			m_sQuick_AshTestTemp;//���ٻҷ������¶�(��);
		
		LONG			m_sGB_WaterTestTemp;//����ˮ�������¶�(��);
		LONG			m_sGB_VolTestTemp;//����ӷ��������¶�(��);
		LONG			m_sGB_AshTestTemp;
		*/
		

		enum M_TSQ{};
		SHORT			m_szTestSequence;		//����˳��
		enum M_ALG{M_GB=0,M_QK	};
		SHORT			m_szalgorithm;			//���Է���

		char            m_bIED_system_type;
		char            m_cReserve[3];
		long            m_reServe[8]; 
	}DownloadWorkParam;
	////////////////////////////////////
	enum CoilPos{MID_MOTOR_UP=0,MID_MOTOR_DN,LEFT_UP,LEFT_DN,};
	union _OutputCoil			  //����Ĵ���
	{
		struct 	
		{	
			UINT BALANCE_MOTOR_UP	:1;		//��������			0
			UINT BALANCE_MOTOR_DN	:1;		//�����½�			1
			UINT LEFT_MOTOR_UP		:1;		//��¯����			2
			UINT LEFT_MOTOR_DN		:1;		//��¯�½�			3
		
			UINT RIGHT_MOTOR_UP		:1;		//��¯����			4
			UINT RIGHT_MOTOR_DN		:1;		//��¯�½�			5

			UINT Reserve1			:2;		//��������׼�����������������ת����	6

			UINT LEFT_OXYGEN		:1;		//��¯��������		8
			UINT RIGHT_OXYGEN		:1;		//��¯��������		9

			UINT WATER_STOVE_FAN	:1;		//ˮ��¯�ӽ�������	10
			UINT LEFT_STOVE_FAN		:1;		//��¯����			11
			UINT RIGHT_STOVE_FAN	:1;		//��¯����			12
	
			UINT SPK				:1;		//������			13
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
			UINT	MANUAL_KEY				:1;  //�ֶ����� 
			UINT    ReserveKey2				:4;	 // 

			UINT	RIGHT_STOVE_TOP			:1;  //���λ
			UINT	RIGHT_STOVE_BOT			:1;  //��λ
			UINT	LEFT_STOVE_TOP			:1;  //���λ
			UINT	LEFT_STOVE_BOT			:1;  //��λ
			UINT    WATER_STOVE_PUT			:1;  //������	
			UINT    WATER_STOVE_WEIGH		:1;  //������ 
			UINT    SAMPLE_POS_1			:1;  //��λ1
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
		long    adInstTemp;   //���廷���¶�
	}ADValue;

	struct  CFuzzy
	{
			enum {ON = 0,OFF=!ON};
			long Reserve;	
		public:
			short WarmFlag;			//���±�־
			short PreviewOut;       //�ϴ������ֵ   
			short du;               //���         
			unsigned short Kp;		//����ϵ��
			unsigned short Ki;		//˥��ϵ��1
			unsigned short KI;		//˥��ϵ��2
			struct PoutParam{ 
				char 	iTimerID;		//�ضϼ��ȶ�ʱ��
				char	iReserve;
				UINT32  *pPWMPort;		//���ȿ��ƶ˿�		   
			}pParam;
			long gDest;  			//Ŀ���¶ȵ�ADֵ
			long *AdValue ;
			long CTEMP;				//��ʼ�����¶� 
			long Error[2];			//ƫ��
			long dErr ;				//ƫ��仯��	  
			long SumErrLimit; 		//���ȳ�ֵ	
	} fuzzy[FUZZY_ARRAY_SIZE];
	struct  
	{
		char	DeviceName[24]; 
		int		CurrWeight;		  
	}Balance;
	struct StepMotorState 
	{										
		bool bRun;							//����
		bool bDir;							//����
		int DestPulseCnt;					//����Ŀ��λ��������
		int DestSamplePos;					//Ŀ�깤λ

		int CurPulseCnt;					//��ǰλ��������
		int CurSamplePos;					//��ǰ��λ 

		int PulsePerRound;					//ÿ�ƶ�һȦ��Ҫ��������
		int Reserve;						//��λ����˿�
	}MtrState; 
	///////////////////////////////////////////////////////////
	enum REST_STATE{REST_INIT=0,REST_START,REST_SAMPLEDISH,
		REST_END}; //��λ״̬
	short ResetState; 
	short ErrorFlag;	//�����־

		
	//enum TEST_STATE {T_INIT=0,T_WARM_WATER_STOVE,T_TEST_WATER,T_WEIGHT_WATER,T_WATER_END,
	//	T_WARM_HIGH,T_TEST_VOL	};	
	
	enum TEST_STATE {T_INIT=0,
		T_WARM_WATER,T_TEST_WATER,T_WATERCOOL,T_WEIGHT_WATER,T_WATER_END,
		T_WARM_HIGH1,T_TEST_VOL,T_BURN_VOL,T_WEIGHT_VOL,T_VOL_END,
		T_WARM_HIGH2,T_TEST_ASH,T_BURN_ASH,T_WEIGHT_ASH,T_ASH_END,	
		T_END};///����״̬
	long TestState;
	enum METAGE_STATE {M_INIT=0,M_POS,M_TARE,M_METAGE,M_SAVE,M_NEXTPOS,M_END};//����״̬		
	long MetageState;
	
	typedef struct ypd      //��Ʒ���ݶ���
	{
		///////////////////////////////////////////////////////////////
		UCHAR cSta;     //��ǰ����״̬,00-������,01-�ȴ�(������),02-������,03-����,04-ˮ��,05-�ӷ���,06-�ҷ�,07-�������.08-������
		UCHAR cCSta;    //�������״̬,b0-������,b1-����,b2-ˮ��,b3-�ӷ���,b4-�ҷ�,b5-������,b7-�������
		UCHAR cItem;    //�������ݣ�b0=1:ˮ�֣�b1=1:�ӷ��֣�b2=1:�ҷ�
		UCHAR reserve;
		union UQUALITY{
			struct{
			long  lMPot;    //��������(unit=0.1mg)
			///////////////////////////////////////////////////////////////
			long  lM0;      //����ǰ��Ʒ����(unit=0.1mg),������������
			long  lM1;      //ˮ���������Ʒ����(unit=0.1mg),������������
			/////////////////////////////////////////////////////////////////
			long  lMPotSample2;//�ӷ���ʵ��ǰ�Ӹ�����
			///////////////////////////////////////////////////////////////
			long  lM2;      //�ӷ����������Ʒ����(unit=0.1mg),������������
			long  lM3;      //�ҷ��������Ʒ����(unit=0.1mg),������������
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
	char ErrorMessage[INFO_LENTH];  //������Ϣ
	/////////////////////////////////////////////////////////////////   
	UCHAR   cID;                //IEDվ��
	UCHAR   ReserveID;

};

