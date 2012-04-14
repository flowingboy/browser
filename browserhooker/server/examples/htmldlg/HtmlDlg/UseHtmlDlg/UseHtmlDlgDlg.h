// UseHtmlDlgDlg.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES

#if !defined(AFX_USEHTMLDLGDLG_H__F555E41D_78AE_4B8E_A2C3_0BFD7C53AD3D__INCLUDED_)
#define AFX_USEHTMLDLGDLG_H__F555E41D_78AE_4B8E_A2C3_0BFD7C53AD3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUseHtmlDlgDlg dialog

class CUseHtmlDlgDlg : public CDialog
{
// Construction
public:
	void ShowDlg();
	CUseHtmlDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUseHtmlDlgDlg)
	enum { IDD = IDD_USEHTMLDLG_DIALOG };
	CWebBrowser2	m_ie;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUseHtmlDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUseHtmlDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USEHTMLDLGDLG_H__F555E41D_78AE_4B8E_A2C3_0BFD7C53AD3D__INCLUDED_)
