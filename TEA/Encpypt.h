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
		char  cCompany[64];  //��˾����
		long  lStartDate;
		long  lEndDate;
		long  lCurrentDate;
		long  reserve;
		char  cReserveInfo[128];     
		char  CRC[4];//CRCУ��ֵ 
	};
	SecruitInfo *pInfo; 
	TEA *pTea;//(key, 16, false);
		//�Է�UIDɢ��
		//�Զ�����Ϣ
			//�����û���λ��Ϣ ��ʼʱ��,����ʱ��,���һ��ʹ��ʱ��
	//
	//���ý��ܹ��� 
	//�鿴У��ֵ�Ƿ�ͨ��,���У��ֵ��ͨ��,�������Ի���
	//�鿴�û���λ�Ƿ�ƥ��,�鿴ʹ�������Ƿ���//����ƥ����ߵ���,�����Ի���,������ܵ��ı�����
	//�����ı�����,�ȿ�У���Ƿ�ͨ��,Ȼ�����ص���λ��
		
public: 
	void encrypt(const byte *in, byte *out,int in_len) 
	{
		ASSERT((in_len%sizeof(long))==0);
		ulong *pIn	= (ulong *)in;
		ulong *pout = (ulong *)out;
		in_len/=sizeof(ulong);
		ASSERT((in_len%2)==0);   //һ�μ�������long
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
