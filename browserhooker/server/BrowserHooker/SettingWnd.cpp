// SettingWnd.cpp: implementation of the CSettingWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BrowserHooker.h"
#include "SettingWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ID_BT_SAVESETTING     0x10001
#define ID_BT_CACELSETTING    0x10002
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettingWnd::CSettingWnd()
{

}

CSettingWnd::~CSettingWnd()
{

}

// Message looping.
BEGIN_MESSAGE_MAP(CSettingWnd, CGuiControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CSettingWnd::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
