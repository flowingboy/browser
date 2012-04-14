// UseHtmlDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UseHtmlDlg.h"
#include "UseHtmlDlgDlg.h"

#include <atlbase.h>		// Ϊ��ʹ�� CComPtr,CComBSTR,CComVariant
#include <atlconv.h>		// Ϊ��ʹ�� T2COLE() ��
#include <Mshtmhst.h>		// ShowHTMLDialog ��������

/*********************************************************************
	���� ShowHTMLDialogEx �� VC6.0 ��û����ͷ�ļ��н��ж���
	��ô����Ķ����Ǳ���ġ��� VC.net �оͲ���Ҫ�ˡ�
**********************************************************************/
#define HTMLDLG_NOUI                     0x10
#define HTMLDLG_MODAL                    0x20
#define HTMLDLG_MODELESS                 0x40
#define HTMLDLG_PRINT_TEMPLATE           0x80
#define HTMLDLG_VERIFY                   0x100
typedef HRESULT STDAPICALLTYPE SHOWHTMLDIALOGEXFN(
			HWND hwndParent,		// ������
			IMoniker *pmk,			// URL Moniker
			DWORD dwDialogFlags,	// ������ HTMLDLG_xxx ��һЩ���
			VARIANT *pvarArgIn,		// ������� window.dialogArguments
			WCHAR* pchOptions,		// �Ի�����ʽ
			VARIANT *pvArgOut);		// ������� window.returnValue
//////////////////////////////////////////////////////////////////////
#pragma comment(lib, "Urlmon")		// ���ӿ� urlmon.lib

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR lpHtml=_T("<html>\
�ٺ٣����Ҳ����ҡ���Ϊ�Ҳ��� HTML �ļ��������ڴ��е�һ���ַ�������Ҳû�������ĵĲ˵���<p>\
<center><input type=\"button\" value=\"HTML �Ի���\" OnClick=\"window.external.ShowDlg()\"></center>\
<img src=\"http://support.microsoft.com/library/images/support/en-US/Help_support_55.gif\"></br>\
<object width=\"400\" height=\"340\"><param name=\"movie\" value=\"http://www.tudou.com/v/r_z_HOUvW-M\"></param><param name=\"allowScriptAccess\" value=\"always\"></param><param name=\"wmode\" value=\"transparent\"></param><embed src=\"http://www.tudou.com/v/ek977kGpDWk\" type=\"application/x-shockwave-flash\" width=\"400\" height=\"340\" allowFullScreen=\"true\" wmode=\"transparent\" allowScriptAccess=\"always\"></embed></object>\
</html>");
/////////////////////////////////////////////////////////////////////////////
// CUseHtmlDlgDlg dialog

CUseHtmlDlgDlg::CUseHtmlDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUseHtmlDlgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUseHtmlDlgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUseHtmlDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUseHtmlDlgDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_ie);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUseHtmlDlgDlg, CDialog)
	//{{AFX_MSG_MAP(CUseHtmlDlgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUseHtmlDlgDlg message handlers
#include <atlbase.h>
BOOL CUseHtmlDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	HRESULT hr;

	m_ie.Navigate2( &CComVariant(_T("about:blank")),NULL,NULL,NULL,NULL);

	CComPtr< IDispatch > spDoc(m_ie.GetDocument());
	ASSERT( spDoc );

	CComQIPtr< IPersistStreamInit, &IID_IPersistStreamInit > spPSI( spDoc );
	ASSERT(spPSI);

	LPTSTR lpMem = (LPTSTR)::GlobalAlloc( GPTR, ::lstrlen( lpHtml )+1 );
	ASSERT( lpMem );
	::lstrcpy( lpMem, lpHtml );

	CComPtr< IStream > spStream;
	::CreateStreamOnHGlobal( lpMem, TRUE, &spStream );
	ASSERT( spStream );

	hr = spPSI->InitNew();
	ASSERT( SUCCEEDED(hr) );
	
	hr = spPSI->Load( spStream );
	ASSERT( SUCCEEDED(hr) );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUseHtmlDlgDlg::OnPaint() 
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
HCURSOR CUseHtmlDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUseHtmlDlgDlg::ShowDlg()
{
	HMODULE hDll = ::LoadLibrary( _T("MSHTML.DLL") );
	ASSERT( hDll );		// ����ɣ���Ȼû��װ IE

	SHOWHTMLDIALOGEXFN	*pfn;
	pfn = ( SHOWHTMLDIALOGEXFN * )GetProcAddress( hDll, _T("ShowHTMLDialogEx") );
	ASSERT( pfn );		// ����ô�������ϵ�����IEѽ?����ҲҪ IE4.0 ѽ

	TCHAR szPathName[MAX_PATH];		// ȡ��ִ���ļ���ȫ·���ļ���
	::GetModuleFileName( AfxGetInstanceHandle(), szPathName, sizeof(szPathName) );

	CString sURL = _T("res://");	// ��λ�ļ��е�HTML��Դ
	sURL += szPathName;
	sURL += _T("/HTML_DLG");

	CComPtr < IMoniker > spMk;
	USES_CONVERSION;
	::CreateURLMoniker( NULL, T2COLE(sURL), &spMk );	// ����URL��moniker
	ASSERT( spMk );		// ���ܹ���IMoniker? ������ COM ��ʼ���˷�

	CComVariant varArgs( _T("����������������֣��������� MS WORD �ļ�������") );
	CComVariant varReturn;							// ����ֵ

	HRESULT hr =(*pfn)(
			m_hWnd,			// ������
			spMk,			// ��moniker������IE����
			HTMLDLG_MODAL,	// ��ʾģʽ�Ի���
			&varArgs,		// �������
			NULL,			// �Ի�����ʽ
			&varReturn);	// ����ֵ
	ASSERT( SUCCEEDED(hr) );				// ��Ȼ���� HTML �Ի���ʧ��
	spMk.Release();

	if( ::SysStringLen(varReturn.bstrVal) )	// �Ի���ִ���� OK ����
	{
		sURL =  _T("res://");	// ��λ�ļ��е�HTML��Դ
		sURL += szPathName;
		sURL += _T("/HTML_TOWORD");
		
		::CreateURLMoniker( NULL, T2COLE(sURL), &spMk );
		ASSERT( spMk );

		hr=(*pfn)(
			m_hWnd,			// ������
			spMk,			// ��moniker������IE����
			HTMLDLG_MODAL|HTMLDLG_NOUI,	// ����ʾ�Ի������棬ֱ��ִ��HTML�еĽű�
			&varReturn,		// ǰ�εķ���ֵ��Ϊ���ε��������
			NULL,			// ��ָ����ʽ
			NULL);			// ��ȡ�÷���ֵ
	}

	::FreeLibrary( hDll );
}
