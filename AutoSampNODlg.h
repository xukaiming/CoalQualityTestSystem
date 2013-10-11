#ifndef _AUTO_SAMPLE_NO_DLG_H_
#define _AUTO_SAMPLE_NO_DLG_H_
#pragma once

#include "XListCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Gyfxy1RDB.h"
// CGyfxyAutoSampNODlg dialog

class CGyfxyAutoSampNODlg : public CDialog
{
	DECLARE_DYNAMIC(CGyfxyAutoSampNODlg)

public:
	CGyfxyAutoSampNODlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGyfxyAutoSampNODlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GYFXY_AUTO_NO };

protected:
	static BOOL   CALLBACK   EnumChildProc(HWND   hwndChild,LPARAM   lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int Max_WaterAshCnt;
	int Max_VolCnt;
	int WaterAshCnt ;
	int VolCnt		;
	HACCEL m_hAccel; 

	////////////////////////////
	CButton m_CheckTestWaterAshPX	;//= ((CButton*)GetDlgItem(IDC_CHECK_WATER_ASH_PX));
	CButton m_CheckTestVolPX		;//= ((CButton*)GetDlgItem(IDC_CHECK_VOL_PX));
	CButton m_CheckTestWater		;//= ((CButton*)GetDlgItem(IDC_CHECK_TEST_WATER));
	CButton m_CheckTestAsh			;//= ((CButton*)GetDlgItem(IDC_CHECK_TEST_ASH));
	CButton m_CheckTestVol			;//= ((CButton*)GetDlgItem(IDC_CHECK_TEST_VOL));
	CButton m_CheckSYNC				;//= ((CButton*)GetDlgItem(IDC_CHECK_SYNC));
	/////////////////////////////
	BOOL bSYNC_Checked				;		//同步	
	BOOL bTest_Ash_Checked			;		//测试灰分
	BOOL bTest_Water_Checked		;		//测试水分
	BOOL bTestVol_Checked			;		//测试挥发分
	BOOL bTestWateAshPX_Checked		;		//测试灰分水分平行样
	BOOL bTestVolPX_Checked			;		//测试挥发分平行样
	//////////////////////////////
public:	
	CXListCtrl m_ListWaterAsh;
	CXListCtrl m_ListVol;

	CGyfxyRDB_G5200		*pRdb;
	CGyfxyImageSlave *pImg;
	
	
	void CheckChkboxState(void);
	void CalcParam(INT nID);
	void CalcSampleCnt(INT nID);
	void GetTestSampleCnt();
	void CheckGUI(INT nID);
	void AutoSample(void);
	void MakeSampleInfo(void);
	void MakeSYNCSampleInfo(void);  //平行样样位信息
	void SetListCtrlCount(CXListCtrl *pListCtrl,int count);
	void SetTestSampleCount(CComboBox * pListBox,int MaxCount,int count=0 );
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck(UINT nID);
 
	//  
	void CheckAllCheckBox(bool bChecked);
	CXTPButton m_OKButton;
	afx_msg void OnCbnSelchangeComboAshCnt();
	afx_msg void OnCbnSelchangeComboVolCnt();
	afx_msg void OnDeleteRecord(); 
	afx_msg LRESULT OnEditEnd(WPARAM nItem, LPARAM nSubItem);
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
};
#endif