// ComPort.cpp: implementation of the CComPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComPort.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComPort::CComPort()
{ 
	m_hComm			= NULL;
	ZeroMemory(&m_ov,sizeof(OVERLAPPED));
	m_ov.hEvent		= NULL; 
	m_hShutdownEvent= NULL;
	m_Thread		= NULL;
	m_szWriteBuffer = NULL; 
	m_nWriteBufferSize = 0;

	ZeroMemory(&m_comstat,sizeof(COMSTAT));
	 
	InitializeCriticalSection(&m_Section);
	m_szWriteBuffer = new unsigned char[MAX_BUFFER_SIZE*2];
	ZeroMemory(m_szWriteBuffer,MAX_BUFFER_SIZE*2);
	m_szReadBuffer = new unsigned char[MAX_BUFFER_SIZE*2];
	ZeroMemory(m_szReadBuffer,MAX_BUFFER_SIZE*2); 
}

CComPort::~CComPort()
{	
	
	
	StopMonitoring(); 
	delete []m_szReadBuffer;
	delete []m_szWriteBuffer;
	
	CloseHandle(m_hComm);   
	CloseHandle(m_ov.hEvent); 
	CloseHandle(m_hShutdownEvent);
	DeleteCriticalSection(&m_Section); 
}

UINT CComPort::CommThread(LPVOID pParam)
{
	CComPort * pComPort = (CComPort * )pParam;
	return pComPort->ThreadFunction();
}

//
// start comm watching
//
BOOL CComPort::StartMonitoring()
{
	SetEvent(m_hShutdownEvent);
	if(!Enabled())
		return FALSE;
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
		return FALSE;
	TRACE(_T("COM%d Monitor Thread started\n"),m_nPortNr);
	return TRUE;	
}
//
// Restart comm watching
//
BOOL CComPort::RestartMonitoring()
{
	m_Thread->ResumeThread();
	TRACE(_T("Thread resumed\n"));
	return TRUE;	
}

//
// stop comm watching
//
BOOL CComPort::StopMonitoring()
{
	if(m_Thread!=NULL)
	{			
		do
		{ 
			SetCommMask(m_hComm,0);  //让 WaitCommEvent 返回
			SetEvent(m_hShutdownEvent);	
			TRACE(_T("stop Com%d Moniter Thread  \n"),m_nPortNr);	
		}
		while(WaitForSingleObject(m_Thread->m_hThread,200)==WAIT_TIMEOUT) ;
	}
	return TRUE;		
}


BOOL	CComPort::InitPort(CWnd* pPortOwner, 
						   UINT portnr, 
						   UINT baud, 
						   TCHAR parity , 
						   UINT databits, 
						   double stopsbits, 
						   DWORD dwCommEvents , 
						   UINT nBufferSize)
{
	BOOL bResult = FALSE;

	PTSTR  szPort = new TCHAR[50];
	PTSTR  szBaud = new TCHAR[50];

	assert(portnr >  0 && portnr <  256);
	assert(pPortOwner!=NULL);

	if (m_ov.hEvent != NULL)
		CloseHandle(m_ov.hEvent);
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 	
	if (m_hShutdownEvent != NULL)
		CloseHandle(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_hShutdownEvent); 
	// initialize the event objects
	
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority		
	m_hEventArray[1] = m_ov.hEvent; 

// set buffersize for writing and save the owner
	m_pOwner		= pPortOwner;
	m_nPortNr		= portnr;
	m_CommEvents = dwCommEvents;
 
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// prepare port strings
	_stprintf(szPort, _T("\\\\.\\COM%d"), portnr);
	_stprintf(szBaud, _T("baud=%d parity=%c data=%d stop=%1.0f"), baud, parity, databits, stopsbits);
	// get a handle to the port
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		m_hComm = NULL;
		// port not found
		delete []szPort;
		delete []szBaud;
		return FALSE;
	}
	// set the timeout values
	GetCommTimeouts(m_hComm,&m_CommTimeouts);

	if(parity==_T('N'))
		iMsPerByte = 1000.0f/(baud/10);
	else
		iMsPerByte = 1000.0f/(baud/11);
	m_CommTimeouts.ReadIntervalTimeout			= 20000/baud; //T15
	m_CommTimeouts.ReadTotalTimeoutConstant		= 40000/baud;
	m_CommTimeouts.ReadTotalTimeoutMultiplier	= 20000/baud;;
	m_CommTimeouts.WriteTotalTimeoutConstant	= 40000/baud;;
	m_CommTimeouts.WriteTotalTimeoutMultiplier	= 20000/baud;;
	
	/*
	m_CommTimeouts.ReadIntervalTimeout			= 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant		= 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier	= 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant	= 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier	= 1000;
	*/
	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{						   
		if (SetCommMask(m_hComm, dwCommEvents))
		{
			ZeroMemory(&m_dcb,sizeof(DCB));
			if (GetCommState(m_hComm, &m_dcb))
			{

				m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
				//m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
				m_dcb.fRtsControl = RTS_CONTROL_TOGGLE;		// set RTS bit RTS_CONTROL_TOGGLE!
				
				if (BuildCommDCB(szBaud, &m_dcb))
				{
					if (SetCommState(m_hComm, &m_dcb))
					{
						if(SetupComm(m_hComm,nBufferSize,nBufferSize)) // SetCommBuffer
						{
							;
						}
						else
							ProcessErrorMessage(_T("SetupComm()"));
					}
					else
						ProcessErrorMessage(_T("SetCommState()"));
				}
				else
					ProcessErrorMessage(_T("BuildCommDCB()"));
			}
			else
				ProcessErrorMessage(_T("GetCommState()"));
		}
		else
			ProcessErrorMessage(_T("SetCommMask()"));
	}
	else
		ProcessErrorMessage(_T("SetCommTimeouts()"));

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	TRACE(_T("Initialization for COMPORT %d completed.\nUse Startmonitor() to communicate\n"), portnr);
 	
	delete []szPort;
	delete []szBaud;
	
	return TRUE;
}





	//DTR 电平控制
BOOL CComPort::SetDTR(BOOL OnOrOff)
{
	return Enabled() ? EscapeCommFunction(m_hComm, OnOrOff ? SETDTR : CLRDTR) != 0 : FALSE;
}

//RTS 电平控制
BOOL CComPort::SetRTS(BOOL OnOrOff)
{
	return Enabled() ? EscapeCommFunction(m_hComm, OnOrOff ? SETRTS : CLRRTS) != 0 : FALSE;
}

//
// If there is a error, give the right message
// 
void CComPort::ProcessErrorMessage(TCHAR* ErrorText)
{
	LPTSTR Temp = new TCHAR[MAX_PATH];
	DWORD nID_Error = 0;
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nID_Error = GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	_stprintf(Temp, _T("WARNING:  %s Failed with the following error: \nError ID = %d\n%s\nPort: %d\n  "), ErrorText,nID_Error, lpMsgBuf, m_nPortNr); 
#ifndef _DEBUG  
	//MessageBox(NULL, Temp, _T("Application Error"), MB_ICONSTOP);
#else
	TRACE(Temp);
#endif

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

int CComPort::ThreadFunction()
{
	// Misc. variables
	DWORD BytesTransfered = 0; 
	DWORD CommEvent = 0;
	DWORD dwError = 0; 
	BOOL  bResult = TRUE;

	
	ResetEvent(m_hShutdownEvent); 
	
	if(!::SetCommMask(m_hComm, m_CommEvents))
	{
			TCHAR szBuffer[256];
			_sntprintf(szBuffer, 255, _T("%s(%d) : COM%d Call WINAPI SetCommMask(%x, %x) Fail, thread work invalid! GetLastError() = %d;"),
				__FILE__, __LINE__, m_nPortNr, m_hComm, m_CommEvents, GetLastError());
			MessageBox(NULL, szBuffer, _T("Class CComPort"), MB_OK);
			return 1;
	}
	 
	// Clear comm buffers at startup 
	if (Enabled())		// check if the port is opened
	{
		FlushFileBuffers(m_hComm);
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		ResetEvent(m_ov.hEvent);  //会清除掉读完成标志
	}  
	for (;;) 
	{    
			if(!::WaitCommEvent(m_hComm, &CommEvent, &m_ov))
			{
				switch(GetLastError())
				{
					case ERROR_IO_PENDING:// asynchronous
						TRACE(_T("COM%d IO PENDING BEGIN\n"),m_nPortNr); 
						::GetOverlappedResult(m_hComm, &m_ov, &BytesTransfered, TRUE);   //等待操作完成
						TRACE(_T("COM%d IO PENDING END\n"),m_nPortNr);
						ProcessErrorMessage(_T("ERROR_IO_PENDING\n"));
						break;
					case	87:
						break;
					default:
						TRACE(_T("COM%d "),m_nPortNr);
						ProcessErrorMessage(_T("WaitCommEvent()"));
						break;
				}

			}
			else      //出错
			{
				bResult = ClearCommError(m_hComm, &dwError, &m_comstat);
				if (m_comstat.cbInQue == 0)
					continue;
			}	  
			bResult = WaitForMultipleObjects(2 , m_hEventArray, FALSE, INFINITE);
			//WAIT_ABANDONED_0
			
			switch(bResult)
			{
				case WAIT_OBJECT_0:							 
					TRACE(_T("Com%d Communication Thread 0x%x End\n"),m_nPortNr,GetCurrentThreadId());
					return 2; 
					break;
				case WAIT_OBJECT_0+1:   //overlapped I/O 
					//ResetEvent(m_ov.hEvent);  //会清除掉读完成标志
					GetCommMask(m_hComm, &CommEvent);	 
					/*
					if (CommEvent & EV_RXFLAG)				
						m_pOwner->SendMessage(WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) m_nPortNr);					
					if (CommEvent & EV_CTS)
						m_pOwner->SendMessage(WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) m_nPortNr);
					if (CommEvent & EV_BREAK)
						m_pOwner->SendMessage(WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM)m_nPortNr);
					if (CommEvent & EV_ERR)
						m_pOwner->SendMessage(WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM)m_nPortNr);
					if (CommEvent & EV_RING)
						m_pOwner->SendMessage(WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM)m_nPortNr);
					*/
					if(CommEvent &EV_TXEMPTY)
					{
						//m_pOwner->SendMessage(WM_COMM_TXEMPTY_DETECTED, (WPARAM) 0, (LPARAM)m_nPortNr);
					}
					if (CommEvent & EV_RXCHAR)	
					{ 
						//TRACE(_T("Receive message!\n"));
						ClearCommError(m_hComm,&dwError, &m_comstat); 
						if((m_pOwner!=NULL)&&(m_comstat.cbInQue<MAX_BUFFER_SIZE))
						{
							
							if(m_comstat.cbInQue>0)
							{
								try
								{  
									//m_pOwner->SendMessage(WM_COMM_RXFLAG_DETECTED, (WPARAM) m_comstat.cbInQue, (LPARAM) m_nPortNr);
								}
								catch(CException &e)
								{
									UNREFERENCED_PARAMETER(e);
								}
							} 						
						} 
						else
						{
							Read(m_szWriteBuffer,m_comstat.cbInQue);  //清空接受缓存
							TRACE(_T("Flush ComPort buffer\n"));
							//FlushFileBuffers(m_hComm);
						}
						
					}  
					
					break;
			}
			
		
	} 
	UNREFERENCED_PARAMETER(m_comstat);
}
 


int CComPort::ReadAll(unsigned char *Buf)
{ 
	DWORD dwError;
	ClearCommError(m_hComm,&dwError, &m_comstat); 
	if(m_comstat.cbInQue>0)
		return Read(Buf,m_comstat.cbInQue);	
	else
		return 0;
} 

int CComPort::WriteDirect(unsigned char *Buf, DWORD Count)
{
	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD       dwErrorFlags;
	DWORD   	dwError;
	DWORD       dwBytesSent=0; 

	OVERLAPPED			WriteOv;
	ZeroMemory(&WriteOv,sizeof(OVERLAPPED));
	WriteOv.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	if (!Enabled()) return -1;
	EnterCriticalSection(&m_Section);

	fWriteStat = WriteFile(m_hComm, Buf, Count,
                     &dwBytesWritten, &WriteOv ) ;
	dwBytesSent += dwBytesWritten;
	 
//#ifdef _DEBUG
	if (!fWriteStat)
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hComm,&WriteOv, &dwBytesWritten, TRUE ) ;    //被拔了也不退出
			dwBytesSent += dwBytesWritten;
		}
		else
		{
			ClearCommError(m_hComm, &dwErrorFlags, &m_comstat ) ;
		}
	}
//#endif
	LeaveCriticalSection(&m_Section); 
	CloseHandle(WriteOv.hEvent);
	ASSERT(dwBytesSent==Count);
	return dwBytesSent ;
}

int CComPort::Read(unsigned char *Buf,
				   DWORD Count,
				   DWORD dwWaitTime)
{
	BOOL       fReadStat ; 
	DWORD      dwErrorFlags;
	DWORD      dwLength ;
	DWORD      dwBytesReceive=0; 
	DWORD      dwError;
	if (!Enabled()) return -1;
	EnterCriticalSection(&m_Section);

	{
		//ClearCommError( m_hComm, &dwErrorFlags, &m_comstat ) ;
		dwLength =  0 ;
		fReadStat = ReadFile( m_hComm, Buf,Count, &dwLength, &m_ov ) ; 		
		if (!fReadStat)
		{ 			
			if (GetLastError() == ERROR_IO_PENDING)
			{
				GetOverlappedResult(m_hComm,&m_ov , &dwLength, TRUE );
				dwBytesReceive += dwLength;	 			
			} 
			else
			{
				ProcessErrorMessage(_T("ReadError! 1\n"));
			}
		} 
		else
		{
			dwBytesReceive += dwLength; 
		}
	}
	LeaveCriticalSection(&m_Section); 
	if(dwBytesReceive!=Count)        //BUG:为啥总是漏掉几个数据呢?读取不齐那么多
									// ClearCommError(m_hComm,&dwError, &m_comstat); 不能在不同的线程内调用
	{
		ProcessErrorMessage(_T("ReadError! 1\n"));
	}
	return dwBytesReceive ;
}


int CComPort::Write(unsigned char *Buf,
					DWORD Count,
					DWORD dwWaitTime)
{
	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD       dwErrorFlags;
	DWORD   	dwError;
	DWORD       dwBytesSent=0;
	DWORD		result;

	OVERLAPPED			WriteOv;
	ZeroMemory(&WriteOv,sizeof(OVERLAPPED));
	WriteOv.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	if (!Enabled()) return -1;
	EnterCriticalSection(&m_Section);

	fWriteStat = WriteFile(m_hComm, Buf, Count,
		&dwBytesWritten, &WriteOv ) ;
	if (!fWriteStat)
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			while(!GetOverlappedResult(m_hComm,&WriteOv, &dwBytesWritten, FALSE ))
			{ 
				dwError = GetLastError();
				if(dwError == ERROR_IO_INCOMPLETE)
				{
					dwBytesSent += dwBytesWritten;		
					result = WaitForSingleObject(WriteOv.hEvent, dwWaitTime); // 结束异步I/O
					switch(result) {
					case WAIT_OBJECT_0: //
						break;
					case WAIT_TIMEOUT:
						break;
					default:
						break;
					}
					continue;
				}
				else
				{
					ClearCommError(m_hComm, &dwErrorFlags, &m_comstat ) ;
					break;
				} 
			}
			dwBytesSent += dwBytesWritten;
		}
		else
		{
			ClearCommError(m_hComm, &dwErrorFlags, &m_comstat ) ;
		}
	}
	else 
		dwBytesSent += dwBytesWritten;
	LeaveCriticalSection(&m_Section); 
	CloseHandle(WriteOv.hEvent);
	return dwBytesSent ;
}
