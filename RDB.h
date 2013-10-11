// RDB.h: interface for the CRDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDB_H__30ECCA14_F2DA_406C_9E99_DA446E85343A__INCLUDED_)
#define AFX_RDB_H__30ECCA14_F2DA_406C_9E99_DA446E85343A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImageSlave //:public CObject
{
	
};

 
class CRDB : public CObject  
{
public:
	CRDB();
	virtual ~CRDB();
	virtual void RefreshRDBTab(CImageSlave *pImageSlave) ;
	virtual BOOL LoadParamFromDB(CString sDeviceName , CString sDeviceType) ;
	virtual BOOL LoadParamFromDB(CString sDeviceType);
	virtual BOOL UpdateParam2DB();
	virtual BOOL UpdateParamFromDB();
	DECLARE_DYNCREATE(CRDB)
public:
	_RecordsetPtr	pRsDev;
	CString			strSQL;
	//////////////////////////////////////////////////
	CString			m_szName;				//�豸����
	CString			m_szType;				//�豸����
	BYTE			m_bID;					//ID��
	SHORT			m_sComPort;				//���ں�	
	LONG			m_lScanPriod_mS;		//ɨ������ 
private:

	virtual void InitRDB(CImageSlave *ImageSlave) ;

};


class IEDResult:public CObject
{
private:
public:
	IEDResult();
	~IEDResult();
public:
	virtual void BuildAutoNO()=0;
	virtual void SaveResult2DB()=0;
	virtual void UpdateResultToDB()=0;
	virtual void CalResult(CImageSlave *pImageSlave)=0; 
	virtual void LoadResultFromDB(CString strAutoNO)=0;
	virtual void DelResultFromDB(CString strAutoNO)=0;
public:
	virtual void SetInstName(CString strInstName);
	CString			m_szName;				//�豸����
	_RecordsetPtr	pRsResult;
};
#endif // !defined(AFX_RDB_H__30ECCA14_F2DA_406C_9E99_DA446E85343A__INCLUDED_)
