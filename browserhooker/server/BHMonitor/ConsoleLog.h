#if !defined(AFX_CONSOLELOG_H__B1F2563D_873E_4E7C_98DF_7DEA00E5DB64__INCLUDED_)
#define AFX_CONSOLELOG_H__B1F2563D_873E_4E7C_98DF_7DEA00E5DB64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConsoleLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConsoleLog window

class CConsoleLog : public CEdit
{
// Construction
public:
	CConsoleLog();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleLog)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConsoleLog();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConsoleLog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLELOG_H__B1F2563D_873E_4E7C_98DF_7DEA00E5DB64__INCLUDED_)
