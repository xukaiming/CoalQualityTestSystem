// LRYCTRL.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LRYCTRL.h"


#include "MainFrm.h"
#include "ChildFrm.h"  
#include "ClyChildWnd.h "

#include "TextProgressCtrl.h" 

#include "DeviceSetupDlg.h"
#include "ClyDlgDeviceSetup.h"
#include "GyfxyDlgDeviceSetup.h"

//#include "LRYQryDoc.h"
//#include "LRYQryView.h"

//调试输出
#include "TraceWin.h" 
 //#include "Debug.h"
 //#include "Output.h"
#include "LryQDBQryView.h"
#include "LryQDBQryDoc.h"
#include "LryHostView.h"
#include "LryHostDoc.h"
#include "LryEDBQryView.h"
#include "LryEDBQryDoc.h"

#include "DBQrySView.h"
#include "DBQrySDocument.h"
//////////////////////////
#include "ClyCoffDlg.h"
///////////////////////////////
#include "float.h"

//////
//测硫仪
#include "ClyHostView.h"
#include "CLyHostDoc.h"
//工业分析仪
#include "GyfxyHostDoc1.h"
#include "GyfxyHostView.h" 
#include "DBQryGDocument.h" 
#include "DBQryGView.h"
//调试工具
#include "DbgHelp.h"  
#pragma  comment(lib,"Dbghelp.lib")


//{{AFX_INCLUDES()

//}}AFX_INCLUDES


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLApp

BEGIN_MESSAGE_MAP(CLRYCTRLApp, CWinApp)
	//{{AFX_MSG_MAP(CLRYCTRLApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_COMPRESSDB, OnCompressdb)
	ON_COMMAND(ID_TEST, OnTest)
	ON_COMMAND(ID_QRY_E, OnQryE)
	ON_COMMAND(ID_QRY_Q, OnQryQ)
	ON_COMMAND(ID_QRY_S, OnQryS)

	ON_COMMAND(ID_QRY_G, OnQryG)
	
	ON_COMMAND(ID_DEVICE_ATTRIB, OnDeviceAttrib)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_CORR_FACT, &CLRYCTRLApp::OnCorrFact)
	ON_UPDATE_COMMAND_UI(ID_CORR_FACT,OnUpdateMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLApp construction
 
CLRYCTRLApp::CLRYCTRLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CLRYCTRLApp object
CLRYCTRLApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLApp initialization
BOOL CLRYCTRLApp::InitInstance()
{ 	
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashHandler); //挂载异常捕获
	 
	//创建互斥量,防止程序运行两次	 
	HANDLE hObject = CreateMutex(NULL,FALSE,_T("煤质综合分析系统"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hWnd = FindWindow(_T("MAIN_HOST"),NULL);				
		if(::IsWindow(hWnd)) 
		{ 
			//如果主窗口已最小化，则恢复其大小。 
			if (::IsIconic(hWnd)) 
			{
				::ShowWindow(hWnd,SW_RESTORE); 
			}
			//将应用程序的主窗口激活。 
			::SetForegroundWindow(hWnd); 			
		}
		CloseHandle(hObject);
		TRACE(_T("程序已经运行!"));
		TerminateThread(this->m_hThread,-1);
		return FALSE;
	} 
	//修改执行 文件名	 
	m_pszOldAppName = (PTSTR)	m_pszAppName ;
	m_pszAppName = new TCHAR[MAX_PATH];
	ZeroMemory((void *)m_pszAppName,sizeof(m_pszAppName));
	CString appName;
	appName.LoadString(IDR_MAINFRAME);
	lstrcpy((PTSTR)m_pszAppName,appName);
	 
	//设置调试输出
//	 CDebug::ToFile(_T("LRY.dmp"));
//	 CDebug::Start();  
	//初始化local
	setlocale(LC_ALL,    "Chinese-simplified "); 
	//加载字体文件
	LoadExternFont(_T("7SEG.ttf"));

	/////////////////////////////////
	//初始化 ole com
	AfxOleInit(); 
	AfxEnableControlContainer();
	//初始化RichEdit
	InitCommonControls();     //初始化RicheditControl
	//初始化GDI+
	InitGDIPlus();
	//数据连结
	HRESULT hr    ;
	try
	{
		hr = pConn.CreateInstance("ADODB.Connection");
		TESTHR(hr);	
		if(SUCCEEDED(hr))
		{ 
			//pConn->Open("File Name=conn.udl","","",adConnectUnspecified);
			pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;\
				Data Source=Database\\LryDb2003.mdb;\
				Mode=Share Deny None;\
				Persist Security Info=False;\
				Jet OLEDB:Database Password=Admin",
				"","",adConnectUnspecified);
		} 

	}
	catch(_com_error &e)
	{		
		dump_com_error(e);
		PrintProviderError(pConn);
	 	return FALSE;
	} 
	/////////////////////////////////  
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
#if _MSC_VER <= 1200 // MFC 6.0 or earlier
	#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	
	CMultiDocTemplate* pDocTemplate;            
	pDocTemplate = new CMultiDocTemplate(           //Q query
		IDR_LRYQRY_DOCTYPE, 
		RUNTIME_CLASS(CLryQDBQryDoc), 
		RUNTIME_CLASS(CMDIChildWnd), // custom MDI child frame
		RUNTIME_CLASS(CLryQDBQryView));
	AddDocTemplate(pDocTemplate);  
		
	pDocTemplate = new CMultiDocTemplate( // E Query
		IDR_LRYEQRY_DOCTYPE,
		RUNTIME_CLASS(CLryEDBQryDoc),		// document class
		RUNTIME_CLASS(CMDIChildWnd),		// frame class
		RUNTIME_CLASS(CLryEDBQryView));		// view class
	AddDocTemplate(pDocTemplate);
	
	pDocTemplate = new CMultiDocTemplate(
		IDR_LRYHOSTVIEW_TMPL,
		RUNTIME_CLASS(CLryHostDoc),		// document class
		RUNTIME_CLASS(CChildFrame),		// frame class  MDI
		//RUNTIME_CLASS(CMDIChildWnd),		// frame class
		RUNTIME_CLASS(CLryHostView));		// view class
	AddDocTemplate(pDocTemplate); 

	pDocTemplate = new CMultiDocTemplate( //测硫仪
			IDR_CLYHOSTVIEW_TMPL,
			RUNTIME_CLASS(CClyHostDoc),		// document class
			RUNTIME_CLASS(CClyChildWnd),		// frame class CXSplitterWnd
			RUNTIME_CLASS(CClyHostView));		// view class
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(  //测硫仪查询
		IDR_CLYSQRY_DOCTYPE,
		RUNTIME_CLASS(CDBQrySDocument),		// document class
		RUNTIME_CLASS(CMDIChildWnd),		// frame class
		RUNTIME_CLASS(CDBQrySView));		// view class
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
			IDR_GYFXYHOSTVIEW_TMPL,         //工业分析仪1      
			RUNTIME_CLASS(CGyfxyHostDoc),
			RUNTIME_CLASS(CClyChildWnd),
			RUNTIME_CLASS(CGyfxyHostView_G5200));
	AddDocTemplate(pDocTemplate);


	pDocTemplate = new CMultiDocTemplate(    //工业分析仪查询
		IDR_DBQRYGVIEW_TMPL,
		RUNTIME_CLASS(CDBQryGDocument),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CDBQryGView));
	AddDocTemplate(pDocTemplate);
	
/*
	pDocTemplate = new CMultiDocTemplate(
		IDR_CLYHOSTVIEW_TMPL,
		RUNTIME_CLASS(CClyHostDoc),		// document class
		RUNTIME_CLASS(CChildFrame),		// frame class MID fixsize
		RUNTIME_CLASS(CClyDebugView));		// view class
	AddDocTemplate(pDocTemplate);
	*/
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	if(cmdInfo.m_nShellCommand==CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand=CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
#ifndef _DEBUG
	pMainFrame->ShowWindow(m_nCmdShow|SW_SHOWMAXIMIZED);
#else
	pMainFrame->ShowWindow(m_nCmdShow);
#endif
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX }; 
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck6();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg) 
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK6, &CAboutDlg::OnBnClickedCheck6)
END_MESSAGE_MAP()

// App command to run the dialog
void CLRYCTRLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLRYCTRLApp message handlers


BOOL CLRYCTRLApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strTimer;
	CMainFrame *pFrame;
	CTime   time;
	static CString strWeek[] =  {"星期天","星期一","星期二","星期三","星期四","星期五","星期六"};
		

	time = CTime::GetCurrentTime();
	strTimer.Format(_T("%4d年%d月%d日 %s %02d:%02d:%02d "),time.GetYear(),time.GetMonth(),time.GetDay(),
				strWeek[time.GetDayOfWeek()-1],time.GetHour(),time.GetMinute(),time.GetSecond() );
	pFrame = (CMainFrame *)AfxGetMainWnd();  
	int id = pFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_DATETIMER);
	pFrame->m_wndStatusBar.SetPaneText(id,strTimer); 
	return CWinApp::OnIdle(lCount);	 
}

int CLRYCTRLApp::ExitInstance() 
{ 
		//关闭调试信息
// 	CDebug::Free();
	GdiplusShutdown(gdiplusToken);
	return CWinApp::ExitInstance();
}







BOOL CheckCPUBigEndian()
{
    union
    {
        unsigned long int i;
        unsigned char s[4];
    }c = {0x12345678};
	return (0x12 != c.s[0]);
}

//标准差
double GetStandardDeviation(long *inPtr,int cnt)
{
	long *ptr  = new long[cnt];
	double StandardDeviation=0;	
	double m_sum = Sum(inPtr,cnt);
	double sumOfSquare = m_sum*m_sum/cnt;   //和的平方
	for(int i=0;i<cnt;i++)
	{
		ptr[i] = inPtr[i]*inPtr[i];	
	}
	double SquareOfsum = Sum(ptr,cnt);		////平方的和
	StandardDeviation	  = sqrt((SquareOfsum-sumOfSquare)/(cnt-1));
	delete ptr;
	return StandardDeviation; 
	
}

//得到相对标准差  
double GetRSD(long *ptr,int cnt)
{
	return GetStandardDeviation(ptr,cnt)/(Sum(ptr,cnt)/cnt) ;
}

void PrintProviderError(_ConnectionPtr pConnection)
{
    // Print Provider Errors from Connection object.
    // pErr is a record object in the Connection's Error collection.
	CString  strError;
    ErrorPtr    pErr  = NULL;
	if(pConnection!=NULL)
	{
		if( (pConnection->Errors->Count) > 0)
		{
			long nCount = pConnection->Errors->Count;
			// Collection ranges from 0 to nCount -1.
			for(long i = 0; i < nCount; i++)
			{
				pErr = pConnection->Errors->GetItem(i);
				strError.Format(_T("错误代码: %x\n%s"), pErr->Number, 
				  (LPTSTR) (pErr->Description));
				MessageBox(NULL,strError,_T("错误"),MB_OK+MB_ICONERROR);
			}
		}
	}
	else
	{
		MessageBox(NULL,strError,_T("_ConnectionPtr = 0x00000000"),MB_OK+MB_ICONERROR);
	}
}

void ProcessErrorMessage(TCHAR * pchar)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	 
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	//::MessageBox( NULL, (LPCTSTR)lpMsgBuf, pchar, MB_OK | MB_ICONINFORMATION );
	TRACE((LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

//压缩数据库    
BOOL CompressDBFile(LPTSTR FileName,LPTSTR OldPassword,LPTSTR NewPassword)
{
		bool	Result    = false;

		IJetEnginePtr   jet(__uuidof(JetEngine));   
		CString SourceFileName	= _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s%s%s%s;Jet OLEDB:Database Password=%s");
		CString DestFileName	= _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=%s;Jet OLEDB:Engine Type=5");

		CString t;
		TCHAR drive[4];		
		TCHAR dir[512];
		TCHAR fname[512];
		TCHAR ext[512];  
		_tsplitpath(FileName,drive,dir, fname, ext );
		t.Format(SourceFileName,drive,dir,_T("tmpDB"),ext,OldPassword);
		SourceFileName = t; 
		t.Format(DestFileName,FileName,NewPassword);
		DestFileName   = t;
		
		int errno ;
		LPTSTR str = _T("%s%s%s%s");
 		t.Format(str,drive,dir,_T("tmpDB"),ext);
 
		try
		{
			CFile::Rename(FileName,t.GetBuffer(t.GetLength()));
			


			IJetEnginePtr jet(__uuidof(JetEngine));
			jet->CompactDatabase(				 
				 SourceFileName.AllocSysString(),DestFileName.AllocSysString()
				 );
			CFile::Remove(t.GetBuffer(t.GetLength()));
			//::MessageBox(NULL, _T("压缩完成!"), _T("提示"), MB_OK+MB_ICONINFORMATION) ;  
		}
		catch(CFileException* pEx )
		{
			//pEx->ReportError();	//无法在多线程方式下打开
			pEx->Delete();

		}
		catch(_com_error &e) 
		{       
		   //  
			dump_com_error(e);
		} 
		return Result;
}

//得到所有字段名
int GetAllFieldName(_RecordsetPtr m_pRecordset,CStringArray *ArryFieldName)
{

	int Result;
 	ASSERT((NULL!=m_pRecordset)&&(NULL!=ArryFieldName));
	CString strFieldName;
	BSTR   bstrFieldName;

	Fields *Fields = NULL;	
	m_pRecordset->get_Fields(&Fields);
	Result   = m_pRecordset->Fields->Count;
	_variant_t *ColNameArray = new _variant_t[Result];
	ArryFieldName->SetSize(Result);
	for(long i=0;i<Result;i++)
	{
		Fields->Item[i]->get_Name(&bstrFieldName);
		strFieldName         = bstrFieldName;
		ArryFieldName->SetAt(i,strFieldName);//保存字段名以备读取记录集	
	}
	delete []ColNameArray;
	return Result;
}

//得到字段类型
CString GetFieldType(FieldPtr pField)
{
	CString strDataType;
   int propType = (int)pField->GetType();

    switch(propType) 
    {
        case adBigInt:
            strDataType = "adBigInt";
            
            break;
        case adBinary:
            strDataType = "adBinary"; 
			break;
        case adBoolean:
            strDataType = "adBoolean";
			break;
        case adBSTR:
            strDataType = "adBSTR";
			break;
        case adChapter:
            strDataType = "adChapter";
            break;
        case adChar:		
            strDataType = "adChar";
			break;
        case adCurrency:
            strDataType = "adCurrency";
			break;
        case adDate:
            strDataType = "adDate";
			break;
        case adDBDate:
            strDataType = "adDBDate";
			break;
        case adDBTime:
            strDataType = "adDBTime";
			break;
        case adDBTimeStamp:
            strDataType = "adDBTimeStamp";
			break;
        case adDecimal:
            strDataType = "adDecimal";
 			break;
        case adDouble:
            strDataType = "adDouble";
			break;
        case adEmpty:
            strDataType = "adEmpty";
            break;
        case adError:
            strDataType = "adError";
			;
        case adFileTime:
            strDataType = "adFileTime";
            break;
        case adGUID:
            strDataType = "adGUID";
			break;
        case adIDispatch:
            strDataType = "adIDispatch";
			break;
        case adInteger:
            strDataType = "adInteger";
			break;
        case adIUnknown:
            strDataType = "adIUnknown";
            break;
        case adLongVarBinary:		
            strDataType = "adLongVarBinary";
			break;
        case adLongVarChar:
            strDataType = "adLongVarChar";
			break;
        case adLongVarWChar:
            strDataType = "adLongVarWChar";
			break;
        case adNumeric:
            strDataType = "adNumeric";
			break;
        case adPropVariant:
            strDataType = "adPropVariant";
            break;
        case adSingle:
            strDataType = "adSingle";
			break;
        case adSmallInt:
            strDataType = "adSmallInt";
			break;
        case adTinyInt:
            strDataType = "adTinyInt";
			break;
        case adUnsignedBigInt:
            strDataType = "adUnsignedBigInt"; 
			break;
        case adUnsignedInt:
            strDataType = "adUnsignedInt";
			break;
        case adUnsignedSmallInt:
            strDataType = "adUnsignedSmallInt";
 			break;
        case adUnsignedTinyInt:
            strDataType = "adUnsignedTinyInt";
			break;
        case adUserDefined:
            strDataType = "adUserDefined";
            break;
        case adVarBinary:
            strDataType = "adVarBinary";
			break;
        case adVarChar:
            strDataType = "adVarChar";
			break;
        case adVariant:
            strDataType = "adVariant";
			break;
        case adVarNumeric:
            strDataType = "adVarNumeric";
            break;
        case adVarWChar:
            strDataType = "adVarWChar";
			break;
        case adWChar:			
            strDataType = "adWChar";
			break;
        default:
            strDataType = "*UNKNOWN*";
            break;
    }
	return strDataType;
}


//DDX_Text(pDX, IDC_TEST_YANGPINZHILIANG, m_SampeQuality)
void AFXAPI DDX_Text2(CDataExchange* pDX, int nIDC, double& value)
{ 
	TCHAR szBuffer[32];
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, nIDC, value); 
	}
	else
	{
		
		_stprintf(szBuffer, _T("%0.4f"), value);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}
 

int RegActivex(LPCTSTR strFileName)
{
	typedef int (CALLBACK* pRegFunc)(void);
	int bResult			= FALSE;
	pRegFunc RegFunc	= NULL;
	HINSTANCE hInst		= LoadLibrary(strFileName);
	if(hInst!=NULL)
	{
		if((RegFunc = GetProcAddress(hInst,"DllRegisterServer"))!=NULL)
		{			
			bResult = RegFunc();
		}
	}
	FreeLibrary(hInst);
	return bResult;
}


void CLRYCTRLApp::OnCompressdb() 
{
	// TODO: Add your command handler code here
	if(pConn->State==adStateOpen)
		pConn->Close(); 
	//启动数据库压缩线程
	CEvent hEvent;	
	AfxBeginThread((AFX_THREADPROC)CompressDB,&hEvent,THREAD_PRIORITY_NORMAL);
	//CompressDB(hEvent);
	//在statusbar上画出进度条
	CRect rect(0,0,0,0);
	CTextProgressCtrl *pProgressCtrl = new CTextProgressCtrl();
	pProgressCtrl->m_bShowText = TRUE;
	CMainFrame    *pFrame   = (CMainFrame *)AfxGetApp()->GetMainWnd();
	int id = pFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_COMPRESSDB);
	pFrame->m_wndStatusBar.GetItemRect (id, &rect);
	pProgressCtrl->Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,rect,&pFrame->m_wndStatusBar,1);
	pProgressCtrl->SetWindowText(_T("正在压缩中..."));	
	pProgressCtrl->SetRange(0,100);
	do{		
		pProgressCtrl->StepIt();	
		MSG msg;                
		while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))                
		{                        
		 ::TranslateMessage(&msg);                        
		 ::DispatchMessage(&msg);                
		}
	}while(WAIT_TIMEOUT==WaitForSingleObject(hEvent.m_hObject,20)); //
	//重新连接数据库	
	try
	{
		pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;\
					Data Source=Database\\LryDb2003.mdb;\
					Mode=Share Deny None;\
					Persist Security Info=False;\
					Jet OLEDB:Database Password=Admin",
					"","",adConnectUnspecified);
	}
	catch(_com_error e)
	{
		dump_com_error(e); 
	}	

	delete pProgressCtrl;
	pProgressCtrl = NULL;	
}

UINT CLRYCTRLApp::CompressDB(LPVOID pParam)
{
	CString DBFileName  ;
	TCHAR pathBuf[1024];
	//getcwd(pathBuf,1024);
	::GetCurrentDirectory(1024,pathBuf);
	bool bResult;
	CEvent *pEvent = (CEvent *)pParam;	
	try
	{
		DBFileName = CString(pathBuf)+_T("\\Database\\LryDb2003.mdb");
		CompressDBFile(DBFileName.GetBuffer(DBFileName.GetLength()),_T("Admin"),_T("Admin"));
		bResult = TRUE; 
		::MessageBox(NULL, _T("压缩完成!"), _T("提示"), MB_OK+MB_ICONINFORMATION) ;  
	}
	catch(_com_error e)
	{
		dump_com_error(e);
		bResult = FALSE;
	}
	pEvent->SetEvent();
	//同步EVENT;
	return bResult; 
}

void CLRYCTRLApp::OnTest() 
{
	// TODO: Add your command handler code here
	CTreeCtrl *TreeCtrl;
	CDocument	   *pDoc = NULL;
	HTREEITEM	   hItem ;
	HTREEITEM	   hParenItem ;
	CString        strIED_Type;
	//找到view 和ctrl 
	TreeCtrl   = &((CMainFrame *)m_pMainWnd)->m_wndDeviceView.m_wndClassView;
	hItem      = TreeCtrl->GetSelectedItem();
	hParenItem = TreeCtrl->GetParentItem(hItem);

	CObList DocList		   ;
	CObList DocTemplateList ;
	CDocTemplate * DocTemplate = NULL;//m_pDocManager


	
	if(hParenItem!=NULL)
	{
		//检查以打开标志
		POSITION pos ;
		GetDocTemplateList(&DocTemplateList);
		pos = DocTemplateList.GetHeadPosition();
		for(int j=0;j<DocTemplateList.GetCount();j++)
		{		
			GetDocList(&DocList,(CDocTemplate *)DocTemplateList.GetNext(pos));
			POSITION posDocList = DocList.GetHeadPosition();
			for(int i=0;i<DocList.GetCount();i++)
			{		
				pDoc = (CDocument *)DocList.GetNext(posDocList);
				//CLryHostDoc * pDoc = (CLryHostDoc *)DocList.GetNext(posDocList);
				//检查文档类型
				if( !((pDoc->IsKindOf(RUNTIME_CLASS(CLryHostDoc)))
					||(pDoc->IsKindOf(RUNTIME_CLASS(CClyHostDoc)))
					||(pDoc->IsKindOf(RUNTIME_CLASS(CGyfxyHostDoc)))))
				{
					break;
				}
				//重复记录退出 
				if(((CIEDDocument *)pDoc)->sDeviceName==TreeCtrl->GetItemText(hItem))
					return ;
			}
		}
		//返回文档句柄，做好以打开标记	
		OpenNewDocument(&pDoc,TreeCtrl->GetItemText(hParenItem));
		//((CMDIFrameWnd*)m_pMainWnd)->MDICascade(ID_WINDOW_ARRANGE);//排列子窗口图标		
	}
	else
	{
		MessageBox(m_pMainWnd->m_hWnd,_T("请选中需要打开的设备"),_T("提示"),MB_OK|MB_ICONINFORMATION);
	}
	
	
}



void CLRYCTRLApp::GetDocTemplateList(CObList *pDocList)
{
	CString strDocName;
	CDocTemplate * pSelectedTemplate;
	pDocList->RemoveAll();
	POSITION pos     = GetFirstDocTemplatePosition();
	POSITION posList = pDocList->GetHeadPosition();	
	while(pos!=NULL)
	{
		pSelectedTemplate = (CDocTemplate*)GetNextDocTemplate(pos);
		if(pSelectedTemplate==NULL)
			break;	
		pDocList->InsertAfter(posList,pSelectedTemplate);
		ASSERT(pSelectedTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
	}
}

void CLRYCTRLApp::GetDocList(CObList *pDocList, CDocTemplate *DocTemplate)
{
	CString strDocName;
	CDocument * pSelectedDoc;
	pDocList->RemoveAll();
	POSITION pos     = DocTemplate->GetFirstDocPosition();
	POSITION posList = pDocList->GetHeadPosition();
	while(pos!=NULL)
	{
		pSelectedDoc = DocTemplate->GetNextDoc(pos);
		if(pSelectedDoc==NULL)
			break;
		ASSERT(pSelectedDoc->GetDocTemplate()->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		pDocList->InsertAfter(posList,pSelectedDoc);
	} 
}

COleDateTime MakeDays(int days)
{
	COleDateTime datetime((DATE)0);
	int year  = datetime.GetYear();
	int month = datetime.GetMonth();
	int day	  = datetime.GetDay();
	int hour  = datetime.GetHour();
	int min   = datetime.GetMinute();
	int sec   = datetime.GetSecond();
	day+=days;
	COleDateTime sday(year,month,day,hour,min,sec);
	return sday;
}

//将OLE数据转换为C++数据
CString VariantToStr(const COleVariant& var)
{
	CString strRet;
	//strRet = _T("Fish"); 
	switch(var.vt){
		case VT_EMPTY:
		case VT_NULL:
//			strRet = _T("NULL");
			strRet = _T("");
			break;
		case VT_I2:
			strRet.Format(_T("%hd"),V_I2(&var));
			break;
		case VT_I4:
			strRet.Format(_T("%d"),V_I4(&var));
			break;
		case VT_R4:
			//strRet.Format(_T("%e"),(double)V_R4(&var));
			//strRet.Format(_T("%.6f"),V_R4(&var));  
			strRet.Format(_T("%f"),V_R4(&var));   
			break;
		case VT_R8:
			strRet.Format(_T("%e"),V_R8(&var));
			break;
		case VT_CY:
			strRet = COleCurrency(var).Format();
			break;
		case VT_DATE:
			strRet = COleDateTime(var).Format(_T("%Y-%m-%d %H:%M:%S"));//%Y=2001,%y=01
			break;
		case VT_BSTR:
			//strRet = V_BSTRT(&var); //为什么转化出来的结果只有第一个字符呢?
			strRet = (LPCSTR)_bstr_t(var);
			break;
		case VT_DISPATCH:
			strRet = _T("VT_DISPATCH");
			break;
		case VT_ERROR:
			strRet = _T("VT_ERROR");
			break;
		case VT_BOOL:
//			return V_BOOL(&var) ? _T("TRUE") : _T("FALSE");
			return V_BOOL(&var) ? _T("是") : _T("");
		case VT_VARIANT:
			strRet = _T("VT_VARIANT");
			break;
		case VT_DECIMAL:               // 16 byte fixed point.
			strRet = _T("VT_DECIMAL");
			break;
		case VT_RECORD:             // User defined type
			strRet = _T("VT_RECORD");
			break;
		case VT_UNKNOWN:
			strRet = _T("VT_UNKNOWN");
			break;
		case VT_I1:
			strRet = _T("VT_I1");
			break;
		case VT_UI1:
			strRet.Format(_T("0x%02hX"),(unsigned short)V_UI1(&var));
			break;
		case VT_UI2:
			strRet = _T("VT_UI2");
			break;
		case VT_UI4:
			strRet = _T("VT_UI4");
			break;

		case VT_I8:
			strRet = _T("VT_I8");
			break;
		case VT_UI8:
			strRet = _T("VT_UI8");
			break;

		case VT_INT:
			strRet = _T("VT_INT");
			break;
		case VT_UINT:
			strRet = _T("VT_UINT");
			break;
		case VT_VOID:
			strRet = _T("VT_VOID");
			break;
		case VT_HRESULT:
			strRet = _T("VT_HRESULT");
			break;
		case VT_PTR:
			strRet = _T("VT_PTR");
			break;
		case VT_SAFEARRAY:
			strRet = _T("VT_SAFEARRAY");
			break;
		case VT_CARRAY:
			strRet = _T("VT_CARRAY");
			break;
		case VT_USERDEFINED:
			strRet = _T("VT_USERDEFINED");
			break;
		case VT_LPSTR:
			strRet = _T("VT_LPSTR");
			break;
		case VT_LPWSTR:
			strRet = _T("VT_LPWSTR");
			break;
		case VT_FILETIME:
			strRet = _T("VT_FILETIME");
			break;
		case VT_BLOB:
			strRet = _T("VT_BLOB");
			break;
		case VT_STREAM:
			strRet = _T("VT_STREAM");
			break;
		case VT_STORAGE:
			strRet = _T("VT_STORAGE");
			break;
		case VT_STREAMED_OBJECT:
			strRet = _T("VT_STREAMED_OBJECT");
			break;
		case VT_STORED_OBJECT:
			strRet = _T("VT_STORED_OBJECT");
			break;
		case VT_BLOB_OBJECT:
			strRet = _T("VT_BLOB_OBJECT");
			break;
		case VT_CF:
			strRet = _T("VT_CF");
			break;
		case VT_CLSID:
			strRet = _T("VT_CLSID");
			break;
		case VT_VECTOR:
			strRet = _T("VT_VECTOR");
			break;
		case VT_ARRAY:
			strRet = _T("VT_ARRAY");
			break;
		case VT_BYREF:
			strRet = _T("VT_BYREF");
			break;
		case VT_RESERVED:
			strRet = _T("VT_RESERVED");
			break;
		case VT_ILLEGAL:
			strRet = _T("VT_ILLEGAL");
			break;
		case VT_ILLEGALMASKED:
			strRet = _T("VT_ILLEGALMASKED");
			break;
		case VT_ARRAY | VT_UI1:
			strRet = _T("BLOBField");
			break;
		default:
			strRet = _T("Default");
			break;
	}
	return strRet;
}

BOOL CLRYCTRLApp::OpenNewDocument(CDocument **pDoc ,const CString &strTarget)
{
	CString strDocName;
	CDocTemplate * pSelectedTemplate;
 
	POSITION pos = GetFirstDocTemplatePosition();
	while(pos!=NULL)
	{
		pSelectedTemplate = (CDocTemplate*)GetNextDocTemplate(pos);
		ASSERT(pSelectedTemplate!=NULL);
		ASSERT(pSelectedTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		pSelectedTemplate->GetDocString(strDocName,CDocTemplate::docName);
		if(strDocName==strTarget)
		{
			*pDoc = pSelectedTemplate->OpenDocumentFile(NULL);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CLRYCTRLApp::OpenNewDocument(const CString &strTarget)
{
	CString strDocName;
	CDocTemplate *pSelectedTemplate;
	POSITION pos = GetFirstDocTemplatePosition();
	while(pos!=NULL)
	{
		pSelectedTemplate = (CDocTemplate*)GetNextDocTemplate(pos);
		ASSERT(pSelectedTemplate!=NULL);
		ASSERT(pSelectedTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		pSelectedTemplate->GetDocString(strDocName,CDocTemplate::docName);
		if(strDocName==strTarget)
		{
			pSelectedTemplate->OpenDocumentFile(NULL);
			return TRUE;
		}
	}
	return FALSE;
}


 

void CLRYCTRLApp::OnQryE() 
{
	// TODO: Add your command handler code here
  	POSITION posDocList;
  	CObList DocTempList;
  	CObList DocList;
  	GetDocTemplateList(&DocTempList);
  	posDocList = DocTempList.GetHeadPosition();
  	for(int j=0;j<DocTempList.GetCount();j++)
  	{		
  			CDocTemplate *DocTemplate =	(CDocTemplate *)DocTempList.GetNext(posDocList);
  			CString DocTypeString;
  			DocTemplate->GetDocString( DocTypeString, CDocTemplate::docName );
  			if(DocTypeString==_T("量热仪热容量查询"))
  			{
  				GetDocList(&DocList,DocTemplate);	
  				if(DocList.GetCount()>0)
  				{
  					//如果已经打开文档,将该文档的视图提前
  					posDocList = DocList.GetHeadPosition();
  					CDocument * pDoc = (CDocument * )DocList.GetNext(posDocList);
  					//ID_RECORD_REFRESH
  					
  					POSITION posView;
  					CView	 *pView;
  					posView = pDoc->GetFirstViewPosition();
  					pView   = pDoc->GetNextView(posView);
  					CFrameWnd *pFrameWnd = (CFrameWnd *)pView->GetParentFrame();
  					((CMDIFrameWnd *)m_pMainWnd)->MDIActivate(pFrameWnd);
  					return;
  				}
  				else
  					OpenNewDocument(_T("量热仪热容量查询"));
  				break;
  			} 
  	} 	
}


void CLRYCTRLApp::OnQryG()
{
	OpenSingleDoc(_T("工业分析仪查询"));
}

void CLRYCTRLApp::OnQryS()
{
	OpenSingleDoc(_T("全硫查询"));
}

void CLRYCTRLApp::OnQryQ() 
{
	// TODO: Add your command handler code here
  	POSITION posDocList;
  	CObList DocTempList;
  	CObList DocList;
  	GetDocTemplateList(&DocTempList);
  	posDocList = DocTempList.GetHeadPosition();
  	for(int j=0;j<DocTempList.GetCount();j++)
  	{		
  			CDocTemplate *DocTemplate =	(CDocTemplate *)DocTempList.GetNext(posDocList);
  			CString DocTypeString;
  			DocTemplate->GetDocString( DocTypeString, CDocTemplate::docName );
  			if(DocTypeString==_T("量热仪发热量查询"))
  			{
  				GetDocList(&DocList,DocTemplate);	
  				if(DocList.GetCount()>0)
  				{
  					//如果已经打开文档,将该文档的视图提前
  					posDocList = DocList.GetHeadPosition();
  					CDocument * pDoc = (CDocument * )DocList.GetNext(posDocList);
  					//ID_RECORD_REFRESH
  					
  					POSITION posView;
  					CView	 *pView;
  					posView = pDoc->GetFirstViewPosition();
  					pView   = pDoc->GetNextView(posView);
  					CFrameWnd *pFrameWnd = (CFrameWnd *)pView->GetParentFrame();
  					((CMDIFrameWnd *)m_pMainWnd)->MDIActivate(pFrameWnd);
  					return;
  				}
  				else
  					OpenNewDocument(_T("量热仪发热量查询"));
  				break;
  			} 
  	} 		
}



void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CLRYCTRLApp::OnDeviceAttrib() 
{
	// TODO: Add your command handler code here
	CString strDeviceName[3];
	strDeviceName[0].LoadString(ID_STR_DEV_LRY );
	strDeviceName[1].LoadString(ID_STR_DEV_CLY); 
	strDeviceName[2].LoadString(ID_STR_DEV_GYFXY);
	CString strParent = GetCurrParentSeleString();
	if(strParent!=_T(""))
	{ 		
		if(strDeviceName[0] ==strParent)
		{ 
			CDeviceSetupDlg dlg;
			dlg.DoModal();
		}
		else if( strDeviceName[1] ==strParent)
		{ 
			CClyDlgDeviceSetup dlgCly;
			dlgCly.DoModal();
		} 
		else if( strDeviceName[2] ==strParent)
		{ 
			CGyfxyDlgDeviceSetup dlgGyfxy;
			dlgGyfxy.DoModal();
		}  
	}
	
}






LONG CLRYCTRLApp::CrashHandler(EXCEPTION_POINTERS *pException)
{
	// 在这里添加处理程序崩溃情况的代码     
	// 这里以弹出一个对话框为例子    
	CreateDumpFile(_T("LRYCTRL.dmp"), pException);  
	return EXCEPTION_EXECUTE_HANDLER;  
}


void CLRYCTRLApp::CreateDumpFile(LPTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
{														//LPCSTR
	// 创建Dump文件   
	//   
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
	
	// Dump信息   
    //   
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
	dumpInfo.ExceptionPointers = pException;  
    dumpInfo.ThreadId = GetCurrentThreadId();  
	dumpInfo.ClientPointers = TRUE;  
	
    // 写入Dump文件内容   
	//   
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  
	
	CloseHandle(hDumpFile);  
}  

CString GetCurrParentSeleString()
{
	CMainFrame * pFrame				= (CMainFrame *)AfxGetApp()->m_pMainWnd;
	COfficeTreeCtrl * pTreeCtrl		= (COfficeTreeCtrl *)&pFrame->m_wndDeviceView.m_wndClassView;
	HTREEITEM    hItem				= pTreeCtrl->GetSelectedItem();
	hItem = pTreeCtrl->GetParentItem(hItem);
	return pTreeCtrl->GetItemText(hItem);
}

CString GetCurrentSeleString(void)
{
	CTreeCtrl  * pTreeCtrl = &((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_wndDeviceView.m_wndClassView;
	HTREEITEM    hItem     = pTreeCtrl->GetSelectedItem();
	return pTreeCtrl->GetItemText(hItem);
}

void CLRYCTRLApp::InitGDIPlus()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}


int CLRYCTRLApp::Run() 
 {
 
 	// TODO: Add your specialized code here and/or call the base class
 	int i;
 #ifdef _DEBUG
 	i = CWinApp::Run();
 #else
 	__try
 	{
 		i= CWinApp::Run();
 	}
 	__except(CrashHandler(GetExceptionInformation()))  
 	{ 	
		AfxMessageBox(_T("程序异常退出,请将LRYCTRL.DMP 文件\n发送到xukaiming@126.com "));
 	}
 #endif
 	return i;
 }
 

void CLRYCTRLApp::OpenSingleDoc(CString DocTypeName)
{
	POSITION posDocList;
	CObList DocTempList;
	CObList DocList;
	GetDocTemplateList(&DocTempList);
	posDocList = DocTempList.GetHeadPosition();
	for(int j=0;j<DocTempList.GetCount();j++)
	{		
		CDocTemplate *DocTemplate =	(CDocTemplate *)DocTempList.GetNext(posDocList);
		CString DocTypeString;
		DocTemplate->GetDocString( DocTypeString, CDocTemplate::docName );
		if(DocTypeString==DocTypeName)
		{
			GetDocList(&DocList,DocTemplate);	
			if(DocList.GetCount()>0)
			{
				//如果已经打开文档,将该文档的视图提前
				posDocList = DocList.GetHeadPosition();
				CDocument * pDoc = (CDocument * )DocList.GetNext(posDocList);
				//ID_RECORD_REFRESH

				POSITION posView;
				CView	 *pView;
				posView = pDoc->GetFirstViewPosition();
				pView   = pDoc->GetNextView(posView);
				CFrameWnd *pFrameWnd = (CFrameWnd *)pView->GetParentFrame();
				((CMDIFrameWnd *)m_pMainWnd)->MDIActivate(pFrameWnd);
				return;
			}
			else
				OpenNewDocument(DocTypeName);
			break;
		} 
	} 	
}

void CLRYCTRLApp::OnCorrFact()
{
	// TODO: Add your command handler code here
	CClyCoffDlg clyDlg;
	clyDlg.DoModal();
}

void CLRYCTRLApp::OnUpdateMenu(CCmdUI *pCmdUI)
{
	CString strCly;
	strCly.LoadString(ID_STR_DEV_CLY);
	//if(GetCurrParentSeleString()!=strCly)
		pCmdUI->Enable(FALSE);
}










////////////////////////////////
//线性回归方程
double GetLxxOrLyy(IN double Matrix[],IN int MaxTrixCount,OUT double &Average)
{
	int i;
	double Result  = 0.0f;
	Average = 0.0f;
	for(i=0;i<MaxTrixCount;i++)
	{
		Average += Matrix[i];
	}
	Average/= MaxTrixCount;
	//求出Xi的平均值
	for(i=0;i<MaxTrixCount;i++)
	{
		Result +=  pow(Matrix[i]-Average,2); 
	}
	return Result; 
}

double GetLxy(double MatrixX[],double MatrixY[],int MaxTrixCount)
{
	int i;
	double Result  = 0.0f;
	double AverageX = 0.0f;
	double AverageY = 0.0f;
	for(i=0;i<MaxTrixCount;i++)
	{
		AverageX += MatrixX[i];
		AverageY += MatrixY[i];
	}
	AverageX/= MaxTrixCount;
	AverageY/= MaxTrixCount;
	//求出Xi和Yi的平均值
	///////////////////////////////////////////////////
	for(i=0;i<MaxTrixCount;i++)
	{
		Result +=  (MatrixX[i]-AverageX)*(MatrixY[i]-AverageY); 
	}
	return Result; 
}

double CheckCorrelationcoeffR(double Lxx,double Lyy,double Lxy)
{
	double R = Lxy/sqrt(Lxx*Lyy); 
	return R ;
}

BOOL GetALinearRegressionEquation(IN double MatrixX[],IN double MatrixY[],IN int MaxTrixCount,
								  OUT double &slope,OUT double &intercept,OUT double &R )//斜率 截距
{
	ASSERT(MaxTrixCount>1);
	BOOL bResult = TRUE; 
	double AverageX = 0;
	double AverageY = 0; 
	double Lxx = GetLxxOrLyy(MatrixX,MaxTrixCount,AverageX);
	double Lyy = GetLxxOrLyy(MatrixY,MaxTrixCount,AverageY);
	double Lxy = GetLxy(MatrixX,MatrixY,MaxTrixCount);
	slope		= Lxy/Lxx;
	intercept	= AverageY-slope*AverageX;
	//得到斜率和截距
	R = CheckCorrelationcoeffR(Lxx,Lyy,Lxy);
	//有效性检查   //Correlation coefficient 
	if(R<0.95)
		bResult = FALSE;
	return bResult;
}

void  LinearRegressionEquation(IN double MatrixX[],IN int MaxTrixCount,
							   IN double slope,IN double intercept, OUT double MatrixY[])
{
	for(int i=0;i<MaxTrixCount;i++)
	{
		MatrixY[i]=MatrixX[i]*slope+intercept;
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString date(__DATE__);
	SetDlgItemText(IDC_DATE,date);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//将wchar_t* 转成char*的实现函数如下：
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len)
{
	size_t nlength=wcslen(pwstr);
	//获取转换后的长度
	size_t nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL );    // we don't want to know if a character didn't make it through the translation
	// make sure the buffer is big enough for this, making it larger if necessary
	if(nbytes>len)   nbytes=len;
	// 通过以上得到的结果，转换unicode 字符为ascii 字符
	WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,   // wide character string to convert
		nlength,   // the number of wide characters in that string
		pcstr, // put the output ascii characters at the end of the buffer
		nbytes,                           // there is at least this much space there
		NULL,      // no replacement character given
		NULL );
	return pcstr ;
}

//将char* 转成wchar_t*的实现函数如下：
//这是把asii字符转换为unicode字符，和上面相同的原理
void c2w(wchar_t *pwstr,size_t len,const char *str)
{
	if(str)
	{
		size_t nu = strlen(str);
		size_t n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str,(int)nu,NULL,0);
		if(n>=len)n=len-1;
		MultiByteToWideChar(CP_ACP,0,(const char *)str,(int)nu,pwstr,(int)n);
		pwstr[n]=0;
	}
}



void CAboutDlg::OnBnClickedCheck6()
{
	// TODO: Add your control notification handler code here
}


void CLRYCTRLApp::LoadExternFont(TCHAR *pFileName)
{  
	if(!AddFontResource(pFileName))
	{
	  TRACE(_T("Load font failed."));
	  return ;
	}
	if(IsWindow(AfxGetMainWnd()->GetSafeHwnd()))
		::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0); 
}


BEGIN_MESSAGE_MAP(COfficeTreeCtrl, CXTPOfficeBorder<CTreeCtrl>)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void COfficeTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CXTPOfficeBorder<CTreeCtrl>::OnLButtonDblClk(nFlags, point);
}
