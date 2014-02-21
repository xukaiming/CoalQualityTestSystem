// AutoSampNODlg.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "AutoSampNODlg.h" 


// CGyfxyAutoSampNODlg dialog

IMPLEMENT_DYNAMIC(CGyfxyAutoSampNODlg, CDialog)

CGyfxyAutoSampNODlg::CGyfxyAutoSampNODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGyfxyAutoSampNODlg::IDD, pParent)
{
	Max_WaterAshCnt = 5;
	Max_VolCnt		= 5;
}

CGyfxyAutoSampNODlg::~CGyfxyAutoSampNODlg()
{
}

void CGyfxyAutoSampNODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WATER_ASH, m_ListWaterAsh);
	DDX_Control(pDX, IDC_LIST_VOL, m_ListVol);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_CHECK_WATER_ASH_PX, m_CheckTestWaterAshPX);
	DDX_Control(pDX, IDC_CHECK_VOL_PX, m_CheckTestVolPX);
	DDX_Control(pDX, IDC_CHECK_TEST_WATER, m_CheckTestWater);
	DDX_Control(pDX, IDC_CHECK_TEST_ASH, m_CheckTestAsh);
	DDX_Control(pDX, IDC_CHECK_TEST_VOL, m_CheckTestVol);
	DDX_Control(pDX, IDC_CHECK_SYNC, m_CheckSYNC);
 
}


BEGIN_MESSAGE_MAP(CGyfxyAutoSampNODlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGyfxyAutoSampNODlg::OnBnClickedOk) 
	ON_COMMAND_RANGE(IDC_CHECK_WATER_ASH_PX, IDC_CHECK_SYNC,&CGyfxyAutoSampNODlg::OnBnClickedCheck) 
	ON_CBN_SELCHANGE(IDC_COMBO_ASH_CNT, &CGyfxyAutoSampNODlg::OnCbnSelchangeComboAshCnt)
	ON_CBN_SELCHANGE(IDC_COMBO_VOL_CNT, &CGyfxyAutoSampNODlg::OnCbnSelchangeComboVolCnt)
	ON_COMMAND(ID_DELETE_RECORD, &CGyfxyAutoSampNODlg::OnDeleteRecord) 
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd) 
	ON_WM_SETFOCUS()
END_MESSAGE_MAP() 
// CGyfxyAutoSampNODlg message handlers

void CGyfxyAutoSampNODlg::MakeSYNCSampleInfo(void)
{ 
	//1.2		5.6		9.10    
	//13.14    17.18	21.22号位放置挥发分坩埚 
	//3.4		7.8		11.12
	//15.16	   19.20	23.24号放置水灰分坩埚  
	CString SampleName;
	LVITEM item;
	int i ,j,k= 0; 
	int VolFixCnt = 0;
	int AshFixCnt = 0;
	/////////////////////
	int SampleCnt = 0;
	int VolSampleCnt=0,WaterSampleCnt=0,AshSampleCnt=0; //已被分配样品数目
	int DisableMask,EnableMask;
	CGyfxyRDB_BaseRdb::ALLSample *pSampleInfo = &pRdb->SampleArray;
	if(bTestVolPX_Checked)  //平行样测试
	{ 
		VolFixCnt = 1;
		AshFixCnt = 1;
		for (i=0;i<MAX_SAMPLE_CNT/2;i+=4)
		{   
			for (j=0;j<2;j++) //2为两个炉子
			{ 
				VolSampleCnt++;
				WaterSampleCnt++;
				AshSampleCnt++; 

				
				if(VolSampleCnt>VolCnt+VolFixCnt) 
				{ 
					//清除实验项目
					EnableMask  =  0;
					DisableMask = ~EnableMask;
					pSampleInfo->RefreshSampleData(pImg,i+1+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);
					pSampleInfo->RefreshSampleData(pImg,i+2+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask); 
					pSampleInfo->RefreshSampleData(pImg,i+3+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);
					pSampleInfo->RefreshSampleData(pImg,i+4+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);	 
					
					//m_ListWaterAsh
					//m_ListVol
					//SampleName
				}
				else
				{ 
					if((i==0)&&(j==0))
					{ 
						SampleName = _T("挥发分校准坩埚");
						pSampleInfo->Sample[i+0+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;
						pSampleInfo->Sample[i+1+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;  
						SampleName = _T("水分/灰分校准坩埚");
						pSampleInfo->Sample[i+2+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName; 
						pSampleInfo->Sample[i+3+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;
					}
					else
					{
						SampleName = m_ListWaterAsh.GetItemText(i/2+j-1,0); 
						pSampleInfo->Sample[i+0+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;
						pSampleInfo->Sample[i+1+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;  
						pSampleInfo->Sample[i+2+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName; 
						pSampleInfo->Sample[i+3+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;
					}
					//1.2号位放置挥发分坩埚
					EnableMask = bTestVol_Checked?CGyfxyRDB_BaseRdb::ALLSample::TEST_VOL:0;
					DisableMask = ~EnableMask;
					pSampleInfo->RefreshSampleData(pImg,i+1+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);
					pSampleInfo->RefreshSampleData(pImg,i+2+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);
					//3.4号放置水灰分坩埚
					EnableMask = (bTest_Ash_Checked?CGyfxyRDB_BaseRdb::ALLSample::TEST_ASH:0)|
						(bTest_Water_Checked?CGyfxyRDB_BaseRdb::ALLSample::TEST_WATER:0);
					DisableMask = ~EnableMask;
					pSampleInfo->RefreshSampleData(pImg,i+3+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);
					pSampleInfo->RefreshSampleData(pImg,i+4+j*MAX_SAMPLE_CNT/2,EnableMask,DisableMask);	   
				}
			}
		} 
	}
	else    //单样测试
	{
		//两个修正坩埚
		VolFixCnt = 2;
		AshFixCnt = 2;
		for (i=0;i<MAX_SAMPLE_CNT/2;i+=4)
		{   
			for (j=0;j<2;j++) //2为两个炉子
			{
				for (k=0;k<2;k++) //2为每个炉子有两根杆子
				{
					VolSampleCnt++;
					WaterSampleCnt++;
					AshSampleCnt++; 
					//SampleName = m_ListWaterAsh.GetItemText(i/2,0);
					if(VolSampleCnt>VolCnt+VolFixCnt) 
					{ 
						//清除实验项目
						EnableMask  =  0;
						DisableMask = ~EnableMask;
						pSampleInfo->RefreshSampleData(pImg,i+1+j*MAX_SAMPLE_CNT/2+k,EnableMask,DisableMask); 
						pSampleInfo->RefreshSampleData(pImg,i+3+j*MAX_SAMPLE_CNT/2+k,EnableMask,DisableMask); 
					}
					else
					{ 
						if((i==0)&&(j==0))
						{ 
							SampleName = _T("挥发分校准坩埚");
							pSampleInfo->Sample[k].cSampleName = SampleName ; 
							SampleName = _T("水分/灰分校准坩埚");
							pSampleInfo->Sample[k+2].cSampleName = SampleName;  
						}
						else 
						{
							SampleName = m_ListWaterAsh.GetItemText(i+j*2+k-2,0); 
							pSampleInfo->Sample[i+0+k+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName;
							pSampleInfo->Sample[i+2+k+j*MAX_SAMPLE_CNT/2].cSampleName = SampleName; 
						}
						//1.2号位放置挥发分坩埚
						EnableMask = bTestVol_Checked?CGyfxyRDB_BaseRdb::ALLSample::TEST_VOL:0;
						DisableMask = ~EnableMask;
						pSampleInfo->RefreshSampleData(pImg,i+1+j*MAX_SAMPLE_CNT/2+k,EnableMask,DisableMask); 
						//3.4号放置水灰分坩埚
						EnableMask = (bTest_Ash_Checked?CGyfxyRDB_BaseRdb::ALLSample::TEST_ASH:0)|
							(bTest_Water_Checked?CGyfxyRDB_BaseRdb::ALLSample::TEST_WATER:0);
						DisableMask = ~EnableMask;
						pSampleInfo->RefreshSampleData(pImg,i+3+j*MAX_SAMPLE_CNT/2+k,EnableMask,DisableMask);   
					}
				}
				
			}
		} 
	} 
	//END:; 
}

void CGyfxyAutoSampNODlg::MakeSampleInfo(void)
{ 
	//如果同步
	if (bSYNC_Checked)
	{
		//全部测试平行样     
		if(bTestWateAshPX_Checked==bTestVolPX_Checked)
		{
			MakeSYNCSampleInfo();	
		}

	}
	//如果不同步
	else
	{
	}

	WaterAshCnt ;
	VolCnt		; 
				;		//测试灰分
				;		//测试水分
				;		//测试挥发分
			;		//测试灰分水分平行样
	bTestVolPX_Checked			;		//测试挥发分平行样
}

void CGyfxyAutoSampNODlg::OnBnClickedOk()
{ 
	//计算RDB
	MakeSampleInfo();
	OnOK();
}

void CGyfxyAutoSampNODlg::CheckChkboxState(void)
{
	bSYNC_Checked				= m_CheckSYNC.GetCheck();			//同步	
	bTest_Ash_Checked			= m_CheckTestAsh.GetCheck();		//测试灰分
	bTest_Water_Checked			= m_CheckTestWater.GetCheck();		//测试水分
	bTestVol_Checked			= m_CheckTestVol.GetCheck();		//测试挥发分
	bTestWateAshPX_Checked		= m_CheckTestWaterAshPX.GetCheck();	//测试灰分水分平行样
	bTestVolPX_Checked			= m_CheckTestVolPX.GetCheck();		//测试挥发分平行样

	//
}

void CGyfxyAutoSampNODlg::AutoSample(void)
{

}

void CGyfxyAutoSampNODlg::CheckGUI(INT nID)
{ 
	CComboBox *pComboVolCnt    		= (CComboBox*)GetDlgItem(IDC_COMBO_VOL_CNT);
	CComboBox *pComboWaterAshCnt	= (CComboBox*)GetDlgItem(IDC_COMBO_ASH_CNT);
	if(bSYNC_Checked)//平行样按钮选中
	{		
		if(IDC_CHECK_VOL_PX==nID)
		{
			m_CheckTestWaterAshPX.SetCheck(bTestVolPX_Checked );
		}
		if(IDC_CHECK_WATER_ASH_PX==nID)
		{
			m_CheckTestVolPX.SetCheck(bTestWateAshPX_Checked);
		} 
		//同步,所有checkbox按钮选中
		if(IDC_CHECK_SYNC==nID)
		{
			CheckAllCheckBox(true);
		}	
	}
	////////////////////////////////////////////////////////////////////////////// 
	if ((nID==IDC_CHECK_TEST_WATER)||(nID==IDC_CHECK_TEST_ASH))
	{
		if((bTest_Ash_Checked==bTest_Water_Checked)&&
			(bTest_Water_Checked==0)) //都等于0的情况下
		{
			m_CheckSYNC.SetCheck(false);
			m_CheckTestWaterAshPX.SetCheck(false);
			m_CheckTestWaterAshPX.EnableWindow(false);
			WaterAshCnt=0;
			//Notify message
			SendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_ASH_CNT,CBN_SELCHANGE),(LPARAM)pComboWaterAshCnt->m_hWnd);
		}
		else
		{
			m_CheckTestWaterAshPX.EnableWindow(true);
		}
		//bTestWateAshPX_Checked
	}
	if(nID==IDC_CHECK_TEST_VOL)
	{
		if (bTestVol_Checked)
		{ 
			//pCheckTestVolPX.SetCheck(true);
			m_CheckTestVolPX.EnableWindow(true);
		} 
		else
		{
			m_CheckSYNC.SetCheck(false);
			m_CheckTestVolPX.SetCheck(false);
			m_CheckTestVolPX.EnableWindow(false);
			VolCnt=0; 
			SendMessage(WM_COMMAND,MAKEWPARAM(IDC_COMBO_VOL_CNT,CBN_SELCHANGE),(LPARAM)pComboVolCnt->m_hWnd);
		}

	}
}

void CGyfxyAutoSampNODlg::GetTestSampleCnt()
{
	CComboBox *pComboVolCnt    		= (CComboBox*)GetDlgItem(IDC_COMBO_VOL_CNT);
	CComboBox *pComboWaterAshCnt	= (CComboBox*)GetDlgItem(IDC_COMBO_ASH_CNT);
	WaterAshCnt = pComboWaterAshCnt->GetCurSel();
	WaterAshCnt = WaterAshCnt>0?WaterAshCnt:0;
	VolCnt		= pComboVolCnt->GetCurSel();
	VolCnt		= VolCnt>0?VolCnt:0;
}

void CGyfxyAutoSampNODlg::CalcSampleCnt(INT nID)
{ 
	CComboBox *pComboVolCnt    		= (CComboBox*)GetDlgItem(IDC_COMBO_VOL_CNT);
	CComboBox *pComboWaterAshCnt	= (CComboBox*)GetDlgItem(IDC_COMBO_ASH_CNT);
	if(bSYNC_Checked)
	{	//如果被选中,各10个单样或者5个平行样	 
		Max_WaterAshCnt = Max_VolCnt = (MAX_SAMPLE_CNT-4)/2;	 
		WaterAshCnt=VolCnt=Max_VolCnt;
		if (bTestWateAshPX_Checked)
		{
			WaterAshCnt = Max_WaterAshCnt>>=1;      
		}
		if (bTestVolPX_Checked)
		{
			VolCnt = Max_VolCnt>>=1;       
		} 
		////////////////////////////////
		if (nID==IDC_COMBO_ASH_CNT)
		{ 
			VolCnt = WaterAshCnt	= pComboWaterAshCnt->GetCurSel();	
		} 
		/////////////////////////////////////////////////////////
		if (nID==IDC_COMBO_VOL_CNT)
		{
			VolCnt = WaterAshCnt	= pComboVolCnt->GetCurSel();  
		} 
	}
	else
	{	
		//如果未被选中,最大做20个单样 		
		///////////////////////////////////////////////////////////////
		int AllCnt	= MAX_SAMPLE_CNT - 4;  //所有的坩埚孔数
		if ((nID==IDC_COMBO_ASH_CNT)||
			(IDC_CHECK_TEST_WATER==nID)||
			(IDC_CHECK_TEST_ASH==nID)||
			(IDC_CHECK_WATER_ASH_PX==nID)
			)
		{ 
			Max_WaterAshCnt = WaterAshCnt;
			if(bTestWateAshPX_Checked)
			{ 
				Max_VolCnt	= (AllCnt-WaterAshCnt*2);	 //做单样最多20个	
				//有效性检查
				if(Max_VolCnt<0) 
				{
					WaterAshCnt = AllCnt>>1; 
					Max_VolCnt	= 0;
				}
				Max_VolCnt =  bTestVolPX_Checked?Max_VolCnt>>1:Max_VolCnt; 
			}
			else
			{
				Max_VolCnt	= (AllCnt-WaterAshCnt);	 //做单样最多20个	 
				Max_VolCnt =  bTestVolPX_Checked?Max_VolCnt>>1:Max_VolCnt; 
			}			
		} 
		/////////////////////////////////////////////////////////
		if ((nID==IDC_COMBO_VOL_CNT)||		
			(IDC_CHECK_TEST_VOL==nID)||
			(IDC_CHECK_VOL_PX==nID)
			)
		{
			Max_VolCnt = VolCnt;
			if(bTestVolPX_Checked)
			{
				Max_WaterAshCnt	= (AllCnt-VolCnt*2);	 //做单样最多20个	

				if(Max_WaterAshCnt<0) 
				{
					VolCnt			= AllCnt>>1; 
					Max_WaterAshCnt	= 0;
				}
				Max_WaterAshCnt =  bTestWateAshPX_Checked?Max_WaterAshCnt>>1:Max_WaterAshCnt; 
			}
			else
			{ 
				Max_WaterAshCnt	= (AllCnt-VolCnt);	 //做单样最多20个	
				Max_WaterAshCnt =  bTestWateAshPX_Checked?Max_WaterAshCnt>>1:Max_WaterAshCnt; 
			}

		}  
	}
}
void CGyfxyAutoSampNODlg::CalcParam(INT nID)
{  
	CComboBox *pComboVolCnt    		= (CComboBox*)GetDlgItem(IDC_COMBO_VOL_CNT);
	CComboBox *pComboWaterAshCnt	= (CComboBox*)GetDlgItem(IDC_COMBO_ASH_CNT);
	CheckChkboxState();  //得到当前的选中状态
	CheckGUI(nID);       //根据点击控件的ID选中/取消 连锁控件的
	CheckChkboxState();  //得到新的选中状态
	GetTestSampleCnt();
	CalcSampleCnt(nID);
	////////////////////////////////////////////////////////
	SetTestSampleCount(pComboWaterAshCnt,Max_WaterAshCnt ,WaterAshCnt );
	SetTestSampleCount(pComboVolCnt,Max_VolCnt ,VolCnt); 
	SetListCtrlCount(&m_ListWaterAsh,WaterAshCnt);
	SetListCtrlCount(&m_ListVol,VolCnt);
	/////////////////////////////////////////////////////////

}

void CGyfxyAutoSampNODlg::SetListCtrlCount(CXListCtrl *pListCtrl,int count)
{
	int i;
	CString str;

	if(pListCtrl->GetItemCount()==count)
		return;
	while((i=pListCtrl->GetItemCount())>count)
	{
		pListCtrl->DeleteItem(i-1);
	}
	while((i=pListCtrl->GetItemCount())<count)
	{
		str.Format(_T("样品编号%d"),i);
		pListCtrl->InsertItem(i,str);
		pListCtrl->SetEdit(i,0);
	}

}

void CGyfxyAutoSampNODlg::SetTestSampleCount(CComboBox * pListBox,int MaxCount,int count)
{
	CString str;
	pListBox->ResetContent();
	for (int i=0;i<=MaxCount;i++)
	{
		str.Format(_T("%d"),i);
		pListBox->InsertString(i,str);
		
	}
	pListBox->SetCurSel(count>MaxCount?MaxCount:count); 
}

BOOL CGyfxyAutoSampNODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

 
	m_ListWaterAsh.InsertColumn(0,_T("编号"));
	m_ListWaterAsh.InsertColumn(1,_T(""));
	m_ListWaterAsh.SetColumnWidth(0,180); 
	m_ListWaterAsh.SetColumnWidth(1,180); 

	m_ListVol.InsertColumn(0,_T("编号"));
	m_ListVol.InsertColumn(1,_T(""));
	m_ListVol.SetColumnWidth(0,180); 
	m_ListVol.SetColumnWidth(1,180); 

	m_ListVol.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListWaterAsh.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	/////////////////////////////////////////////
	CheckAllCheckBox(true);
	CalcParam(0);

	m_OKButton.SetBitmap(0, IDB_OK);
	m_OKButton.SetUseVisualStyle(true);	
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGyfxyAutoSampNODlg::OnBnClickedCheck(UINT nID)
{
	CalcParam(nID);
}

BOOL   CALLBACK   CGyfxyAutoSampNODlg::EnumChildProc(HWND   hwndChild,LPARAM   lParam)
{
	TCHAR   szWnd[1024]; 
	GetClassName(hwndChild,szWnd,1024);	
	CWnd *pWnd=CWnd::FromHandle(hwndChild); 
	if(_tcscmp(szWnd,_T("Button"))==0) 
	{
		CButton *pBut = (CButton *)pWnd;
		pBut->SetCheck(lParam); 
		if (lParam==TRUE)
		{
			pBut->EnableWindow(lParam); 
		}
	}
	return TRUE;
}

//  
void CGyfxyAutoSampNODlg::CheckAllCheckBox(bool bChecked)
{
	EnumChildWindows(m_hWnd,(WNDENUMPROC)EnumChildProc,LPARAM(bChecked));
}

void CGyfxyAutoSampNODlg::OnCbnSelchangeComboAshCnt()
{
	CalcParam(IDC_COMBO_ASH_CNT); 
}

void CGyfxyAutoSampNODlg::OnCbnSelchangeComboVolCnt()
{
	CalcParam(IDC_COMBO_VOL_CNT);
}

void CGyfxyAutoSampNODlg::OnDeleteRecord()
{
	//http://support.microsoft.com/?kbid=222829
	//在dialog中实现快捷键
	CWnd *pWnd = GetFocus();

	CComboBox *pComboVolCnt    		= (CComboBox*)GetDlgItem(IDC_COMBO_VOL_CNT);
	CComboBox *pComboWaterAshCnt	= (CComboBox*)GetDlgItem(IDC_COMBO_ASH_CNT);

	CXListCtrl *pListCtrl = NULL;
	int selectId;
	if(((pListCtrl=dynamic_cast<CXListCtrl*>(pWnd))!=NULL)&&
		(pListCtrl->GetCurSel()!=-1)&&
		(ID_OK==AfxMessageBox(_T("确认删除该编号?"),MB_OKCANCEL)))
	{
		selectId = pListCtrl->GetCurSel();
		WaterAshCnt = pComboWaterAshCnt->GetCurSel();
		VolCnt		= pComboVolCnt->GetCurSel();
		
		if(m_CheckSYNC.GetCheck()==TRUE)
		{
			m_ListWaterAsh.DeleteItem(selectId);
			m_ListVol.DeleteItem(selectId);
			pComboWaterAshCnt->SetCurSel(WaterAshCnt-1);
			pComboVolCnt->SetCurSel(VolCnt-1);

			CalcParam(IDC_COMBO_ASH_CNT);
		}
		else
		{
			pListCtrl->DeleteItem(selectId);
			if(&m_ListVol==pListCtrl)
			{
				pComboVolCnt->SetCurSel(VolCnt-1);
				CalcParam(IDC_COMBO_VOL_CNT);				
			}
			else
			{
				pComboWaterAshCnt->SetCurSel(WaterAshCnt-1);
				CalcParam(IDC_COMBO_ASH_CNT);
			}
		}
			
	}
	
}

BOOL CGyfxyAutoSampNODlg::PreTranslateMessage(MSG* pMsg)
{ 	 
	if (m_hAccel&&::TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) 
	{
		return(TRUE);
	} 
	return CDialog::PreTranslateMessage(pMsg); 
}

LRESULT CGyfxyAutoSampNODlg::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
	CXListCtrl *pListCtrl ;
	return 0;
}

 