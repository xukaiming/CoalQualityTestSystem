// ComPort.h: interface for the CComPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPORT_H__46C77595_F1FD_480C_BD5D_502369F4A891__INCLUDED_)
#define AFX_COMPORT_H__46C77595_F1FD_480C_BD5D_502369F4A891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"
#include <afxmt.h>

#define WM_COMM_BREAK_DETECTED		WM_USER+101	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+102	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+103	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+104	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+105	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+106	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+107	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+108	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+109	// The last character in the output buffer was sent.  

#define MAX_BUFFER_SIZE 1024
//void (*ReadFunction)(char *pReadBuf,int length);
//typedef int    (*ReadFunction)(unsigned char *pReadBuf,int length);
//friend class CModBusServer;
class CComPort  
{
	friend class CModBusServer;
public:
	CComPort();
	virtual ~CComPort();
	inline BOOL Enabled()
	{
		return m_hComm != NULL;
	}
	 BOOL InterruptIOComm()
	{
		FlushFileBuffers(m_hComm);
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		return TRUE;
	}
//// Implementation
public:
	int WriteDirect(unsigned char *Buf,DWORD Count);
	int ReadAll(unsigned char *Buf);
	//ReadFunction pReadFunction;
	int		Write(unsigned char *Buf, DWORD Count,DWORD dwWaitTime = 200);
	int		Read (unsigned char *Buf, DWORD Count,DWORD dwWaitTime = 200);
	// port initialisation											
	BOOL	InitPort(CWnd* pPortOwner, UINT portnr, UINT baud , TCHAR parity = 'N', UINT databits = 8, double stopsbits=1, DWORD dwCommEvents = EV_RXCHAR , UINT nBufferSize = MAX_BUFFER_SIZE);
	BOOL	StartMonitoring();
	BOOL	RestartMonitoring();
	BOOL	StopMonitoring();
	BOOL	SetDTR(BOOL OnOrOff);
	BOOL	SetRTS(BOOL OnOrOff);
	int					m_nPortNr;
	double		iMsPerByte;
	DCB					m_dcb ; 
	//HANDLE 				m_OwnerLock;
private:
	virtual int ThreadFunction();
	void	ProcessErrorMessage(TCHAR* ErrorText);
	unsigned char*		m_szWriteBuffer;
	DWORD				m_nWriteBufferSize;
	unsigned char*      m_szReadBuffer;
	WORD				m_nReadBufferSize;


protected:
	//ReadFunction   pFunction;
	
	static UINT CommThread(LPVOID pParam);

	// thread
	CWinThread*			m_Thread;
	CRITICAL_SECTION 	m_Section; 

	HANDLE				m_hShutdownEvent; 
	HANDLE				m_hComm;
	CWnd*				m_pOwner;	
	CModBusServer*		m_pModbus;	

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	HANDLE				m_hEventArray[2];
	// owner window
	DWORD				m_CommEvents;
	COMMTIMEOUTS		m_CommTimeouts;	
	OVERLAPPED			m_ov;
	COMSTAT				m_comstat;

};

#endif // !defined(AFX_COMPORT_H__46C77595_F1FD_480C_BD5D_502369F4A891__INCLUDED_)
