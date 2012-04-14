// BrowserHooker.h : main header file for the BROWSERHOOKER application
//

#if !defined(AFX_BROWSERHOOKER_H__84908042_F99F_4C64_BD10_B05A0B879B1A__INCLUDED_)
#define AFX_BROWSERHOOKER_H__84908042_F99F_4C64_BD10_B05A0B879B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

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
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERHOOKER_H__84908042_F99F_4C64_BD10_B05A0B879B1A__INCLUDED_)
