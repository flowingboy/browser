#if !defined(AFX_HKRULESETTINGDLG_H__2B89E81F_6444_402F_B5DE_DA0DBCD06CDD__INCLUDED_)
#define AFX_HKRULESETTINGDLG_H__2B89E81F_6444_402F_B5DE_DA0DBCD06CDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HKRuleSettingDlg.h : header file
//
//#include "MyListCtrl.h"
#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CHKRuleSettingDlg dialog

class CHKRuleSettingDlg : public CDialog
{
// Construction
public:
	CHKRuleSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHKRuleSettingDlg)
	enum { IDD = IDD_HKRULE_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHKRuleSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
    pair<CString, CString>            m_UrlMapList[1000];
	int                               m_iMapNum;
protected:

	ListCtrlEx::CListCtrlEx           m_listHkRules;
	int                               m_iRuleNum;
	// Generated message map functions
	//{{AFX_MSG(CHKRuleSettingDlg)
	afx_msg BOOL OnInitDialog();
	afx_msg void OnBtSaveHkrules();
	afx_msg void OnBtAddHkrules();
	afx_msg void OnBtDelHkrules();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HKRULESETTINGDLG_H__2B89E81F_6444_402F_B5DE_DA0DBCD06CDD__INCLUDED_)
