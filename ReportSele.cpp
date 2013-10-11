// ReportSele.cpp : implementation file
//

#include "stdafx.h" 
#include "LRYCTRL.h"
#include "ReportSele.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportSele dialog


CReportSele::CReportSele(CWnd* pParent /*=NULL*/)
	: CDialog(CReportSele::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportSele)
	cFile = _T("");
	m_iSelect = 0;
	//}}AFX_DATA_INIT
}


void CReportSele::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportSele) 
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFileName);
	DDX_LBString(pDX, IDC_LIST_FILE, cFile);
	DDX_Radio(pDX, IDC_RADIO_PRINT, m_iSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportSele, CDialog)
	//{{AFX_MSG_MAP(CReportSele)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportSele message handlers

BOOL CReportSele::OnInitDialog() 
{
	CDialog::OnInitDialog();
	FindFile();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CReportSele::FindFile()
{
	CFileFind fileFind;
	int bWorking;
	CString strFile;
#ifndef _DEBUG
	//GetCurrentDirectory(MAX_PATH,cPath);
	//	TCHAR   cPath[MAX_PATH]; 
	GetModuleFileName(AfxGetApp()-> m_hInstance,cPath,MAX_PATH); 
	//strFile+=_T("\\reports\\∑¢»»¡ø*.rmf");
	LPTSTR   p=_tcsrchr(cPath, _T('\\'));p++;   //Ωÿ∂œ
	p[0]=0;
#else
	GetCurrentDirectory(MAX_PATH,cPath);
	_tcscat(cPath,_T("\\"));
#endif

	strFile = (cPath);
	strFile+=_T("reports\\");
	strFile+=cFileName;
	bWorking = fileFind.FindFile(strFile);
	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();
		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (fileFind.IsDots())
			 continue;

		// if it's a directory, recursively search it

		if (!fileFind.IsDirectory())
		{ 
			m_ListFileName.AddString(fileFind.GetFileName());
		}
		
	}

	fileFind.Close();

}
 
