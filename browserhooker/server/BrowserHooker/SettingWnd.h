// SettingWnd.h: interface for the CSettingWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGWND_H__DE96A50E_45B2_4D1F_85E6_9570518EC1CE__INCLUDED_)
#define AFX_SETTINGWND_H__DE96A50E_45B2_4D1F_85E6_9570518EC1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "guicontrolbar.h"

class CSettingWnd : public CGuiControlBar  
{
public:
	CSettingWnd();
	virtual ~CSettingWnd();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


private:
	// Attributes
};

#endif // !defined(AFX_SETTINGWND_H__DE96A50E_45B2_4D1F_85E6_9570518EC1CE__INCLUDED_)
