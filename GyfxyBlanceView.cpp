// GyfxyBlanceView.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "GyfxyBlanceView.h"


// CGyfxyBlanceView

IMPLEMENT_DYNCREATE(CGyfxyBlanceView, CFormView)

CGyfxyBlanceView::CGyfxyBlanceView()
	: CFormView(CGyfxyBlanceView::IDD)
{

}

CGyfxyBlanceView::~CGyfxyBlanceView()
{
}

void CGyfxyBlanceView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_POT, m_PotWight);
	DDX_Control(pDX, IDC_STATIC_BALANCE, m_BalanceWeight);
	DDX_Control(pDX, IDC_STATIC_SAMPLE, m_SampleWeight);
	DDX_Control(pDX, IDC_STATIC_POT_LABEL, Label1);
	DDX_Control(pDX, IDC_STATIC_BALANCE_LABEL, label2);
	DDX_Control(pDX, IDC_STATIC_SAMPLE_LABEL, label3);
}

BEGIN_MESSAGE_MAP(CGyfxyBlanceView, CFormView)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGyfxyBlanceView diagnostics

#ifdef _DEBUG
void CGyfxyBlanceView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGyfxyBlanceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
CGyfxyHostDoc_G5200  * CGyfxyBlanceView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGyfxyHostDoc_G5200)));
	return (CGyfxyHostDoc_G5200*)m_pDocument;
} 


#else
CGyfxyHostDoc_G5200  * CGyfxyBlanceView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGyfxyHostDoc_G5200)));
	return (CGyfxyHostDoc_G5200*)m_pDocument;
} 
#endif //_DEBUG
 

// CGyfxyBlanceView message handlers

void CGyfxyBlanceView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
}

void CGyfxyBlanceView::OnInitialUpdate()
{
	pGyfxyRdb = &GetDocument()->GyfxyRDB;
	pHostCtrl = GetDocument()->pHostCtrl;

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
	
	GetParentFrame()->ModifyStyle(FWS_ADDTOTITLE,0); 
	GetParentFrame()->SetWindowText(_T("天平实时重量"));
	//
	Label1.LoadFont(_T("微软雅黑"),50);
	label2.LoadFont(_T("微软雅黑"),50);
	label3.LoadFont(_T("微软雅黑"),100);
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


void CGyfxyBlanceView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	//位置
	//天平读数
	//样品重量
	//皮重
	int pos				= pGyfxyRdb->status.cSampleDiskCurrPos>0?pGyfxyRdb->status.cSampleDiskCurrPos-1:0;
	double WeightSample	= pGyfxyRdb->status.CurrWeight;
	double WeightPot	= pGyfxyRdb->SampleArray.Sample[pos].UQuality.Quality[0];

	CString strWeight;
	strWeight.Format(_T("%d#坩埚"),pos+1);
	Label1.SetWindowText(strWeight);
	strWeight.Format(_T("%6.4f"),WeightPot);
	m_PotWight.SetWindowText(strWeight);
	strWeight.Format(_T("%6.4f"),WeightSample);
	m_BalanceWeight.SetWindowText(strWeight);
	strWeight.Format(_T("%6.4f"),WeightSample-WeightPot);
	m_SampleWeight.SetWindowText(strWeight); 

	
	for (int i=1;i<3;i++){ 
		SetTimer(i,300*i,NULL);
	}
	
}
  
void CGyfxyBlanceView::OnTimer(UINT_PTR nIDEvent)
{
	int pos				= pGyfxyRdb->status.cSampleDiskCurrPos;
	pHostCtrl->AddCommand(COMMAND_GETQUALITY,pos,0,nIDEvent);
}