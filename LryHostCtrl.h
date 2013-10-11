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
#define COMMAND_RRT		0x01		//����IED�ͺ�
#define COMMAND_SID		0x02		//����IED ID
#define COMMAND_SPS		0x03		//����ע����
#define COMMAND_LOG		0x04		//��֤ע����
#define COMMAND_SST		0x05		//����ʱ��
#define COMMAND_RST		0x06		//��ʱ��
#define COMMAND_RES		0x07		//��λIED
#define COMMAND_DLD		0x08		//��������

#define	COMMAND_RDI		0x09		//���ݱ���
#define COMMAND_JDQ		0x0A		//�̵�������
#define COMMAND_CQD		0x0B		//��������
#define COMMAND_CTZ		0x0C		//ֹͣ����
#define COMMAND_RDF		0x0D		//��������������


#define SCH_CMD_DISCURVE	1		//��ʾ��������
#define SCH_CMD_ENTEST		2		//ʹ�ܲ��԰�Ť
#define SCH_CMD_DISTEST		3		//�رղ��԰�Ť
#define SCH_CMD_CLOSESYS	4		//�ر�ϵͳ
#define SCH_CMD_LOGOK		5		//ע��ɹ�
*/

#define COMMAND_SETID		0x0E		//����ID
#define  CMD_INNERJIAOBAN2   0  	//��Ͱ����	DC
#define  CMD_HEATCTRL        1 		//���ȿ���
#define  CMD_COOLCTRL		 2 		//�������
#define  CMD_FILLWATER 		 3  	//��ˮ 
#define  CMD_OUTWATER  		 4  	//��ˮ
#define  CMD_OUTERJIAOBAN    5  	//��Ͳ����
#define  CMD_INNERJIAOBAN1   6  	//��Ͱ����	AC	
#define  CMD_FIRE			 7 		//���  
//#define LRY_TYPE "TF_LRY-2011"


class CLryHostCtrl : public CHostCtrl
{
enum LRYSTATE{	
		STATE_NULL = 0,
		STATE_TEST_START,		//��ʼ����
		STATE_JINSHUI,			//��ˮ
		STATE_STABLISH,			//�ȶ�
		STATE_START	,			//����

		STATE_DIANHUO,			//���	
		
		STATE_DIANHUO_OK,		//���ɹ�
		STATE_DIANHUO_BAD,		//���ʧ��	
		STATE_MAIN ,		   	//����
											 
		STATE_TEST_END,			//����ĩ��
		STATE_TEST_REPORT,		//���ݱ���
		STATE_TEST_STOP	,		//�������  
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
