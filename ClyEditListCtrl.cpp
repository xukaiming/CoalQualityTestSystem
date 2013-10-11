// ClyEditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LRYCTRL.h"
#include "ClyEditListCtrl.h"


// CClyEditListCtrl

IMPLEMENT_DYNAMIC(CClyEditListCtrl, CEditListCtrl)//CEditListCtrl

CClyEditListCtrl::CClyEditListCtrl()
{
	font = NULL;
}

CClyEditListCtrl::~CClyEditListCtrl()
{
	delete font;
}


BEGIN_MESSAGE_MAP(CClyEditListCtrl, CEditListCtrl)
ON_WM_SIZE()
END_MESSAGE_MAP()



// //填充标题栏



void CClyEditListCtrl::FillHeaderCtrl(CClyHostDoc *pDoc)
{
	
	int i =0;
	int iStrLen = 0;
	HRESULT hr= S_OK;
	_variant_t  str;
	CStringArray ColNameArray;
	BeginWaitCursor() ;
	try
	{ 
		_RecordsetPtr Rs ;
		CString strSQL;
		Rs.CreateInstance("ADODB.Recordset");
		strSQL = _T("select * from 测硫仪实验数据");
		if(Rs->State==adStateOpen)
			Rs->Close();
		//IADORecordBindingPtr pIRB(pRsIED);
		hr = Rs->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	
		TESTHR(hr);
		//得到所有字段名称
		GetAllFieldName(Rs,&ColNameArray);
  
		SetRedraw(FALSE);
		//清除所有数据

		DeleteAllItems();
		//清除所有标题
		do
		{
			DeleteColumn(0);
		}
		while(GetHeaderCtrl()->GetItemCount()>0); 
		//写字段标题
		for(i=ColNameArray.GetSize()-1;i>=0;i--)
		{
			CDC *pDC = GetDC();
			iStrLen	= pDC->GetTextExtent(ColNameArray.GetAt(i)).cx*1.5;				
			InsertColumn(0,ColNameArray.GetAt(i),LVCFMT_CENTER,iStrLen ,-1);
			ReleaseDC(pDC);	
		}  
		///////////////////////////////////
		/*
		LONG RecordCount = 0;
		RecordCount = Rs->GetRecordCount();
		SetItemCount(RecordCount);

		if(RecordCount>0)
		{
			if(!Rs->adoEOF)
				Rs->MoveLast();

			while(!Rs->BOF)
			{		
				for(i=0;i<Rs->Fields->Count;i++)
				{
					str = Rs->GetCollect((_bstr_t)ColNameArray.GetAt(i));
					if(i==0)
						InsertItem(0,VariantToStr(str));
					else
						SetItemText(0,i,VariantToStr(str));
				}
				Rs->MovePrevious();
			}  
			Rs->MoveFirst();
		}
		*/
		//////////////////////////////////////
		SetRedraw(TRUE);  
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	EndWaitCursor();
	
}

BOOL CClyEditListCtrl::ValidCheck(int nRow, int nCol, LPCTSTR lpszText)
{
	BOOL bResult=FALSE;
	COleDateTime time = COleDateTime::GetCurrentTime();
	COleVariant vtime(lpszText);     //将字符串转换为时间
	double temp;
	switch (nCol)
	{ 
	case 1:  
		break;
	case 2:
		try
		{
			vtime.ChangeType(VT_DATE);
			time = vtime;
		} 
		catch (CException* e)
		{
			CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
			_tcscpy((TCHAR*)lpszText,strTime);		 
		}

		break;
	case 3:  //样品重量
		temp = _tcstod(lpszText,NULL);
		if((temp<100)&&(temp>5))
			_stprintf((TCHAR*)lpszText,_T("%2.2f"),temp);
		else
			_stprintf((TCHAR*)lpszText,_T("%2.2f"),50.0f); 
	case 4:  //水分百分比 
		temp = _tcstod(lpszText,NULL);
		if(temp<100)
			_stprintf((TCHAR*)lpszText,_T("%2.2f"),temp);
		else
			_stprintf((TCHAR*)lpszText,_T("%2.2f"),1.2f); 
		break;
	}
	return TRUE;
}

void CClyEditListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CEditListCtrl::OnSize(nType, cx, cy);
	SetFontHeight(20*(cy+200)/500.0f);
}

void CClyEditListCtrl::SetFontHeight(int iHeight)
{	
	CFont *pFont = GetFont();
	::ZeroMemory(&lf,   sizeof(lf)); 
	pFont->GetLogFont(&lf);
	lf.lfHeight   =   -iHeight; 
	if(font!=NULL)
		delete font;
	font = new CFont();
	/*	
	lf.lfWidth     =   0; 
	lf.lfEscapement   =   0; 
	lf.lfOrientation   =   0; 
	lf.lfWeight   =   0;//FW_BOLD; 
	lf.lfItalic   =   FALSE; 
	lf.lfUnderline   =   FALSE; 
	lf.lfCharSet   =   DEFAULT_CHARSET; 
	lf.lfOutPrecision   =   OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision   =   CLIP_DEFAULT_PRECIS; 
	lf.lfQuality   =   ANTIALIASED_QUALITY; 
	*/
	font->CreateFontIndirect(&lf);  
	SetFont(font);  
}
