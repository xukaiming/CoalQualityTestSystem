// CLYHostCtrl.h: interface for the CCLYHostCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLYHOSTCTRL_H__8AC1B9A7_69D5_468A_89CC_1AE709610EE7__INCLUDED_)
#define AFX_CLYHOSTCTRL_H__8AC1B9A7_69D5_468A_89CC_1AE709610EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HostCtrl.h"
#include "ClyRDB.h"


#define  CMD_DC_SAMPLE_DIR			0  	//��Ʒת�����з���
#define  CMD_DC_SAMPLE_RUN			1 	//��Ʒת������
#define  CMD_DJ						2 	//���̵���
#define  CMD_DC_SPEAKER 			3  	//���� 
#define  CMD_DC_SAMPLE_POS_DIR		4  	//���������з���
#define  CMD_DC_SAMPLE_POS_RUN		5  	//����������
#define  CMD_DC_JB					6  	//����	AC	
#define  CMD_DC_PUMB				7 	//����


class CCLYHostCtrl : public CHostCtrl  
{
public:
	CCLYHostCtrl();
	virtual ~CCLYHostCtrl();
public:
	double ADtoVol(long ADVAlue);
	CClyRDB				*pRdb;
	CClyImageSlave		*pImageSlave;
public:
	BOOL StopCommThread();
private:
	virtual void StateProc() ;
	virtual void SetRDB(CRDB *pRdb); 
	virtual int CommandCode(UCHAR buf[], UCHAR cCommand, UCHAR cCommandParam1,UCHAR cCommandParam2=0,UCHAR cCommandParam3=0,void* pCommandParam=NULL)   ;
	enum{TEST_NONE=0,TEST_INIT,TEST_PREDECOMP1,TEST_PREDECOMP2,TEST_DECOMP,TEST_END,TEST_POS_MOVE};
	enum CMD{
			CMD_NAK=0,            //������
            CMD_SETID,            //����ID
            CMD_SETPW,            //��������
            CMD_LOGON,            //��¼
            CMD_SETDATETIME,      //����ʱ��
            CMD_READDATETIME,     //��ȡʱ��
            CMD_RESET_INST,       //��λ�豸
            CMD_MOVEMOTORSAMPLE,  //�ƶ�����           param1 ����λ0 �м�λ1 �м�λ2 ����λ3
            CMD_MOVEMOTORSAMPLENO,//�ƶ���Ʒת��       PARAM  Ŀ����λ
            CMD_START_MEASURE,    //��������
            CMD_STOP_MEASURE,     //ֹͣ����
            CMD_ELE,              //����100mA���       param1 1 0
            CMD_WARMUP,           //�¶�����            param1 1 0 
	} ;
public:
	int DecryptDevInfo(void);
	void EncryptDevInfo(byte *pSecuriteArrary,Encpypt *pEncrypt);
};

#endif // !defined(AFX_CLYHOSTCTRL_H__8AC1B9A7_69D5_468A_89CC_1AE709610EE7__INCLUDED_)
