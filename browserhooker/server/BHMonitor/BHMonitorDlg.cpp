// BHMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BHMonitor.h"
#include "BHMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBHMonitorDlg dialog

CBHMonitorDlg::CBHMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBHMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBHMonitorDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBHMonitorDlg)
	DDX_Control(pDX, IDC_CONSOLE_LOGS, m_CtrlConLog);
	DDX_Control(pDX, IDC_LIST_CTRL_CLIENTS, m_CtrlClientList);
	//}}AFX_DATA_MAP
}


////////////////////////////////////////////////////////////////
// Customer function defined here.

void CBHMonitorDlg::AppendLog(CString msg)
{
	TRACE("%s\r\n",msg);
	int nLen = msg.GetLength();
	char *pBuffer = new char[nLen+1];
	strcpy(pBuffer,(const char*)msg);
	pBuffer[nLen]='\0';
	BOOL bRet=::PostMessage(m_hWnd, WM_LOGG_APPEND, 0, (LPARAM) pBuffer);
}

BOOL CBHMonitorDlg::StartServer(){
	// TODO: Add your control notification handler code here
	m_myIocp.m_StatusLock.Lock();
	m_myIocp.m_hWnd=m_hWnd;

	int i = 0;
	while(i < m_HKRuleSettingDlg.m_iMapNum){
		m_sSendText += m_HKRuleSettingDlg.m_UrlMapList[i].first + "-" + m_HKRuleSettingDlg.m_UrlMapList[i].second;
		m_sSendText += "|";

		i++;
	}

	m_sSendText.TrimRight("|");

	m_myIocp.m_sSendText=m_sSendText;
	m_myIocp.m_bFlood=FALSE;
	m_myIocp.m_StatusLock.Unlock();
	
	AppendLog(m_sSendText+"\n");
	// here we need to check the parameters whether they are in legal range.
	// maybe we can define a function called DoServerParametersCheck to do this.
	if(!m_myIocp.Start(m_ConfigDlg.m_iPortNr,
		m_ConfigDlg.m_iMaxNumberOfConnections,
		m_ConfigDlg.m_iNrOfIOWorkers,
		m_ConfigDlg.m_iNrOfLogicalWorkers,
		//-1, // No buffer reuse
		m_ConfigDlg.m_iMaxNrOfFreeBuff,
		//-1, // No context reuse
		m_ConfigDlg.m_iMaxNrOfFreeContext,
		m_ConfigDlg.m_iSendInOrder,
		m_ConfigDlg.m_bReadInOrder,
		m_ConfigDlg.m_iNrOfPendlingReads)){

		AppendLog(_T("Error: could not start the Monitor!"));
		return FALSE;
	}

	return TRUE;
}

// call BuildPackageAndSend(m_iCurrentClientID,m_sSendTxt) to send package. - flowingboy
// then I need to find where to add this function.
/*
void CIOCPDlg::OnSend() 
{
	
	
	if(m_iCurrentClientID>0)
	{
		UpdateData(TRUE);	
		for(int i=0;i<20;i++)
		{
			m_sSendTxt.Format("%iCDEFGHIKLMNOPQRST1234567890",i);
			if(!m_iocp.BuildPackageAndSend(m_iCurrentClientID,m_sSendTxt))
				AfxMessageBox("Error send failed!");
		}
		if(!m_iocp.BuildPackageAndSend(m_iCurrentClientID,m_sSendTxt))
		{
			DisableClientPanel();
			int SItem=m_CtrlClientList.GetNextItem(-1,LVNI_SELECTED);
			if(SItem!=-1)
				m_CtrlClientList.SetItemState(SItem,LVNI_ALL, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE);
			m_CtrlClientList.SetFocus();
			AfxMessageBox("Send not successfull.!");
		}else
		{
			UpdateClientData();
			m_CtrlClientList.SetFocus();
		}
	}
	
}
*/

void CBHMonitorDlg::EnableAllPanels(){}

void CBHMonitorDlg::DisableAllPanels(){}

// Begin message map loop here.
BEGIN_MESSAGE_MAP(CBHMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CBHMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_BT_START_SERVER, OnBtStartServer)
	ON_WM_LBUTTONDOWN()
	ON_WM_CANCELMODE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BT_COMMUNICATION_SETTING, OnBtCommunicationSetting)
	ON_BN_CLICKED(IDC_BT_STOP_SERVER, OnBtStopServer)
	ON_MESSAGE(WM_NEW_CONNECTION, OnNewClient)
	ON_MESSAGE(WM_LOGG_APPEND, OnAppendLog)
	ON_MESSAGE(WM_CLIENTDISCONNECTED, OnClientDisconnected)
	ON_BN_CLICKED(IDC_BT_HKRULE_SETTING, OnBtHkruleSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBHMonitorDlg message handlers

BOOL CBHMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_CtrlClientList.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_CtrlClientList.SetExtendedStyle(m_CtrlClientList.GetExtendedStyle() |LVS_EX_FULLROWSELECT );

	m_CtrlClientList.InsertColumn(0, _T("Normal Text"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_CtrlClientList.InsertColumn(1, _T("Check Box"), 100, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_CtrlClientList.InsertColumn(2, _T("Edit Box"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_CtrlClientList.InsertColumn(3, _T("Combo Box"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_CtrlClientList.InsertColumn(4, _T("Radio Box"), 100, ListCtrlEx::RadioBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_CtrlClientList.InsertColumn(5, _T("Progress Bar"), 100, ListCtrlEx::Progress, LVCFMT_CENTER, ListCtrlEx::SortByString);
	
	int nRow = m_CtrlClientList.InsertItem(m_CtrlClientList.GetItemCount(), _T("Item 1"));
	for(int i = 0; i <= m_CtrlClientList.GetColumnCount(); i++){
		CString sub;
		sub.Format(_T("sub item %d %d"), nRow, i);
		m_CtrlClientList.SetItemText(nRow, i, sub);
		if(ListCtrlEx::ComboBox == m_CtrlClientList.GetColumnType(i))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(sub);
			CString str;
			str.Format(_T("some other %d, %d"), nRow, i);
			strlist.push_back(str);
			str.Format(_T("some other %d, %d"), nRow+1, i+1);
			strlist.push_back(str);
			m_CtrlClientList.SetCellStringList(nRow, i, strlist);
		}
		else if (ListCtrlEx::Progress == m_CtrlClientList.GetColumnType(i))
		{
			m_CtrlClientList.SetCellProgressMaxValue(nRow, i, 100*i);
			m_CtrlClientList.SetCellProgressValue(nRow, i, 100*nRow);
			
		}

		m_CtrlClientList.SetCellEnabled(nRow, i, (nRow != i));
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBHMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBHMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBHMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/* 
 * how to display a context menu in CListCtrl:
 * http://support.microsoft.com/kb/222905
 */
void CBHMonitorDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

}

void CBHMonitorDlg::OnBtStartServer() 
{
	CButton *pBCtrl1=NULL;
	CButton *pBCtrl2=NULL;
	CButton *pBCtrl3=NULL;
	CButton *pBCtrl4=NULL;

	pBCtrl1=(CButton*)GetDlgItem(IDC_BT_START_SERVER);
	pBCtrl2=(CButton*)GetDlgItem(IDC_BT_COMMUNICATION_SETTING);
	pBCtrl3=(CButton*)GetDlgItem(IDC_BT_HKRULE_SETTING);
	pBCtrl4=(CButton*)GetDlgItem(IDC_BT_EXPORT_CLIENT);


	if(pBCtrl1 && pBCtrl2 && pBCtrl3 && pBCtrl4){
		if(m_myIocp.IsStarted())
		{
			m_myIocp.ShutDown();
			pBCtrl1->SetWindowText("Start Monitor");
			pBCtrl2->EnableWindow(TRUE);
			pBCtrl3->EnableWindow(TRUE);
			pBCtrl4->EnableWindow(TRUE);
			DisableAllPanels();
			
		}else
		{
			if(StartServer()){
				pBCtrl1->SetWindowText("Stop Monitor");
				pBCtrl2->EnableWindow(FALSE);	
				pBCtrl3->EnableWindow(FALSE);
				pBCtrl4->EnableWindow(FALSE);
				EnableAllPanels();
			}else{
				AppendLog(_T("Start Monitor failed!"));
			}
		}
	}
}

void CBHMonitorDlg::OnBtStopServer() 
{
	// TODO: Add your control notification handler code here
	
}

void CBHMonitorDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	CDialog::OnLButtonDown(nFlags, point);
}

void CBHMonitorDlg::UpdateClientData()
{
	
	ClientContext* pContext=NULL;
	// to be sure that pContext Suddenly does not dissapear.. 
	m_myIocp.m_ContextMapLock.Lock();
	pContext=m_myIocp.FindClient(m_iCurrentClientID);
	if(pContext!=NULL)
	{
	   pContext->m_ContextLock.Lock();
	   m_sReceivedText=pContext->m_sReceived;
	   pContext->m_ContextLock.Unlock();
	}
	m_myIocp.m_ContextMapLock.Unlock();
	UpdateData(FALSE);
	   
	/*
	if(!m_myIocp.IsStarted())
	{
		DisableClientPanel();
	}

	CWnd *pCtrl=NULL;
	pCtrl=GetDlgItem(IDC_SEND);

	if(pCtrl!=NULL&&m_myIocp.IsStarted())
	{ 
	   pCtrl->ModifyStyle(WS_DISABLED,0,0);
	   pCtrl->Invalidate(); // Show the changes
	}
	*/
}

LRESULT CBHMonitorDlg::OnNewClient(WPARAM wParam, LPARAM lParam){
	unsigned int* piID = reinterpret_cast<unsigned int*>(lParam);

	CString msg;
	msg.Format("The client id is %d", *piID);
	AppendLog(msg);
	
	//is it nessesary?
	m_GUIListLock.Lock();
	
	ITEMINFO* pItem=new ITEMINFO;
	ClientContext* pContext=NULL;
	// to be sure that pContext Suddenly does not disappear.. 
	m_myIocp.m_ContextMapLock.Lock();
	pContext=m_myIocp.FindClient(*piID);
	if(pContext!=NULL)
	{
		// for debug.
		msg.Format("The received data is:%s\n", pContext->m_sReceived);
		AppendLog(msg);

		pContext->m_ContextLock.Lock();
		pItem->m_ClientAddress=m_myIocp.GetHostAddress(pContext->m_Socket);
		pItem->m_ID=pContext->m_Socket;
		pItem->m_iNumberOfReceivedMsg=0;
		pItem->m_bFileSendMode=FALSE;
		pItem->m_bFileReceivedMode=FALSE;
		pItem->m_iMaxFileBytes=-1;
		pContext->m_ContextLock.Unlock();
	}

	m_myIocp.m_ContextMapLock.Unlock();

	// Add the new client to the list.
	// InsertColumn instread of AddItemToList
	//if(!m_CtrlClientList.AddItemToList(pItem))
	//	AfxMessageBox("Fatal error!");
	
	UpdateData(TRUE);

	if(!m_myIocp.BuildPackageAndSend(*piID, m_myIocp.m_sSendText))
	{
		//DisableClientPanel();
	    int SItem=m_CtrlClientList.GetNextItem(-1,LVNI_SELECTED);
	    if(SItem!=-1)
		   m_CtrlClientList.SetItemState(SItem,LVNI_ALL, LVIF_TEXT | LVIF_IMAGE | LVIF_STATE);
		
		m_CtrlClientList.SetFocus();
		AppendLog(_T("Sending not successfull!\n"));
	}

	AppendLog(_T("Sending successfull!\n"));

	UpdateClientData();

	m_CtrlClientList.SetFocus();
	m_GUIListLock.Unlock();
	UpdateData(FALSE);

	// avoiding memory leak?
	if(piID!=NULL){
		delete piID;
		piID = NULL;
	}
   
	return 0;
}

LRESULT CBHMonitorDlg::OnAppendLog(WPARAM wParam, LPARAM lParam)
{
	char* msg = reinterpret_cast<char*>(lParam);
	if(msg!=NULL)
	{
		
		m_CtrlConLog.AppendString(msg);
	}
	delete[] msg;
	return 0;
}

LRESULT CBHMonitorDlg::OnClientDisconnected(WPARAM wParam, LPARAM lParam){
	unsigned int* piID = reinterpret_cast<unsigned int*>(lParam);
	BOOL bRepaint=FALSE;
	ClientContext *pContext=NULL;
	ITEMINFO* pItem=NULL;
	
	m_GUIListLock.Lock();	
	int nCount= m_CtrlClientList.GetItemCount();	
	for(int i=0; i<nCount;i++)
	{   
		pItem=(ITEMINFO*)m_CtrlClientList.GetItemData(i);
		if(pItem!=NULL)
		{
			// Disconnected 
			if(pItem->m_ID==*piID)
			{
				m_CtrlClientList.DeleteItem(i);
				nCount= m_CtrlClientList.GetRowCount();
				bRepaint=TRUE;
			}
		}
	}
	m_GUIListLock.Unlock();
	if(bRepaint)
	{
		int SItem=m_CtrlClientList.GetNextItem(-1,LVNI_SELECTED);
		if(SItem<0)
		{
			m_iCurrentClientID=0;
		}
		m_CtrlClientList.SetFocus();
		
//		m_CtrlClientList.ReSort();
	}
	UpdateData(FALSE);
	if(piID!=NULL){
		delete piID;
		piID = NULL;
	}

	return 0;
}

void CBHMonitorDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CBHMonitorDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_myIocp.IsStarted())
		m_myIocp.ShutDown();

	CDialog::OnOK();
}

void CBHMonitorDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(AfxMessageBox(IDS_EXIT_PROGRAM, MB_YESNO) == IDYES){
		if(m_myIocp.IsStarted())
			m_myIocp.ShutDown();

		CDialog::OnClose();
	}
}

void CBHMonitorDlg::OnBtCommunicationSetting() 
{
	// TODO: Add your control notification handler code here
	m_ConfigDlg.DoModal();
}

void CBHMonitorDlg::OnBtHkruleSetting() 
{
	// TODO: Add your control notification handler code here
	m_HKRuleSettingDlg.DoModal();
}
