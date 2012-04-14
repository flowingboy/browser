#if !defined(AFX_SETTINGDLG_H__0FBC7B8C_B44F_49B7_8B1A_BF0FED21E07A__INCLUDED_)
#define AFX_SETTINGDLG_H__0FBC7B8C_B44F_49B7_8B1A_BF0FED21E07A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//

#include "BHMonitor.h"
/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

class CSettingDlg : public CDialog
{
// Construction
public:
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SEETING_DIALOG };
	int		m_iPortNr;
	int		m_iMaxNumberOfConnections;
	int		m_iNrOfIOWorkers;
	int		m_iNrOfLogicalWorkers;
	int		m_iMaxNrOfFreeBuff;
	int		m_iMaxNrOfFreeContext;
	BOOL	m_iSendInOrder;
	BOOL	m_bReadInOrder;
	int		m_iNrOfPendlingReads;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	afx_msg void OnBtSaveSeeting();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__0FBC7B8C_B44F_49B7_8B1A_BF0FED21E07A__INCLUDED_)
