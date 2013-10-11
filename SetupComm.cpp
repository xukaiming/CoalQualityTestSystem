// SetupComm.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "SetupComm.h"


// CSetupComm dialog

IMPLEMENT_DYNAMIC(CSetupComm, CDialog)

CSetupComm::CSetupComm(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupComm::IDD, pParent)
{

}

CSetupComm::~CSetupComm()
{
}

void CSetupComm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMM, m_XlistCtrl);
}


BEGIN_MESSAGE_MAP(CSetupComm, CDialog)
END_MESSAGE_MAP()


// CSetupComm message handlers

#include   <setupapi.h>   
#include   <devguid.h>   
#include   <regstr.h>   
#include   <shlwapi.h>   
#pragma       comment(lib,"setupapi.lib")   

unsigned char CSetupComm::EnumCommPort(CStringArray &PortArray,CStringArray &PortDescriptArray)
{
	//利用windows SDK 库函数读取硬件信息  
     HDEVINFO hDevInfo;
     SP_DEVINFO_DATA DeviceInfoData;
     DWORD DeviceIndex;
     // Create a HDEVINFO with all present devices.
     hDevInfo=SetupDiGetClassDevs((LPGUID) &GUID_DEVCLASS_PORTS,0,0,DIGCF_PRESENT);
     /*
     GUID_DEVCLASS_FDC软盘控制器
     GUID_DEVCLASS_DISPLAY显示卡
     GUID_DEVCLASS_CDROM光驱
     GUID_DEVCLASS_KEYBOARD键盘
     GUID_DEVCLASS_COMPUTER计算机
     GUID_DEVCLASS_SYSTEM系统
     GUID_DEVCLASS_DISKDRIVE磁盘驱动器
     GUID_DEVCLASS_MEDIA声音、视频和游戏控制器
     GUID_DEVCLASS_MODEMMODEM
     GUID_DEVCLASS_MOUSE鼠标和其他指针设备
     GUID_DEVCLASS_NET网络设备器
     GUID_DEVCLASS_USB通用串行总线控制器
     GUID_DEVCLASS_FLOPPYDISK软盘驱动器
     GUID_DEVCLASS_UNKNOWN未知设备
     GUID_DEVCLASS_SCSIADAPTERSCSI 和 RAID 控制器
     GUID_DEVCLASS_HDCIDE ATA/ATAPI 控制器
     GUID_DEVCLASS_PORTS端口（COM 和 LPT）
     GUID_DEVCLASS_MONITOR监视器
 */
     if (hDevInfo == INVALID_HANDLE_VALUE)
     {
         DWORD ErrorCode = GetLastError();
         // Insert error handling here.
         return 1;
     }
     // Enumerate through all devices in Set.
 
     DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
     
     for (DeviceIndex=0;SetupDiEnumDeviceInfo(hDevInfo,DeviceIndex,&DeviceInfoData);DeviceIndex++)
     {
         DWORD DataPropertyType;
         LPTSTR buffer = NULL;
		 DWORD buffersize = 0; 
				 // 
		 // Call function with null to begin with, 
		 // then use the returned buffer size 
		 // to Alloc the buffer. Keep calling until
		 // success or an unknown failure.
		 // 
         while (!SetupDiGetDeviceRegistryProperty(
             hDevInfo,
             &DeviceInfoData,
             SPDRP_FRIENDLYNAME,
             &DataPropertyType,
             (PBYTE)buffer,
             buffersize,
             &buffersize))
         {
             DWORD ErrorCode=GetLastError();
             if (ErrorCode== 
                 ERROR_INSUFFICIENT_BUFFER)
             {
                 // Change the buffer size.
                 if (buffer) LocalFree(buffer);
                 buffer = (LPTSTR)LocalAlloc(LPTR,buffersize);
             }
             else
             {
                 // Insert error handling here.
                 break;
             }
         }
         CString FriendlyPortName=buffer;   /*Extract Serial Port Name*/
         int pos=FriendlyPortName.Find('('); 
         CString SerialPortName=FriendlyPortName.Right(FriendlyPortName.GetLength()-pos-4);
         SerialPortName.TrimRight(')');
		 PortArray.Add(SerialPortName); 
		 PortDescriptArray.Add(FriendlyPortName);
         if(buffer) 
         {
             LocalFree(buffer);
         } 
     } 
     if (GetLastError()!=NO_ERROR
         &&GetLastError()!=ERROR_NO_MORE_ITEMS )
     {
         return 1;
     }
     // Cleanup
     SetupDiDestroyDeviceInfoList(hDevInfo);
     return 0; 
}

BOOL CSetupComm::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillHeader();
	EnumCommPort(m_PortArray,m_PorttDescriptArray);
	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupComm::FillHeader(void)
{
	CStringArray HeadTitleStr;
	HRESULT hr= S_OK;
	//CString strSQL;
	try
	{ 
		pRsDev.CreateInstance("ADODB.Recordset");
		strSQL.Format(_T("select * from 通信口")); 
		hr = pRsDev->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);

		GetAllFieldName(pRsDev,&HeadTitleStr); 
		//写字段标题
		for(int  i=HeadTitleStr.GetSize()-1;i>=0;i--)
		{
			CDC *pDC = GetDC();
			int iStrLen	= pDC->GetTextExtent(HeadTitleStr.GetAt(i)).cx*2;				
			m_XlistCtrl.InsertColumn(0,HeadTitleStr.GetAt(i),LVCFMT_CENTER,iStrLen ,35);
			ReleaseDC(pDC);	
		} 
		////////////////////////////////////////////////////////////

		DWORD ExStyle = m_XlistCtrl.GetExtendedStyle() ;
		m_XlistCtrl.SetExtendedStyle(ExStyle| LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//
		m_XlistCtrl.EnableToolTips(TRUE); 
		////////////////////////////////////////////////////////////
		m_XlistCtrl.InsertColumn(0,_T("使用"),LVCFMT_CENTER,70 ,55); 
		HDITEM hditem={0};
		TCHAR szText[40]=_T("");
		hditem.pszText = szText;
		hditem.cchTextMax = 40;
		hditem.mask = HDI_IMAGE | HDI_FORMAT|HDI_TEXT;
		m_XlistCtrl.m_HeaderCtrl.GetItem(0, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		//hditem.state = HDIS_FOCUSED; 
		m_XlistCtrl.m_HeaderCtrl.SetItem(0, &hditem);
		////////////////////////////////////////////////////////////

		////////////////////////////////////
		//设置字体高度
		CFont* pFont = (CFont*)m_XlistCtrl.GetFont();
		if (pFont) {
			LOGFONT lf;
			pFont->GetLogFont(&lf);
			lf.lfHeight = -18;
			m_listFont.CreateFontIndirect(&lf);
			m_XlistCtrl.SetFont(&m_listFont);
		} 
	}
	catch(_com_error &e)
	{ 
		e.ErrorMessage();
		dump_com_error(e);
	}   
}

void CSetupComm::FillList(void)
{
	int cnt = m_PortArray.GetCount();
	////////////////////////////////////////////

	///////////////////////////////////////////
	strCommType.RemoveAll();
	strCommType.Add(_T("RS485"));
	strCommType.Add(_T("RS232"));

	strCommSpeed.RemoveAll();
	strCommSpeed.Add(_T("9600"));

	strCommCheckBit.RemoveAll();
	strCommCheckBit.Add(_T("NONE"));

	strCommStopBit.RemoveAll();
	strCommStopBit.Add(_T("1"));

	strCommBit.RemoveAll();
	strCommBit.Add(_T("8"));
	///////////////////////////////////////////////
	//m_ListCtrl.SetEdit(i, 0); 
	for (int i=0;i<cnt;i++)
	{  
		m_XlistCtrl.InsertItem(i,_T(""));	
		m_XlistCtrl.SetCheckbox(i,0,1);

		m_XlistCtrl.SetItemText(i, 1,m_PortArray.GetAt(i));
		m_XlistCtrl.SetItemText(i, 2, _T("RS485"));
		m_XlistCtrl.SetItemText(i, 3, _T("9600"));
		m_XlistCtrl.SetItemText(i, 4, _T("NONE"));
		m_XlistCtrl.SetItemText(i, 5, _T("1"));
		m_XlistCtrl.SetItemText(i, 6, _T("8"));
		CString strDescript = m_PorttDescriptArray.GetAt(i);
		m_XlistCtrl.SetItemText(i, 7,strDescript);
		
		m_XlistCtrl.SetComboBox(i, 2, 
			TRUE,		// enable combobox
			&strCommType,// pointer to persistent CStringArray
			10,			// size of dropdown
			0,			// initial combo selection
			FALSE);		// sort CStringArray
		//m_XlistCtrl.SetEdit(i, 1); 
	}
	m_XlistCtrl.SetColumnWidth(7,500);
}
