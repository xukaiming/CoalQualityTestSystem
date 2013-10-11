#pragma once 
#include "tea.h" 

#define KEY_LEN 16

//#define assert ASSERT
class Encpypt
{ 
	unsigned char Key[KEY_LEN];
public:
	struct SecruitInfo
	{ 
		char  uID[8];
		char  cCompany[64];  //公司名称
		long  lStartDate;
		long  lEndDate;
		long  lCurrentDate;
		long  reserve;
		char  cReserveInfo[128];     
		char  CRC[4];//CRC校验值 
	};
	SecruitInfo *pInfo; 
	TEA *pTea;//(key, 16, false);
		//对方UID散列
		//自定义消息
			//包含用户单位信息 起始时间,结束时间,最后一次使用时间
	//
	//调用解密功能 
	//查看校验值是否通过,如果校验值不通过,不弹出对话框
	//查看用户单位是否匹配,查看使用日期是否到期//若不匹配或者到期,弹出对话框,输入加密的文本序列
	//根据文本序列,先看校验是否通过,然后下载到下位机
		
public: 
	void encrypt(const byte *in, byte *out,int in_len) 
	{
		ASSERT((in_len%sizeof(long))==0);
		ulong *pIn	= (ulong *)in;
		ulong *pout = (ulong *)out;
		in_len/=sizeof(ulong);
		ASSERT((in_len%2)==0);   //一次加密两个long
		in_len/=2;
		for (int i=0;i<in_len;i++)
		{
			pTea->encrypt(pIn,pout);
			pIn++;pout++;pIn++;pout++;
			
		}
	}
	void decrypt(const byte *in, byte *out,int in_len) 
	{
		ASSERT((in_len%sizeof(long))==0);
		ulong *pIn	= (ulong *)in;
		ulong *pout = (ulong *)out;
		in_len/=sizeof(ulong);
		in_len/=2;
		 
		for (int i=0;i<in_len;i++)
		{ 
			pTea->decrypt(pIn,pout);
			pIn++;pout++;pIn++;pout++;
		}
	} 
	Encpypt(void);
	~Encpypt(void);
};
