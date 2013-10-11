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
	int StaffSize;							//坐标轴宽度
	//vector <float> PlotDatas;				//保存历史数据
	CArray <int> PlotDatas;
	Gdiplus::Color CurveBackgroundColor;	//曲线背景色
	Gdiplus::Color CurveColor;				//曲线颜色
	Gdiplus::Color StaffBackgroundColor;	//坐标轴背景颜色
	Gdiplus::Color m_clrBorderColor;	
	Gdiplus::Color StaffColor;				//坐标轴颜色
	float DataShowMin;						//纵轴显示的最小值
	float DataShowMax;						//纵轴显示的最大值
	float DataShowBeginPosition;
	float DataShowEndPosition;
	float SlopeX;
	float SlopeY;
	int offsetX;
public:
	afx_msg void OnPaint();
	// //X坐标轴长度
	int XAxial;
	// //Y坐标轴
	int YAxial;
	// //X坐标轴单位
	CString XAxialUnit;
	//Y坐标轴单位
	CString YAxialUnit;
	FontFamily fontFamily; 
	Gdiplus::Bitmap		* CanvasBitmap;
	Gdiplus::Graphics	* graphics;
	Font font	;		//显示字体样式
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


