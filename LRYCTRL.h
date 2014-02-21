// LRYCTRL.h : main header file for the LRYCTRL application
//

#if !defined(AFX_LRYCTRL_H__584D6D80_CF14_4B1F_AB88_0A6A9AE5F89A__INCLUDED_)
#define AFX_LRYCTRL_H__584D6D80_CF14_4B1F_AB88_0A6A9AE5F89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Template.H"

#include "UserLogon.h" 
#include "MyMultiDocTemplate.h" 

//#define CDocTemplate CMyDocTemplate  
/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLApp:
// See LRYCTRL.cpp for the implementation of this class
//

class CLRYCTRLApp : public CWinApp
{
public:
	class  _UserInfo{
	public:
		_UserInfo(){m_bLogon = FALSE;}
	private:
		BOOL	m_bLogon;
	public:
		CString cID;
		CString UserName;	
		BOOL    bEval;
		CString UserPassword;
		CString Company;
		
		//inline void login(){m_bLogon = TRUE;} 
		BOOL bLogon()
		{
			//if(!m_bLogon){AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_MENU_USERLOGIN);}
			CUserLogon dlg;
			while((!m_bLogon)&&(IDOK==dlg.DoModal()))
			{
				if(dlg.m_DBUserPassword==dlg.m_UserPassword)
				{
					AfxMessageBox(_T("密码正确"),MB_OK+MB_ICONINFORMATION);
					m_bLogon = TRUE;
				} 
				if(m_bLogon) 
					break;		
			}
			return m_bLogon ;
		}
		void Logout(){
			if((!m_bLogon)||(IDYES==AfxMessageBox(_T("您肯定注销吗？ "),MB_YESNO)))
			{
				AfxMessageBox(_T("已注销"),MB_OK+MB_ICONINFORMATION);
				m_bLogon = FALSE;
			}
		}


	}UserInfo;
	
	
	BOOL OpenNewDocument(CDocument **pDoc ,const CString &strTarget);
	BOOL OpenNewDocument(const CString &strTarget);
	void GetDocList(CObList * pDocList ,CDocTemplate * DocTemplate);
	void GetDocTemplateList(CObList * pDocList);
	int Run() ;
	static  UINT CompressDB(LPVOID pParam);
	_ConnectionPtr pConn;
	CLRYCTRLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLRYCTRLApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CLRYCTRLApp)
	afx_msg void OnAppAbout();
	afx_msg void OnCompressdb();
	afx_msg void OnTest();
	afx_msg void OnQryE();
	afx_msg void OnQryQ();
	afx_msg void OnQryS();
	afx_msg void OnQryG(); 
	afx_msg void OnDeviceAttrib();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	void InitGDIPlus();
	void LoadExternFont(TCHAR *pFileName);
	static LONG  CrashHandler(EXCEPTION_POINTERS *pException);
	static void CreateDumpFile(LPTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  ;
	LPCTSTR m_pszOldAppName;
	

public:
	void OpenSingleDoc(CString DocTypeName);
	afx_msg void OnCorrFact();
	afx_msg void OnUpdateMenu(CCmdUI *pCmdUI);
	// void CWinApp::(* )
	void AddDocTemplate(CDocTemplate* pTemplate);
};

AFX_INLINE BOOL CreateImageList(CImageList& il, UINT nID)
{
	if (!il.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1))
		return FALSE;
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(nID));
	
	il.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));
	
	return TRUE;
}

class COfficeTreeCtrl : public CXTPOfficeBorder<CTreeCtrl>
{
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)		
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		
		switch (pNMHDR->code)
		{
		case TTN_SHOW:
			{
				// get a pointer to the tooltip control.
				HWND hWnd = TreeView_GetToolTips(m_hWnd);
				if (hWnd != NULL)
				{
					// make sure the tooltip is at the top of the "Z" order, otherwise
					// it will appear behind popup windows....
					::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0,
						SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				}
			}
			break;
		}
		return CXTPOfficeBorder<CTreeCtrl>::OnNotify(wParam, lParam, pResult);
	}
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
extern  CLRYCTRLApp theApp;

BOOL CheckCPUBigEndian();
#define BigLittleSwap16(A)        ((((SHORT)(A) & 0xFF00) >> 8) | (((SHORT)(A) & 0x00FF) << 8))
#define BigLittleSwap32(A)        ((((LONG)(A) & 0xFFFF0000) >> 16) | (((LONG)(A) & 0x0000FFFF) << 16))




double GetStandardDeviation(long *ptr,int cnt);
double GetRSD(long *ptr,int cnt); 
void PrintProviderError(_ConnectionPtr pConnection);
void ProcessErrorMessage(TCHAR * pchar);
BOOL CompressDBFile(LPTSTR FileName,LPTSTR OldPassword,LPTSTR NewPassword);
int GetAllFieldName(_RecordsetPtr m_pRecordset,CStringArray *ArryFieldName);
CString GetFieldType(FieldPtr pField); 
void AFXAPI DDX_Text2(CDataExchange* pDX, int nIDC, double& value);
int RegActivex(LPCTSTR strFileName); 
CString VariantToStr(const COleVariant& var); 
COleDateTime MakeDays(int days);
inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};
CString GetCurrParentSeleString(); 
CString GetCurrentSeleString(void);
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len);
void c2w(wchar_t *pwstr,size_t len,const char *str);
BOOL GetALinearRegressionEquation(IN double MatrixX[],IN double MatrixY[],IN int MaxTrixCount,
								  OUT double &slope,OUT double &intercept,OUT double &R );
void  LinearRegressionEquation(IN double MatrixX[],IN int MaxTrixCount,
							   IN double slope,IN double intercept, OUT double MatrixY[]); 
 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LRYCTRL_H__584D6D80_CF14_4B1F_AB88_0A6A9AE5F89A__INCLUDED_)
