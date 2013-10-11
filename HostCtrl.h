// HostCtrl.h: interface for the CHostCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOSTCTRL_H__D17438DE_8C1B_4B42_98DE_2DC63F562F70__INCLUDED_)
#define AFX_HOSTCTRL_H__D17438DE_8C1B_4B42_98DE_2DC63F562F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModBusServer.h"
#include "RDB.h"
#define  THREADPRIOD   30
//#define  THREADPRIOD   100

#define WM_START_PROGBAR		WM_USER+200
#define WM_IMAGE_DATA_CHANGE	WM_USER+203
#define WM_CALC_DATA			WM_USER+204

#define COMMAND_RRT		0x01		//����IED�ͺ�
#define COMMAND_SID		0x02		//����IED ID
#define COMMAND_SPS		0x03		//����ע����
#define COMMAND_LOG		0x04		//��֤ע����
#define COMMAND_SST		0x05		//����ʱ��
#define COMMAND_RST		0x06		//��ʱ��
#define COMMAND_RES		0x07		//��λIED
#define COMMAND_DLD		0x08		//��������

#define	COMMAND_RDI		0x09		//���ݱ���
#define COMMAND_JDQ		0x0A		//�̵�������
	/*
	#define  CMD_INNERJIAOBAN2   0  	//��Ͱ����	DC
	#define  CMD_HEATCTRL        1 		//���ȿ���
	#define  CMD_COOLCTRL		 2 		//�������
	#define  CMD_FILLWATER 		 3  	//��ˮ 
	#define  CMD_OUTWATER  		 4  	//��ˮ
	#define  CMD_OUTERJIAOBAN    5  	//��Ͳ����
	#define  CMD_INNERJIAOBAN1   6  	//��Ͱ����	AC	
	#define  CMD_FIRE			 7 		//���  

  */
	
#define COMMAND_CQD		0x0B		//��������
#define COMMAND_CTZ		0x0C		//ֹͣ����
#define COMMAND_RDF		0x0D		//��������������
#define COMMAND_WUP     0X0E		//��ʼ����
#define COMMAND_DJ      0X0F		//��ʼ����
#define COMMAND_MTR     0x10		//�ƶ����
#define COMMAND_MTR_POS 0x11		//�ƶ����̵��

#define SCH_CMD_DISCURVE	1		//��ʾ��������
#define SCH_CMD_ENTEST		2		//ʹ�ܲ��԰�Ť
#define SCH_CMD_ENSTOP		3
#define SCH_CMD_DISTEST		4		//�رղ��԰�Ť
#define SCH_CMD_CLOSESYS	5		//�ر�ϵͳ
#define SCH_CMD_LOGOK		6		//ע��ɹ�

#define TIME_EVENT_PROCBAR		1
#define TIME_EVENT_CLOSEWINDOWS 2
#define TIME_EVENT_ISLOGIN		3

class CHostCtrl : public CObject  
{
public:
	CHostCtrl();
	virtual ~CHostCtrl();
	inline UINT GetID(){return pRdb->m_bID; };

	BOOL StopCommThread();
	void AddCommand(UCHAR cCommand, UCHAR cCommandParameter1,UCHAR cCommandParameter2=0,UCHAR cCommandParameter3=0,void* pCommandParam=NULL);
	void AddCycCommand(UCHAR cCommand, UCHAR cCommandParameter);
	void RemoveAllCommand();
	BOOL InitDevice(CRDB *pRdb);
	virtual DWORD WaitT35(); 
	void PostM(TCHAR *cMessage, UINT iLen, UINT iCommand);
	void SendM(TCHAR *cMessage, UINT iLen, UINT iCommand);
	virtual void SetRDB(CRDB *pRdb)=0;
	virtual void StateProc() ;
public:
	CWnd				*m_pOwner;
	CModBusServer		*pModbusMaster;  	
	CWinThread*			m_pThread; 
	HANDLE				m_hModbusShutdownEvent;     //֪ͨ�̹߳رվ��
	HANDLE				m_Lock[2]; 
	CRDB				*pRdb;
	CImageSlave			*pImageSlave;

private:
	BOOL InitPort( );
	BOOL StartCommThread();
	
	void SuspendCommThread();
	void ResumeCommThread();
	int CommThreadFunction();
	static UINT CommThread(LPVOID pParam);
	
	void ModbusProc();
	BOOL SendCommand();
	
private:	
	
	virtual int	CommandCode(UCHAR buf[], UCHAR cCommand, UCHAR cCommandParam2,UCHAR cCommandParam1,UCHAR cCommandParam3,void *m_pCommandParam) =0;  
private:
	UCHAR buf[MAX_BUFFER_SIZE];
	ULONG OkCnt; 
	ULONG errorCnt;       //�������
	BOOL bReadResult ;


protected:	
	class Cmd
	{ 
	public:
		struct  {
			UCHAR m_cCycCommand;
			UCHAR m_cCommandParam;
			int  m_lTime;			
		}CycleCmd;			
		class  ONECMD:public CObject
		{
		public:
			UCHAR m_cCommand;
			UCHAR m_cCommandParam1;
			UCHAR m_cCommandParam2;
			UCHAR m_cCommandParam3; 
			void *m_pCommandParam;
		};	
		struct
		{
			UCHAR m_cCommand;
			UCHAR m_cCommandParam1;
			UCHAR m_cCommandParam2;
			UCHAR m_cCommandParam3;
			void *m_pCommandParam;
		}SendCmd;
		CObList OneCmdArray;
		///////////////////////////////////////////
		Cmd()
		{ 
			CycleCmd.m_cCycCommand	= 0;
			CycleCmd.m_lTime		= 0;
			SendCmd.m_cCommand		= 0; 
			OneCmdArray.RemoveAll();
		}
		////////////////////////////////////////////
		~Cmd()
		{
			RemoveAllCommand();
		}
		void RemoveAllCommand()
		{
			int j = OneCmdArray.GetCount();
			ONECMD *pCmd = NULL;
			for(int i=0;i<j;i++)
			{								
				pCmd = (ONECMD *)OneCmdArray.GetHead();
				OneCmdArray.RemoveHead();
				delete pCmd;
			}
			SendCmd.m_cCommand =0;
		}
		////////////////////////////////////////////
	};
	Cmd Command;  
};



#endif // !defined(AFX_HOSTCTRL_H__D17438DE_8C1B_4B42_98DE_2DC63F562F70__INCLUDED_)
