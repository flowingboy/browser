// WorkingWnd.cpp : implementation file
//

#include "stdafx.h"
#include "gui_devstudio.h"
#include "WorkingWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkingWnd

IMPLEMENT_DYNCREATE(CWorkingWnd, CMDIChildWnd)

CWorkingWnd::CWorkingWnd()
{
}

CWorkingWnd::~CWorkingWnd()
{
}

BOOL CWorkingWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,       // TODO: adjust the number of rows, columns
		CSize(10, 10),  // TODO: adjust the minimum pane size
		pContext);
}


BEGIN_MESSAGE_MAP(CWorkingWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CWorkingWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkingWnd message handlers

BOOL CWorkingWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
}
