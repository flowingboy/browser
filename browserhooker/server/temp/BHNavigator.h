// BHNavigator.h: interface for the CBHNavigator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BHNAVIGATOR_H__B6B72310_2259_494E_8CBC_A4124F3CF5DF__INCLUDED_)
#define AFX_BHNAVIGATOR_H__B6B72310_2259_494E_8CBC_A4124F3CF5DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma once
#include "guicontrolbar.h"
#include "GuiTabWnd.h"
#include "GuiContainer.h"
#include "GuiMiniTool.h"

class CBHNavigator : public CGuiControlBar  
{
public:
	CBHNavigator();
	virtual ~CBHNavigator();
	void FillerControlPanel();
	void FillTreeClassView();
	int  CreatContExplorer();
	int CreatContClassView();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	CGuiTabWnd	m_TabControlPanel;
	CTreeCtrl   m_TreeControlPanel;
	CTreeCtrl   m_TreeClassView;
	CTreeCtrl   m_TreeResourceView;
	CImageList	m_imgList;
	CGuiContainer m_ctControlPanel;
	CGuiMiniTool  m_miControlPanel;

	CGuiContainer m_ctClassView;
	CGuiMiniTool  m_miClassView;

public:
	void Onproperties();
	void OnNewFolder();
	virtual void StyleDispl(DWORD dwDsp=GUISTYLE_XP)
	{
		m_StyleDisplay=dwDsp;
		m_miControlPanel.StyleDispl(dwDsp);
		m_miClassView.StyleDispl(dwDsp);
		m_TabControlPanel.StyleDispl(dwDsp);
		CGuiControlBar::StyleDispl(dwDsp);
	}
};

#endif // !defined(AFX_BHNAVIGATOR_H__B6B72310_2259_494E_8CBC_A4124F3CF5DF__INCLUDED_)
