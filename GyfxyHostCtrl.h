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

#define  COIL_BALANCE_MOTOR_UP	0		//称样上升
#define  COIL_BALANCE_MOTOR_DN	1		//称样下降
#define  COIL_LEFT_MOTOR_UP		2		//左炉上升
#define  COIL_LEFT_MOTOR_DN		3		//左炉下降

#define  COIL_RIGHT_MOTOR_UP	4		//右炉上升
#define  COIL_RIGHT_MOTOR_DN	5		//右炉下降	



#define  COIL_LEFT_OXYGEN		8		//氧气开关
#define  COIL_RIGHT_NITROGEN	9		//氮气开关

#define  COIL_WATER_STOVE_FAN	10		//水分炉子交流风扇
#define  COIL_LEFT_STOVE_FAN	11		//左炉风扇
#define  COIL_RIGHT_STOVE_FAN	12		//右炉风扇

#define  COIL_SPK				13		//蜂鸣器


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
	enum CMD{CMD_NAK=0,            //无命令
		CMD_SETID,	             //设置ID
		CMD_SETPW,	         //设置密码
		CMD_LOGON,	          //登录
		CMD_SETDATETIME,       //设置时间
		CMD_READDATETIME,     //读取时间
		CMD_RESET_INST,       //复位设备 
		CMD_MOVEMOTOR,			//移动送样电机
		CMD_MOVEMOTORSAMPLENO, 
		CMD_WARMUP,
		CMD_TARE,
		CMD_WEIGHT,
		CMD_START_TEST,		//启动测试
		CMD_STOP_TEST 			//停止测试
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

