// LryHostCtrl.cpp: implementation of the CLryHostCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "LryHostCtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "math.h"





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLryHostCtrl::CLryHostCtrl()
{ 
	bReadResult = FALSE;
}

CLryHostCtrl::~CLryHostCtrl()
{ 
}

void CLryHostCtrl::SetRDB(CRDB *pRDB)
{
	this->pRdb = (LRYRDB *)pRDB;  
	CHostCtrl::SetRDB(pRDB);
}
 
enum CMD{CMD_NONE =0,CMD_START_TEST,CMD_STOP_TEST,CMD_REPORT,CMD_SETNEWID} ;
//int CLryHostCtrl::CommandCode(UCHAR *buf, UCHAR cCommand, UCHAR cCode)
int	CLryHostCtrl::CommandCode(UCHAR buf[], UCHAR cCommand,
						UCHAR cCode,UCHAR cCommandParam2,
						UCHAR cCommandParam3,void *m_pCommandParam) 
{
	unsigned short  length=0;
	int iResult =0 ; 

	switch(cCommand)
	{
	case COMMAND_RRT:  
 		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,

			(unsigned short*)sLryImageSlave.UploadRegister.sType-                //��Ҫ��ȡ���ַ����׵�ַ
			(unsigned short*)&sLryImageSlave,									 //���ݽṹ�׵�ַ
			sizeof(sLryImageSlave.UploadRegister.sType)/sizeof(short)); 
		if((WaitT35()==WAIT_TIMEOUT)
			&&((length=pModbusMaster->Respond(buf))!=0))
		{ 
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)sLryImageSlave.UploadRegister.sType);
		 	if((iResult>0)&&CheckCPUBigEndian())
				pModbusMaster->StrSwapEndian(sLryImageSlave.UploadRegister.sType,sizeof(sLryImageSlave.UploadRegister.sType));
		 	if(iResult>0) 
				if(CString(sLryImageSlave.UploadRegister.sType)!=pRdb->m_szType)//�豸���Ͳ�����
				{
					//�رմ��� 
					PostM(_T("IED�ͺŲ���"),0,SCH_CMD_DISTEST);  
				}
				else
				{
					AddCommand(COMMAND_DLD,0);	//�ͺ�OK����������
					PostM(_T("�������ز���"),0,0);
				}

		}
		break;
	case COMMAND_DLD:	//���ز���
		pRdb->InitRDB(&sLryImageSlave);  ////��ʼ�����ؼ�¼��
		if( CheckCPUBigEndian())
			pModbusMaster->StrSwapEndian(sLryImageSlave.DownloadWorkParam.m_szAlgorithm,sizeof(sLryImageSlave.DownloadWorkParam.m_szAlgorithm));
		pModbusMaster->FunctionWriteMultipleRegister((unsigned char)pRdb->m_bID,0,sizeof(sLryImageSlave.DownloadWorkParam)/sizeof(SHORT), (unsigned short *)&sLryImageSlave.DownloadWorkParam );  
		  if((WaitT35()==WAIT_TIMEOUT)
			  &&((length=pModbusMaster->Respond(buf))!=0))
		  {
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
			if(iResult>0){
				PostM(_T("�ȴ�����..."),0,SCH_CMD_LOGOK);
				Beep(1000,50);
				AddCycCommand(0,0);
				AddCycCommand(COMMAND_RDI,0);	//��������OK��ѭ������
			}
		  }
		break;
	case COMMAND_RDI:			//���ݱ���		
 		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
			(unsigned short*)&sLryImageSlave.UploadRegister-                //��Ҫ��ȡ���ַ����׵�ַ
			(unsigned short*)&sLryImageSlave,									 //���ݽṹ�׵�ַ
			(sizeof(sLryImageSlave.UploadRegister)+
			sizeof(sLryImageSlave.InputStatus)+
			sizeof(sLryImageSlave.OutputCoil)+
			sizeof(sLryImageSlave.Report)-
			sizeof(sLryImageSlave.Report.lTi)) /sizeof(short) 
			//sizeof(sLryImageSlave.Report))̫��,������������,ֻ��һ��������
			//
			); 
		if((WaitT35()==WAIT_TIMEOUT) 
			&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&sLryImageSlave.UploadRegister);
			if((iResult>0)&&CheckCPUBigEndian())
			{
				pModbusMaster->StrSwapEndian(sLryImageSlave.UploadRegister.sType,sizeof(sLryImageSlave.UploadRegister.sType));
				pModbusMaster->LongSwapEndian(sLryImageSlave.UploadRegister.AD7714Value,sizeof(sLryImageSlave.UploadRegister.AD7714Value)/sizeof(long)+1);
				pModbusMaster->LongSwapEndian(&sLryImageSlave.UploadRegister.Cnt,sizeof(sLryImageSlave.UploadRegister.Cnt)/sizeof(long));
				pModbusMaster->LongSwapEndian((long *)&sLryImageSlave.Report,sizeof(sLryImageSlave.Report)/sizeof(long)); 
				//��ǰ�¶� 
				//����״̬�ó�
				StateProc();
			}
		}  
		break;
	case COMMAND_JDQ:		//�̵�������

		pModbusMaster->FunctionWriteSingle((unsigned char)pRdb->m_bID,
											WRITE_SINGLE_COIL,
											cCode&0x7F,
											cCode&0x80) ;
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeWriteSingleCoil(buf,length,&sLryImageSlave.OutputCoil.Output2.Coil8);
		} 
		break; 

	case	COMMAND_LOG:
		//AddCycCommand(COMMAND_RDI,0);		//��ѭ������
		//AddCommand(COMMAND_RDI,0);		//��ѭ������ 
		AddCommand(COMMAND_RRT,0); 
		iResult = 1;
		break;
	case   COMMAND_CQD	:		//��������
		//m_WorkCmd
		sLryImageSlave.DownloadWorkParam.m_WorkCmd = CMD_START_TEST;
		pModbusMaster->FunctionWriteSingle((unsigned char)pRdb->m_bID,WRITE_SINGLE_REGISTER,
			(short*)&sLryImageSlave.DownloadWorkParam.m_WorkCmd
			-(short*)&sLryImageSlave,
			sLryImageSlave.DownloadWorkParam.m_WorkCmd);
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteSingleRegister(
				buf,length,
				(unsigned short *)&sLryImageSlave.DownloadWorkParam.m_WorkCmd);
		}
		break;
	case COMMAND_CTZ:			//ֹͣ����
		sLryImageSlave.DownloadWorkParam.m_WorkCmd = CMD_STOP_TEST;
		pModbusMaster->FunctionWriteSingle((unsigned char)pRdb->m_bID,WRITE_SINGLE_REGISTER,
			(short*)&sLryImageSlave.DownloadWorkParam.m_WorkCmd
			-(short*)&sLryImageSlave,
			sLryImageSlave.DownloadWorkParam.m_WorkCmd);
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteSingleRegister(
				buf,length,
				(unsigned short *)&sLryImageSlave.DownloadWorkParam.m_WorkCmd);
		}
		break;
	case COMMAND_RDF:
 		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
			(unsigned short*)&sLryImageSlave.Report-                //��Ҫ��ȡ���ַ����׵�ַ
			(unsigned short*)&sLryImageSlave,						//���ݽṹ�׵�ַ
			sizeof(sLryImageSlave.Report)/sizeof(short)
			); 
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&sLryImageSlave.Report);
			if((iResult>0)&&CheckCPUBigEndian())
			{
				pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
					(unsigned short*)&sLryImageSlave.ReportT0Tn-                //��Ҫ��ȡ���ַ����׵�ַ
					(unsigned short*)&sLryImageSlave,						//���ݽṹ�׵�ַ
					sizeof(sLryImageSlave.ReportT0Tn)/sizeof(short)
					); 
				if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
					iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&sLryImageSlave.ReportT0Tn);
				if((iResult>0)&&CheckCPUBigEndian())
				{ 
					pModbusMaster->LongSwapEndian((long *)&sLryImageSlave.Report,sizeof(sLryImageSlave.Report)/sizeof(long));
					pModbusMaster->LongSwapEndian((long *)&sLryImageSlave.ReportT0Tn,sizeof(sLryImageSlave.ReportT0Tn)/sizeof(long));

					//��ǰ�¶� 
					//����״̬�ó� У��ֵ
					pRdb->RefreshRDBReportTab(&sLryImageSlave);  
					//������
					//���浽���ݿ�		
					m_pOwner->SendMessage(WM_CALC_DATA,0,0);
					bReadResult = TRUE;
					//AddCommand( COMMAND_CTZ,0);  
				}
			}

		}  
		break;
		//Report
	case COMMAND_SETID:
		sLryImageSlave.DownloadWorkParam.m_WorkCmd = CMD_SETNEWID; 
		sLryImageSlave.DownloadWorkParam.m_NewcID = cCode;
		sLryImageSlave.DownloadWorkParam.m_Reserver2 = 0;
		sLryImageSlave.DownloadWorkParam.m_Reserver3[0] = 0;
		sLryImageSlave.DownloadWorkParam.m_Reserver3[1] = 0;
		ZeroMemory(sLryImageSlave.DownloadWorkParam.m_szAlgorithm,
			sizeof(sLryImageSlave.DownloadWorkParam.m_szAlgorithm));
		pModbusMaster->FunctionWriteMultipleRegister(0xFF,
			(short*)&sLryImageSlave.DownloadWorkParam.m_WorkCmd
			-(short*)&sLryImageSlave,
			(sizeof(sLryImageSlave.DownloadWorkParam.m_WorkCmd)+
			sizeof(sLryImageSlave.DownloadWorkParam.m_szAlgorithm)+
			sizeof(sLryImageSlave.DownloadWorkParam.m_NewcID)+
			sizeof(sLryImageSlave.DownloadWorkParam.m_Reserver2))/sizeof(short)
			,(unsigned short*)&sLryImageSlave.DownloadWorkParam.m_WorkCmd);
		Sleep(1000);
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{ 
			iResult = pModbusMaster->DecodeWriteMultipleRegister(
				buf,length); 
			if(iResult>0)
				AddCommand(COMMAND_RRT,0);
		}
		break;
	default:
		break;
	} 
	return iResult;
}


DWORD CLryHostCtrl::WaitT35() 
{ 
	double *pdMsPerByte = &pModbusMaster->m_pPort->iMsPerByte;
	int sendbyteLength = pModbusMaster->SendLength;
	double dSendTime = *pdMsPerByte*sendbyteLength*2.0f;
	return WaitForSingleObject(m_hModbusShutdownEvent,T35*2+dSendTime);
}
 
void CLryHostCtrl::StateProc()
{ 
	pRdb->RefreshRDBTempTab(&sLryImageSlave);
	
	switch(sLryImageSlave.UploadRegister.TestState)
	{
		case STATE_NULL:  
			PostM(_T("�ȴ�ʵ��.."),0,SCH_CMD_ENTEST);
			sLryImageSlave.Report.lTiCnt = 0;
			bReadResult = FALSE;
			break;
		case STATE_TEST_START:			
			PostM(_T("���ڽ�ˮ"),pRdb->m_sFillWaterTime,SCH_CMD_DISTEST);
			break;		//��ʼ����
		case STATE_JINSHUI:
			PostM(_T("�ȴ�ˮλ�ȶ�"),30,SCH_CMD_DISTEST);
			break;			//��ˮ
		case STATE_STABLISH:
			//TRACE(_T("sLryImageSlave.UploadRegister.TestState=%d\n"),sLryImageSlave.UploadRegister.TestState);//����״̬  
			PostM(_T("Ԥ����"),pRdb->m_sStartJiaoBanTime1,SCH_CMD_DISTEST);
			break;			//�ȶ�
		case STATE_START	:
			PostM(_T("���ڽ���"),pRdb->m_sStartJiaoBanTime2,SCH_CMD_DISCURVE);
			break;			//����
		case STATE_DIANHUO:
			PostM(_T("���ڵ��"),pRdb->m_sFireTime+30,SCH_CMD_DISCURVE);//pRdb->m_sFireTime
			break;			//���			
		case STATE_DIANHUO_OK:
			PostM(_T("���ɹ�,������������"),0,SCH_CMD_DISCURVE);
			break;		//���ɹ�
		case STATE_DIANHUO_BAD:
			PostM(_T("���ʧ��,������ֹ��"),0,0);
			break;		//���ʧ��	
		case STATE_MAIN :
			PostM(_T("������������.."),900,SCH_CMD_DISCURVE);
			break;		   	//���� 
		case STATE_TEST_END:
			PostM(_T("ĩ�ڽ���"),pRdb->m_sEndtJiaoBanTime,SCH_CMD_DISCURVE);
			break;			//����ĩ��
		case STATE_TEST_REPORT:  //��ȡ�����¶�
			AddCommand(COMMAND_RDF,0);
			//bReadResult = TRUE;
			break;
		case STATE_TEST_STOP	:
			PostM(_T("ʵ�����.."),10,SCH_CMD_ENTEST);
			break;		//�������  
		case STATE_OUT_WATER:
			if((sLryImageSlave.Report.lTiCnt>4))
			{
				if(!bReadResult)
				{
					TRACE(_T("û�ж�ȡʵ����\n"));
					AddCommand(COMMAND_RDF,0);
					//bReadResult = TRUE; 
				} 
			} 
			PostM(_T("���ڷ�ˮ.."),pRdb->m_sOutWaterTime,SCH_CMD_ENTEST);
			break;		//�������  
		default:
			break;
			//���� 
	}
}

 
void CLryHostCtrl::SaveResultToDatabase()
{
	//CString AutoNo = BuildAutoNO(CString sWorkType,CString strAB);
	if(pRdb->strWorkType!=_T("������")) 
		pRdb->SaveEtoDatabase();
	else
		pRdb->SaveQtoDatabase(); 
}
 

 
