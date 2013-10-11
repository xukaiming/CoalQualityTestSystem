#if !defined(AFX_PICTURE_H__1225EFF4_5C55_4A31_A685_3C4CB8266EDF__INCLUDED_)
#define AFX_PICTURE_H__1225EFF4_5C55_4A31_A685_3C4CB8266EDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Picture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPicture window

class CPicture : public CStatic
{
// Construction
public:
	CPicture();

// Attributes
public:

// Operations
public:
	BOOL    bStretch;
	Image*  m_pImage;						 // GDI+ Image object
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicture)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveToField(_RecordsetPtr  Dataset ,_bstr_t FieldName);
	BOOL LoadFromField(_RecordsetPtr  Dataset ,_bstr_t FieldName);
	void Free();
	CSize GetImageSize(CDC* pDC=NULL) const;
	BOOL Load(LPCTSTR pszPathName);
	BOOL Load(UINT nIDRes);
	BOOL Load(IStream* pstm);	
	// render to device context
	BOOL Render(CDC* pDC, CRect rc=CRect(0,0,0,0)) const;
	virtual ~CPicture();
	Image* GetImage() {
		return m_pImage;
	}
	// Generated message map functions
protected:
	//{{AFX_MSG(CPicture)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClicked();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_DYNAMIC(CPicture)
	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_bUseEmbeddedColorManagement;	 // set this before loading

	HGLOBAL m_hMem;							 // used for resource image
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTURE_H__1225EFF4_5C55_4A31_A685_3C4CB8266EDF__INCLUDED_)
