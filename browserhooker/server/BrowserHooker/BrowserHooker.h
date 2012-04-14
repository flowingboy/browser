// BrowserHooker.h : main header file for the BROWSERHOOKER application
//

#if !defined(AFX_BROWSERHOOKER_H__FD957440_524B_4E85_AADE_747297134996__INCLUDED_)
#define AFX_BROWSERHOOKER_H__FD957440_524B_4E85_AADE_747297134996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerApp:
// See BrowserHooker.cpp for the implementation of this class
//

class CBrowserHookerApp : public CWinApp
{
public:
	CBrowserHookerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserHookerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBrowserHookerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERHOOKER_H__FD957440_524B_4E85_AADE_747297134996__INCLUDED_)
