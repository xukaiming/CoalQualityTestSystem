// PageCurve.cpp : implementation file
//

#include "stdafx.h"
#include "lryctrl.h"
#include "PageCurve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageCurve property page

IMPLEMENT_DYNCREATE(CPageCurve, CPropertyPage)

CPageCurve::CPageCurve() : CPropertyPage(CPageCurve::IDD)
{
	//{{AFX_DATA_INIT(CPageCurve)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iCurveXMax=	25;
	bShowCurve  =	FALSE; 
	m_iPriod	=	10;  
	fCurvePtr	= 0;
#ifdef _DEBUG
	fileCurve	= NULL;
#endif
}

CPageCurve::~CPageCurve()
{ 
#ifdef _DEBUG
	if(fileCurve!=NULL)
		delete fileCurve;
#endif  
}

void CPageCurve::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageCurve)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageCurve, CPropertyPage)
	//{{AFX_MSG_MAP(CPageCurve)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IMAGE_DATA_CHANGE,OnImageDataChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageCurve message handlers

//void CPageCurve::DisCurve(float *pfCurve)
void CPageCurve::DisCurve( )
{
	double *pfCurve = m_pCtrl->pRdb->fCurve;
	CFont font;
	CFont*	pOldFont;
	int iHeight,i,v;
	CString s;
	int iStartX,iStartY,iCX,iCY,iSpaceY,iSpaceX,iNumX,iNumY;

	iNumX=12;         //横线个数
	iNumY=11;		  //竖线个数
	iStartX=25;		  //开始X坐标位置
	iStartY=10;		  //开始Y坐标位置
	iSpaceY=21;		  //X坐标单位宽度
	iSpaceX=25;       //Y坐标单位宽度
	iCX=iSpaceX*iNumX;
	iCY=iSpaceY*iNumY;

	CWnd * pWnd=GetDlgItem(IDC_TEST_QVXIAN);

	CDC * pControlDC=pWnd->GetDC();
//	pWnd->Invalidate();
	pWnd->UpdateWindow();
	iHeight=14;
	font.CreateFont(iHeight, 0, 0, 0, FW_NORMAL, false, false,
	                0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                DEFAULT_PITCH | FF_MODERN, _T("Times New Roman"));
	pOldFont = (CFont*) (pControlDC->SelectObject(&font));
	pControlDC->SetBkMode(TRANSPARENT);

	//显示曲线框
	CBrush *brush;
	brush = new CBrush(RGB(245,245,245));				// Create a solid or hatched brush.
	CBrush* oldBrush = pControlDC->SelectObject(brush);	// Select the new brush into the DC.
//	pControlDC->Rectangle(iStartX-1, -1, iCX+iStartX+1, iCY+1);			// Draw the rectangle.
	pControlDC->RoundRect(iStartX-1, iStartY-1, iCX+iStartX+1, iStartY+iCY+1,15,15);	// Draw the Round rectangle.
	pControlDC->SelectObject(oldBrush);					// Restore the DC and delete the brush.
	delete brush;
	//显示横坐标文字
	pControlDC->SetTextColor(RGB(128,128,0));
//	int maxt=12;//30,18,42,12,54
	for(i=0;i<iNumX/2+1;i++)
	{
		s.Format(_T("%02d"),i*2*m_iCurveXMax/iNumX);
		pControlDC->TextOut(iSpaceX*i*2+iStartX-6, iStartY+iCY, s);
	}
	pControlDC->TextOut(iSpaceX*(i-1)*2+iStartX+7, iStartY+iCY+1, "分");
	//显示纵坐标文字
//	int maxc=(int)(*pfCurve+5+0.5);
	int maxc=(int)(*pfCurve+5);
	pControlDC->TextOut(iStartX-16, iStartY-iHeight, "℃");
	for(i=0;i<iNumY/2+1;i++)
	{
		s.Format(_T("%02d"),maxc-i);
		pControlDC->TextOut(iStartX-16, iStartY+i*iSpaceY*2-iHeight/2, s);
	}
	pControlDC->SelectObject(pOldFont);
	//显示网格
	CPen pen(PS_SOLID, 1, RGB(220, 220, 220));
	CPen* oldPen = pControlDC->SelectObject(&pen);
	for(i=1;i<iNumY;i++)		//显示网格横线
	{
		pControlDC->MoveTo(iStartX, iStartY+iSpaceY*i);
		pControlDC->LineTo(iCX+iStartX, iStartY+iSpaceY*i);
	}
	for(i=1;i<iNumX;i++)		//显示网格竖线
	{
		pControlDC->MoveTo(iStartX+i*iSpaceX, iStartY);
		pControlDC->LineTo(iStartX+i*iSpaceX, iStartY+iCY);
	}
	//显示曲线
	CPen pen1(PS_SOLID, 1, RGB(225, 0, 0));
	oldPen = pControlDC->SelectObject(&pen1);

	float k=5.5f/iCY;
	pControlDC->MoveTo(iStartX, iStartY+(int)((maxc-*pfCurve)/k));
	//if(bShowCurve)
	{
		for(i=0;i<MAX_CURVE;i++)
		{
			v=(int)*(pfCurve+i);
			if(v<=0)
				break;
			int vv = (int)(1.0f*(maxc-*(pfCurve+i))/k);
			pControlDC->LineTo(iStartX+i, iStartY+vv);
			pControlDC->MoveTo(iStartX+i, iStartY+vv);
		}
		pControlDC->SelectObject(oldPen);
		if(i)
		{
			pControlDC->SetTextColor(RGB(0,128,128));
			s.Format(_T("%3.4f"),*(pfCurve+i-1));
			pControlDC->TextOut(iStartX+30,iStartY+2,s);	//显示当前温度
			//v=(int)pfCurve[i-1];
			//pControlDC->TextOut(iStartX+i-1,iCY-v,s);	//动态显示当前温度
		}
	}
	pWnd->ReleaseDC(pControlDC);
}

 

void CPageCurve::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DisCurve( );
}

LRESULT CPageCurve::OnImageDataChange(WPARAM sText, LPARAM lP)
{ 
	double *m_pfCurve = m_pCtrl->pRdb->fCurve;
	if((bShowCurve)&&(FlagDisp==0))
	{ 
		m_pfCurve[fCurvePtr] = m_pCtrl->pRdb->ft;
		fCurvePtr = (++fCurvePtr)%MAX_CURVE;
		m_pfCurve[fCurvePtr] = -1;
		Invalidate(TRUE);
	}
	FlagDisp=(++FlagDisp)%m_iPriod;
#ifdef _DEBUG
	if(bShowCurve)
		m_pCtrl->pRdb->SaveCurveToFile();
#endif
	return 0;
}

void CPageCurve::EnableUpdateCurve(BOOL bEnable,int iCurveXMax)
{	
	double *m_pfCurve	= m_pCtrl->pRdb->fCurve;
	m_iCurveXMax		= iCurveXMax/60; 
	bShowCurve			= bEnable;
	if(bEnable)
	{
		fCurvePtr			= 0;
		m_iPriod			= iCurveXMax*1000/MAX_CURVE/m_pCtrl->pRdb->m_lScanPriod_mS; 

		for(int i=0;i<MAX_CURVE;i++)
			m_pfCurve[i]=-1;
	}
}

BOOL CPageCurve::OnInitDialog() 
{
	CPropertyPage::OnInitDialog(); 
	return TRUE;  
}
