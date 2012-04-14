// WipeImgBHO.h : Declaration of the CWipeImgBHO

#pragma once
#include "resource.h"       // main symbols
#include "WipeImg.h"
#include <shlguid.h>
#include <exdispid.h>
#include <mshtml.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CWipeImgBHO

class ATL_NO_VTABLE CWipeImgBHO :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWipeImgBHO, &CLSID_WipeImgBHO>,
	public IObjectWithSiteImpl<CWipeImgBHO>,
	public IDispatchImpl<IWipeImgBHO, &IID_IWipeImgBHO, &LIBID_WipeImgLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1, CWipeImgBHO, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
	CWipeImgBHO()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WIPEIMGBHO)

DECLARE_NOT_AGGREGATABLE(CWipeImgBHO)

BEGIN_COM_MAP(CWipeImgBHO)
	COM_INTERFACE_ENTRY(IWipeImgBHO)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()

BEGIN_SINK_MAP(CWipeImgBHO)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
END_SINK_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct(){return S_OK;}
	void FinalRelease(){}

public:
	STDMETHOD(SetSite)(IUnknown *pUnkSite);
	void STDMETHODCALLTYPE OnDocumentComplete(IDispatch *pDispatch, VARIANT *pvarURL);

private:
	CComPtr<IWebBrowser2> m_WebBrowser;
	BOOL m_Advised;
	void RemoveImages(IHTMLDocument2 *pDocument);
};

OBJECT_ENTRY_AUTO(__uuidof(WipeImgBHO), CWipeImgBHO)
