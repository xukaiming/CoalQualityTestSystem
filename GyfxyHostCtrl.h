#ifndef _GYFXY_HOSTCTRL_H_
#define _GYFXY_HOSTCTRL_H_
#pragma once
#include "HostCtrl.h"
#include "Gyfxy1RDB.h"
// CGyfxyHostCtrl
 
#define COMMAND_USER_DEFINE COMMAND_MTR_POS+1
#define COMMAND_GETQUALITY  COMMAND_MTR_POS+2
#define COMMAND_SETQUALITY	COMMAND_MTR_POS+3
#define COMMAND_BALANCE		COMMAND_MTR_POS+4

#define WM_SAMPLE_QUALITY_UPDATE		WM_CALC_DATA+1

#define  COIL_BALANCE_MOTOR_UP	0		//��������
#define  COIL_BALANCE_MOTOR_DN	1		//�����½�
#define  COIL_LEFT_MOTOR_UP		2		//��¯����
#define  COIL_LEFT_MOTOR_DN		3		//��¯�½�

#define  COIL_RIGHT_MOTOR_UP	4		//��¯����
#define  COIL_RIGHT_MOTOR_DN	5		//��¯�½�	



#define  COIL_LEFT_OXYGEN		8		//��������
#define  COIL_RIGHT_NITROGEN	9		//��������

#define  COIL_WATER_STOVE_FAN	10		//ˮ��¯�ӽ�������
#define  COIL_LEFT_STOVE_FAN	11		//��¯����
#define  COIL_RIGHT_STOVE_FAN	12		//��¯����

#define  COIL_SPK				13		//������


class CGyfxyHostCtrl : public CHostCtrl
{ 

public:
	CGyfxyHostCtrl();
	virtual ~CGyfxyHostCtrl();

protected: 
public:
	CGyfxyRDB_BaseRdb	* pRdb;
	CGyfxyImageSlave	* pImageSlave;
	TCHAR str[MAX_PATH];
private:
	virtual void StateProc() ;
	virtual int CommandCode(UCHAR  buf[], UCHAR cCommand,UCHAR cCommandParam1, 
							UCHAR cCommandParam2, UCHAR cCommandParam3,void* pCommandParam);
public:
	enum CMD{CMD_NAK=0,            //������
		CMD_SETID,	             //����ID
		CMD_SETPW,	         //��������
		CMD_LOGON,	          //��¼
		CMD_SETDATETIME,       //����ʱ��
		CMD_READDATETIME,     //��ȡʱ��
		CMD_RESET_INST,       //��λ�豸 
		CMD_MOVEMOTOR,			//�ƶ��������
		CMD_MOVEMOTORSAMPLENO, 
		CMD_WARMUP,
		CMD_TARE,
		CMD_WEIGHT,
		CMD_START_TEST,		//��������
		CMD_STOP_TEST 			//ֹͣ����
		};
	enum WEIGHT_TYPE{T_POT=0,T_SAMPLE,T_WATER,T_COVER,T_VOL,T_ASH};
	enum METAGE_SPEED{T_NORMAL=0,T_QUICK};
	
	virtual void SetRDB(CRDB * pRdb);
	void ProcCustomCmd(
		UCHAR cCommandParam1,
		UCHAR cCommandParam2, 
		UCHAR cCommandParam3,
		void* pCommandParam);
private:
	void UploadQuality(void);
public:
	int GetQuality(UCHAR  buf[],int SampleNO);
	int SetQuality(UCHAR  buf[],int SampleNO);
	void UploadAllSample(void);
	void DownloadAllSample(void);
};
#endif

