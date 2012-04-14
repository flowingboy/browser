// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BrowserHooker.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CGuiMDIFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CGuiMDIFrame)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SPOOLERSTATUS()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_VIEW_LOOK_2003, OnViewLook2003)
	ON_COMMAND(ID_VIEW_LOOK_XP, OnViewLookXp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT Toolbars[] = {
	IDR_MAINFRAME
//	IDR_TOOLBARS,
//	IDR_NEWITEM	
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	
	
	
	

	// for control panel.
	if (!m_BHNavigator.Create(_T("Workspace"),WS_CHILD | WS_VISIBLE, this, 0x999))
    {
        TRACE0("Failed to create m_GuiSolEx\n");
        return -1;      
	}

	m_BHNavigator.SetBarStyle(m_BHNavigator.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	EnableDocking(CBRS_ALIGN_ANY);
	m_BHNavigator.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar, m_dockLeft);
	DockControlBar(&m_BHNavigator, AFX_IDW_DOCKBAR_RIGHT);

	m_wndToolBar.SetWindowText("¹¤¾ßÌõ");

	sProfile = _T("Gui_DevStudio");
	LoadBars();
	InitMDITabbed();
	m_MdiTabbed.SetTabsMenu(IDR_MENUTABS);
	
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
	m_wndCool.LoadToolbars(Toolbars, _countof(Toolbars));

	RecalcLayout();
	m_MdiTabbed.SetTabMenu(0,0);
	if(m_StyleDisplay == GUISTYLE_2003)
		OnViewLook2003();
	else
		OnViewLookXp();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CGuiMDIFrame::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CGuiMDIFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CGuiMDIFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CGuiMDIFrame::OnSize(nType, cx, cy);
//	m_MdiTabbed::OnSize((nType, cx, cy);
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnViewLookXp() 
{
	// TODO: Add your command handler code here
	GuiDrawLayer::m_Style=GUISTYLE_XP;
	GuiDrawLayer::m_Theme=0;
	m_BHNavigator.StyleDispl();
	StyleDispl();
	RecalcLayout();
	Invalidate();
	UpdateWindow();
}

void CMainFrame::OnViewLook2003() 
{
	// TODO: Add your command handler code here
	GuiDrawLayer::m_Style=GUISTYLE_2003;
	StyleDispl(GUISTYLE_2003);
	m_BHNavigator.StyleDispl(GUISTYLE_2003);
	
}


void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CGuiMDIFrame::OnClose();
}

void CMainFrame::OnSpoolerStatus(UINT nStatus, UINT nJobs) 
{
	CGuiMDIFrame::OnSpoolerStatus(nStatus, nJobs);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnSysColorChange() 
{
	CGuiMDIFrame::OnSysColorChange();
	GuiDrawLayer::IsThemeXP();
	// TODO: Add your message handler code here
	
}

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CGuiMDIFrame::OnNotify(wParam, lParam, pResult);
}

BOOL CMainFrame::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CGuiMDIFrame::OnChildNotify(message, wParam, lParam, pLResult);
}
