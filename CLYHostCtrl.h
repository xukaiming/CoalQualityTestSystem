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


#define  CMD_DC_SAMPLE_DIR			0  	//样品转盘运行方向
#define  CMD_DC_SAMPLE_RUN			1 	//样品转盘运行
#define  CMD_DJ						2 	//电解继电器
#define  CMD_DC_SPEAKER 			3  	//喇叭 
#define  CMD_DC_SAMPLE_POS_DIR		4  	//送样干运行方向
#define  CMD_DC_SAMPLE_POS_RUN		5  	//送样干运行
#define  CMD_DC_JB					6  	//搅拌	AC	
#define  CMD_DC_PUMB				7 	//气泵


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
			CMD_NAK=0,            //无命令
            CMD_SETID,            //设置ID
            CMD_SETPW,            //设置密码
            CMD_LOGON,            //登录
            CMD_SETDATETIME,      //设置时间
            CMD_READDATETIME,     //读取时间
            CMD_RESET_INST,       //复位设备
            CMD_MOVEMOTORSAMPLE,  //移动样车           param1 放样位0 中间位1 中间位2 高温位3
            CMD_MOVEMOTORSAMPLENO,//移动样品转盘       PARAM  目标样位
            CMD_START_MEASURE,    //启动测试
            CMD_STOP_MEASURE,     //停止测试
            CMD_ELE,              //启动100mA电解       param1 1 0
            CMD_WARMUP,           //温度升温            param1 1 0 
	} ;
public:
	int DecryptDevInfo(void);
	void EncryptDevInfo(byte *pSecuriteArrary,Encpypt *pEncrypt);
};

#endif // !defined(AFX_CLYHOSTCTRL_H__8AC1B9A7_69D5_468A_89CC_1AE709610EE7__INCLUDED_)
