// GyfxyHostCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyHostCtrl.h"


// CGyfxyHostCtrl
 

CGyfxyHostCtrl::CGyfxyHostCtrl()
{
	pImageSlave = new CGyfxyImageSlave();
}

CGyfxyHostCtrl::~CGyfxyHostCtrl()
{
	delete pImageSlave;
}
 

int CGyfxyHostCtrl::CommandCode(UCHAR  buf[], UCHAR cCommand,UCHAR cCommandParam1, 
								UCHAR cCommandParam2, UCHAR cCommandParam3,void* pCommandParam)
{
	unsigned short  length=0;
	int iResult =0 ; 
	int id = pRdb->m_bID;;
	switch (cCommand)
	{
	case COMMAND_RRT:							//报告设备类型
		pModbusMaster->FunctionRead(
			(unsigned char)pRdb->m_bID,
			READ_HOLDING_REGISTER,
			(unsigned short*)pImageSlave->InstrumentName-(unsigned short*)pImageSlave,

			(sizeof(pImageSlave->InstrumentName)		
			+sizeof(pImageSlave->cID)
			+sizeof(pImageSlave->ReserveID))
			/sizeof(short)); 
		if(
			(WaitT35()==WAIT_TIMEOUT)&&
			((length=pModbusMaster->Respond(buf))!=0))
		{ 
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)pImageSlave->InstrumentName);
			if(iResult>0)
			{	 
				if(CString(pImageSlave->InstrumentName)!=pRdb->m_szType)//设备类型不符合
				{					//关闭窗口 
					PostM(_T("IED型号不符"),0,SCH_CMD_DISTEST);	
					iResult = 0;
				}
				else
				{
					AddCommand(COMMAND_DLD,0);	//型号OK，下载数据
					PostM(_T("正在下载参数"),0,0);
				}
				
			} 
		}
		break;
	case COMMAND_DLD:
		pRdb->InitRDB(pImageSlave);  ////初始化下载记录集 
		pRdb->SampleArray.SetInstName(pRdb->m_szName);
		//pRdb->SampleArray.CreateTempTab();
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->DownloadWorkParam-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			(sizeof(pImageSlave->DownloadWorkParam))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->DownloadWorkParam 
			);  
		if((WaitT35()==WAIT_TIMEOUT)
			&&((length=pModbusMaster->Respond(buf))!=0))
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
		pModbusMaster->FunctionRead(
			(unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER, 
			(unsigned short*)&pImageSlave->OutputCoil- 
			(unsigned short*)pImageSlave,									 //数据结构首地址
			(sizeof(pImageSlave->OutputCoil)
			+sizeof(pImageSlave->InputStatus)
			+sizeof(pImageSlave->ADValue) 
			+sizeof(pImageSlave->fuzzy)
			+sizeof(pImageSlave->Balance) 
			//////////////////////////////
			+sizeof(pImageSlave->MtrState)
			+sizeof(pImageSlave->ResetState) 
			+sizeof(pImageSlave->TestState) 
			+sizeof(pImageSlave->MetageState)
			)/sizeof(short)
			); 
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->OutputCoil);
			if(iResult>0)
			{
				//根据状态得出
				StateProc();
			} 
		}
		break;
	case COMMAND_LOG:
		//TODO:添加一个注册命令
		//AddCycCommand(COMMAND_RDI,0);		//清循环命令
		//AddCommand(COMMAND_RDI,0);		//清循环命令 
		AddCommand(COMMAND_RRT,0);		
		iResult = 1;	
		break;
	case COMMAND_JDQ:
		pModbusMaster->FunctionWriteSingle((unsigned char)pRdb->m_bID,
			WRITE_SINGLE_COIL,
			cCommandParam1,
			cCommandParam2) ;
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeWriteSingleCoil(buf,length,(UCHAR*)&pImageSlave->OutputCoil.coil8);
		} 		 
		break; 
	case COMMAND_MTR_POS:
		
		pImageSlave->Cmd.CmdType	= CMD_MOVEMOTORSAMPLENO;
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;    
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;	 
		pModbusMaster->FunctionWriteMultipleRegister(

			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			(sizeof(pImageSlave->Cmd))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{			
			iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length);
		}	
		
		break;
	case COMMAND_MTR:
		
		pImageSlave->Cmd.CmdType	= CMD_MOVEMOTOR;
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;    //电机编号
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;	 //是否允许
		pImageSlave->Cmd.CmdParam3	= cCommandParam3;    //电机运行目标
		pModbusMaster->FunctionWriteMultipleRegister(

			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			(sizeof(pImageSlave->Cmd))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{			
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{  
			}
		}	
		break;
	case COMMAND_GETQUALITY:
		ASSERT(MAX_SAMPLE_CNT>=cCommandParam1);
		iResult = GetQuality(buf,cCommandParam1); 
		break;
	case COMMAND_SETQUALITY:
		ASSERT(MAX_SAMPLE_CNT>=cCommandParam1);
		iResult = SetQuality(buf,cCommandParam1); 
		break;
	case COMMAND_CQD:
		//m_WorkCmd 			
		pImageSlave->Cmd.CmdType	= CMD_START_TEST;//启动测试
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave, 
			(sizeof(pImageSlave->Cmd))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd );
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(
				buf,length);
		}
		break;
	case COMMAND_CTZ:			//停止试验
		pImageSlave->Cmd.CmdType = CMD_STOP_TEST;			//停止测试
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave, 
			(sizeof(pImageSlave->Cmd))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd );
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))
		{
			iResult = pModbusMaster->DecodeWriteMultipleRegister(
				buf,length);
		}
		break;
	case COMMAND_BALANCE:
		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER, 
			(unsigned short*)&pImageSlave->Balance.CurrWeight- 
			(unsigned short*)pImageSlave,									 //数据结构首地址
			(sizeof(pImageSlave->Balance.CurrWeight)  
			+sizeof(pImageSlave->MtrState)
			 )/sizeof(short)
			); 
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->Balance.CurrWeight);
			if(iResult>0)
				pRdb->RefreshRDBTab(pImageSlave);	
		}
		break;
	case COMMAND_USER_DEFINE:
		ProcCustomCmd(cCommandParam1,
			cCommandParam2,cCommandParam3,pCommandParam);
		if ( pImageSlave->Cmd.CmdType ==CMD_SETID )
		{ 
			pRdb->m_bID = 0xFF;
		} 
		pModbusMaster->FunctionWriteMultipleRegister( 
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//需要读取的字符串首地址
			(sizeof(pImageSlave->Cmd))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->Cmd 
			);  
		if((WaitT35()==WAIT_TIMEOUT)
			&&((length=pModbusMaster->Respond(buf))!=0))
		{			
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{  
				if ( pImageSlave->Cmd.CmdType ==CMD_SETID )
					AddCommand(COMMAND_LOG,0);
			}
		}
		pRdb->m_bID = id;
		break;
	default:
		TRACE(_T("一个没有处理的消息\n"));
		PostM(_T("一个没有处理的消息..."),0,0);   
		break;
	}
	return iResult;
}

void CGyfxyHostCtrl::StateProc() 
{
	ASSERT(pImageSlave!=NULL);
	ASSERT(pRdb!=NULL);
	pRdb->RefreshRDBTab(pImageSlave);	

	if(pImageSlave->ResetState==CGyfxyImageSlave::REST_INIT)//,REST_SAMPLEDISH,REST_END
	{
		PostM(_T("复位设备..."),0,0);			//发现未被复位,肯定是重新开过机,重新下载参数,发复位指令
		{ 
			AddCommand(COMMAND_DLD,0);  
			AddCommand(COMMAND_USER_DEFINE,CMD_RESET_INST);  //复位设备
			AddCommand(COMMAND_RDI,0);
			//TODO: SCH_CMD_DISTEST
			//增加允许/禁止测试按钮代码
		}
		return ;
	}
	/*

	enum TEST_STATE {T_INIT=0,T_WARM_WATER,T_TEST_WATER,T_WEIGHT_WATER,T_WATERCOOL,T_WATER_END,
	T_WARM_HIGH1,T_TEST_VOL,T_BURN_VOL,T_WEIGHT_VOL,T_VOL_END,
	T_WARM_HIGH2,T_TEST_ASH,T_BURN_ASH,T_WEIGHT_ASH,T_ASH_END,	
	T_END};///测试状态
	*/
	switch(pImageSlave->TestState)
	{
	case CGyfxyImageSlave::T_INIT:
		UploadQuality();  
		break;
	/////////////////////////////////////////////////////////////
	case CGyfxyImageSlave::T_WARM_WATER: 
		PostM(_T("正在升温............."),0,0);   
		break;
	case CGyfxyImageSlave::T_TEST_WATER:
		PostM(_T("正在测试水分............."),0,0);  
		break;
	case CGyfxyImageSlave::T_WATERCOOL:
		PostM(_T("正在冷却样品............."),0,0);  
		break;
	case CGyfxyImageSlave::T_WEIGHT_WATER: 
		//  上传重量数据
		UploadQuality();
		break;
	case CGyfxyImageSlave::T_WATER_END:
		//TODO:关闭天平窗口 
		//TODO:刷新临时数据库中的记录
		PostM(_T("水分测试完毕!"),0,0); 
		break;
	//////////////////////////////////////////////////////////////
	case CGyfxyImageSlave::T_WARM_HIGH1:
	case CGyfxyImageSlave::T_TEST_VOL:
		PostM(_T("高温炉正在升温"),0,0); 
		break;
	case CGyfxyImageSlave::T_BURN_VOL:
		PostM(_T("正在测试挥发份..........."),0,0);  
		break;
	case CGyfxyImageSlave::T_WEIGHT_VOL: 
		UploadQuality();  
		break;
	case CGyfxyImageSlave::T_VOL_END: 
		//TODO:关闭天平窗口
		//TODO:刷新临时数据库中的记录
		PostM(_T("挥发分测试完毕"),0,0); 
		break;
	//////////////////////////////////////////////////////////
	case CGyfxyImageSlave::T_WARM_HIGH2: 
	case CGyfxyImageSlave::T_TEST_ASH: 
		PostM(_T("高温炉正在升温"),0,0); 
		break;
	case CGyfxyImageSlave::T_BURN_ASH:

		PostM(_T("正在测试灰份..........."),0,0);  
		break;
	case CGyfxyImageSlave::T_WEIGHT_ASH: 
		//TODO:关闭天平窗口
		UploadQuality();  
		break;
	case CGyfxyImageSlave::T_ASH_END:
		//TODO:刷新临时数据库中的记录
		PostM(_T("灰分测试完毕"),0,0); 
		break;
	case CGyfxyImageSlave::T_END:
		//TODO:计算,校正,写入数据库
		pRdb->SampleArray.SaveResult2DB();
		PostM(_T("实验完毕"),0,0); 
		break;
	default:
		break;
	}
}

//
void CGyfxyHostCtrl::UploadQuality(void)
{
	CGyfxyImageSlave::YP_DATA *pSample = pImageSlave->Sample;
	int SamplePos	= pImageSlave->MtrState.CurSamplePos;
	switch (pImageSlave->MetageState)
	{
	case CGyfxyImageSlave::M_INIT: 

		PostM(_T("等待实验."),0,0); 
		break;
	case CGyfxyImageSlave::M_POS:
		PostM(_T("到达目标位置 !"),0,0);  
		break;
	case CGyfxyImageSlave::M_TARE:
		PostM(_T("正在除皮.........."),0,0);  
		break;
	case CGyfxyImageSlave::M_METAGE:
		PostM(_T("正在称量.........."),0,0);  
		break;
	case CGyfxyImageSlave::M_SAVE:
		PostM(_T("等待天平稳定.........."),0,0);  
		
		m_pOwner->PostMessage(WM_COMMAND,ID_BALANCE,0);
		//弹出称重窗口
		
		break;
	case CGyfxyImageSlave::M_NEXTPOS:
		PostM(_T("移动到下一个样位.........."),0,0);  
		//当前位置的重量数据 
		// :更新这个样位的数据
		AddCommand(COMMAND_GETQUALITY,SamplePos);
		break;
	case CGyfxyImageSlave::M_END:
		PostM(_T("称量完毕 "),0,0);  
		break;//称量状态	
	default:
		break;
	}
}

//  
void CGyfxyHostCtrl::SetRDB(CRDB * pRdb)
{
	this->pRdb = (CGyfxyRDB_BaseRdb*)pRdb;
	CHostCtrl::SetRDB(pRdb);
}

void CGyfxyHostCtrl::ProcCustomCmd(UCHAR cCommandParam1,
								   UCHAR cCommandParam2, 
								   UCHAR cCommandParam3,
								   void* pCommandParam)
{
	pImageSlave->Cmd.CmdType	= cCommandParam1;
	pImageSlave->Cmd.CmdParam1	= cCommandParam2;
	pImageSlave->Cmd.CmdParam2	= cCommandParam3;
	pImageSlave->Cmd.CmdParam3	= 0;
	pImageSlave->Cmd.CmdReserve	= 0;
	switch(cCommandParam1)
	{
		case CMD_WARMUP:
			/* 
			CGyfxyHostCtrl::CMD_WARMUP,   cmdtype
			LEFT_STOVE ,				  //炉子编号
			SW,                           //是否升温
			&DestTemp[LEFT_STOVE]);       //目标温度AD
			*/
			if(cCommandParam3)
				pImageSlave->Cmd.CmdReserve	= CGyfxyRDB::TemptoAD(*(LONG*)pCommandParam);
			break; 
		default:
			break;
	}
}


//SampleNO = 0;读取所有的样品
//SampleNO > 0;读取单个的样品
int CGyfxyHostCtrl::GetQuality(UCHAR  buf[],int SampleNO)
{ 
	int iResult=0,length=0;
	if(SampleNO>0)
	{
		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
			(unsigned short*)&pImageSlave->Sample[SampleNO-1]-   //cCommandParam1为样位编号
			(unsigned short*)pImageSlave,							  //数据结构首地址
			(sizeof(CGyfxyImageSlave::YP_DATA) 
			)/sizeof(short)
			); 

		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->Sample[SampleNO>0?SampleNO-1:0]);			
			pRdb->SampleArray.RefreshSampleData(pImageSlave,SampleNO);    
			// 增加计算过程
			pRdb->SampleArray.CalcResult(SampleNO);
			pRdb->SampleArray.UpdateResult2TempTab(SampleNO);
			//保存数据到临时数据库
			//update database 

		}  
		if(IsWindow(m_pOwner->m_hWnd))
			m_pOwner->PostMessage(WM_SAMPLE_QUALITY_UPDATE,SampleNO);
	}
	else
	{ 
		pRdb->SampleArray.UpdateResult2TempTab(0); //建立空的临时数据库
		UploadAllSample();
		iResult = 0xff;//
	} 
	return iResult;
}

int CGyfxyHostCtrl::SetQuality(UCHAR  buf[],int SampleNO)
{
	int iResult=0,length=0; 
	if(SampleNO>0)
	{
		pModbusMaster->FunctionWriteMultipleRegister((unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Sample[SampleNO-1]-   //cCommandParam1为样位编号
			(unsigned short*)pImageSlave,							  //数据结构首地址
			(sizeof(CGyfxyImageSlave::YP_DATA) 
			)/sizeof(short),
			(unsigned short *)&pImageSlave->Sample[SampleNO-1] 
			); 
	
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{ 
				_stprintf(str,_T("下载%d#坩埚参数成功!"),SampleNO); 
				PostM(str,0,0);
			} 
		} 
	}
	else
	{ 
		DownloadAllSample();
		iResult = 0xff;//
	}
	return iResult;
}

void CGyfxyHostCtrl::UploadAllSample(void)
{	
	int cnt = pRdb->attrib.m_btMaxSampleCnt;
	for(int i=1;i<=cnt;i++)
		AddCommand(COMMAND_GETQUALITY,i); 
	
}

void CGyfxyHostCtrl::DownloadAllSample(void)
{
	int cnt = pRdb->attrib.m_btMaxSampleCnt;
	for(int i=1;i<=cnt;i++)
		AddCommand(COMMAND_SETQUALITY,i);
}
