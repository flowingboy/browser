// UseHtmlDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UseHtmlDlg.h"
#include "UseHtmlDlgDlg.h"

#include <atlbase.h>		// 为了使用 CComPtr,CComBSTR,CComVariant
#include <atlconv.h>		// 为了使用 T2COLE() 宏
#include <Mshtmhst.h>		// ShowHTMLDialog 函数声明

/*********************************************************************
	由于 ShowHTMLDialogEx 在 VC6.0 还没有在头文件中进行定义
	那么下面的定义是必须的。在 VC.net 中就不需要了。
**********************************************************************/
#define HTMLDLG_NOUI                     0x10
#define HTMLDLG_MODAL                    0x20
#define HTMLDLG_MODELESS                 0x40
#define HTMLDLG_PRINT_TEMPLATE           0x80
#define HTMLDLG_VERIFY                   0x100
typedef HRESULT STDAPICALLTYPE SHOWHTMLDIALOGEXFN(
			HWND hwndParent,		// 父窗口
			IMoniker *pmk,			// URL Moniker
			DWORD dwDialogFlags,	// 是上面 HTMLDLG_xxx 的一些组合
			VARIANT *pvarArgIn,		// 输入参数 window.dialogArguments
			WCHAR* pchOptions,		// 对话窗样式
			VARIANT *pvArgOut);		// 输出参数 window.returnValue
//////////////////////////////////////////////////////////////////////
#pragma comment(lib, "Urlmon")		// 连接库 urlmon.lib

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR lpHtml=_T("<html>\
嘿嘿，你找不到我。因为我不是 HTML 文件，我是内存中的一个字符串。我也没有上下文的菜单。<p>\
<center><input type=\"button\" value=\"HTML 对话窗\" OnClick=\"window.external.ShowDlg()\"></center>\
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
	ASSERT( hDll );		// 不会吧？居然没有装 IE

	SHOWHTMLDIALOGEXFN	*pfn;
	pfn = ( SHOWHTMLDIALOGEXFN * )GetProcAddress( hDll, _T("ShowHTMLDialogEx") );
	ASSERT( pfn );		// 你怎么还再用老掉牙的IE呀?至少也要 IE4.0 呀

	TCHAR szPathName[MAX_PATH];		// 取得执行文件的全路径文件名
	::GetModuleFileName( AfxGetInstanceHandle(), szPathName, sizeof(szPathName) );

	CString sURL = _T("res://");	// 定位文件中的HTML资源
	sURL += szPathName;
	sURL += _T("/HTML_DLG");

	CComPtr < IMoniker > spMk;
	USES_CONVERSION;
	::CreateURLMoniker( NULL, T2COLE(sURL), &spMk );	// 捆绑URL到moniker
	ASSERT( spMk );		// 不能构造IMoniker? 你做了 COM 初始化了否！

	CComVariant varArgs( _T("你在这里输入的文字，将会生成 MS WORD 文件的内容") );
	CComVariant varReturn;							// 返回值

	HRESULT hr =(*pfn)(
			m_hWnd,			// 父窗口
			spMk,			// 由moniker启动的IE对象
			HTMLDLG_MODAL,	// 显示模式对话窗
			&varArgs,		// 输入参数
			NULL,			// 对话窗样式
			&varReturn);	// 返回值
	ASSERT( SUCCEEDED(hr) );				// 居然调用 HTML 对话窗失败
	spMk.Release();

	if( ::SysStringLen(varReturn.bstrVal) )	// 对话窗执行了 OK 返回
	{
		sURL =  _T("res://");	// 定位文件中的HTML资源
		sURL += szPathName;
		sURL += _T("/HTML_TOWORD");
		
		::CreateURLMoniker( NULL, T2COLE(sURL), &spMk );
		ASSERT( spMk );

		hr=(*pfn)(
			m_hWnd,			// 父窗口
			spMk,			// 由moniker启动的IE对象
			HTMLDLG_MODAL|HTMLDLG_NOUI,	// 不显示对话窗界面，直接执行HTML中的脚本
			&varReturn,		// 前次的返回值，为本次的输入参数
			NULL,			// 不指定样式
			NULL);			// 不取得返回值
	}

	::FreeLibrary( hDll );
}
