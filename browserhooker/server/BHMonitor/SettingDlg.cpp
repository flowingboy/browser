// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BHMonitor.h"
#include "SettingDlg.h"
#include "BHMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

// data initialization is very important!
CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_iPortNr = 6789;
	m_iMaxNumberOfConnections = 1024;
	m_iNrOfIOWorkers = 4;
	m_iNrOfLogicalWorkers = 0;
	m_iMaxNrOfFreeBuff = 0;
	m_iMaxNrOfFreeContext = 0;
	m_iSendInOrder = FALSE;
	m_bReadInOrder = FALSE;
	m_iNrOfPendlingReads = 4;
	//}}AFX_DATA_INIT
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Text(pDX, IDC_PORT, m_iPortNr);
	DDX_Text(pDX, IDC_MAX_CONNECTION, m_iMaxNumberOfConnections);
	DDV_MinMaxInt(pDX, m_iMaxNumberOfConnections, 1, 10240);
	DDX_Text(pDX, IDC_NUM_IO_WORKERS, m_iNrOfIOWorkers);
	DDX_Text(pDX, IDC_NUM_LG_WORKERS, m_iNrOfLogicalWorkers);
	DDX_Text(pDX, IDC_MAX_UNSAFE_BUFFER, m_iMaxNrOfFreeBuff);
	DDX_Text(pDX, IDC_MAX_UNUSED_CLIENTS, m_iMaxNrOfFreeContext);
	DDX_Check(pDX, IDC_CK_SEND_ORDERED, m_iSendInOrder);
	DDX_Check(pDX, IDC_CK_RECV_ORDERED, m_bReadInOrder);
	DDX_Text(pDX, IDC_NUM_PENDING_RLOOP, m_iNrOfPendlingReads);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_BN_CLICKED(IDC_BT_SAVE_SEETING, OnBtSaveSeeting)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

void CSettingDlg::OnBtSaveSeeting() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString msg;
	msg.Format("%d", m_iMaxNumberOfConnections);
	//AfxMessageBox(msg, MB_OK, 0);
	PostMessage(WM_CLOSE);
}

void CSettingDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
