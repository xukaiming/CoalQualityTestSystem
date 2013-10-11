// RDB.cpp: implementation of the CRDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "RDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRDB, CObject)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRDB::CRDB()
{ 
	pRsDev.CreateInstance("ADODB.Recordset");
}

CRDB::~CRDB()
{
	if(pRsDev->State==adStateOpen)
		 pRsDev->Close();
}

void CRDB::RefreshRDBTab(CImageSlave *pImageSlave)
{
}
 

void CRDB::InitRDB(CImageSlave *ImageSlave)
{

}

BOOL CRDB::LoadParamFromDB(CString sDeviceName , CString sDeviceType)
{

	BOOL bResult = TRUE;
	HRESULT hr= S_OK;
	//CString strSQL;
	try
	{ 
		if(pRsDev->State==adStateClosed)
		{
			strSQL.Format(_T("select * from %s where Ãû³Æ= '%s'"),sDeviceType,sDeviceName); 
			hr = pRsDev->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			if (pRsDev->RecordCount>0)
			{
				pRsDev->MoveFirst(); 
			}
		}
		else
		{
			pRsDev->MoveFirst();
			for (int i=0;i<pRsDev->RecordCount;i++) 
			{
				_variant_t var = pRsDev->Fields->GetItem("Ãû³Æ")->Value;
				
				if((VARIANT(var).vt!=VT_NULL)&&(CString(var.bstrVal)==sDeviceName))
				{	 	
					break;
				}
				pRsDev->MoveNext();		
			}
		}
		bResult = UpdateParamFromDB(); 
	}
	catch(_com_error &e)
	{
		bResult = FALSE;
		e.ErrorMessage();
		dump_com_error(e);
	}  
	return bResult;
}

BOOL CRDB::LoadParamFromDB(CString sDeviceType)
{

	BOOL bResult = TRUE;
	HRESULT hr= S_OK;
	//CString strSQL;
	try
	{ 
		if(pRsDev->State==adStateClosed)
		{
			strSQL.Format(_T("select * from %s"),sDeviceType); 
			hr = pRsDev->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);
			if (pRsDev->RecordCount>0)
			{
				pRsDev->MoveFirst(); 
			}
		} 
		bResult = UpdateParamFromDB(); 
	}
	catch(_com_error &e)
	{
		bResult = FALSE;
		e.ErrorMessage();
		dump_com_error(e);
	}  
	return bResult;
}
BOOL CRDB::UpdateParam2DB( )
{
	return FALSE;
}

BOOL CRDB::UpdateParamFromDB()
{ 
	return FALSE;
}

IEDResult::IEDResult()
{
	if(pRsResult==NULL) 
		pRsResult.CreateInstance("ADODB.Recordset");
}

IEDResult::~IEDResult()
{
	if(pRsResult->State==adStateOpen)
		pRsResult->Close();
}

void IEDResult::SaveResult2DB()
{

}
void IEDResult::CalResult(CImageSlave *pImageSlave)
{

}
void IEDResult::LoadResultFromDB(CString strAutoNO)
{

}
void IEDResult::DelResultFromDB(CString strAutoNO)
{

}

void IEDResult::SetInstName(CString strInstName) 
{
	m_szName = strInstName;
}

void IEDResult::BuildAutoNO()
{

}