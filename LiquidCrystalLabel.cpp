// LiquidCrystalLabel.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "LiquidCrystalLabel.h"


// CLiquidCrystalLabel

IMPLEMENT_DYNAMIC(CLiquidCrystalLabel, CStatic)

CLiquidCrystalLabel::CLiquidCrystalLabel()
{
	pFont = NULL;
}

CLiquidCrystalLabel::~CLiquidCrystalLabel()
{
	if(pFont!=NULL)
		delete pFont;
}


BEGIN_MESSAGE_MAP(CLiquidCrystalLabel, CStatic)
ON_WM_DESTROY()
END_MESSAGE_MAP()



// CLiquidCrystalLabel message handlers 
 


void CLiquidCrystalLabel::PreSubclassWindow()
{ 
	CStatic::PreSubclassWindow();
	CRect rcClient;	
	GetClientRect(&rcClient);
	LoadFont( _T("7SEG"),rcClient.Height()-4);
}

void CLiquidCrystalLabel::OnDestroy()
{
	// 最后不需要的时候释放字体资源
	CStatic::OnDestroy(); 
}

int CLiquidCrystalLabel::LoadFont(TCHAR * FontName, int FontHeight)
{	
	// 初始化函数
	LOGFONT lf;
	if(pFont)
		delete pFont;
	pFont = new CFont();
	memset(&lf, 0, sizeof(LOGFONT));    
	lf.lfHeight = FontHeight;
	lf.lfWidth	= 0;//FontHeight/2;
	lf.lfEscapement		= 0;    //旋转角度
	lf.lfOrientation	= 0;
	lf.lfWeight = FW_NORMAL; 
	lf.lfItalic = 0; 
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0; 
	lf.lfCharSet		= DEFAULT_CHARSET; 
	lf.lfOutPrecision	= OUT_DEVICE_PRECIS; 
	lf.lfClipPrecision	= CLIP_TT_ALWAYS;
	lf.lfQuality		= ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = 0; 
	_tcscpy(lf.lfFaceName,FontName); // 这里就是字体名 
	pFont->CreateFontIndirect(&lf);
	ASSERT(pFont->GetSafeHandle());
	
	SetFont(pFont);
	return 0;
}

