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

#define COMMAND_RRT		0x01		//报告IED型号
#define COMMAND_SID		0x02		//设置IED ID
#define COMMAND_SPS		0x03		//设置注册码
#define COMMAND_LOG		0x04		//验证注册码
#define COMMAND_SST		0x05		//设置时间
#define COMMAND_RST		0x06		//读时间
#define COMMAND_RES		0x07		//复位IED
#define COMMAND_DLD		0x08		//下载数据

#define	COMMAND_RDI		0x09		//数据报告
#define COMMAND_JDQ		0x0A		//继电器控制
	/*
	#define  CMD_INNERJIAOBAN2   0  	//内桶搅拌	DC
	#define  CMD_HEATCTRL        1 		//加热控制
	#define  CMD_COOLCTRL		 2 		//制冷控制
	#define  CMD_FILLWATER 		 3  	//进水 
	#define  CMD_OUTWATER  		 4  	//放水
	#define  CMD_OUTERJIAOBAN    5  	//外筒搅拌
	#define  CMD_INNERJIAOBAN1   6  	//内桶搅拌	AC	
	#define  CMD_FIRE			 7 		//点火  

  */
	
#define COMMAND_CQD		0x0B		//启动试验
#define COMMAND_CTZ		0x0C		//停止试验
#define COMMAND_RDF		0x0D		//报告试验结果数据
#define COMMAND_WUP     0X0E		//开始升温
#define COMMAND_DJ      0X0F		//开始升温
#define COMMAND_MTR     0x10		//移动电机
#define COMMAND_MTR_POS 0x11		//移动样盘电机

#define SCH_CMD_DISCURVE	1		//显示温升曲线
#define SCH_CMD_ENTEST		2		//使能测试按扭
#define SCH_CMD_ENSTOP		3
#define SCH_CMD_DISTEST		4		//关闭测试按扭
#define SCH_CMD_CLOSESYS	5		//关闭系统
#define SCH_CMD_LOGOK		6		//注册成功

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
	HANDLE				m_hModbusShutdownEvent;     //通知线程关闭句柄
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
	ULONG errorCnt;       //错误计数
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
