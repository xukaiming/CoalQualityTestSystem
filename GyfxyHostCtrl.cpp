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
	case COMMAND_RRT:							//�����豸����
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
				if(CString(pImageSlave->InstrumentName)!=pRdb->m_szType)//�豸���Ͳ�����
				{					//�رմ��� 
					PostM(_T("IED�ͺŲ���"),0,SCH_CMD_DISTEST);	
					iResult = 0;
				}
				else
				{
					AddCommand(COMMAND_DLD,0);	//�ͺ�OK����������
					PostM(_T("�������ز���"),0,0);
				}
				
			} 
		}
		break;
	case COMMAND_DLD:
		pRdb->InitRDB(pImageSlave);  ////��ʼ�����ؼ�¼�� 
		pRdb->SampleArray.SetInstName(pRdb->m_szName);
		//pRdb->SampleArray.CreateTempTab();
		pModbusMaster->FunctionWriteMultipleRegister(
			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->DownloadWorkParam-(unsigned short*)pImageSlave,//��Ҫ��ȡ���ַ����׵�ַ
			(sizeof(pImageSlave->DownloadWorkParam))/sizeof(SHORT), 
			(unsigned short *)&pImageSlave->DownloadWorkParam 
			);  
		if((WaitT35()==WAIT_TIMEOUT)
			&&((length=pModbusMaster->Respond(buf))!=0))
		{			
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{
				Beep(1000,50);
				PostM(_T("�ȴ�����..."),0,SCH_CMD_LOGOK);   
				AddCycCommand(COMMAND_RDI,0);	//��������OK��ѭ������
			}
		}	
		break;
	case COMMAND_RDI:			//���ݱ���		 
		pModbusMaster->FunctionRead(
			(unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER, 
			(unsigned short*)&pImageSlave->OutputCoil- 
			(unsigned short*)pImageSlave,									 //���ݽṹ�׵�ַ
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
				//����״̬�ó�
				StateProc();
			} 
		}
		break;
	case COMMAND_LOG:
		//TODO:���һ��ע������
		//AddCycCommand(COMMAND_RDI,0);		//��ѭ������
		//AddCommand(COMMAND_RDI,0);		//��ѭ������ 
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
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//��Ҫ��ȡ���ַ����׵�ַ
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
		pImageSlave->Cmd.CmdParam1	= cCommandParam1;    //������
		pImageSlave->Cmd.CmdParam2	= cCommandParam2;	 //�Ƿ�����
		pImageSlave->Cmd.CmdParam3	= cCommandParam3;    //�������Ŀ��
		pModbusMaster->FunctionWriteMultipleRegister(

			(unsigned char)pRdb->m_bID,
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//��Ҫ��ȡ���ַ����׵�ַ
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
		pImageSlave->Cmd.CmdType	= CMD_START_TEST;//��������
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
	case COMMAND_CTZ:			//ֹͣ����
		pImageSlave->Cmd.CmdType = CMD_STOP_TEST;			//ֹͣ����
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
			(unsigned short*)pImageSlave,									 //���ݽṹ�׵�ַ
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
			(unsigned short*)&pImageSlave->Cmd-(unsigned short*)pImageSlave,//��Ҫ��ȡ���ַ����׵�ַ
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
		TRACE(_T("һ��û�д������Ϣ\n"));
		PostM(_T("һ��û�д������Ϣ..."),0,0);   
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
		PostM(_T("��λ�豸..."),0,0);			//����δ����λ,�϶������¿�����,�������ز���,����λָ��
		{ 
			AddCommand(COMMAND_DLD,0);  
			AddCommand(COMMAND_USER_DEFINE,CMD_RESET_INST);  //��λ�豸
			AddCommand(COMMAND_RDI,0);
			//TODO: SCH_CMD_DISTEST
			//��������/��ֹ���԰�ť����
		}
		return ;
	}
	/*

	enum TEST_STATE {T_INIT=0,T_WARM_WATER,T_TEST_WATER,T_WEIGHT_WATER,T_WATERCOOL,T_WATER_END,
	T_WARM_HIGH1,T_TEST_VOL,T_BURN_VOL,T_WEIGHT_VOL,T_VOL_END,
	T_WARM_HIGH2,T_TEST_ASH,T_BURN_ASH,T_WEIGHT_ASH,T_ASH_END,	
	T_END};///����״̬
	*/
	switch(pImageSlave->TestState)
	{
	case CGyfxyImageSlave::T_INIT:
		UploadQuality();  
		break;
	/////////////////////////////////////////////////////////////
	case CGyfxyImageSlave::T_WARM_WATER: 
		PostM(_T("��������............."),0,0);   
		break;
	case CGyfxyImageSlave::T_TEST_WATER:
		PostM(_T("���ڲ���ˮ��............."),0,0);  
		break;
	case CGyfxyImageSlave::T_WATERCOOL:
		PostM(_T("������ȴ��Ʒ............."),0,0);  
		break;
	case CGyfxyImageSlave::T_WEIGHT_WATER: 
		//  �ϴ���������
		UploadQuality();
		break;
	case CGyfxyImageSlave::T_WATER_END:
		//TODO:�ر���ƽ���� 
		//TODO:ˢ����ʱ���ݿ��еļ�¼
		PostM(_T("ˮ�ֲ������!"),0,0); 
		break;
	//////////////////////////////////////////////////////////////
	case CGyfxyImageSlave::T_WARM_HIGH1:
	case CGyfxyImageSlave::T_TEST_VOL:
		PostM(_T("����¯��������"),0,0); 
		break;
	case CGyfxyImageSlave::T_BURN_VOL:
		PostM(_T("���ڲ��Իӷ���..........."),0,0);  
		break;
	case CGyfxyImageSlave::T_WEIGHT_VOL: 
		UploadQuality();  
		break;
	case CGyfxyImageSlave::T_VOL_END: 
		//TODO:�ر���ƽ����
		//TODO:ˢ����ʱ���ݿ��еļ�¼
		PostM(_T("�ӷ��ֲ������"),0,0); 
		break;
	//////////////////////////////////////////////////////////
	case CGyfxyImageSlave::T_WARM_HIGH2: 
	case CGyfxyImageSlave::T_TEST_ASH: 
		PostM(_T("����¯��������"),0,0); 
		break;
	case CGyfxyImageSlave::T_BURN_ASH:

		PostM(_T("���ڲ��Իҷ�..........."),0,0);  
		break;
	case CGyfxyImageSlave::T_WEIGHT_ASH: 
		//TODO:�ر���ƽ����
		UploadQuality();  
		break;
	case CGyfxyImageSlave::T_ASH_END:
		//TODO:ˢ����ʱ���ݿ��еļ�¼
		PostM(_T("�ҷֲ������"),0,0); 
		break;
	case CGyfxyImageSlave::T_END:
		//TODO:����,У��,д�����ݿ�
		pRdb->SampleArray.SaveResult2DB();
		PostM(_T("ʵ�����"),0,0); 
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

		PostM(_T("�ȴ�ʵ��."),0,0); 
		break;
	case CGyfxyImageSlave::M_POS:
		PostM(_T("����Ŀ��λ�� !"),0,0);  
		break;
	case CGyfxyImageSlave::M_TARE:
		PostM(_T("���ڳ�Ƥ.........."),0,0);  
		break;
	case CGyfxyImageSlave::M_METAGE:
		PostM(_T("���ڳ���.........."),0,0);  
		break;
	case CGyfxyImageSlave::M_SAVE:
		PostM(_T("�ȴ���ƽ�ȶ�.........."),0,0);  
		
		m_pOwner->PostMessage(WM_COMMAND,ID_BALANCE,0);
		//�������ش���
		
		break;
	case CGyfxyImageSlave::M_NEXTPOS:
		PostM(_T("�ƶ�����һ����λ.........."),0,0);  
		//��ǰλ�õ��������� 
		// :���������λ������
		AddCommand(COMMAND_GETQUALITY,SamplePos);
		break;
	case CGyfxyImageSlave::M_END:
		PostM(_T("������� "),0,0);  
		break;//����״̬	
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
			LEFT_STOVE ,				  //¯�ӱ��
			SW,                           //�Ƿ�����
			&DestTemp[LEFT_STOVE]);       //Ŀ���¶�AD
			*/
			if(cCommandParam3)
				pImageSlave->Cmd.CmdReserve	= CGyfxyRDB::TemptoAD(*(LONG*)pCommandParam);
			break; 
		default:
			break;
	}
}


//SampleNO = 0;��ȡ���е���Ʒ
//SampleNO > 0;��ȡ��������Ʒ
int CGyfxyHostCtrl::GetQuality(UCHAR  buf[],int SampleNO)
{ 
	int iResult=0,length=0;
	if(SampleNO>0)
	{
		pModbusMaster->FunctionRead((unsigned char)pRdb->m_bID,READ_HOLDING_REGISTER,
			(unsigned short*)&pImageSlave->Sample[SampleNO-1]-   //cCommandParam1Ϊ��λ���
			(unsigned short*)pImageSlave,							  //���ݽṹ�׵�ַ
			(sizeof(CGyfxyImageSlave::YP_DATA) 
			)/sizeof(short)
			); 

		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			iResult = pModbusMaster->DecodeReadREGISTER(buf,length,(unsigned short *)&pImageSlave->Sample[SampleNO>0?SampleNO-1:0]);			
			pRdb->SampleArray.RefreshSampleData(pImageSlave,SampleNO);    
			// ���Ӽ������
			pRdb->SampleArray.CalcResult(SampleNO);
			pRdb->SampleArray.UpdateResult2TempTab(SampleNO);
			//�������ݵ���ʱ���ݿ�
			//update database 

		}  
		if(IsWindow(m_pOwner->m_hWnd))
			m_pOwner->PostMessage(WM_SAMPLE_QUALITY_UPDATE,SampleNO);
	}
	else
	{ 
		pRdb->SampleArray.UpdateResult2TempTab(0); //�����յ���ʱ���ݿ�
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
			(unsigned short*)&pImageSlave->Sample[SampleNO-1]-   //cCommandParam1Ϊ��λ���
			(unsigned short*)pImageSlave,							  //���ݽṹ�׵�ַ
			(sizeof(CGyfxyImageSlave::YP_DATA) 
			)/sizeof(short),
			(unsigned short *)&pImageSlave->Sample[SampleNO-1] 
			); 
	
		if((WaitT35()==WAIT_TIMEOUT)&&((length=pModbusMaster->Respond(buf))!=0))			
		{
			if((iResult = pModbusMaster->DecodeWriteMultipleRegister(buf,length))>=0)
			{ 
				_stprintf(str,_T("����%d#���������ɹ�!"),SampleNO); 
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
