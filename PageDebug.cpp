// PageDebug.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "PageDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDebug property page

IMPLEMENT_DYNCREATE(CPageDebug, CPropertyPage)

CPageDebug::CPageDebug() : CPropertyPage(CPageDebug::IDD)
{
	//{{AFX_DATA_INIT(CPageDebug)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageDebug::~CPageDebug()
{
}

void CPageDebug::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDebug)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDebug, CPropertyPage)
	//{{AFX_MSG_MAP(CPageDebug)
	ON_BN_CLICKED(IDC_DEBUG_DIANHUO, OnDebugDianhuo)
	ON_BN_CLICKED(IDC_DEBUG_JINSHUI, OnDebugJinshui)
	ON_BN_CLICKED(IDC_DEBUG_FANGSHUI, OnDebugFangshui)
	ON_BN_CLICKED(IDC_DEBUG_JIAOBAN, OnDebugJiaoban)
	ON_BN_CLICKED(IDC_DEBUG_OUTJIAOBAN, OnDebugOutjiaoban)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IMAGE_DATA_CHANGE,OnImageDataChange)
	ON_BN_CLICKED(IDC_DEBUG_MODIFYID, &CPageDebug::OnBnClickedDebugModifyid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDebug message handlers

void CPageDebug::OnDebugDianhuo() 
{
	SLryImageSlave::_Output1  *pCoil =  &m_pCtrl->sLryImageSlave.OutputCoil.Output1;
	m_pCtrl->AddCommand(COMMAND_JDQ,CMD_FIRE|(pCoil->m_cFIRE?0:0x80));
}

void CPageDebug::OnDebugJinshui() 
{
	SLryImageSlave::_Output1  *pCoil =  &m_pCtrl->sLryImageSlave.OutputCoil.Output1;
	m_pCtrl->AddCommand(COMMAND_JDQ,CMD_FILLWATER|(pCoil->m_cFILLWATER?0:0x80)); 
}

void CPageDebug::OnDebugFangshui() 
{
	SLryImageSlave::_Output1  *pCoil =  &m_pCtrl->sLryImageSlave.OutputCoil.Output1;
	m_pCtrl->AddCommand(COMMAND_JDQ,CMD_OUTWATER|(pCoil->m_cOUTWATER?0:0x80)); 
}


void CPageDebug::OnDebugJiaoban() 
{
	SLryImageSlave::_Output1  *pCoil =  &m_pCtrl->sLryImageSlave.OutputCoil.Output1;
	m_pCtrl->AddCommand(COMMAND_JDQ,CMD_INNERJIAOBAN1|(pCoil->m_cINNERJIAOBAN1?0:0x80)); 
}

void CPageDebug::OnDebugOutjiaoban() 
{
	SLryImageSlave::_Output1  *pCoil =  &m_pCtrl->sLryImageSlave.OutputCoil.Output1;
	m_pCtrl->AddCommand(COMMAND_JDQ,CMD_OUTERJIAOBAN|(pCoil->m_cOUTERJIAOBAN?0:0x80)); 	
}

LRESULT CPageDebug::OnImageDataChange(WPARAM sText, LPARAM lP)
{

	int ADPtr	=		m_pCtrl->sLryImageSlave.UploadRegister.AD7714ValuePtr;
	ADPtr%=5;
	int ADValue = 		m_pCtrl->sLryImageSlave.UploadRegister.AD7714Value[ADPtr];
	CString strAD ;
	SLryImageSlave::_Output1  *pCoil =  &m_pCtrl->sLryImageSlave.OutputCoil.Output1;
	
	SetDlgItemText(IDC_DEBUG_DIANHUO,pCoil->m_cFIRE?_T("ֹͣ���"):_T("���"));
	SetDlgItemText(IDC_DEBUG_JINSHUI,pCoil->m_cFILLWATER?_T("ֹͣ��ˮ"):_T("��Ͱ��ˮ"));
	SetDlgItemText(IDC_DEBUG_FANGSHUI,pCoil->m_cOUTWATER?_T("ֹͣ��ˮ"):_T("��Ͱ��ˮ"));
	SetDlgItemText(IDC_DEBUG_JIAOBAN,pCoil->m_cINNERJIAOBAN1?_T("ֹͣ�ڽ���"):_T("��Ͱ����"));
	SetDlgItemText(IDC_DEBUG_OUTJIAOBAN,pCoil->m_cOUTERJIAOBAN?_T("ֹͣ�����"):_T("��Ͱ����"));
	
	strAD.Format(_T("%d"),m_pCtrl->pRdb->nSpeed);
	SetDlgItemText(IDC_DEBUG_SPEED,strAD); 

	strAD.Format(_T("%X"),m_pCtrl->pRdb->iAd);			//ADֵ
	SetDlgItemText(IDC_DEBUG_AD,strAD);

	strAD.Format(_T("%.1f"), m_pCtrl->pRdb->fRt) ;		//����ֵ
	SetDlgItemText(IDC_DEBUG_RT,strAD);

	strAD.Format(_T("%.5f"), m_pCtrl->pRdb->fAdV) ;		//��ѹֵ
	SetDlgItemText(IDC_DEBUG_V,strAD);

	strAD.Format(_T("%.5f"), m_pCtrl->pRdb->fty) ; //�¶�ֵ
	SetDlgItemText(IDC_DEBUG_WENDU2,strAD);

	strAD.Format(_T("%.5f"), m_pCtrl->pRdb->ft) ; //У���¶�ֵ
	SetDlgItemText(IDC_DEBUG_WENDU,strAD);
	  
	strAD.Format(_T("%d"), m_pCtrl->GetID());					//ID��
	SetDlgItemText(IDC_DEBUG_NEWID,strAD);		
	

	return 0;
}

 

BOOL CPageDebug::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDebug::OnBnClickedDebugModifyid()
{
	m_pCtrl->RemoveAllCommand();
	m_pCtrl->AddCommand(COMMAND_SETID,m_pCtrl->GetID()); 
}
