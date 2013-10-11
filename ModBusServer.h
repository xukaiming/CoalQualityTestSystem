// ModBusServer.h: interface for the CModBusServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODBUSSERVER_H__0D9F3BDC_8B41_428B_89B6_1AB21E03ABD7__INCLUDED_)
#define AFX_MODBUSSERVER_H__0D9F3BDC_8B41_428B_89B6_1AB21E03ABD7__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComPort.h"

#define READ_COIL_STATUS		01
#define READ_INPUT_STATUS		02 

#define READ_HOLDING_REGISTER	03 
#define READ_INPUT_REGISTER		04 

#define WRITE_SINGLE_COIL		05
#define WRITE_SINGLE_REGISTER	06 

#define WRITE_MULTIPLE_COIL		15 
#define WRITE_MULTIPLE_REGISTER 16  


#define WRITE_REG_INFO			85 
#define READ_REG_INFO			86  
 
extern const int	T15	;
extern const int	T35	;
 
#define CRC_ERROR -1
#define COMMAND_TYPE_ERROR -2
#define LENGTH_ERROR	-3

#define BIG_ENDIAN   //大小端模式    51为大端模式
//#undef  BIG_ENDIAN

 
class CModBusServer  
{
 
public: 
	int DecodeWriteSingleCoil (const unsigned char * const pBuf,unsigned short length, unsigned char * pData);
	void LongSwapEndian(long * pBuf,unsigned int Length);
	void StrSwapEndian(char * pBuf,unsigned int Length); 
	int DecodeReadSTATUS(const unsigned char *pBuf,unsigned short length, unsigned char *const pCoilMem);
	int DecodeReadREGISTER(const unsigned char *pBuf,unsigned short length, unsigned short *const pCoilMem); 
	int DecodeWriteMultipleCoil(unsigned char *pBuf, unsigned short length );
	int DecodeWriteMultipleRegister(unsigned char *pBuf,unsigned short length );
	//写单寄存器,单继电器
	//参数:起始地址
	void FunctionWriteSingle(unsigned char addr,unsigned char CmdType,unsigned short tempAddr,unsigned short data);

	//参数 .子站ID ,命令类型,内存地址,读取个数
	//01 读线圈状态  02 读输入位状态03 读保持寄存器 04 读输入寄存器
	void FunctionRead(unsigned char SlaveAddr, unsigned char CmdType, unsigned short MemAddr,unsigned int count); 
	//写多个寄存器
	//参数 子站ID,内存地址,写入长度(short 类型),需要修改的寄存器缓冲区(unsigned short)首地址指针
	void FunctionWriteMultipleRegister(unsigned char addr,  unsigned short tempAddr,unsigned short length,const unsigned short * data);
	//写多个继电器线圈
	//参数 子站ID,内存地址,写入长度(char 类型),需要修改的寄存器缓冲区(unsigned char )首地址指针,一个char对应一个继电器
	void FunctionWriteMultipleCoil( unsigned char addr,  unsigned short tempAddr,unsigned short length,  const unsigned char *  pdata);
	int Respond(unsigned char *pBuf);
	int DecodeWriteSingleRegister( unsigned char *const pBuf,unsigned short length, unsigned short *const pData);
	CComPort *m_pPort;
	int SendLength;
	HANDLE  MutexLock ;  //Mutex
	CModBusServer();
	virtual ~CModBusServer();


	void SetPort(CComPort *pPort);
private: 
	int  Read(unsigned char *pReadBuf,int length);
	int  Write(unsigned char *pReadBuf,int length);
	
	//unsigned char receBuf[MAX_BUFFER_SIZE];
	unsigned char sendBuf[MAX_BUFFER_SIZE];
	unsigned short int temp[64]; 
	//CRC 校验字查表
	static unsigned char   auchCRCHi[];         
	static unsigned char   auchCRCLo[];
	/* CRC 高位字节值表 */  
public:
	static unsigned short int CRC_16(unsigned char  *const puchMsg, unsigned int usDataLen);
 	//写入注册信息 
	void FunctionSetRegInfo(unsigned char SlaveAddr, unsigned short MemAddr , unsigned short length, unsigned short* pdata);
	virtual bool CRCCheck(const unsigned char* pBuf, unsigned short length );
};


#endif // !defined(AFX_MODBUSSERVER_H__0D9F3BDC_8B41_428B_89B6_1AB21E03ABD7__INCLUDED_)
