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
	// �����Ҫ��ʱ���ͷ�������Դ
	CStatic::OnDestroy(); 
}

int CLiquidCrystalLabel::LoadFont(TCHAR * FontName, int FontHeight)
{	
	// ��ʼ������
	LOGFONT lf;
	if(pFont)
		delete pFont;
	pFont = new CFont();
	memset(&lf, 0, sizeof(LOGFONT));    
	lf.lfHeight = FontHeight;
	lf.lfWidth	= 0;//FontHeight/2;
	lf.lfEscapement		= 0;    //��ת�Ƕ�
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
	_tcscpy(lf.lfFaceName,FontName); // ������������� 
	pFont->CreateFontIndirect(&lf);
	ASSERT(pFont->GetSafeHandle());
	
	SetFont(pFont);
	return 0;
}

