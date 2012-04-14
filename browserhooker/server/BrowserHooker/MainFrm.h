// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9807AC78_993A_4C69_A3E3_209569ACEA2B__INCLUDED_)
#define AFX_MAINFRM_H__9807AC78_993A_4C69_A3E3_209569ACEA2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiFrameWnd.h"
#include "GuiSysTray.h"
#include "BHNavigator.h"
#include "SettingWnd.h"

class CMainFrame : public CGuiFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)
public: // create from serialization only
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBHNavigator    m_BHNavigator;
	CSettingWnd     m_BHSettingWnd;
	CGuiSysTray     m_BHSystray;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSpoolerStatus(UINT nStatus, UINT nJobs);
	afx_msg void OnSysColorChange();
	afx_msg void OnViewLook2003();
	afx_msg void OnViewLookXp();
	afx_msg void OnAppExit();
	afx_msg void OnViewToolbar();
	afx_msg void OnOutput();
	afx_msg void OnControlpanel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9807AC78_993A_4C69_A3E3_209569ACEA2B__INCLUDED_)
