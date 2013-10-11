// GyfxyDebugView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyDebugView.h"


// CGyfxyDebugView

IMPLEMENT_DYNCREATE(CGyfxyDebugView, CFormView)

CGyfxyDebugView::CGyfxyDebugView()
	: CFormView(CGyfxyDebugView::IDD)
{

}

CGyfxyDebugView::~CGyfxyDebugView()
{
}

void CGyfxyDebugView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEFT_POS_TOP, m_bLeftStoveTop); 
	DDX_Control(pDX, IDC_LEFT_POS_BOT, m_bLeftStoveBot);
	DDX_Control(pDX, IDC_RIGHT_POS_TOP, m_bRightStoveTop);
	DDX_Control(pDX, IDC_RIGHT_POS_BOT, m_bRightStoveBot);
	DDX_Control(pDX, IDC_WATER_STOVE_POS_TOP, m_bWaterStoveTop);
	DDX_Control(pDX, IDC_WATER_STOVE_POS_BOT, m_bWaterStoveBot);  
}

BEGIN_MESSAGE_MAP(CGyfxyDebugView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_OXYGEN, &CGyfxyDebugView::OnBnClickedButtonLeftOxygen)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_UP, &CGyfxyDebugView::OnBnClickedButtonLeftUp)
	ON_BN_CLICKED(IDC_BUTTON_TARE, &CGyfxyDebugView::OnBnClickedButtonTare)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_OXYGEN, &CGyfxyDebugView::OnBnClickedButtonRightOxygen)
	ON_BN_CLICKED(IDC_BUTTON_WATER_STOVE_UP, &CGyfxyDebugView::OnBnClickedButtonWaterStoveUp)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_DN, &CGyfxyDebugView::OnBnClickedButtonLeftDn)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_UP, &CGyfxyDebugView::OnBnClickedButtonRightUp)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_DN, &CGyfxyDebugView::OnBnClickedButtonRightDn)
	ON_BN_CLICKED(IDC_BUTTON_WATER_STOVE_DN, &CGyfxyDebugView::OnBnClickedButtonWaterStoveDn)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_FAN, &CGyfxyDebugView::OnBnClickedButtonLeftFan)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_FAN, &CGyfxyDebugView::OnBnClickedButtonRightFan)
	ON_BN_CLICKED(IDC_BUTTON_WATER_STOVE, &CGyfxyDebugView::OnBnClickedButtonWaterStove) 
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE_ROUND, &CGyfxyDebugView::OnBnClickedButtonSampleRound) 
	ON_BN_CLICKED(IDC_BUTTON_SAMPLENO_MOVE, &CGyfxyDebugView::OnBnClickedButtonSamplenoMove)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_WARM, &CGyfxyDebugView::OnBnClickedButtonLeftWarm)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_WARM, &CGyfxyDebugView::OnBnClickedButtonRightWarm)
	ON_BN_CLICKED(IDC_BUTTON_WATER_STOVE_WARM, &CGyfxyDebugView::OnBnClickedButtonWaterStoveWarm)
	ON_BN_CLICKED(IDC_BUTTON_SETID, &CGyfxyDebugView::OnBnClickedButtonSetid)
END_MESSAGE_MAP()


// CGyfxyDebugView diagnostics

#ifdef _DEBUG
void CGyfxyDebugView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGyfxyDebugView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGyfxyDebugView message handlers

void CGyfxyDebugView::OnInitialUpdate()
{
	
	////////////////////////////////////////////////
	CGyfxyHostDoc* pDoc = (CGyfxyHostDoc*)GetDocument();
	pGyfxyRdb			= &pDoc->GyfxyRDB;
	pHostCtrl			= pDoc->pHostCtrl;

	///////////////////////////////////////////////
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
	// TODO: Add your specialized code here and/or call the base class
	InitParam();
	CFormView::OnInitialUpdate();
}



void CGyfxyDebugView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = GetDlgItem(IDC_STATIC_TEMP);

	pGyfxyRdb = &((CGyfxyHostDoc*)GetDocument())->GyfxyRDB;
	CString strText;
	
	int i = pGyfxyRdb->status.Temperature[LEFT]+0.5f;
	SetDlgItemInt(IDC_EDIT_LEFT_TEMP,i);
	i = pGyfxyRdb->status.Temperature[MIDDLE]+0.5f;
	SetDlgItemInt(IDC_EDIT_WATER_STOVE_TEMP,i);
	i = pGyfxyRdb->status.Temperature[RIGHT]+0.5f;
	SetDlgItemInt(IDC_EDIT_RIGHT_TEMP,i);

	CGyfxyImageSlave::CFuzzy * pFuzzy	=  pHostCtrl->pImageSlave->fuzzy;
	GetDlgItem(IDC_BUTTON_LEFT_WARM)->SetWindowText(pFuzzy[LEFT_STOVE].WarmFlag?_T("停止"):_T("升温"));
	GetDlgItem(IDC_BUTTON_RIGHT_WARM)->SetWindowText(pFuzzy[RIGHT_STOVE].WarmFlag?_T("停止"):_T("升温"));
	GetDlgItem(IDC_BUTTON_WATER_STOVE_WARM)->SetWindowText(pFuzzy[WATER_STOVE].WarmFlag?_T("停止"):_T("升温"));
	

	double t = pGyfxyRdb->status.TempVoltage[LEFT];
	strText.Format(_T("%.2f"),pGyfxyRdb->status.TempVoltage[LEFT]); 
	GetDlgItem(IDC_EDIT_LEFT_MV)->SetWindowText(strText);

	t = pGyfxyRdb->status.TempVoltage[MIDDLE];
	strText.Format(_T("%.2f"),pGyfxyRdb->status.TempVoltage[MIDDLE]); 
	GetDlgItem(IDC_EDIT_WATER_STOVE_MV)->SetWindowText(strText);

	t = pGyfxyRdb->status.TempVoltage[RIGHT];
	strText.Format(_T("%.2f"),pGyfxyRdb->status.TempVoltage[RIGHT]); 
	GetDlgItem(IDC_EDIT_RIGHT_MV)->SetWindowText(strText);

	SetDlgItemInt(IDC_EDIT_CUR_POS,pGyfxyRdb->status.cSampleDiskCurrPos);
	GetDlgItem(IDC_BUTTON_SAMPLE_ROUND)->SetWindowText(pGyfxyRdb->status.bSampleDiskMoving?_T("停止"):_T("旋转"));
	GetDlgItem(IDC_BUTTON_SAMPLENO_MOVE)->SetWindowText(pGyfxyRdb->status.bSampleDiskMoving?_T("停止"):_T("移动"));
	
	
	CGyfxyImageSlave::_InputStatus * pInport	=  &pHostCtrl->pImageSlave->InputStatus;
	m_bLeftStoveTop.Load(pInport->LEFT_STOVE_TOP?IDB_PNG_ORG:IDB_PNG_GREY); 
	m_bLeftStoveBot.Load(pInport->LEFT_STOVE_BOT?IDB_PNG_ORG:IDB_PNG_GREY); 
	m_bRightStoveTop.Load(pInport->RIGHT_STOVE_TOP?IDB_PNG_ORG:IDB_PNG_GREY);
	m_bRightStoveBot.Load(pInport->RIGHT_STOVE_BOT?IDB_PNG_ORG:IDB_PNG_GREY);
	m_bWaterStoveTop.Load(pInport->WATER_STOVE_PUT?IDB_PNG_ORG:IDB_PNG_GREY);
	m_bWaterStoveBot.Load(pInport->WATER_STOVE_WEIGH?IDB_PNG_ORG:IDB_PNG_GREY);

	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;
	GetDlgItem(IDC_BUTTON_LEFT_OXYGEN)->SetWindowText(pOutport->LEFT_OXYGEN?_T("氧气关"):_T("氧气开"));
	GetDlgItem(IDC_BUTTON_RIGHT_OXYGEN)->SetWindowText(pOutport->RIGHT_OXYGEN?_T("氮气关"):_T("氮气开"));
	GetDlgItem(IDC_BUTTON_LEFT_FAN)->SetWindowText(pOutport->LEFT_STOVE_FAN?_T("左炉风扇关"):_T("左炉风扇开"));
	GetDlgItem(IDC_BUTTON_RIGHT_FAN)->SetWindowText(pOutport->RIGHT_STOVE_FAN?_T("右炉风扇关"):_T("右炉风扇开"));
	GetDlgItem(IDC_BUTTON_WATER_STOVE)->SetWindowText(pOutport->WATER_STOVE_FAN?_T("恒温炉风扇关"):_T("恒温炉风扇开"));

 	GetDlgItem(IDC_BUTTON_LEFT_UP)->SetWindowText(pOutport->LEFT_MOTOR_UP?_T("停止"):_T("上升"));
	GetDlgItem(IDC_BUTTON_LEFT_DN)->SetWindowText(pOutport->LEFT_MOTOR_DN?_T("停止"):_T("下降"));

	GetDlgItem(IDC_BUTTON_RIGHT_UP)->SetWindowText(pOutport->RIGHT_MOTOR_UP?_T("停止"):_T("上升"));
	GetDlgItem(IDC_BUTTON_RIGHT_DN)->SetWindowText(pOutport->RIGHT_MOTOR_DN?_T("停止"):_T("下降"));

	GetDlgItem(IDC_BUTTON_WATER_STOVE_UP)->SetWindowText(pOutport->BALANCE_MOTOR_UP?_T("停止"):_T("上升"));
	GetDlgItem(IDC_BUTTON_WATER_STOVE_DN)->SetWindowText(pOutport->BALANCE_MOTOR_DN?_T("停止"):_T("下降"));

	

	double j = pGyfxyRdb->status.CurrWeight;
	strText.Format(_T("%.4fg"),j); 
	GetDlgItem(IDC_EDIT_WEIGHT)->SetWindowText(strText);

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_LIST_WEIGHT);
	if(pListBox->GetCount()>MAX_WEIGHT_BUF)
	{
		pListBox->DeleteString(MAX_WEIGHT_BUF-1);
	}
	pListBox->InsertString(0,strText);


}

void CGyfxyDebugView::InitParam(void)
{
	CComboBox * pWnd = ((CComboBox*)GetDlgItem(IDC_COMBO_DEST));
	pWnd->AddString(_T("下一工位"));
	for(int i=1;i<25;i++)
	{
		CString str;
		str.Format(_T("%d"),i);
		pWnd->AddString(str); 
	}
	pWnd->SetCurSel(0);
	//////////////////////////////////////////////
	memset(DestTemp,0,sizeof(DestTemp));
	//////////////////////////////////////////////
	SetDlgItemInt( IDC_EDIT_WATER_STOVE_DEST,105);
	SetDlgItemInt( IDC_EDIT_RIGHT_DEST,850);
	SetDlgItemInt( IDC_EDIT_LEFT_DEST,920);

	SetDlgItemInt(IDC_EDIT_ID,pGyfxyRdb->m_bID);
}

void CGyfxyDebugView::OnBnClickedButtonTare()
{ 
	pHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_TARE,0);
}

void CGyfxyDebugView::OnBnClickedButtonRightOxygen()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	pHostCtrl->AddCommand(COMMAND_JDQ,COIL_RIGHT_NITROGEN,pOutport->RIGHT_OXYGEN?0:1);
}

void CGyfxyDebugView::OnBnClickedButtonLeftOxygen()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	pHostCtrl->AddCommand(COMMAND_JDQ,COIL_LEFT_OXYGEN,pOutport->LEFT_OXYGEN?0:1);
		/*
		
		#define  COIL_BALANCE_MOTOR_UP	0;		//称样上升
		#define  COIL_BALANCE_MOTOR_DN	1;		//称样下降
		#define  COIL_LEFT_MOTOR_UP		2;		//左炉上升
		#define  COIL_LEFT_MOTOR_DN		3;		//左炉下降

		#define  COIL_RIGHT_MOTOR_UP	4;		//右炉上升
		#define  COIL_RIGHT_MOTOR_DN	5;		//右炉下降	

		#define  COIL_WATER_STOVE_FAN	:8;		//水分炉子交流风扇 
		#define  COIL_LEFT_STOVE_FAN	:11;		//左炉风扇

		#define  COIL_RIGHT_STOVE_FAN	:12;		//右炉风扇
		#define  COIL_SPK				:13;		//蜂鸣器*/
}

void CGyfxyDebugView::OnBnClickedButtonLeftUp()
{ 

	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  	
	
	
	//cCommandParam1;    //电机编号
	//cCommandParam2;	 //是否允许
	//cCommandParam3;    //电机运行目标
	if(GetAsyncKeyState(VK_LCONTROL))
		pHostCtrl->AddCommand(COMMAND_JDQ,COIL_LEFT_MOTOR_UP,pOutport->LEFT_MOTOR_UP?0:1);
	else
		pHostCtrl->AddCommand(COMMAND_MTR,LEFT_STOVE,pOutport->LEFT_MOTOR_DN+pOutport->LEFT_MOTOR_UP?0:1,CGyfxyRDB_G5200::CStatus::POS_TOP);
}


void CGyfxyDebugView::OnBnClickedButtonLeftDn()
{ 
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	if(GetAsyncKeyState(VK_LCONTROL))
		pHostCtrl->AddCommand(COMMAND_JDQ,COIL_LEFT_MOTOR_DN,pOutport->LEFT_MOTOR_DN?0:1);
	else
		pHostCtrl->AddCommand(COMMAND_MTR,LEFT_STOVE,pOutport->LEFT_MOTOR_DN+pOutport->LEFT_MOTOR_UP?0:1,CGyfxyRDB_G5200::CStatus::POS_BOT);
}

void CGyfxyDebugView::OnBnClickedButtonWaterStoveUp()
{ 
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	if(GetAsyncKeyState(VK_LCONTROL))
		pHostCtrl->AddCommand(COMMAND_JDQ,COIL_BALANCE_MOTOR_UP,pOutport->BALANCE_MOTOR_UP?0:1);
	else
		pHostCtrl->AddCommand(COMMAND_MTR,WATER_STOVE,pOutport->BALANCE_MOTOR_UP+pOutport->BALANCE_MOTOR_DN?0:1,CGyfxyRDB_G5200::CStatus::POS_TOP);
}

void CGyfxyDebugView::OnBnClickedButtonWaterStoveDn()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	if(GetAsyncKeyState(VK_LCONTROL))
		pHostCtrl->AddCommand(COMMAND_JDQ,COIL_BALANCE_MOTOR_DN,pOutport->BALANCE_MOTOR_DN?0:1);
	else
		pHostCtrl->AddCommand(COMMAND_MTR,WATER_STOVE,pOutport->BALANCE_MOTOR_UP+pOutport->BALANCE_MOTOR_DN?0:1,CGyfxyRDB_G5200::CStatus::POS_BOT);
	//
}


void CGyfxyDebugView::OnBnClickedButtonRightUp()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	if(GetAsyncKeyState(VK_LCONTROL))
		pHostCtrl->AddCommand(COMMAND_JDQ,COIL_RIGHT_MOTOR_UP,pOutport->RIGHT_MOTOR_UP?0:1);
	else
		pHostCtrl->AddCommand(COMMAND_MTR,RIGHT_STOVE,pOutport->RIGHT_MOTOR_DN+pOutport->RIGHT_MOTOR_UP?0:1,CGyfxyRDB_G5200::CStatus::POS_TOP);
}

void CGyfxyDebugView::OnBnClickedButtonRightDn()
{

	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	if(GetAsyncKeyState(VK_LCONTROL))
		pHostCtrl->AddCommand(COMMAND_JDQ,COIL_RIGHT_MOTOR_DN,pOutport->RIGHT_MOTOR_DN?0:1);
	else
		pHostCtrl->AddCommand(COMMAND_MTR,RIGHT_STOVE,pOutport->RIGHT_MOTOR_DN+pOutport->RIGHT_MOTOR_UP?0:1,CGyfxyRDB_G5200::CStatus::POS_BOT);
}


void CGyfxyDebugView::OnBnClickedButtonLeftFan()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	pHostCtrl->AddCommand(COMMAND_JDQ,COIL_LEFT_STOVE_FAN,pOutport->LEFT_STOVE_FAN?0:1);
}

void CGyfxyDebugView::OnBnClickedButtonRightFan()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	pHostCtrl->AddCommand(COMMAND_JDQ,COIL_RIGHT_STOVE_FAN,pOutport->RIGHT_STOVE_FAN?0:1);
}

void CGyfxyDebugView::OnBnClickedButtonWaterStove()
{
	CGyfxyImageSlave::_OutputCoil * pOutport	=  &pHostCtrl->pImageSlave->OutputCoil;  
	pHostCtrl->AddCommand(COMMAND_JDQ,COIL_WATER_STOVE_FAN,pOutport->WATER_STOVE_FAN?0:1);
}
 

void CGyfxyDebugView::OnBnClickedButtonSampleRound()
{
	pHostCtrl->AddCommand(COMMAND_MTR_POS,0,pGyfxyRdb->status.bSampleDiskMoving?0:1); 
}
 
void CGyfxyDebugView::OnBnClickedButtonSamplenoMove()
{
		CComboBox *pWnd = (CComboBox *)GetDlgItem(IDC_COMBO_DEST);
		int i = pWnd->GetCurSel();
		if(i==0)	 
			i = pGyfxyRdb->status.cSampleDiskCurrPos+1;	
		pHostCtrl->AddCommand(COMMAND_MTR_POS,i,pGyfxyRdb->status.bSampleDiskMoving?0:1); 
}

void CGyfxyDebugView::OnBnClickedButtonLeftWarm()
{
	// TODO: Add your control notification handler code here
	//pHostCtrl->AddCommand(COMMAND_MTR_POS,i,pGyfxyRdb->status.bSampleDiskMoving?0:1); 
	//long	DestTemp[3];
	CGyfxyImageSlave::CFuzzy * pFuzzy	=  pHostCtrl->pImageSlave->fuzzy;
	int SW	= !pFuzzy[LEFT_STOVE].WarmFlag;
	DestTemp[LEFT_STOVE] = GetDlgItemInt( IDC_EDIT_LEFT_DEST);
	pHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_WARMUP,LEFT_STOVE ,SW,&DestTemp[LEFT_STOVE]);
}

void CGyfxyDebugView::OnBnClickedButtonRightWarm()
{
	CGyfxyImageSlave::CFuzzy * pFuzzy	=  pHostCtrl->pImageSlave->fuzzy;
	int SW	= !pFuzzy[RIGHT_STOVE].WarmFlag;
	DestTemp[RIGHT_STOVE] = GetDlgItemInt( IDC_EDIT_RIGHT_DEST);
	pHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_WARMUP,RIGHT_STOVE ,SW,&DestTemp[RIGHT_STOVE]);

}

void CGyfxyDebugView::OnBnClickedButtonWaterStoveWarm()
{
	CGyfxyImageSlave::CFuzzy * pFuzzy	=  pHostCtrl->pImageSlave->fuzzy;
	int SW	= !pFuzzy[WATER_STOVE].WarmFlag;
	DestTemp[WATER_STOVE] = GetDlgItemInt( IDC_EDIT_WATER_STOVE_DEST);
	pHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_WARMUP,WATER_STOVE ,SW,&DestTemp[WATER_STOVE]);
}

void CGyfxyDebugView::OnBnClickedButtonSetid()
{
	int id = 0xFF;
	id = GetDlgItemInt(IDC_EDIT_ID);

	pHostCtrl->RemoveAllCommand();
	pHostCtrl->AddCommand(COMMAND_USER_DEFINE,CGyfxyHostCtrl::CMD_SETID,id);

}
