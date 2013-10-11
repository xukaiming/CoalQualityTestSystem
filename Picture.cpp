// Picture.cpp : implementation file
//

#include "stdafx.h" 
#include "Picture.h"
#include "Gdiplus.h"
//#include "GdiplusFileOpenDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicture

CPicture::CPicture() 
{
	m_bUseEmbeddedColorManagement = FALSE;
	m_hMem   = NULL;
	m_pImage = NULL;
	bStretch = FALSE;
}

CPicture::~CPicture()
{
	Free();
}

IMPLEMENT_DYNAMIC(CPicture, CStatic)
BEGIN_MESSAGE_MAP(CPicture, CStatic)
	//{{AFX_MSG_MAP(CPicture)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicture message handlers

void CPicture::Free()
{
	if (m_pImage) {
		delete m_pImage;
		m_pImage = NULL;
	}
	if (m_hMem) {
		GlobalFree(m_hMem);
		m_hMem = NULL;
	}
}

//////////////////
// Render to device context.
//
BOOL CPicture::Render(CDC* pDC, CRect rc) const
{
	ASSERT(pDC);
	if (rc.IsRectNull()) {
		CSize sz = GetImageSize(pDC);
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	Graphics graphics(pDC->m_hDC);
	graphics.DrawImage(m_pImage, rc.left, rc.top, rc.Width(), rc.Height());
#ifdef _DEBUG
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
	//::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	//TRACE((LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
#endif
	return TRUE;
}

void CPicture::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);
	CRect rcClient;
	GetClientRect(&rcClient);
	if (m_pImage!=NULL) 
	{
		CRect rcImage(CPoint(0,0),GetImageSize());
		CRect rc;
		if(bStretch)
		{
			Render(&dc, rcClient);
		}
		else
		{
			rc.IntersectRect(&rcImage, &rcClient);
			Render(&dc, rc);
		}
	}
	else
	{		
		CBrush *pBrush = dc.GetCurrentBrush();
		dc.FillRect(&rcClient,pBrush);
	}
}

CSize CPicture::GetImageSize(CDC* pDC) const
{
	CSize sz(0,0);
	if (m_pImage) {
		sz = CSize(m_pImage->GetWidth(), m_pImage->GetHeight());
	}
	return sz;
}


BOOL CPicture::Load(LPCTSTR pszPathName)
{

	Free();
	USES_CONVERSION;
#ifndef _UNICODE
	m_pImage = Image::FromFile(A2W(pszPathName),
		m_bUseEmbeddedColorManagement);
#else
	m_pImage = Image::FromFile(pszPathName,
		m_bUseEmbeddedColorManagement);
#endif
	if(m_hWnd!=NULL)
		Invalidate();
	return ((m_pImage!=NULL)&&(m_pImage->GetLastStatus()==Ok)); 
}

BOOL CPicture::Load(IStream* pstm)
{
	Free();
	m_pImage = Image::FromStream(pstm, m_bUseEmbeddedColorManagement);
	if(m_hWnd!=NULL)
		Invalidate();
	return ((m_pImage!=NULL)&&(m_pImage->GetLastStatus()==Ok)); 
}

void CPicture::SaveToField(_RecordsetPtr  Dataset ,_bstr_t FieldName)
{
	HGLOBAL   g_hMem    = NULL;
	IStorage* pIStorage = NULL;
	IStream*  pIStream  = NULL;

	CLSID     jpgClsid;
	//compress quality control
	EncoderParameters encoderParameters;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	int quality = 100;
	encoderParameters.Parameter[0].Value = &quality;
	
	//GetEncoderClsid(L"image/jpeg", &jpgClsid);	

	TCHAR szTempName[MAX_PATH]; 
	GetTempPath(MAX_PATH,szTempName);
	GetTempFileName(szTempName,_T("JpgTmp.jpg"),0,szTempName);
	
	if((m_pImage==NULL)||(S_OK!=m_pImage->Save(_bstr_t(szTempName),&jpgClsid,&encoderParameters)))
		return;	

	CFile JpgFile(szTempName,CFile::modeReadWrite);
	long  FileLength     = JpgFile.GetLength();
	if(FileLength>0)
	{
		HGLOBAL pGlobalMemory = GlobalAlloc(GHND,FileLength);
		PVOID   pBuffer       = GlobalLock(pGlobalMemory);
		JpgFile.Read(pBuffer,FileLength);

		VARIANT			varBLOB;
		SAFEARRAY		*psa;
		SAFEARRAYBOUND	rgsabound;		
		if(pBuffer!=NULL)
		{    
				char *pCharBuf  = NULL;
				pCharBuf		= (char *)pBuffer;
				rgsabound.lLbound = 0;
				rgsabound.cElements = FileLength;
				psa = SafeArrayCreate(VT_UI1, 1, &rgsabound);
	

				for (long i = 0; i < FileLength; i++)
					SafeArrayPutElement (psa, &i, pCharBuf++);
				varBLOB.vt = VT_ARRAY | VT_UI1;
				varBLOB.parray = psa;
				Dataset->GetFields()->GetItem(FieldName)->AppendChunk(varBLOB);
				SafeArrayDestroy(psa);
		}
		JpgFile.Close();
		GlobalUnlock(pBuffer);
		GlobalFree(pGlobalMemory);
		DeleteFile(szTempName);
	}
}

BOOL CPicture::LoadFromField(_RecordsetPtr Dataset ,_bstr_t FieldName)
{
	Free();
	if((Dataset->RecordCount>0)||(!Dataset->adoEOF)||(!Dataset->BOF))
	{
 
		try
		{
			long nSize = Dataset->GetFields()->GetItem(FieldName)->ActualSize;
			if(nSize>0)
			{
				_variant_t	varBLOB;
				varBLOB = Dataset->GetFields()->GetItem(FieldName)->GetChunk(nSize);
				if(varBLOB.vt == (VT_ARRAY | VT_UI1))
				{
						BOOL bResult = FALSE;
						char *pBuf   = NULL;
						SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);		 										
						m_hMem   = GlobalAlloc(GMEM_FIXED, nSize  );
						PVOID  pData		= GlobalLock(m_hMem);
						memcpy(pData, pBuf, nSize);	///复制数据到缓冲区m_pBMPBuffer					
						IStream* pStream = NULL; 
						
						if(CreateStreamOnHGlobal(m_hMem, TRUE, &pStream) == S_OK)
						{ 
							m_pImage = Image::FromStream(pStream, m_bUseEmbeddedColorManagement);				
						}					
						else // S_OK
						{
							pStream->Release();
							pStream = NULL;
							bResult = TRUE;
						}
						
						GlobalUnlock(m_hMem);
						SafeArrayUnaccessData (varBLOB.parray); 	
				
						
				}
			}
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
	}
	if(m_hWnd!=NULL)
		Invalidate();
	return TRUE; 
}

BOOL CPicture::Load (UINT nIDRes)
{
	Free();
	// find resource in resource file
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst,
		MAKEINTRESOURCE(nIDRes),
		_T("PNG")); // type
	if (!hRsrc)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;

	// Allocate global memory on which to create stream
	m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm  = NULL;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	
	// load from stream
	m_pImage = Image::FromStream(pstm, m_bUseEmbeddedColorManagement);
	pstm->Release();
	// free/release stuff
	GlobalUnlock(m_hMem);	
	FreeResource(lpRsrc);
	if(m_hWnd!=NULL)
		Invalidate();
	return ((m_pImage!=NULL)&&(m_pImage->GetLastStatus()==Ok)); 
}

BOOL CPicture::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return CStatic::OnEraseBkgnd(pDC);
}

void CPicture::OnClicked() 
{
	// TODO: Add your control notification handler code here
	/*
	CGdiplusFileOpenDialog dlg;
	if(dlg.DoModal()==ID_OK)
	{
	
		CString FileName = dlg.GetFileName();
		Load(FileName);
	}
	*/
}

void CPicture::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	Invalidate(FALSE);
	// TODO: Add your message handler code here
	
}
