// HKRuleSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BHMonitor.h"
#include "HKRuleSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHKRuleSettingDlg dialog


CHKRuleSettingDlg::CHKRuleSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHKRuleSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHKRuleSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iRuleNum = -1;
}


void CHKRuleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHKRuleSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_HOOKING_RULES, m_listHkRules);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHKRuleSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CHKRuleSettingDlg)
	ON_BN_CLICKED(IDC_BT_SAVE_HKRULES, OnBtSaveHkrules)
	ON_BN_CLICKED(IDC_BT_ADD_HKRULES, OnBtAddHkrules)
	ON_BN_CLICKED(IDC_BT_DEL_HKRULES, OnBtDelHkrules)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHKRuleSettingDlg message handlers
BOOL CHKRuleSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listHkRules.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_listHkRules.SetExtendedStyle(m_listHkRules.GetExtendedStyle() |LVS_EX_FULLROWSELECT );

	m_listHkRules.InsertColumn(0, _T("Rule No."), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_listHkRules.InsertColumn(1, _T("Original Url"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_listHkRules.InsertColumn(2, _T("Replaced Url"), 210, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	
	// 判断是否存在影射规则的配置文件，如果没有，则创建
	// 有，则读取其中的配置，并在listctrl中显示
	CStdioFile cfgFile;
	if(!cfgFile.Open(_T("hkrules.txt"), CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate)){
		AfxMessageBox(_T("Open config file failed!"));
		return FALSE;
	}

	CString line, oUrl, rUrl;
	int pos, nItem, i = 0;
	while(cfgFile.ReadString(line)){
		pos = line.Find('-');
		oUrl = line.Left(pos);
		rUrl = line.Right(line.GetLength() - pos - strlen("-"));
		// store the map relation into m_UrlMapList
		m_UrlMapList[i].first = oUrl;
		m_UrlMapList[i].second = rUrl;
		i++;

		// update the client area of CListCtrl.
		nItem = m_listHkRules.InsertItem(m_listHkRules.GetItemCount(), _T("Rule"));
		m_listHkRules.SetItemText(nItem, 1, (LPCTSTR)oUrl);
		m_listHkRules.SetItemText(nItem, 2, (LPCTSTR)rUrl);
		m_listHkRules.Update(nItem);
	}

	m_iMapNum = i;

	return TRUE;
}

void CHKRuleSettingDlg::OnBtSaveHkrules() 
{
	// TODO: Add your control notification handler code here
	// 保存配置信息
	int nItem = m_listHkRules.GetItemCount();
	int i = 0;
	CString oUrl, rUrl;   // original url and replaced url.
	BOOL bFullFilled = TRUE;
	while(i < nItem){
		oUrl = m_listHkRules.GetItemText(i, 1);
		rUrl = m_listHkRules.GetItemText(i, 2);
		oUrl.TrimLeft();
		oUrl.TrimRight();
		rUrl.TrimLeft();
		rUrl.TrimRight();
		// 这里暂时不考虑https://开头的URL
		if(oUrl == ""
			|| -1 == oUrl.Find("http://")
			){
			
			AfxMessageBox(_T("Please fill the original url!"));
			bFullFilled = FALSE;
			break;

		}else if(rUrl == ""
				|| -1 == rUrl.Find("http://")
				){

			AfxMessageBox(_T("Please fill the replaced url!"));
			bFullFilled = FALSE;
			break;

		}

		i++;
	}

	if(bFullFilled){
		// 打开配置文件
		CFile cfgFile;
		if(!cfgFile.Open(_T("./hkrules.txt"), CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate)){
			AfxMessageBox(_T("Open config file failed!"));
		}else{
			// 首先将文件清零
			cfgFile.SetLength(0);

			// 将信息写入配置文件
			i = 0;
			CString strUrlMap;
			while(i < nItem){
				m_UrlMapList[i].first = m_listHkRules.GetItemText(i, 1);
				m_UrlMapList[i].second = m_listHkRules.GetItemText(i, 2);
				strUrlMap = m_UrlMapList[i].first + "-" + m_UrlMapList[i].second + "\n";
				cfgFile.Write((LPCTSTR)strUrlMap, strUrlMap.GetLength());
				// 千万别忘记了:D
				i++;
			}

			m_iMapNum = i;

			cfgFile.Flush();
			cfgFile.Close();
		}

		CDialog::OnOK();
	}
}

void CHKRuleSettingDlg::OnBtAddHkrules() 
{
	// TODO: Add your control notification handler code here
	CString itemStr;
	itemStr.Format("Rule %3d", m_iRuleNum+1);
	m_iRuleNum = m_listHkRules.InsertItem(m_listHkRules.GetItemCount(), itemStr);
	m_listHkRules.Update(m_iRuleNum);
}

void CHKRuleSettingDlg::OnBtDelHkrules() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listHkRules.GetFirstSelectedItemPosition();
	int nItem = 0;
	// 这里需要更新item的序号
	if(pos){
		nItem = m_listHkRules.GetNextSelectedItem(pos);
		m_listHkRules.DeleteItem(nItem);
		m_iRuleNum--;
	}
}

void CHKRuleSettingDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(AfxMessageBox(IDS_EXITHKRULES_PROMPT, MB_YESNO) == IDYES)
		CDialog::OnClose();
}
