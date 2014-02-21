// ModBusServer.cpp: implementation of the CModBusServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "ModBusServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
const int	T15	= 20;
const int	T35	= 100;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
unsigned char  CModBusServer:: auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
//CRC低位字节值表  
unsigned char CModBusServer:: auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ; 

CModBusServer::CModBusServer()
{ 
	m_pPort = NULL; 
	MutexLock =  
	CreateMutex (
                NULL,                       // No security descriptor
                FALSE,                      // Mutex object not owned
                NULL); // Object name


}

CModBusServer::~CModBusServer()
{   
	CloseHandle(MutexLock);
}

int CModBusServer::Read(unsigned char *pReadBuf, int length)
{
	if(m_pPort!=NULL) 
		m_pPort->Read(pReadBuf,length);
	return TRUE;
}

int CModBusServer:: Write(unsigned char *pReadBuf,int length)
{
	SendLength = length;
	if(m_pPort!=NULL) 
		//return	m_pPort->Write(pReadBuf,length);  
		return	m_pPort->WriteDirect(pReadBuf,length);  
	else
		return 0;
}

void CModBusServer::SetPort(CComPort *pPort)
{
	m_pPort = pPort;
	pPort->m_pModbus = this; 
}

unsigned short int CModBusServer::CRC_16(unsigned char * puchMsg, unsigned int usDataLen)
{
	unsigned char  uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
	unsigned char  uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */
	DWORD  uIndex ;					 /* CRC循环中的索引 */
	ASSERT(usDataLen<MAX_BUFFER_SIZE-2);
	while (usDataLen--)				 /* 传输消息缓冲区 */
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}

//读 状态

//#define READ_COIL_STATUS		01
//#define READ_INPUT_STATUS		02 
//#define READ_HOLDING_REGISTER	03 
//#define READ_INPUT_REGISTER	04 

void CModBusServer::FunctionRead(unsigned char SlaveAddr,unsigned char CmdType,unsigned short MemAddr,unsigned int count)
{  
	ASSERT((CmdType==READ_COIL_STATUS)
		||(CmdType==READ_INPUT_STATUS)
		||(CmdType==READ_HOLDING_REGISTER)
		||(CmdType==READ_INPUT_REGISTER)
		||(CmdType==READ_REG_INFO)
		);
	ASSERT(count<0x7F);		//一次性读取的缓冲区长度不得超过254个字节
	unsigned short crcData; 
	unsigned char  i=0,k=0;    
	ZeroMemory(sendBuf,sizeof(sendBuf)/sizeof(char)); 
	//
	sendBuf[i++] = SlaveAddr;       //写地址         
	sendBuf[i++] = CmdType;			//命令ID	
	sendBuf[i++] = MemAddr>>8;		//起始地址高位
	sendBuf[i++] = MemAddr&0xFF;	//起始地址地位
	sendBuf[i++] = count>>8;
	sendBuf[i++] = count&0xff;
	crcData		 = CRC_16(sendBuf,i);
	sendBuf[i++] = crcData >> 8;
	sendBuf[i++] = crcData &0xff;
 
	Write(sendBuf,i); 
}

//写单个节点  单个线圈,寄存器
void CModBusServer::FunctionWriteSingle(unsigned char SlaveAddr, unsigned char CmdType, unsigned short MemAddr,unsigned short data)  
{
	ASSERT((CmdType==WRITE_SINGLE_COIL)||(CmdType==WRITE_SINGLE_REGISTER));
	unsigned short crcData; 
	unsigned char  i=0,k=0;    
	ZeroMemory(sendBuf,sizeof(sendBuf)/sizeof(char)); 
	//
	
	sendBuf[i++] = SlaveAddr;       //写地址         
	sendBuf[i++] = CmdType;		 //命令ID	
	sendBuf[i++] = MemAddr>>8;    //起始地址高位
	sendBuf[i++] = MemAddr&0xFF;  //其实位置地位
	if(CmdType==WRITE_SINGLE_REGISTER)    
	{
		#ifdef BIG_ENDIAN
			sendBuf[i++] = data>>8;         
			sendBuf[i++] = data&0xff;
		#else
			sendBuf[i++] = data&0xff;
			sendBuf[i++] = data>>8;    
		#endif
	}
	else
	{
		sendBuf[i++] = (data!=0)?0xFF:0x00;         
		sendBuf[i++] = 00;
	}
	crcData		 = CRC_16(sendBuf,i);
	sendBuf[i++] = crcData >> 8;
	sendBuf[i++] = crcData &0xff;
 
	Write(sendBuf,i); 
}

void CModBusServer::FunctionWriteMultipleRegister(unsigned char SlaveAddr,  unsigned short MemAddr,unsigned short length,const  unsigned short * pdata)
{ 
	unsigned short crcData=0; 
	unsigned char  i=0,k=0;    
	ZeroMemory(sendBuf,sizeof(sendBuf)/sizeof(char)); 
	//
	
	sendBuf[i++] = SlaveAddr;					//写地址         
	sendBuf[i++] = WRITE_MULTIPLE_REGISTER;		 //命令ID	
	sendBuf[i++] = MemAddr>>8;					//起始地址高位
	sendBuf[i++] = MemAddr&0xFF;				//其实位置地位
 
	//多个寄存器
	sendBuf[i++] = length>>8;
	sendBuf[i++] = length&0xFF;	
	//数据段长度
	sendBuf[i++] =	length<<1; 
	for(k=0;k<length;k++)
	{
		#ifdef BIG_ENDIAN
		sendBuf[i++] = pdata[k]>>8;
		sendBuf[i++] = pdata[k]&0xff;
		#else
		sendBuf[i++] = pdata[k]&0xff;
		sendBuf[i++] = pdata[k]>>8;
		#endif
	}

	crcData		 = CRC_16(sendBuf,i);
	sendBuf[i++] = crcData >> 8;
	sendBuf[i++] = crcData &0xff;
 
	Write(sendBuf,i); 
}
 
void  CModBusServer::FunctionWriteMultipleCoil( unsigned char SlaveAddr,  unsigned short MemAddr,unsigned short length,  const unsigned char *  pdata)
{ 
	unsigned short crcData = 0;
	unsigned char  cCoil   = 0; 
	unsigned char  byteLen = 0;
	unsigned char  k=0,j=0,i=0;    
	ZeroMemory(sendBuf,sizeof(sendBuf)/sizeof(char)); 
	//
	
	sendBuf[i++] = SlaveAddr;					//写地址         
	sendBuf[i++] = WRITE_MULTIPLE_REGISTER;		//命令ID	
	sendBuf[i++] = MemAddr>>8;					//起始地址高位
	sendBuf[i++] = MemAddr&0xFF;				//其实位置地位
 
	//多个寄存器
	sendBuf[i++] = length>>8;
	sendBuf[i++] = length&0xFF;	
	//数据段长度
	sendBuf[i++] = byteLen = length%8==0?length/8:length/8+1;
	for(k=0;k<byteLen;k++)
	{
		/*                       //这里为读取任意位
		cCoil = 0;
		for(j=0;j<8;j++)
		{
			cCoil |=	(*(pdata++)>0)?1:0;
			cCoil>>=1;
			length--;
			if(length==0)
				break;
		}
		
		sendBuf[i++]=cCoil;	
		*/
		sendBuf[i++]= *pdata;   //直接读取8bit
	} 
	crcData		 = CRC_16(sendBuf,i);
	sendBuf[i++] = crcData >> 8;
	sendBuf[i++] = crcData &0xff;
 
	Write(sendBuf,i); 
} 
  

//int DecodeWriteSingleCoil (const unsigned char * const pBuf,unsigned short length, unsigned char * const pData)
int CModBusServer::DecodeWriteSingleRegister (unsigned char * const pBuf,unsigned short length, unsigned short * const pData)
{ 
	/*
	unsigned short crcData = CRC_16(pBuf,length-2);
	if(crcData!=(pBuf[length-2]<<8|pBuf[length-1]))
		return CRC_ERROR;		//CRC错误
		*/
	if (!CRCCheck(pBuf,length))
	{
		TRACE(_T("MODBUS %d CRC Error!\n"),m_pPort->m_nPortNr);
		return CRC_ERROR;
	}
	if( pBuf[1]==WRITE_SINGLE_REGISTER )
	{
		#ifdef  BIG_ENDIAN
			pData[pBuf[2]<<8|pBuf[3]] = pBuf[4]<<8|pBuf[5];
		#else
			pData[pBuf[2]<<8|pBuf[3]] = pBuf[5]<<8|pBuf[4];
		#endif
		return length;
	}
	else
		return COMMAND_TYPE_ERROR;		//命令类型错误 
}
 

int CModBusServer::DecodeWriteMultipleCoil(unsigned char *pBuf,unsigned short length )
{ 
	/*
	unsigned short crcData = CRC_16(pBuf,length-2);
	if(crcData!=(pBuf[length-2]<<8|pBuf[length-1]))
		return CRC_ERROR;		//CRC错误
		*/
	if (!CRCCheck(pBuf,length))
	{
		TRACE(_T("MODBUS %d CRC Error!\n"),m_pPort->m_nPortNr);
		return CRC_ERROR;
	}
	if(pBuf[1]!=WRITE_MULTIPLE_COIL)
		return -2;      //命令类型错误 
	return pBuf[4]<<8|pBuf[5];       //写继电器个数
}

int CModBusServer::DecodeWriteMultipleRegister(unsigned char *pBuf,unsigned short length)
{ 
	/*
	unsigned short crcData = CRC_16(pBuf,length-2);
	if(crcData!=(pBuf[length-2]<<8|pBuf[length-1]))
		return CRC_ERROR;		//CRC错误
		*/
	if (!CRCCheck(pBuf,length))
	{
		TRACE(_T("MODBUS %d CRC Error!\n"),m_pPort->m_nPortNr);
		return CRC_ERROR;
	}
	if(pBuf[1]!=WRITE_MULTIPLE_REGISTER)
		return COMMAND_TYPE_ERROR;				//命令类型错误 
	return pBuf[4]<<8|pBuf[5];       //写寄存器个数
}

int CModBusServer::DecodeReadSTATUS(const unsigned char *pBuf,unsigned short length, unsigned char *const pCoilMem)
{ 
	short Length = 0;
	short i = 0;
	/*
	unsigned short crcData = CRC_16((unsigned char *)pBuf,length-2);
	if(crcData!=(pBuf[length-2]<<8|pBuf[length-1]))
		return CRC_ERROR;		//CRC错误 
		*/
	if (!CRCCheck(pBuf,length))
	{
		TRACE(_T("MODBUS %d CRC Error!\n"),m_pPort->m_nPortNr);
		return CRC_ERROR;
	}
	if((pBuf[1]!=READ_COIL_STATUS)
		&&(pBuf[1]!=READ_INPUT_STATUS))
		return COMMAND_TYPE_ERROR;				//命令类型错误 
	for(i=0;i<pBuf[2];i++)
	{
		pCoilMem[i]=pBuf[3+i];
	}
	return pBuf[2]*8;         //读取继电器个数
}

bool CModBusServer::CRCCheck(const unsigned char* pBuf, unsigned short length)
{
	unsigned short crcData	= CRC_16((unsigned char *)pBuf,length-2);  
	unsigned short crcValue = pBuf[length-2]<<8|pBuf[length-1];  
	if(crcData!=crcValue)
		return false;
	else
		return true;
}

int CModBusServer::DecodeReadREGISTER(const unsigned char *pBuf,unsigned short length, unsigned short *const pCoilMem)
{   
	unsigned short i = 0; 
	if (!CRCCheck(pBuf,length))
	{
		TRACE(_T("MODBUS %d CRC Error!\n"),m_pPort->m_nPortNr);
		return CRC_ERROR;
	}
	if((pBuf[1]!=READ_HOLDING_REGISTER)
		&&(pBuf[1]!=READ_INPUT_REGISTER))
	{
		return COMMAND_TYPE_ERROR;				//命令类型错误 
	}
	for(i=0;i<pBuf[2]/2;i++)
	{
		#ifdef BIG_ENDIAN
			pCoilMem[i]=(pBuf[3+2*i]<<8)+pBuf[4+2*i];
		#else
			pCoilMem[i]=pBuf[3+2*i]+(pBuf[4+2*i]<<8);
		#endif
	} 
	return pBuf[2]/2;         //读取寄存器个数
}

int CModBusServer::Respond(unsigned char *pBuf)
{
	int iAllReceiveCnt = 0;
	int iReceiveCnt = 0;   
	while((iReceiveCnt=m_pPort->ReadAll(pBuf+iAllReceiveCnt))!=0)
	{
		Sleep(T15);
		iAllReceiveCnt+=iReceiveCnt;
		if (iReceiveCnt>MAX_BUFFER_SIZE)
		{
			iReceiveCnt = 0;
		}
	}
	if(iAllReceiveCnt<6)        //少于6个的让CRC校验出错
	{ 
		TRACE(_T("COM%d返回数据长度为%d\n"),m_pPort->m_nPortNr,iAllReceiveCnt);
		iAllReceiveCnt = 0;
	}
	return iAllReceiveCnt;
}


 

void CModBusServer::StrSwapEndian(char *pBuf, unsigned int Length)
{
	ASSERT(Length%2==0);
	short *ptr = (short *)pBuf;
	Length>>=1;
	for(unsigned int i=0;i<Length;i++)
	{
		*ptr = BigLittleSwap16(*ptr);
		ptr++;
	}
}

void CModBusServer::LongSwapEndian(long *pBuf, unsigned int Length)
{ 
	for(unsigned int i=0;i<Length;i++)
	{
		*pBuf = BigLittleSwap32(*pBuf);
		pBuf++;
	}
}

int CModBusServer::DecodeWriteSingleCoil(const unsigned char *const pBuf, unsigned short length, unsigned char *pData)
{   
	unsigned short crcData = CRC_16(sendBuf,length-2); 
	unsigned short addrHi	=(pBuf[2]<<8|pBuf[3])/8;
	unsigned short addrLow	= pBuf[3]%8;
	if(crcData!=(pBuf[length-2]<<8|pBuf[length-1]))
		return CRC_ERROR;		//CRC错误
	if( pBuf[1]==WRITE_SINGLE_COIL )
	{  
		pData+=addrHi;
		if(pBuf[4]<<8|pBuf[5])
		{
			*pData|=1<<addrLow;
		}
		else
		{ 
			*pData&=~(1<<addrLow);
		}
		return length;
	}
	else
		return COMMAND_TYPE_ERROR;		//命令类型错误 
}

 
void CModBusServer::FunctionSetRegInfo(unsigned char SlaveAddr, unsigned short MemAddr , unsigned short length, unsigned short* pdata)
{
	unsigned short crcData=0; 
	unsigned char  i=0,k=0;    
	ZeroMemory(sendBuf,sizeof(sendBuf)/sizeof(char)); 
	//

	sendBuf[i++] = SlaveAddr;					//写地址         
	sendBuf[i++] = WRITE_REG_INFO;		 //命令ID	
	sendBuf[i++] = MemAddr>>8;					//起始地址高位
	sendBuf[i++] = MemAddr&0xFF;				//其实位置地位

	//多个寄存器
	sendBuf[i++] = length>>8;
	sendBuf[i++] = length&0xFF;	
	//数据段长度
	sendBuf[i++] =	length<<1; 
	for(k=0;k<length;k++)
	{
#ifdef BIG_ENDIAN
		sendBuf[i++] = pdata[k]>>8;
		sendBuf[i++] = pdata[k]&0xff;
#else
		sendBuf[i++] = pdata[k]&0xff;
		sendBuf[i++] = pdata[k]>>8;
#endif
	}

	crcData		 = CRC_16(sendBuf,i);
	sendBuf[i++] = crcData >> 8;
	sendBuf[i++] = crcData &0xff;

	Write(sendBuf,i); 
}

