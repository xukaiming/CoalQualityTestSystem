// HostCtrl.cpp: implementation of the CHostCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "HostCtrl.h"
#include "TEA\\Encpypt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "MainFrm.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHostCtrl::CHostCtrl()

{
	bReadResult = FALSE;
	m_pOwner	= NULL;
	m_hModbusShutdownEvent	= NULL; 
	m_pThread				= NULL; 
	pImageSlave				= NULL;
}

CHostCtrl::~CHostCtrl()
{
	StopCommThread(); 
	if(m_hModbusShutdownEvent != NULL)
		CloseHandle(m_hModbusShutdownEvent);
}

void CHostCtrl::RemoveAllCommand()
{ 	
	SuspendCommThread();
	Command.RemoveAllCommand();
	ResumeCommThread();
}
void CHostCtrl::AddCommand(UCHAR cCommand, UCHAR cCommandParameter1,UCHAR cCommandParameter2,UCHAR cCommandParameter3,void *m_pCommandParam)
{
	Cmd::ONECMD *pOneCmd ;
	if(Command.OneCmdArray.GetCount()!=0)
	{
		pOneCmd = (Cmd::ONECMD *)Command.OneCmdArray.GetTail();
		if((pOneCmd->m_cCommand!=cCommand)||													//命令不同
			((pOneCmd->m_cCommand==cCommand)&&(pOneCmd->m_cCommandParam1!=cCommandParameter1))) //命令相同而参数不同
		{
			pOneCmd = new Cmd::ONECMD();
			pOneCmd->m_cCommand=cCommand;
			pOneCmd->m_cCommandParam1=cCommandParameter1;
			pOneCmd->m_cCommandParam2=cCommandParameter2;
			pOneCmd->m_cCommandParam3=cCommandParameter3;
			pOneCmd->m_pCommandParam =m_pCommandParam;
			Command.OneCmdArray.AddTail(pOneCmd);	
		}
		
	} 
	else					//一次命令集合为空直接添加
	{
		pOneCmd = new Cmd::ONECMD();
		pOneCmd->m_cCommand=cCommand;
		pOneCmd->m_cCommandParam1=cCommandParameter1;
		pOneCmd->m_cCommandParam2=cCommandParameter2;
		pOneCmd->m_cCommandParam3=cCommandParameter3;
		pOneCmd->m_pCommandParam =m_pCommandParam;
		Command.OneCmdArray.AddTail(pOneCmd);	
	}
}

void CHostCtrl::AddCycCommand(UCHAR cCommand, UCHAR cCommandParameter)
{ 
	Command.CycleCmd.m_lTime			= 0;
	Command.CycleCmd.m_cCycCommand		= cCommand;
	Command.CycleCmd.m_cCommandParam	= cCommandParameter;
}


int CHostCtrl::CommThreadFunction()
{
	int iResult = 0; 
	m_Lock[0]	= m_hModbusShutdownEvent;
	m_Lock[1]	= pModbusMaster->MutexLock ;
	for(;;)
	{
		switch(WaitForSingleObject(m_hModbusShutdownEvent,THREADPRIOD)) //线程切换分辨率为20ms
		{
		case WAIT_ABANDONED:
		case WAIT_OBJECT_0:
			TRACE(_T("%s Modbus Master communication Thread has been be  killed!\n"),pRdb->m_szName); 
			return iResult = 2;
			break;
		case WAIT_TIMEOUT:
			ModbusProc();
			break;
		} 
	}
	return iResult ;
}

UINT CHostCtrl::CommThread(LPVOID pParam)
{
	CHostCtrl * pHostCtrl = (CHostCtrl * )pParam;
	return pHostCtrl->CommThreadFunction();
}


int CHostCtrl::CommandCode(UCHAR buf[], UCHAR cCommand, UCHAR cCommandParam1=0,UCHAR cCommandParam2=0,UCHAR cCommandParam3=0,void* pCommandParam=NULL)   
//int CCLYHostCtrl::

{
	ASSERT(FALSE);
	return 0; //永远返回失败,必须重载
}

void CHostCtrl::StateProc()
{
	ASSERT(pImageSlave!=NULL);
	ASSERT(pRdb!=NULL);
	pRdb->RefreshRDBTab(pImageSlave);
}

BOOL CHostCtrl::StopCommThread()
{	
	Command.RemoveAllCommand();
	if(m_pThread!=NULL)
	{
		do
		{
			TRACE(_T("Want to stop %s Modbus Master communication Thread \n"),pRdb->m_szName);
			SetEvent(m_hModbusShutdownEvent);	//m_Thread
		}  
		//while(WaitForSingleObject(m_pThread->m_hThread,INFINITE)==WAIT_TIMEOUT);
		while((m_pThread->m_hThread!=NULL)&&(WaitForSingleObject(m_pThread->m_hThread,500)==WAIT_TIMEOUT));
	}	
	return TRUE;		//INFINITE
}

void CHostCtrl::SuspendCommThread()
{
	m_pThread->SuspendThread();
}

void CHostCtrl::ResumeCommThread()
{
	m_pThread->ResumeThread();
}

void CHostCtrl::ModbusProc()
{ 
	Cmd::ONECMD *pSendCmd;
	if(Command.SendCmd.m_cCommand)	//有待发命令
	{ 
		switch(WaitForMultipleObjects(sizeof(m_Lock)/sizeof(HANDLE),m_Lock, FALSE,INFINITE))
		{
			case WAIT_OBJECT_0:         //防止死等串口发送,在此唤醒等待,不发送就直接退出
				TRACE(_T("%s shouted:老子不等了,走了   byebye! \n") ,pRdb->m_szName);
				//CancelIo
				return;
			case WAIT_OBJECT_0+1:
  				if(SendCommand())          //已经发送完毕
				{
					Command.SendCmd.m_cCommand		= 0; 
					++OkCnt;

					#ifdef _DEBUG
					if(OkCnt%100==0)
					{
						TRACE(_T("正确发送次数%d\n "),OkCnt);
					}
					#endif
				} 
				else
				{			
					
					TRACE(_T("Modbus发送失败,重新发送  "));
					TRACE(_T("失败次数,%d \n"),++errorCnt);
				}
				ReleaseMutex (pModbusMaster->MutexLock );
			break;

		} 
		return;
	} 

 	if(Command.OneCmdArray.GetCount()!=0)                  //有一次命令,发一次命令
	{ 
		pSendCmd = (Cmd::ONECMD*)Command.OneCmdArray.GetHead();
		Command.SendCmd.m_cCommand			= pSendCmd->m_cCommand;
		Command.SendCmd.m_cCommandParam1	= pSendCmd->m_cCommandParam1;
		Command.SendCmd.m_cCommandParam2	= pSendCmd->m_cCommandParam2;
		Command.SendCmd.m_cCommandParam3	= pSendCmd->m_cCommandParam3;
		Command.SendCmd.m_pCommandParam		= pSendCmd->m_pCommandParam;
		Command.OneCmdArray.RemoveHead();
		delete pSendCmd	; 
		return;
	}
	if(Command.CycleCmd.m_cCycCommand)		//有循环命令
	{
		if(Command.CycleCmd.m_lTime--<=0)
		{
			Command.CycleCmd.m_lTime			=	(pRdb->m_lScanPriod_mS-T15-T35)/THREADPRIOD;//ThreadPriod ;
			Command.SendCmd.m_cCommand			=	Command.CycleCmd.m_cCycCommand; 
			Command.SendCmd.m_cCommandParam1	=	Command.CycleCmd.m_cCommandParam; 
		}			
	} 
	return ;
}

BOOL CHostCtrl::SendCommand()
{	 
	
	ASSERT(pModbusMaster!=NULL);
	int bResult =  CommandCode(buf,Command.SendCmd.m_cCommand,
					Command.SendCmd.m_cCommandParam1,
					Command.SendCmd.m_cCommandParam2,
					Command.SendCmd.m_cCommandParam3,
					Command.SendCmd.m_pCommandParam);  
	if(bResult>0)
	{
		if(IsWindow(m_pOwner->m_hWnd))
		{
			//更新LRYCTRL数据库...........
			m_pOwner->PostMessage(WM_IMAGE_DATA_CHANGE,0,0);	
		}
		else
		{
			TRACE(_T("%s主窗体已被销毁!\n") ,pRdb->m_szName);
		}
		return TRUE;
	}

	return FALSE;
}

void CHostCtrl::PostM(TCHAR *cMessage, UINT iLen, UINT iCommand)
{
	ASSERT(m_pOwner!=NULL);
	if(IsWindow(m_pOwner->m_hWnd))
	{
		LPARAM lP=(iCommand<<16)&0xffff0000 | iLen&0x0000ffff;
		m_pOwner->PostMessage(WM_START_PROGBAR, (WPARAM)cMessage,lP);
	}
}

void CHostCtrl::SendM(TCHAR *cMessage, UINT iLen, UINT iCommand)
{
	ASSERT(m_pOwner!=NULL);
	if(IsWindow(m_pOwner->m_hWnd))
	{
		LPARAM lP=(iCommand<<16)&0xffff0000 | iLen&0x0000ffff;
		m_pOwner->SendMessage(WM_START_PROGBAR, (WPARAM)cMessage,lP);
	}
}

BOOL CHostCtrl::InitDevice(CRDB *pRdb)
{
	SetRDB(pRdb);
	//初始化通信口线程参数并启动线程
	if(!InitPort())
	{
		AfxMessageBox(_T("串口初始化失败,请检查后再试"),MB_OK+MB_ICONERROR);
		return FALSE;
	}	
	PostM(_T("系统正在注册..."),30,0);
	//启动调度线程
	if(!StartCommThread())
	{
		AfxMessageBox(_T("MODBUS通信线程初始化失败,请检查后再试"),MB_OK+MB_ICONERROR);
	} 
	AddCommand(COMMAND_LOG,0);	//验证注册码 
	return TRUE;
}


BOOL CHostCtrl::InitPort( )
{
	BOOL bResult = FALSE;
	CMainFrame * pFrame    = (CMainFrame *)AfxGetApp()->m_pMainWnd; 
	CComPort *m_pComPort   = pFrame->m_pComPort; 
	CModBusServer *pModbusMaster = pFrame->pModbusMaster;
	for(int i = 0;i<pFrame->m_ComPortCount;i++)
	{
		
		if((m_pComPort[i].m_nPortNr==pRdb->m_sComPort)&&(m_pComPort[i].Enabled()==TRUE))	
		{ 
			this->pModbusMaster = &pModbusMaster[i] ;
			bResult = TRUE;
			break;
		}
	}
	return bResult;
}

BOOL CHostCtrl::StartCommThread()
{ 
	if(m_hModbusShutdownEvent!=NULL)
		CloseHandle(m_hModbusShutdownEvent);
	m_hModbusShutdownEvent  = CreateEvent(NULL, TRUE, FALSE, NULL); 
	
	//启动通信线程
	errorCnt = OkCnt = 0;
	if ((m_pThread = AfxBeginThread(CommThread, this))!=NULL)  
	{
		TRACE(_T("%s Modbus Master communication Thread started!\n"),pRdb->m_szName);
		return TRUE;
	}
	else
	{
		TRACE(_T("%s Modbus Master communication Thread started Failed!\n"),pRdb->m_szName);
		return FALSE;
	}
}


DWORD CHostCtrl::WaitT35()
{
	double *pdMsPerByte = &pModbusMaster->m_pPort->iMsPerByte;
	int sendbyteLength = pModbusMaster->SendLength;
	double dSendTime = *pdMsPerByte*sendbyteLength*2.0f;
	return WaitForSingleObject(m_hModbusShutdownEvent,T35+dSendTime);
}
 

void CHostCtrl::SetRDB(CRDB *pRdb)
{
	this->pRdb = pRdb; 
}
