#pragma once

#include <vector>
// CCurveEdit
#include "ClyRDB.h"
class CCurveEdit : public CWnd
{

	DECLARE_DYNAMIC(CCurveEdit)

public:
	CCurveEdit();
	virtual ~CCurveEdit();

protected:
	DECLARE_MESSAGE_MAP()
private:
	int i;
	CRect rect; 
	int BlodScaleX;
	int BlodScaleY;
	int StaffSize;							//��������
	//vector <float> PlotDatas;				//������ʷ����
	CArray <int> PlotDatas;
	Gdiplus::Color CurveBackgroundColor;	//���߱���ɫ
	Gdiplus::Color CurveColor;				//������ɫ
	Gdiplus::Color StaffBackgroundColor;	//�����ᱳ����ɫ
	Gdiplus::Color m_clrBorderColor;	
	Gdiplus::Color StaffColor;				//��������ɫ
	float DataShowMin;						//������ʾ����Сֵ
	float DataShowMax;						//������ʾ�����ֵ
	float DataShowBeginPosition;
	float DataShowEndPosition;
	float SlopeX;
	float SlopeY;
	int offsetX;
public:
	afx_msg void OnPaint();
	// //X�����᳤��
	int XAxial;
	// //Y������
	int YAxial;
	// //X�����ᵥλ
	CString XAxialUnit;
	//Y�����ᵥλ
	CString YAxialUnit;
	FontFamily fontFamily; 
	Gdiplus::Bitmap		* CanvasBitmap;
	Gdiplus::Graphics	* graphics;
	Font font	;		//��ʾ������ʽ
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:

	void DrawStaff(Gdiplus::Graphics* graphics);
	void DrawString(Gdiplus::Graphics* graphics);
	void DrawRoundedRectangle(Graphics*  pGraphics, int width, int height, int Radius);
	void DrawValue(Gdiplus::Graphics* CanvasGraphics);
public:
	void AddData(int fData);
	void ClearData();
	CClyRDB	*pRDB;
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


