// CLYHostCtrl.cpp: implementation of the CCLYHostCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "CLYHostCtrl.h"
#include "DlgRegInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCLYHostCtrl::CCLYHostCtrl()	
{
	pImageSlave = new CClyImageSlave();	
}

CCLYHostCtrl::~CCLYHostCtrl()
{
	delete pImageSlave;
}

 void CCLYHostCtrl::StateProc() 
 {
	 ASSERT(pImageSlave!=NULL);
	 ASSERT(pRdb!=NULL);
	 pRdb->RefreshRDBTab(pImageSlave);	
	 switch (pImageSlave->cTestState)
	 {
	 case TEST_NONE:
		 
		// if(pRdb->status.cSampleDiskCurrPos==0)
		if(!pRdb->status.m_bReseted)//如果第一次启动
		 {
			 PostM(_T("复位设备..."),0,SCH_CMD_DISTEST);   
			 { 
				AddCommand(COMMAND_DLD,0); 
				AddCommand(COMMAND_RES,0);  //复位设备
			 }
		 }
		 else
		 {
 			pRdb->result.m_bReadResult = FALSE;
			PostM(_T("等待实验.."),0,SCH_CMD_ENTEST); 		
			if(pRdb->status.m_bStartTest)   //如果样品作样未完毕,继续下一次
				AddCommand(COMMAND_CQD,0);
		 }
		 break;
	 case TEST_INIT:
		 pRdb->result.m_bReadResult = FALSE;
		 PostM(_T("正在送样"),30,SCH_CMD_DISTEST);
		 break;
	 case TEST_PREDECOMP1:
		 PostM(_T("预分解1"),pRdb->attrib.m_sPreDecomposeTim1+20,SCH_CMD_DISTEST);
		 break;
	 case TEST_PREDECOMP2:
		 PostM(_T("预分解2"),pRdb->attrib.m_sPreDecomposeTim2+20,SCH_CMD_DISTEST);
		 break;
	 case TEST_DECOMP:
		 PostM(_T("高温分解"),300,SCH_CMD_DISTEST);
		 break;
	 case TEST_END:
		PostM(_T("正在退车"),30,SCH_CMD_ENTEST);
		if(!pRdb->result.m_bReadResult )
		{			
			m_pOwner->SendMessage(WM_CALC_DATA,0,0);
		}		
		 break;
	 case TEST_POS_MOVE:
		PostM(_T("移动转盘..."),60,SCH_CMD_ENTEST);
	 default:
		 break;
	 }
 }

 BOOL CCLYHostCtrl::StopCommThread()
 {
	return CHostCtrl::StopCommThread();	 
 }


 int CCLYHostCtrl::CommandCode(UCHAR buf[], UCHAR cCommand, UCHAR cCommandParam1,UCHAR cCommandParam2,UCHAR cCommandParam3,void* pCommandParam)   
 {
	unsigned short  length=0;
	int iResult =0 ; 

	unsigned int AdValue;

	
	switch(cCommand)
	{
	case COMMAND_RRT:   
		/*
 		pModbusMaster->FunctionRead(
			(unsigned char)pRdb->m_bID,
			READ_HOLDING_REGISTER,
			0,
			sizeof(CClyImageSlave)/sizeof(short)); 
		*/
		pModbusMaster->FunctionRead(
			(unsigned char)pRdb->m_bID,
			READ_HOLDING_REGISTER,
			(unsigned short*)&pImageSlave->PosCtrl-                //需要读取的字符串首地址
			(unsigned short*)pImageSlave,				  //数据结构首地址

			(sizeof(pImageSlave->PosCtrl)+
			sizeof(pImageSlave->cID)+
			sizeof(pImageSlave->cTestState)+
			sizeof(pImageSlave->bAutoCly)+
			sizeof(pImageSlave->cResetState)+ 
			sizeof(pImageSlave->cStateTime)+
			sizeof(pImageSlave->cTestTimer)+
			sizeof(pImageSlave->InstrumentName) 
			)/sizeof(short));
		if((WaitT35()==WAIT_TIMEOUT)&&
			((length=pModbusMaster->Respond(buf))!=0))
		{ 
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->PosCtrl);
		 	if(iResult>0)
			{
				if(CString(pImageSlave->InstrumentName)!=pRdb->m_szType)//设备类型不符合
				{					//关闭窗口 
					PostM(_T("IED型号不符"),0,SCH_CMD_DISTEST);	
				}
				else
				{
					AddCommand(COMMAND_DLD,0);	//型号OK，下载数据
					PostM(_T("正在下载参数"),0,0);
				}
			} 
		}		
		break; 
	case COMMAND_DLD:	//下载参数				
		pRdb->InitRDB(pImageSlave);  ////初始化下载记录集
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->DownloadWorkParam-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->DownloadWorkParam)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->DownloadWorkParam 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		  {	
			  /*
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{
				Beep(1000,50);
				PostM(_T("等待试验..."),0,SCH_CMD_LOGOK);   
				AddCycCommand(COMMAND_RDI,0);	//下载数据OK后循环读数
			}
			*/
		  }	/*
			UCHAR	cID;                					//IED站号  
			UCHAR	cTestState;
			UCHAR   bAutoCly;								//自动测硫仪
			UCHAR   cResetState;
			*/
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->cID-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			(sizeof(pImageSlave->cID)
			+sizeof(pImageSlave->cTestState)
			+sizeof(pImageSlave->bAutoCly)
			+sizeof(pImageSlave->cResetState)
			)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->cID 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{			
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{
				Beep(1000,50);
				PostM(_T("等待试验..."),0,SCH_CMD_LOGOK);   
				AddCycCommand(COMMAND_RDI,0);	//下载数据OK后循环读数
			}
		}	
		break;
	case COMMAND_RDI:			//数据报告			
 		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
			(unsigned short*)&pImageSlave->OutputCoil-                //需要读取的字符串首地址
			(unsigned short*)pImageSlave,									 //数据结构首地址
			(sizeof(pImageSlave->OutputCoil)+
			sizeof(pImageSlave->InputStatus)+
			sizeof(pImageSlave->AdValue)+
			sizeof(pImageSlave->Freq)+
			sizeof(pImageSlave->MotorCtrl)+
			sizeof(pImageSlave->Ai_CTRL)+
			sizeof(pImageSlave->PosCtrl)+
			sizeof(pImageSlave->cID)+
			sizeof(pImageSlave->cTestState)+
			sizeof(pImageSlave->cStateTime)+
			sizeof(pImageSlave->bAutoCly)+
			sizeof(pImageSlave->cResetState)+
			sizeof(pImageSlave->cTestTimer)  
			)/sizeof(short)
			); 
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->OutputCoil);
			if(iResult>0)
			{
				//求当前温度 
				//根据状态得出
				StateProc();
			} 
		}  
		
		break;
	case COMMAND_JDQ:		//继电器控制		
		pModbusMaster->FunctionWriteSingle((unsigned char)pRdb->m_bID,
											WRITE_SINGLE_COIL,
											cCommandParam1&0x7F,
 											cCommandParam1&0x80) ;
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeWriteSingleCoil(buf,length,&pImageSlave->OutputCoil.Coil8);
		} 		
		break; 
	case	COMMAND_LOG:
		{ 
			/*
			//添加一个注册命令 
			pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
				(unsigned short*)&pImageSlave->Info-                //需要读取的字符串首地址
				(unsigned short*)pImageSlave,									 //数据结构首地址
				(sizeof(pImageSlave->Info)/sizeof(short))
				); 
			if((WaitT35()==WAIT_TIMEOUT)
				&&((length=pModbusMaster->Respond(buf))!=0))			
			{
				iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->Info); 
				if(iResult>0)
				{	
					if(DecryptDevInfo()>0) 
						AddCommand(COMMAND_RRT,0);  //查询设备类型				
				}   
			} */
			AddCommand(COMMAND_RRT,0);  //查询设备类型
			iResult = 1;	
			
		}
		break;
	case	COMMAND_CQD	:		//启动试验
	case	COMMAND_CTZ:			//停止试验
		if(COMMAND_CQD==cCommand)
			pImageSlave->Cmd.CmdType	= CMD_START_MEASURE;
		else
			pImageSlave->Cmd.CmdType	= CMD_STOP_MEASURE;
		pImageSlave->Cmd.CmdParam1	= 0;
		pImageSlave->Cmd.CmdParam2	= 0;
		pImageSlave->Cmd.CmdParam3	= 0; 
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
		}
		break;
	case COMMAND_RES:
		//pRdb->status.m_bReseted		=TRUE; 
		pRdb->status.m_bStartTest	= FALSE;
		pImageSlave->Cmd.CmdType	= CMD_RESET_INST;
		pImageSlave->Cmd.CmdParam1	= 0;
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
		}
		break;
	case COMMAND_WUP: 
//		AdValue = pRdb->VoltoAD(pRdb->GetVoltage(1150));
		
		pImageSlave->Cmd.CmdType	= CMD_WARMUP;
		pImageSlave->Cmd.CmdParam1	= Command.SendCmd.m_cCommandParam1;
		pImageSlave->Cmd.CmdParam2	= 0;
		pImageSlave->Cmd.CmdParam3	= 0; 
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
		}
		
		break;
	case COMMAND_DJ:
		pImageSlave->Cmd.CmdType	= CMD_ELE;
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;
		pImageSlave->Cmd.CmdParam3	= cCommandParam3; 
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
		}
		break;
	case COMMAND_MTR:                                           //移动样车
		pImageSlave->Cmd.CmdType	= CMD_MOVEMOTORSAMPLE;
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;
		pImageSlave->Cmd.CmdParam3	= cCommandParam3; 
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
		}
		break;
	case COMMAND_MTR_POS:                                           //移动样盘
		pImageSlave->Cmd.CmdType	= CMD_MOVEMOTORSAMPLENO;
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;
		pImageSlave->Cmd.CmdParam3	= cCommandParam3; 
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
		}
		break;
	case COMMAND_RDF: 
		/*
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
				//pModbusMaster->StrSwapEndian(sLryImageSlave.UploadRegister.sType,sizeof(sLryImageSlave.UploadRegister.sType));
				pModbusMaster->LongSwapEndian((long *)&sLryImageSlave.Report,sizeof(sLryImageSlave.Report)/sizeof(long));
				
				//求当前温度 
				//根据状态得出 校正值
				pRdb->RefreshRDBReportTab(&sLryImageSlave);  
				//计算结果
				//保存到数据库		
				m_pOwner->SendMessage(WM_CALC_DATA,0,0);
				AddCommand( COMMAND_CTZ,0); 
			}

		}  
		*/
		break; 
	case COMMAND_SID:
		pImageSlave->Cmd.CmdType	= CMD_SETID;
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;
		pImageSlave->Cmd.CmdParam3	= cCommandParam3; 
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)0xFF,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			sizeof(pImageSlave->Cmd)/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length); 	
			if(iResult>0)
			{ 
				AddCommand(COMMAND_LOG,0);
			}

		}
		break;
	default:
		break;
	}
	return iResult;
 }

 int CCLYHostCtrl::DecryptDevInfo(void)
 {	
	 int result = 0;
	 Encpypt *pEncryptInfo = new Encpypt();
	 Encpypt::SecruitInfo *pInfo = pEncryptInfo->pInfo = &pImageSlave->Info;  
	 short CrcValue = 0;		
	 int InfoSize = sizeof(Encpypt::SecruitInfo);
	 int CrcSize  = sizeof(pInfo->CRC); 
	 byte pSecuriteArrary [sizeof(Encpypt::SecruitInfo)];
	 ZeroMemory(pSecuriteArrary,InfoSize);
	 ASSERT((InfoSize-CrcSize)%8==0);
	 /////////////////////////////////////////////////////////////////////// 

	 CrcValue =CModBusServer::CRC_16((unsigned char *)pInfo,InfoSize-CrcSize); 
	 //CRC校验,防止未加密版本被读取
	 if((pInfo->CRC[0]<<8)+pInfo->CRC[1]==CrcValue)
	 { 
		 memcpy(pSecuriteArrary,pInfo,sizeof(Encpypt::SecruitInfo));
		 ZeroMemory(pInfo,InfoSize); //解密  
		 pEncryptInfo->decrypt(pSecuriteArrary,(byte*)pInfo,InfoSize-CrcSize);
		 COleDateTime todayDate = COleDateTime::GetCurrentTime();
		 if(pInfo->lEndDate>todayDate)
		 { //如果没有过期 报设备型号
			 AddCommand(COMMAND_RRT,0);	  //添加
		 }
		 else
		 {
			 CDlgRegInfo dlgInfo;
			 dlgInfo.bOldSecretArray = pSecuriteArrary;
			 dlgInfo.pInfo.pInfo = pInfo;
			 dlgInfo.DoModal();
		 } 
		 //如果已经过期,弹出对话框输入新的序列号
		 result = 1;
	 }  
	 else
	 {
		 AfxMessageBox(_T("下位机版本不符,请升级程序\n"),MB_ICONWARNING+MB_OK);
	 }
	 delete pEncryptInfo;	
	 return result;
 }

 void CCLYHostCtrl::EncryptDevInfo(byte* pSecuriteArrary,Encpypt *pEncrypt)
 {
	 int InfoSize = sizeof(Encpypt::SecruitInfo);
	 Encpypt::SecruitInfo *pInfo = pEncrypt->pInfo;
	 int CrcSize  = sizeof(pInfo->CRC); 
	 strcpy(pInfo->uID,"\x01\x02\x03\x04\x05\x06\x07\x08");
	 strcpy(pInfo->cCompany,"长沙通发电气科技有限公司");
	 short CrcValue = CModBusServer::CRC_16((unsigned char *)pInfo,InfoSize-CrcSize);
	 pInfo->CRC[0] = CrcValue>>8;
	 pInfo->CRC[1] = CrcValue&0xFF;
	 pEncrypt->encrypt((byte*)pInfo,pSecuriteArrary,InfoSize-CrcSize);
	 ///////////////////////////////////////////////////////////////////// 
 } 

void CCLYHostCtrl::SetRDB(CRDB *pRdb)
{
	this->pRdb = (CClyRDB*)pRdb;
	CHostCtrl::SetRDB(pRdb);
}

double CCLYHostCtrl::ADtoVol(long ADVAlue)
{
	double voltage,temp;		  
	voltage = ((double)(ADVAlue/4)*VREF)/0xFFFFFF;	
	//temp = VolToTemp(voltage);
	return voltage;
}

 

