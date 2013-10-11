// LryHostCtrl.h: interface for the CLryHostCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LRYHOSTCTRL_H__ABD44FF8_B624_4953_8077_B7CF08F8E080__INCLUDED_)
#define AFX_LRYHOSTCTRL_H__ABD44FF8_B624_4953_8077_B7CF08F8E080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LRYRDB.H"	// Added by ClassView
#include "ModBusServer.h"
#include "HostCtrl.h"
//#include "RealTimeDb.h"
#include "LryImageSlave.H"
#define WM_START_PROGBAR	WM_USER+200
#define WM_IMAGE_DATA_CHANGE WM_USER+203
#define WM_CALC_DATA		 WM_USER+204

/*
#define COMMAND_RRT		0x01		//报告IED型号
#define COMMAND_SID		0x02		//设置IED ID
#define COMMAND_SPS		0x03		//设置注册码
#define COMMAND_LOG		0x04		//验证注册码
#define COMMAND_SST		0x05		//设置时间
#define COMMAND_RST		0x06		//读时间
#define COMMAND_RES		0x07		//复位IED
#define COMMAND_DLD		0x08		//下载数据

#define	COMMAND_RDI		0x09		//数据报告
#define COMMAND_JDQ		0x0A		//继电器控制
#define COMMAND_CQD		0x0B		//启动试验
#define COMMAND_CTZ		0x0C		//停止试验
#define COMMAND_RDF		0x0D		//报告试验结果数据


#define SCH_CMD_DISCURVE	1		//显示温升曲线
#define SCH_CMD_ENTEST		2		//使能测试按扭
#define SCH_CMD_DISTEST		3		//关闭测试按扭
#define SCH_CMD_CLOSESYS	4		//关闭系统
#define SCH_CMD_LOGOK		5		//注册成功
*/

#define COMMAND_SETID		0x0E		//设置ID
#define  CMD_INNERJIAOBAN2   0  	//内桶搅拌	DC
#define  CMD_HEATCTRL        1 		//加热控制
#define  CMD_COOLCTRL		 2 		//制冷控制
#define  CMD_FILLWATER 		 3  	//进水 
#define  CMD_OUTWATER  		 4  	//放水
#define  CMD_OUTERJIAOBAN    5  	//外筒搅拌
#define  CMD_INNERJIAOBAN1   6  	//内桶搅拌	AC	
#define  CMD_FIRE			 7 		//点火  
//#define LRY_TYPE "TF_LRY-2011"


class CLryHostCtrl : public CHostCtrl
{
enum LRYSTATE{	
		STATE_NULL = 0,
		STATE_TEST_START,		//开始试验
		STATE_JINSHUI,			//进水
		STATE_STABLISH,			//稳定
		STATE_START	,			//初期

		STATE_DIANHUO,			//点火	
		
		STATE_DIANHUO_OK,		//点火成功
		STATE_DIANHUO_BAD,		//点火失败	
		STATE_MAIN ,		   	//主期
											 
		STATE_TEST_END,			//试验末期
		STATE_TEST_REPORT,		//数据报告
		STATE_TEST_STOP	,		//试验结束  
		STATE_OUT_WATER
		
		};
public:
	void SaveResultToDatabase();

	virtual void CLryHostCtrl::SetRDB(CRDB *pRDB);
	inline UINT GetID(){return pRdb->m_bID; };
	SLryImageSlave sLryImageSlave;
	LRYRDB *pRdb; 
	CLryHostCtrl();
	virtual ~CLryHostCtrl();
public: 
	DATA_E m_DataE;
	DATA_Q m_DataQ;
private:   
	void StateProc();

	virtual DWORD WaitT35(); 
	virtual int	CommandCode(UCHAR buf[], UCHAR cCommand, 
						UCHAR cCommandParam1=0,UCHAR cCommandParam2=0,
						UCHAR cCommandParam3=0,void *m_pCommandParam=NULL) ;  
	BOOL bReadResult ;
};

#endif // !defined(AFX_LRYHOSTCTRL_H__ABD44FF8_B624_4953_8077_B7CF08F8E080__INCLUDED_)
