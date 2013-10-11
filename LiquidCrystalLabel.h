#pragma once


// CLiquidCrystalLabel

class CLiquidCrystalLabel : public CStatic
{
	DECLARE_DYNAMIC(CLiquidCrystalLabel)

public:
	CLiquidCrystalLabel();
	virtual ~CLiquidCrystalLabel(); 
protected:
	DECLARE_MESSAGE_MAP()
public:
	int LoadFont( TCHAR * FontName, int FontHeight);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnDestroy();
private:
	CFont *pFont;
	TCHAR szPath[MAX_PATH];
	
public:
//	afx_msg void OnPaint();
};


