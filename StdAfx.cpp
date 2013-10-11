// stdafx.cpp : source file that includes just the standard includes
//	LRYCTRL.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
void dump_com_error(_com_error &e)
{ 
	CString strError;

	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	// Print Com errors. 
	strError.Format(_T("Database Engine Error\n\nCode  %08lx\nmean:%s\nSource: %s\nDescription: %s\n"),
		e.Error(), e.ErrorMessage(), (LPTSTR) bstrSource, (LPTSTR) bstrDescription);
	AfxMessageBox(strError,MB_OK+MB_ICONERROR); 
}


