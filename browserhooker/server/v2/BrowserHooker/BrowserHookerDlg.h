// BrowserHookerDlg.h : header file
//

#if !defined(AFX_BROWSERHOOKERDLG_H__334F08F4_B8E0_45D9_8974_B05E6C10B538__INCLUDED_)
#define AFX_BROWSERHOOKERDLG_H__334F08F4_B8E0_45D9_8974_B05E6C10B538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerDlg dialog

class CBrowserHookerDlg : public CDialog
{
// Construction
public:
	CBrowserHookerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBrowserHookerDlg)
	enum { IDD = IDD_BROWSERHOOKER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserHookerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBrowserHookerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERHOOKERDLG_H__334F08F4_B8E0_45D9_8974_B05E6C10B538__INCLUDED_)
