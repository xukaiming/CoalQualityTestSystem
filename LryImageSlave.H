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
		UCHAR  m_cINNERJIAOBAN2		:1; //��Ͱ����	DC
		UCHAR  m_cHEATCTRL			:1;	//���ȿ���
		UCHAR  m_cCOOLCTRL			:1; //�������
		UCHAR  m_cFILLWATER 		:1; //��ˮ 
		UCHAR  m_cOUTWATER  		:1; //��ˮ
		UCHAR  m_cOUTERJIAOBAN		:1; //��Ͳ����
		UCHAR  m_cINNERJIAOBAN1		:1; //��Ͱ����	AC	
		UCHAR  m_cFIRE				:1; //���	 			
	};
	//Modbus SHORT 8�ֽڶ���
	struct  _downloadparam
	{
		SHORT			m_sFillWaterTime;     //עˮʱ��
		SHORT			m_sOutWaterTime;      //��ˮʱ��
		SHORT			m_sStartJiaoBanTime1; //���ڽ���1
		SHORT			m_sStartJiaoBanTime2; //���ڽ���2

		SHORT			m_sEndtJiaoBanTime;   //ĩ�ڽ���ʱ��
		SHORT			m_sFireTime;          //���ʱ��(S)
		SHORT			m_sSenserPowerONTime; //Reserve
		SHORT			m_sBeepTime;		  //����ʱ��(ms)
		/*
		unsigned short	m_fFireOKtemp;		  //���ɹ��жϵ�
		unsigned short	m_fEndCheckTemp;	  //ĩ���¶��жϵ�		
		SHORT           m_WorkCmd;            //����״̬
		SHORT			m_Reserver1;		  //���,�Ա�����
		*/
		unsigned short	m_fFireOKtemp_Hi;	  //���ɹ��жϵ�
		unsigned short	m_fFireOKtemp_Lo;	  //���ɹ��жϵ�
		unsigned short	m_fEndCheckTemp;	  //ĩ���¶��жϵ�		
		SHORT           m_WorkCmd;            //����״̬ 
		
		CHAR			m_szAlgorithm[8] ; 	  //�㷨 
		CHAR			m_Reserver2;
		CHAR			m_NewcID;
		CHAR			m_Reserver3[2];		  //���,�Ա����� 	
	}
	DownloadWorkParam;
	//Modbus SHORT �ֽڶ���
	struct _uploadReg
	{
		LONG  AD7714Value[MAX_AD_LENTH];		 	   
		LONG  m_JiaoBanSpeed;                  //�����ٶ� 
		SHORT  AD7714ValuePtr;	  			   //ADת��ֵָ��
		SHORT  TestState;    				   //����״̬
		LONG   Cnt;							   //ʵ��ʱ����� 1S
		CHAR  sType[24];                       //IED�ͺ�
	}
	UploadRegister; 
	union _InputStatus			  //����Ĵ���
	{
		struct _Input1             //����Ϊ��λ��ǰ 
		{
			UCHAR  cReserve3			:8;
			UCHAR  FireWire				:1;  //���˿�ж�  
			UCHAR  WaterLevel			:7;  //ˮλ�Ĵ���				
		}Input1;
		struct _Input2
		{
			UCHAR  cReserve4	;
			UCHAR InPuts8;   
		}Input2;
	}InputStatus;
	union _OutputCoil			  //����Ĵ���
	{
		_Output1 Output1;
		struct _Output2
		{
			UCHAR m_cReserve6; 
			UCHAR Coil8;  
		}Output2;
	}OutputCoil;
	struct _Report{
		LONG lTj;			//��Ͳ�¶�
		LONG lT0;			//����ʱ��Ͱ�¶�
		LONG lt0;			//���ʱ��Ͱ�¶�
		LONG lT140;			//1'40''��Ͱ�¶�
		LONG ltn;			//�յ���Ͱ�¶�
		LONG lTn;			//ĩ����Ͱ�¶�  
		LONG lTiCnt;		//t0��tn�ķ�����n
		LONG lT0Cnt;
		LONG lTnCnt;
		LONG lReserveCnt; 
		LONG lTi[MAX_TI_CNT];	//�����¶�  
	}Report;
	struct _ReportT0Tn
	{ 
		LONG lTT0[MAX_T0_CNT];
		LONG lTTn[MAX_Tn_CNT];
	}ReportT0Tn;
	UCHAR 	TimerFireID;		  				//���ʱ��ID   
	UCHAR 	TimeFillWaterID;	  				//��ˮʱ�䶨ʱ��ID
	UCHAR 	TimeWaterStableID;  				//�ȶ���ʱ��ID
	UCHAR 	TimeOutWaterID;	  					//��ˮʱ�䶨ʱ��ID
	UCHAR 	TimeOutJiaobanID;	  				//��Ͳ���趨ʱ��ID
	UCHAR 	TimeEarlyStateID;   				//���ڽ׶ζ�ʱ��ID
	UCHAR 	TimeEndStateID;	  					//ĩ�ڽ׶ζ�ʱ��ID
	UCHAR	cID;                				//IEDվ��
};
#endif