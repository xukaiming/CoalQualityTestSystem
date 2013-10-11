// ClyDebugView.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "ClyDebugView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CLyHostDoc.h"

IMPLEMENT_DYNCREATE(CClyDebugView, CFormView)

CClyDebugView::CClyDebugView()
: CFormView(CClyDebugView::IDD)
{
	pHexFont = new CFont();
	pHexFont->CreateFont(16, 8, 0, 0, FW_NORMAL, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN, NULL);//_T("Times New Roman")
	
}

CClyDebugView::~CClyDebugView()
{ 
	delete pHexFont;
}

/////////////////////////////////////////////////////////////////////////////
// CClyDebugView message handlers
/////////////////////////////////////////////////////////////////////////////
// CClYDebugView
BEGIN_MESSAGE_MAP(CClyDebugView, CFormView)
	//{{AFX_MSG_MAP(CClyDebugView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP 
	ON_BN_CLICKED(IDC_BTN_ID, &CClyDebugView::OnBnClickedBtnId)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_FREQ_ADJ, &CClyDebugView::OnBnClickedButtonFreqAdj)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CClyDebugView::OnBnClickedButtonMove)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_POS_MOV, &CClyDebugView::OnBnClickedBtnPosMov)
//	ON_EN_CHANGE(IDC_EDIT_CURR_POS, &CClyDebugView::OnEnChangeEditCurrPos)
ON_BN_CLICKED(IDC_BTN_POS_MOV2, &CClyDebugView::OnBnClickedBtnPosMov2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClyDebugView drawing

void CClyDebugView::OnDraw(CDC* pDC)
{
	CClyHostDoc* pDoc = (CClyHostDoc*)GetDocument(); 
}

/////////////////////////////////////////////////////////////////////////////
// CClyDebugView diagnostics

#ifdef _DEBUG
void CClyDebugView::AssertValid() const
{
	CFormView::AssertValid();
}

void CClyDebugView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG



 
void CClyDebugView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLYDebugView)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_POS1, m_pPos1);
	DDX_Control(pDX, IDC_STATIC_POSN, m_pPosN);
	DDX_Control(pDX, IDC_STATIC_PUT_POS, m_pPosPutSamplePos);
	DDX_Control(pDX, IDC_STATIC_HIGHT_POS, m_pPosHighTemp); 
}


/////////////////////////////////////////////////////////////////////////////
// CCLYDebugView message handlers

void CClyDebugView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy); 
}

void CClyDebugView::OnInitialUpdate() 
{
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);		 
	CComboBoxEx *pComboBox  = (CComboBoxEx*)GetDlgItem(IDC_COMBO_DEST_POS);
	SetDlgItemInt(IDC_EDIT_DEST_POS,1);
	pComboBox->SetCurSel(0);		
	
	GetDlgItem(IDC_EDIT_SENSOR_AD)->SetFont(pHexFont);
	CClyHostDoc* pDoc = (CClyHostDoc*)GetDocument();
	pClyRdb		= &pDoc->ClyRDB;
	pHostCtrl	= pDoc->pHostCtrl;
	CFormView::OnInitialUpdate();
}




void CClyDebugView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{	 
	CWnd *pWnd = GetDlgItem(IDC_STATIC_TEMP);

	pClyRdb = &((CClyHostDoc*)GetDocument())->ClyRDB;
	CString strText;

	 
	//int i = pClyRdb->status.Temperature; 

	strText.Format(_T("%.1f"),pClyRdb->status.Temperature);
	GetDlgItem(IDC_STATIC_TEMP)->SetWindowText(strText);

	strText.Format(_T("%.3f"),pClyRdb->status.TempVoltage);
	GetDlgItem(IDC_EDIT_TEMP_VOL)->SetWindowText(strText);

	strText.Format(_T("0x%03X"),pClyRdb->status.TempAdValue);
	GetDlgItem(IDC_STATIC_TEMP_AD)->SetWindowText(strText);

	strText.Format(_T("0x%03X"),pClyRdb->status.SensorAdValue);
	GetDlgItem(IDC_EDIT_SENSOR_AD)->SetWindowText(strText);

	strText.Format(_T("%.3f"),pClyRdb->status.SensorVoltage);
	GetDlgItem(IDC_EDIT_SENSOR)->SetWindowText(strText);
	SetDlgItemInt(IDC_EDIT_FREQ,pClyRdb->status.DJFrequency);
	strText.Format(_T("%.3f"),pClyRdb->status.DJCurrent);
	GetDlgItem(IDC_EDIT_CURRENT)->SetWindowText(strText);
	//////////
	SetDlgItemText(IDC_EDIT_CUR_POS,pClyRdb->status.strCurrPos);
	SetDlgItemInt(IDC_EDIT_ID,pClyRdb->m_bID);
	CString strCap = pClyRdb->status.DJSwitch?_T("ֹͣ"):_T("У׼");
	SetDlgItemText(IDC_BUTTON_FREQ_ADJ,strCap);
	strCap = pClyRdb->status.bMoving?_T("ֹͣ"):_T("�ƶ�");
	SetDlgItemText(IDC_BUTTON_MOVE,strCap);
	///////////////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_EDIT_CURR_POS,pClyRdb->status.strSampleDiskCurrPos);   
	strCap = pClyRdb->status.bSampleDiskMoving?_T("ֹͣ"):_T("�ƶ�");
	SetDlgItemText(IDC_BTN_POS_MOV,strCap);

	CClyImageSlave::_InputStatus *pin	=  &pHostCtrl->pImageSlave->InputStatus;
	m_pPos1.Load(pin->DC_SAMPLE_NO1?IDB_PNG_ORG:IDB_PNG_GREY);
	m_pPosN.Load(pin->DC_SAMPLE_NON?IDB_PNG_ORG:IDB_PNG_GREY);
	m_pPosHighTemp.Load(pin->DC_POS_HIGH_TEMP?IDB_PNG_ORG:IDB_PNG_GREY);
	m_pPosPutSamplePos.Load(pin->DC_POS_PUT_SAMPLE?IDB_PNG_ORG:IDB_PNG_GREY);
	
 
}

 

void CClyDebugView::OnBnClickedBtnId()
{ 
	pHostCtrl->RemoveAllCommand();
	pHostCtrl->AddCommand(COMMAND_SID,pClyRdb->m_bID); 
}

void CClyDebugView::OnPaint()
{
	//CPaintDC dc(this); // device context for painting 
	// Do not call CFormView::OnPaint() for painting messages
	CPaintDC dc(this); 
	
	OnPrepareDC(&dc);
	OnDraw(&dc); 
}

void CClyDebugView::OnBnClickedButtonFreqAdj()
{ 
	//    struct
	CClyImageSlave::FREQ *pFreq = &pHostCtrl->pImageSlave->Freq; 
	int DJCurrent = 100L;
	pHostCtrl->AddCommand(COMMAND_DJ,pFreq->DJSwitch?0:1,(DJCurrent>>8),DJCurrent&0xFF); //20mA
}

void CClyDebugView::OnBnClickedButtonMove()
{ 
	CComboBoxEx *pCombox = (CComboBoxEx*)GetDlgItem(IDC_COMBO_DEST_POS);
	int Dest =	pCombox->GetCurSel()+1;
	
	pHostCtrl->AddCommand(COMMAND_MTR,pClyRdb->status.bMoving?0:Dest); 
}

void CClyDebugView::OnDestroy()
{
	CFormView::OnDestroy();
 
}

void CClyDebugView::OnBnClickedBtnPosMov()
{
	CComboBoxEx *pCombox = (CComboBoxEx*)GetDlgItem(IDC_COMBO_DEST_POS);
	int Dest = GetDlgItemInt(IDC_EDIT_DEST_POS); 
	pHostCtrl->AddCommand(COMMAND_MTR_POS,pClyRdb->status.bSampleDiskMoving?0:Dest); 
}
 

void CClyDebugView::OnBnClickedBtnPosMov2()
{ 
	pHostCtrl->AddCommand(COMMAND_MTR_POS,pClyRdb->status.cSampleDiskCurrPos+3); 
	SetDlgItemInt(IDC_EDIT_DEST_POS,(pClyRdb->status.cSampleDiskCurrPos+2)%24+1);
}
