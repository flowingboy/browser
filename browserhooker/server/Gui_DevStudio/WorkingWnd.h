#if !defined(AFX_WORKINGWND_H__6C87E9DC_AE19_4250_A643_D89370A4D95A__INCLUDED_)
#define AFX_WORKINGWND_H__6C87E9DC_AE19_4250_A643_D89370A4D95A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkingWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkingWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWorkingWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CWorkingWnd)
public:
	CWorkingWnd();           // protected constructor used by dynamic creation

// Attributes
protected:
	CGuiSplitterWnd    m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkingWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkingWnd();

	// Generated message map functions
	//{{AFX_MSG(CWorkingWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKINGWND_H__6C87E9DC_AE19_4250_A643_D89370A4D95A__INCLUDED_)
