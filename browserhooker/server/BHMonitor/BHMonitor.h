// BHMonitor.h : main header file for the BHMONITOR application
//

#if !defined(AFX_BHMONITOR_H__036575CC_10E8_4036_9EBC_33120A8E13E3__INCLUDED_)
#define AFX_BHMONITOR_H__036575CC_10E8_4036_9EBC_33120A8E13E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBHMonitorApp:
// See BHMonitor.cpp for the implementation of this class
//

class CBHMonitorApp : public CWinApp
{
public:
	CBHMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBHMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBHMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BHMONITOR_H__036575CC_10E8_4036_9EBC_33120A8E13E3__INCLUDED_)
