// UseHtmlDlg.h : main header file for the USEHTMLDLG application
//

#if !defined(AFX_USEHTMLDLG_H__7C380247_5EA4_42D7_BC55_180DE92B8E25__INCLUDED_)
#define AFX_USEHTMLDLG_H__7C380247_5EA4_42D7_BC55_180DE92B8E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "External.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CUseHtmlDlgApp:
// See UseHtmlDlg.cpp for the implementation of this class
//

class CUseHtmlDlgApp : public CWinApp
{
public:
	CUseHtmlDlgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUseHtmlDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUseHtmlDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CExternal m_external;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USEHTMLDLG_H__7C380247_5EA4_42D7_BC55_180DE92B8E25__INCLUDED_)
