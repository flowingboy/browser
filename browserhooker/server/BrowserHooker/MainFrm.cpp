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

IMPLEMENT_DYNCREATE(CMainFrame, CGuiFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CGuiFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SPOOLERSTATUS()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_VIEW_LOOK_2003, OnViewLook2003)
	ON_COMMAND(ID_VIEW_LOOK_XP, OnViewLookXp)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_OUTPUT, OnOutput)
	ON_COMMAND(ID_CONTROLPANEL, OnControlpanel)
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
	IDR_MAINFRAME,
	IDR_SIDEBAR,
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
	if (CGuiFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitMenu(IDR_MAINFRAME);
	InitToolBar(IDR_MAINFRAME);
	InitStatusBar(indicators,sizeof(indicators)/sizeof(UINT));
	m_wndToolBar.SetWindowText("ToolBar");

	// Create systray icon.
	// Get Icon, two methods: 
	// hIcon = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_MYICON));
	// hIcon = AfxGetApp()->LoadIcon(IDI_MYICON);
	m_BHSystray.Create(this, IDI_SYSTRAY, 
					   WM_RBUTTONDOWN | WM_LBUTTONDOWN
					   | WM_RBUTTONUP | WM_LBUTTONUP
					   | WM_LBUTTONDBLCLK,
					   AfxGetApp()->LoadIcon(IDI_SYSTRAY),
					   _T("BrowserHooker in systray."));

	// Load systray menu
	// 这里使用CMenu menu初始化变量会失败, 必须使用指针然后new的方式
	CMenu *pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENUSYSTRAY);
	m_BHSystray.SetSysMenu(pMenu);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	// for control panel.
	if (!m_BHNavigator.Create(_T("Control Panel"),WS_CHILD | WS_VISIBLE, this, 0x999))
    {
        TRACE0("Failed to create m_BHNavigator\n");
        return -1;
	}

//	if(!m_BHSettingWnd.Create(_T("Setting"), WS_CHILD | WS_VISIBLE, this, 0x1000))
//	{
//		TRACE0("Failed to create m_BHSettingWnd\n");
//		return -1;
//	}

	m_BHNavigator.SetBarStyle(m_BHNavigator.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

//	m_BHSettingWnd.SetBarStyle(m_BHSettingWnd.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// side bar create
	m_wndToolBar.SetButtonDropDown(ID_BT_CONTROLPANEL, 8, IDR_FLOATMENU);
	m_wndToolBar.SetMenuContext(IDR_FLOATMENU);

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_BHNavigator.EnableDocking(CBRS_ALIGN_ANY);
//	m_BHSettingWnd.EnableDocking(CBRS_ALIGN_ANY);

	// register the menu bar.
	m_BHNavigator.SetMenuContext(IDR_FLOATMENU);
//	m_BHSettingWnd.SetMenuContext(IDR_FLOATMENU);
	m_wndMenuBar.SetMenuContext(IDR_FLOATMENU);

	// set the control to be dockable.
	DockControlBar(&m_wndMenuBar, m_dockTop);
	DockControlBar(&m_wndToolBar, m_dockLeft);
	DockControlBar(&m_BHNavigator, AFX_IDW_DOCKBAR_LEFT);
//	DockControlBar(&m_BHSettingWnd, AFX_IDW_DOCKBAR_BOTTOM);

	// here sProfile standards for App name.
	sProfile = _T("BrowserHooker");
	LoadBars();
	
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
	m_wndCool.LoadToolbars(Toolbars, _countof(Toolbars));

	RecalcLayout();
	if(m_StyleDisplay == GUISTYLE_2003)
		OnViewLook2003();
	else
		OnViewLookXp();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CGuiFrameWnd::PreCreateWindow(cs) )
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
	CGuiFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CGuiFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CGuiFrameWnd::OnSize(nType, cx, cy);
//	m_MdiTabbed::OnSize((nType, cx, cy);
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnViewLookXp() 
{
	// TODO: Add your command handler code here
	GuiDrawLayer::m_Style=GUISTYLE_XP;
	GuiDrawLayer::m_Theme=0;
	m_BHNavigator.StyleDispl();
	StyleDispl(GUISTYLE_XP);
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
	AfxGetMainWnd()->ShowWindow(SW_HIDE);
	// just hide the window instead of closing it.
	//CGuiFrameWnd::OnClose();
}

void CMainFrame::OnSpoolerStatus(UINT nStatus, UINT nJobs) 
{
	CGuiFrameWnd::OnSpoolerStatus(nStatus, nJobs);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnSysColorChange() 
{
	CGuiFrameWnd::OnSysColorChange();
	GuiDrawLayer::IsThemeXP();
	// TODO: Add your message handler code here
	
}

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CGuiFrameWnd::OnNotify(wParam, lParam, pResult);
}

BOOL CMainFrame::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CGuiFrameWnd::OnChildNotify(message, wParam, lParam, pLResult);
}


void CMainFrame::OnAppExit() 
{
	// TODO: Add your command handler code here
	CGuiFrameWnd::OnClose();
}

void CMainFrame::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	ShowHideBar(&m_wndToolBar);
	
}

void CMainFrame::OnOutput() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnControlpanel() 
{
	// TODO: Add your command handler code here
	ShowHideBar(&m_BHNavigator);
	ShowHideBar(&m_BHSettingWnd);
	
}
