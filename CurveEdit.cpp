// CurveEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "CurveEdit.h"


#include "TextProgressCtrl.h "
// CCurveEdit 

IMPLEMENT_DYNAMIC(CCurveEdit, CWnd)

CCurveEdit::CCurveEdit()
: XAxial(360)
,BlodScaleX(60)
,YAxial(450)
,BlodScaleY(150)
,XAxialUnit(_T("S"))
,YAxialUnit(_T("mA"))
,DataShowMax(450)
,DataShowMin(0)
,SlopeX(1.0)
,SlopeY(1.0)
,offsetX(0) 
,CurveBackgroundColor(Gdiplus::Color::DarkGray)
,CurveColor(Gdiplus::Color::Gold)
,StaffSize(25) 
,DataShowBeginPosition(480)
,DataShowEndPosition(0)
,pRDB(NULL)
,fontFamily((L"Times New Roman"))
,font(&fontFamily, 15, FontStyleRegular, UnitPixel)
{
	CanvasBitmap			= NULL; 
	m_clrBorderColor.SetFromCOLORREF(GetSysColor(COLOR_ACTIVEBORDER));
	StaffBackgroundColor.SetFromCOLORREF(GetSysColor(COLOR_BTNFACE));			//坐标轴背景颜色
	StaffColor.SetFromCOLORREF(GetSysColor(COLOR_GRADIENTACTIVECAPTION));					//坐标轴颜色 	 
	//CurveBackgroundColor	= Gdiplus::Color::DarkGray;
	//PlotDatas.resize(480);					//480点
	//PlotDatas.SetSize(500);
}

CCurveEdit::~CCurveEdit()
{
	ClearData();
	if(CanvasBitmap!=NULL)
		delete CanvasBitmap;
}


BEGIN_MESSAGE_MAP(CCurveEdit, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
//	ON_WM_CREATE()
END_MESSAGE_MAP()



// CCurveEdit message handlers



void CCurveEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	Gdiplus::SolidBrush StaffBrush(StaffBackgroundColor);
	Gdiplus::SolidBrush CurveBrush(CurveBackgroundColor);	
	int i;
	GetClientRect(&rect);   
	Gdiplus::Graphics ClientGraphics(dc.m_hDC);	
	///////////////////////////////////////////////////////////////////////////////////////////// 
	if((CanvasBitmap==NULL)||(
		CanvasBitmap->GetWidth()!=rect.Width() || CanvasBitmap->GetHeight()!=rect.Height()))
	{
		delete CanvasBitmap; 
		CanvasBitmap=new Gdiplus::Bitmap(rect.Width(),rect.Height());
		////////////////////////////////////////////////////////////////////////////////////////////	 
		graphics = new Gdiplus::Graphics(CanvasBitmap);
		/////////////////////////////////////////////////////////////////////////////////////////////
		//DrawRoundedRectangle(graphics,rect.Width(),rect.Height(),15);									//绘制外框
		/////////////////////////////////绘制坐标轴/

		graphics->FillRectangle(&StaffBrush,0,0,StaffSize,rect.Height()-StaffSize);						//纵坐标背景擦除
		graphics->FillRectangle(&StaffBrush,0,rect.Height()-StaffSize,rect.Width(),rect.Height());		//横坐标背景擦除	


		graphics->FillRectangle(&CurveBrush,StaffSize,0,rect.Width()-StaffSize,rect.Height()-StaffSize);	//曲线背景擦除
		DrawStaff(graphics);																			//绘制坐标
		delete graphics;

		
	} 
	Gdiplus::Bitmap *bitmap = CanvasBitmap->Clone(0,0,rect.right,rect.bottom,PixelFormatDontCare);  //拷贝图像
	Gdiplus::Graphics gs(bitmap);	 			
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	gs.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);//
	/////////////////////////////////////	  	
	DrawValue(&gs);		//绘制曲线
	DrawString(&gs); 
	Gdiplus::CachedBitmap CanvasBitmapCachedBitmap(bitmap,&ClientGraphics);
	ClientGraphics.DrawCachedBitmap(&CanvasBitmapCachedBitmap,0,0); 
	delete bitmap; 
	
	
}

BOOL CCurveEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

#define  pStrTemp  strTemp.AllocSysString() 
void CCurveEdit::DrawString(Gdiplus::Graphics* graphics)
{
	SolidBrush solidBrush(Color(100,100,100));
	SolidBrush RedBrush(Color(255,0,0));
	CString strTemp ;	 
	///////////////////////////////////// 绘制显示的字
	WCHAR string1[] = L"        温度"; 
	WCHAR string2[] = L"电解电流"; 
	WCHAR string3[] = L"    硫含量"; 
	graphics->DrawString(string1, (INT)wcslen(string1), &font, PointF(rect.Width()-150, 10), &solidBrush);	
	if(pRDB!=NULL)strTemp.Format(_T("%.f℃"),pRDB->status.Temperature); 
	//pStrTemp = strTemp.AllocSysString();
	graphics->DrawString(pStrTemp, (INT)wcslen(pStrTemp), &font, PointF(rect.Width()-80, 10), &RedBrush);
	//如果正在工作,报告出电解电流,大致硫含量
	{
		if(pRDB!=NULL)	strTemp.Format(_T("%.f%s"),pRDB->status.DJCurrent,YAxialUnit); 
		graphics->DrawString(pStrTemp, (INT)wcslen(pStrTemp), &font, PointF(rect.Width()-80, 30), &RedBrush);
		graphics->DrawString(string2, (INT)wcslen(string2), &font, PointF(rect.Width()-150, 30), &solidBrush);
		if(pRDB!=NULL)	strTemp.Format(_T("%.3fmg"),pRDB->status.SQuality);
		
		graphics->DrawString(pStrTemp, (INT)wcslen(pStrTemp), &font, PointF(rect.Width()-80, 50), &RedBrush);
		graphics->DrawString(string3, (INT)wcslen(string3), &font, PointF(rect.Width()-150, 50), &solidBrush);
	}

}

#define pStrStaff  StaffString.AllocSysString()
void CCurveEdit::DrawStaff(Gdiplus::Graphics* graphics)
{	
	 
 
	CString strTemp ;


	Gdiplus::Pen BoldStaffPen(StaffColor,2);
	Gdiplus::Pen StaffPen(StaffColor);

	Pen penGrid(Color(220, 220, 220),1);
	Pen pen(Color(255,255,0,0),2); //createpen 
	SolidBrush AxiasBrush(Color(128,128,0));//RGB(128,128,0)
	Gdiplus::SolidBrush StringBrush(StaffColor);
	CString StaffString;
	Gdiplus::PointF StringPosition;

	Gdiplus::StringFormat VerticalStaffStringFormat;
	VerticalStaffStringFormat.SetFormatFlags(Gdiplus::StringFormatFlagsDirectionVertical);
	graphics->DrawLine(&BoldStaffPen,StaffSize,0,StaffSize,rect.Height()-StaffSize);
	graphics->DrawLine(&BoldStaffPen,StaffSize,rect.Height()-StaffSize,rect.Width(),rect.Height()-StaffSize);
	for (int i=0;i<=YAxial;i++)
	{
		if (i%BlodScaleY==0)
		{
			float x1=StaffSize/5.0f*2.0f;
			float y1=rect.Height()-StaffSize-(i*(float)(rect.Height()-StaffSize-1)/(float)YAxial);
			float x2=StaffSize;
			float y2=y1;
			graphics->DrawLine(&BoldStaffPen,x1,y1,x2,y2);
			if(i>0)graphics->DrawLine(&penGrid,(float)(StaffSize-1),y1,(float)(rect.Width()),y2);

			//float Value=((float)i*(DataShowMax-DataShowMin)/YAxial+DataShowMin)*SlopeX;
			float Value=((float)i*(DataShowMax-DataShowMin)/YAxial+DataShowMin)*SlopeX;
			StaffString.Format(TEXT("%.0f"),Value);
			StringPosition = Gdiplus::PointF(-StaffSize/7,y1);
			graphics->DrawString(pStrStaff,wcslen(pStrStaff),&font,StringPosition,&VerticalStaffStringFormat,&StringBrush);
		}
		else if(i%(BlodScaleY/2)==0)
		{
			float x1=StaffSize/5.0f*3.0f;
			float y1=rect.Height()-StaffSize-(i*(float)(rect.Height()-StaffSize-1)/(float)YAxial);
			float x2=StaffSize;
			float y2=y1;
			graphics->DrawLine(&StaffPen,x1,y1,x2,y2);
		}
	}
	StringPosition.X+=StaffSize+5; 
	StaffString.Format(TEXT("%s"),YAxialUnit);
	//graphics->DrawString(StaffString,StaffString.GetLength(),&font,StringPosition,&StringBrush);
	graphics->DrawString(pStrStaff,wcslen(pStrStaff),&font,StringPosition,&StringBrush);
	for (int i=0;i<=XAxial;i++)
	{
		if (i%BlodScaleX==0)
		{
			float x1=StaffSize+i*(float)(rect.Width()-StaffSize-1)/(float)XAxial;
			float y1=rect.Height()-StaffSize;
			float x2=x1;
			float y2=rect.Height()-StaffSize+StaffSize/5*3;
			graphics->DrawLine(&BoldStaffPen,x1,y1,x2,y2);
			if(i>0)graphics->DrawLine(&penGrid,x1,0.0,x1,(float)(rect.Height()-StaffSize-1)); //背景
			float Value=(i*(DataShowBeginPosition-DataShowEndPosition)/XAxial+offsetX)*SlopeY;
			StaffString.Format(TEXT("%d"),i);
			StringPosition =Gdiplus::PointF (x1,rect.Height()-StaffSize/7*5);
			//graphics->DrawString(StaffString,StaffString.GetLength(),&font,StringPosition,&StringBrush);
			graphics->DrawString(pStrStaff,wcslen(pStrStaff),&font,StringPosition,&StringBrush);
		}
		
		else if(i%(BlodScaleX/2)==0)
		{
			float x1=StaffSize+i*(float)(rect.Width()-StaffSize-1)/(float)XAxial;
			float y1=rect.Height()-StaffSize;
			float x2=x1;
			float y2=rect.Height()-StaffSize+StaffSize/5*2;
			graphics->DrawLine(&StaffPen,x1,y1,x2,y2);
		}
	}	 
	StringPosition.X-=20;
	StringPosition.Y-=StaffSize;
	StaffString.Format(TEXT("%s"),XAxialUnit);
	//graphics->DrawString(StaffString,StaffString.GetLength(),&font,StringPosition,&StringBrush);
	graphics->DrawString(pStrStaff,wcslen(pStrStaff),&font,StringPosition,&StringBrush);
	int k,l;
	StaffString = _T("我爱你!");
	k = StaffString.GetLength();
	l = wcslen(pStrStaff);
	/*	 
	CString strTemp ;
	Pen pen(Color(255,255,0,0),2); //createpen 
	Pen penGrid(Color(220, 220, 220),1);
	SolidBrush AxiasBrush(Color(128,128,0));//RGB(128,128,0)
	//显示X坐标文字 
	int widthStep =	rect.Width()/XAxial;
	for(i=1;i<XAxial;i++) //8分钟
	{
		strTemp.Format(_T("%d"),i);
		graphics->DrawLine(&pen,PointF(i*widthStep,rect.bottom),PointF(i*widthStep,rect.bottom-5));		
		graphics->DrawString(strTemp, (INT)wcslen(strTemp), &font, PointF(i*widthStep+2, rect.bottom-20), &AxiasBrush);
		graphics->DrawLine(&penGrid,PointF(i*widthStep,rect.bottom),PointF(i*widthStep,rect.top));
	}  
	//绘制X坐标单位
	graphics->DrawString(XAxialUnit, (INT)wcslen(XAxialUnit), &font, PointF(i*widthStep-25, rect.bottom-30), &AxiasBrush);
	//显示Y坐标文字 
	int HeightStep =	rect.Height()/6;
	for(i=1;i<YAxial;i++) //500mA
	{
		strTemp.Format(_T("%d"),i*100);
		graphics->DrawLine(&pen,PointF(rect.left,rect.Height()-i*HeightStep ),PointF(rect.left+5,rect.Height()-i*HeightStep));		
		graphics->DrawString(strTemp, (INT)wcslen(strTemp), &font, PointF(rect.left+5,rect.Height()-i*HeightStep-15 ), &AxiasBrush);
		graphics->DrawLine(&penGrid,PointF(rect.left,rect.Height()-i*HeightStep ),PointF(rect.right+5,rect.Height()-i*HeightStep));
	}  
	//绘制Y坐标单位
	graphics->DrawString(YAxialUnit, (INT)wcslen(YAxialUnit), &font, PointF(30, 5), &AxiasBrush);	
	*/ 
}


void CCurveEdit::DrawValue(Gdiplus::Graphics* CanvasGraphics)
{
	Gdiplus::Pen CurvePen(CurveColor); 
	//int DataNumber= PlotDatas.size();
	int DataNumber = PlotDatas.GetCount();
	int ShowNumber=DataShowBeginPosition-DataShowEndPosition;

	int RangY=CanvasBitmap->GetHeight()-StaffSize;
	int RangX=CanvasBitmap->GetWidth()-StaffSize;
	float DataIntervalX=(float)RangX/(float)ShowNumber;
	float DataIntervalY=(DataShowMax-DataShowMin)/RangY;

	offsetX=DataNumber-ShowNumber;
	if (offsetX<0) offsetX=0;

	int Begin=DataNumber;
	int End=DataNumber-DataShowBeginPosition+DataShowEndPosition;
	if (End<0)End=0;

	//if (nPlotType==1)
	{
		for(int i=Begin-2;i>=End;i--)
		{
			float x1=(i+1-offsetX)*DataIntervalX+StaffSize;
			float y1=(PlotDatas[i+1]-DataShowMin)/DataIntervalY;
			float x2=(i-offsetX)*DataIntervalX+StaffSize;
			float y2=(PlotDatas[i]-DataShowMin)/DataIntervalY;
			float k=(y2-y1)/(x2-x1);
			float c=y1-x1*k;
			if(y1<0.0) {y1=0.0;x1=(y1-c)/k;}
			if(y2<0.0) {y2=0.0;x2=(y2-c)/k;}
			if(y1>RangY){y1=(float)RangY;x1=(y1-c)/k;}
			if(y2>RangY){y2=(float)RangY;x2=(y2-c)/k;}
			y1=RangY-y1;
			y2=RangY-y2;
			CanvasGraphics->DrawLine(&CurvePen,x1,y1,x2,y2);
		}
	}
}

void CCurveEdit::AddData(int fData)
{
	PlotDatas.Add(fData);
	Invalidate ( false ) ;
}

void CCurveEdit::ClearData()
{
	PlotDatas.RemoveAll();
}

void CCurveEdit::DrawRoundedRectangle(Graphics*  pGraphics, int iWidth, int iHeight, int Radius)
{
	GraphicsPath path;
	path.StartFigure();

	REAL width = iWidth - 1.5;
	REAL height = iHeight - 1.0;
	Rect r(0, 0, width, height);
	int dia = Radius;
	// diameter can't exceed width or height
	if(dia > r.Width)    dia = r.Width;
	if(dia > r.Height)    dia = r.Height;

	// define a corner 
	Rect Corner(r.X, r.Y, dia, dia);

	// top left
	path.AddArc(Corner, 180, 90);                    
	// tweak needed for radius of 10 (dia of 20)
	if(dia == 20)
	{
		Corner.Width += 1; 
		Corner.Height += 1; 
		r.Width -=1; r.Height -= 1;
	}

	// top right
	Corner.X += (r.Width - dia - 1);
	path.AddArc(Corner, 270, 90);    

	// bottom right
	Corner.Y += (r.Height - dia - 1);
	path.AddArc(Corner,   0, 90);    

	// bottom left
	Corner.X -= (r.Width - dia - 1);
	path.AddArc(Corner, 90, 90);

	path.CloseFigure();                

	Gdiplus::Color solidBrushColor;
	solidBrushColor.SetFromCOLORREF(GetSysColor(COLOR_3DDKSHADOW));
	SolidBrush solidBrush(solidBrushColor);// Color(m_clrBackColor));
	pGraphics->FillRectangle(&solidBrush, 0.0, 0.0, width, height);        
	solidBrush.SetColor(StaffBackgroundColor);						//Color(StaffBackgroundColor)
	pGraphics->FillPath(&solidBrush, &path);

	//是否绘制边线
	//if (m_bBorderVisible)  //Radius
	{
		Pen pen(RGB(255,0,255), 2);  // Color(m_clrBorderColor) ,m_nBorderWidth
		pGraphics->DrawPath(&pen, &path);
	}
}

void CCurveEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int i=0;
	
	//if (nIDEvent==GRID_TIMER)
	{
		i++; 
		AddData(sin(6.28*i*2/360)*250+250);
		
	}
	CWnd::OnTimer(nIDEvent);
}

//BOOL CCurveEdit::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	
//	return CWnd::PreCreateWindow(cs);
//}

//int CCurveEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CWnd::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  Add your specialized creation code here
//	SetTimer(0,1000,NULL);
//	return 0;
//}
