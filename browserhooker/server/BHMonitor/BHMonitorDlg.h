// BHMonitorDlg.h : header file
//

#if !defined(AFX_BHMONITORDLG_H__C32D2A4E_BE15_4EDE_A59C_98E7371CF8C7__INCLUDED_)
#define AFX_BHMONITORDLG_H__C32D2A4E_BE15_4EDE_A59C_98E7371CF8C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyIOCP.h"
#include "MyListCtrl.h"
#include "ListCtrlEx.h"
#include "SettingDlg.h"
#include "HKRuleSettingDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CBHMonitorDlg dialog

class CBHMonitorDlg : public CDialog
{
// Construction
public:
	CBHMonitorDlg(CWnd* pParent = NULL);	// standard constructor

	void AppendLog(CString msg);
	BOOL StartServer();

	void EnableAllPanels();
	void DisableAllPanels();
	void UpdateClientData();

// Dialog Data
	//{{AFX_DATA(CBHMonitorDlg)
	enum { IDD = IDD_BHMONITOR_DIALOG };
	CHistoryEdit	m_CtrlConLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBHMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON                 m_hIcon;
	MyIOCP                m_myIocp;
	CSettingDlg           m_ConfigDlg;
	CHKRuleSettingDlg     m_HKRuleSettingDlg;
	int                   m_iCurrentClientID;
	CCriticalSection      m_GUIListLock;

	CString               m_sReceivedText;
	CString               m_sSendText;

	// replaced with ListCtrlEx::ListCtrlEx control.
	//MyListCtrl            m_CtrlClientList;
	ListCtrlEx::CListCtrlEx m_CtrlClientList;
	// Generated message map functions
	//{{AFX_MSG(CBHMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBtStartServer();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBtCommunicationSetting();
	afx_msg void OnBtStopServer();
	afx_msg void OnBtHkruleSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	LRESULT OnClientDisconnected(WPARAM wParam, LPARAM lParam);
	LRESULT OnAppendLog(WPARAM wParam, LPARAM lParam);
	LRESULT OnNewClient(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BHMONITORDLG_H__C32D2A4E_BE15_4EDE_A59C_98E7371CF8C7__INCLUDED_)
