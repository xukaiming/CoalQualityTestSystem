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

			(unsigned short*)sLryImageSlave.UploadRegister.sType-                //需要读取的字符串首地址
			(unsigned short*)&sLryImageSlave,									 //数据结构首地址
			sizeof(sLryImageSlave.UploadRegister.sType)/sizeof(short)); 
		if((WaitT35()==WAIT_TIMEOUT)
			&&((length=pModbusMaster->Respond(buf))!=0))
		{ 
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)sLryImageSlave.UploadRegister.sType);
		 	if((iResult>0)&&CheckCPUBigEndian())
				pModbusMaster->StrSwapEndian(sLryImageSlave.UploadRegister.sType,sizeof(sLryImageSlave.UploadRegister.sType));
		 	if(iResult>0) 
				if(CString(sLryImageSlave.UploadRegister.sType)!=pRdb->m_szType)//设备类型不符合
				{
					//关闭窗口 
					PostM(_T("IED型号不符"),0,SCH_CMD_DISTEST);  
				}
				else
				{
					AddCommand(COMMAND_DLD,0);	//型号OK，下载数据
					PostM(_T("正在下载参数"),0,0);
				}

		}
		break;
	case COMMAND_DLD:	//下载参数
		pRdb->InitRDB(&sLryImageSlave);  ////初始化下载记录集
		if( CheckCPUBigEndian())
			pModbusMaster->StrSwapEndian(sLryImageSlave.DownloadWorkParam.m_szAlgorithm,sizeof(sLryImageSlave.DownloadWorkParam.m_szAlgorithm));
		pModbusMaster->FunctionWriteMultipleRegister((unsigned char)pRdb->m_bID,0,sizeof(sLryImageSlave.DownloadWorkParam)/sizeof(SHORT), (unsigned short *)&sLryImageSlave.DownloadWorkParam );  
		  if((WaitT35()==WAIT_TIMEOUT)
			  &&((length=pModbusMaster->Respond(buf))!=0))
		  {
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
			if(iResult>0){
				PostM(_T("等待试验..."),0,SCH_CMD_LOGOK);
				Beep(1000,50);
				AddCycCommand(0,0);
				AddCycCommand(COMMAND_RDI,0);	//下载数据OK后循环读数
			}
		  }
		break;
	case COMMAND_RDI:			//数据报告		
 		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
			(unsigned short*)&sLryImageSlave.UploadRegister-                //需要读取的字符串首地址
			(unsigned short*)&sLryImageSlave,									 //数据结构首地址
			(sizeof(sLryImageSlave.UploadRegister)+
			sizeof(sLryImageSlave.InputStatus)+
			sizeof(sLryImageSlave.OutputCoil)+
			sizeof(sLryImageSlave.Report)-
			sizeof(sLryImageSlave.Report.lTi)) /sizeof(short) 
			//sizeof(sLryImageSlave.Report))太大,送上了有困难,只送一部分上来
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
				//求当前温度 
				//根据状态得出
				StateProc();
			}
		}  
		break;
	case COMMAND_JDQ:		//继电器控制

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
		//AddCycCommand(COMMAND_RDI,0);		//清循环命令
		//AddCommand(COMMAND_RDI,0);		//清循环命令 
		AddCommand(COMMAND_RRT,0); 
		iResult = 1;
		break;
	case   COMMAND_CQD	:		//启动试验
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
	case COMMAND_CTZ:			//停止试验
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
			(unsigned short*)&sLryImageSlave.Report-                //需要读取的字符串首地址
			(unsigned short*)&sLryImageSlave,						//数据结构首地址
			sizeof(sLryImageSlave.Report)/sizeof(short)
			); 
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&sLryImageSlave.Report);
			if((iResult>0)&&CheckCPUBigEndian())
			{
				pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
					(unsigned short*)&sLryImageSlave.ReportT0Tn-                //需要读取的字符串首地址
					(unsigned short*)&sLryImageSlave,						//数据结构首地址
					sizeof(sLryImageSlave.ReportT0Tn)/sizeof(short)
					); 
				if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
					iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&sLryImageSlave.ReportT0Tn);
				if((iResult>0)&&CheckCPUBigEndian())
				{ 
					pModbusMaster->LongSwapEndian((long *)&sLryImageSlave.Report,sizeof(sLryImageSlave.Report)/sizeof(long));
					pModbusMaster->LongSwapEndian((long *)&sLryImageSlave.ReportT0Tn,sizeof(sLryImageSlave.ReportT0Tn)/sizeof(long));

					//求当前温度 
					//根据状态得出 校正值
					pRdb->RefreshRDBReportTab(&sLryImageSlave);  
					//计算结果
					//保存到数据库		
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
			PostM(_T("等待实验.."),0,SCH_CMD_ENTEST);
			sLryImageSlave.Report.lTiCnt = 0;
			bReadResult = FALSE;
			break;
		case STATE_TEST_START:			
			PostM(_T("正在进水"),pRdb->m_sFillWaterTime,SCH_CMD_DISTEST);
			break;		//开始试验
		case STATE_JINSHUI:
			PostM(_T("等待水位稳定"),30,SCH_CMD_DISTEST);
			break;			//进水
		case STATE_STABLISH:
			//TRACE(_T("sLryImageSlave.UploadRegister.TestState=%d\n"),sLryImageSlave.UploadRegister.TestState);//测试状态  
			PostM(_T("预搅拌"),pRdb->m_sStartJiaoBanTime1,SCH_CMD_DISTEST);
			break;			//稳定
		case STATE_START	:
			PostM(_T("初期搅拌"),pRdb->m_sStartJiaoBanTime2,SCH_CMD_DISCURVE);
			break;			//初期
		case STATE_DIANHUO:
			PostM(_T("正在点火"),pRdb->m_sFireTime+30,SCH_CMD_DISCURVE);//pRdb->m_sFireTime
			break;			//点火			
		case STATE_DIANHUO_OK:
			PostM(_T("点火成功,进入主期升温"),0,SCH_CMD_DISCURVE);
			break;		//点火成功
		case STATE_DIANHUO_BAD:
			PostM(_T("点火失败,试验终止！"),0,0);
			break;		//点火失败	
		case STATE_MAIN :
			PostM(_T("进入主期升温.."),900,SCH_CMD_DISCURVE);
			break;		   	//主期 
		case STATE_TEST_END:
			PostM(_T("末期搅拌"),pRdb->m_sEndtJiaoBanTime,SCH_CMD_DISCURVE);
			break;			//试验末期
		case STATE_TEST_REPORT:  //读取报表温度
			AddCommand(COMMAND_RDF,0);
			//bReadResult = TRUE;
			break;
		case STATE_TEST_STOP	:
			PostM(_T("实验结束.."),10,SCH_CMD_ENTEST);
			break;		//试验结束  
		case STATE_OUT_WATER:
			if((sLryImageSlave.Report.lTiCnt>4))
			{
				if(!bReadResult)
				{
					TRACE(_T("没有读取实验结果\n"));
					AddCommand(COMMAND_RDF,0);
					//bReadResult = TRUE; 
				} 
			} 
			PostM(_T("正在放水.."),pRdb->m_sOutWaterTime,SCH_CMD_ENTEST);
			break;		//试验结束  
		default:
			break;
			//搅拌 
	}
}

 
void CLryHostCtrl::SaveResultToDatabase()
{
	//CString AutoNo = BuildAutoNO(CString sWorkType,CString strAB);
	if(pRdb->strWorkType!=_T("发热量")) 
		pRdb->SaveEtoDatabase();
	else
		pRdb->SaveQtoDatabase(); 
}
 

 
