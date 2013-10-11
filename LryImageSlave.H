//LryImageSlave.H
#ifndef	_LRY_IMAGE_SLAVE_H
#define _LRY_IMAGE_SLAVE_H

#define MAX_AD_LENTH 5
#define MAX_TI_CNT   35

#define MAX_T0_CNT   16
#define MAX_Tn_CNT   29
 


struct SLryImageSlave
{ 
	struct _Output1
	{
		UCHAR  m_cReserve5			:8;
		UCHAR  m_cINNERJIAOBAN2		:1; //内桶搅拌	DC
		UCHAR  m_cHEATCTRL			:1;	//加热控制
		UCHAR  m_cCOOLCTRL			:1; //制冷控制
		UCHAR  m_cFILLWATER 		:1; //进水 
		UCHAR  m_cOUTWATER  		:1; //放水
		UCHAR  m_cOUTERJIAOBAN		:1; //外筒搅拌
		UCHAR  m_cINNERJIAOBAN1		:1; //内桶搅拌	AC	
		UCHAR  m_cFIRE				:1; //点火	 			
	};
	//Modbus SHORT 8字节对齐
	struct  _downloadparam
	{
		SHORT			m_sFillWaterTime;     //注水时间
		SHORT			m_sOutWaterTime;      //排水时间
		SHORT			m_sStartJiaoBanTime1; //初期搅拌1
		SHORT			m_sStartJiaoBanTime2; //初期搅拌2

		SHORT			m_sEndtJiaoBanTime;   //末期搅拌时间
		SHORT			m_sFireTime;          //点火时间(S)
		SHORT			m_sSenserPowerONTime; //Reserve
		SHORT			m_sBeepTime;		  //蜂鸣时间(ms)
		/*
		unsigned short	m_fFireOKtemp;		  //点火成功判断点
		unsigned short	m_fEndCheckTemp;	  //末期温度判断点		
		SHORT           m_WorkCmd;            //命令状态
		SHORT			m_Reserver1;		  //填充,以备对齐
		*/
		unsigned short	m_fFireOKtemp_Hi;	  //点火成功判断点
		unsigned short	m_fFireOKtemp_Lo;	  //点火成功判断点
		unsigned short	m_fEndCheckTemp;	  //末期温度判断点		
		SHORT           m_WorkCmd;            //命令状态 
		
		CHAR			m_szAlgorithm[8] ; 	  //算法 
		CHAR			m_Reserver2;
		CHAR			m_NewcID;
		CHAR			m_Reserver3[2];		  //填充,以备对齐 	
	}
	DownloadWorkParam;
	//Modbus SHORT 字节对齐
	struct _uploadReg
	{
		LONG  AD7714Value[MAX_AD_LENTH];		 	   
		LONG  m_JiaoBanSpeed;                  //搅拌速度 
		SHORT  AD7714ValuePtr;	  			   //AD转换值指针
		SHORT  TestState;    				   //测试状态
		LONG   Cnt;							   //实验时间计数 1S
		CHAR  sType[24];                       //IED型号
	}
	UploadRegister; 
	union _InputStatus			  //输入寄存器
	{
		struct _Input1             //联合为高位在前 
		{
			UCHAR  cReserve3			:8;
			UCHAR  FireWire				:1;  //点火丝判断  
			UCHAR  WaterLevel			:7;  //水位寄存器				
		}Input1;
		struct _Input2
		{
			UCHAR  cReserve4	;
			UCHAR InPuts8;   
		}Input2;
	}InputStatus;
	union _OutputCoil			  //输入寄存器
	{
		_Output1 Output1;
		struct _Output2
		{
			UCHAR m_cReserve6; 
			UCHAR Coil8;  
		}Output2;
	}OutputCoil;
	struct _Report{
		LONG lTj;			//外筒温度
		LONG lT0;			//初期时内桶温度
		LONG lt0;			//点火时内桶温度
		LONG lT140;			//1'40''内桶温度
		LONG ltn;			//终点内桶温度
		LONG lTn;			//末期内桶温度  
		LONG lTiCnt;		//t0到tn的分钟数n
		LONG lT0Cnt;
		LONG lTnCnt;
		LONG lReserveCnt; 
		LONG lTi[MAX_TI_CNT];	//主期温度  
	}Report;
	struct _ReportT0Tn
	{ 
		LONG lTT0[MAX_T0_CNT];
		LONG lTTn[MAX_Tn_CNT];
	}ReportT0Tn;
	UCHAR 	TimerFireID;		  				//点火定时器ID   
	UCHAR 	TimeFillWaterID;	  				//进水时间定时器ID
	UCHAR 	TimeWaterStableID;  				//稳定定时器ID
	UCHAR 	TimeOutWaterID;	  					//出水时间定时器ID
	UCHAR 	TimeOutJiaobanID;	  				//外筒搅拌定时器ID
	UCHAR 	TimeEarlyStateID;   				//初期阶段定时器ID
	UCHAR 	TimeEndStateID;	  					//末期阶段定时器ID
	UCHAR	cID;                				//IED站号
};
#endif