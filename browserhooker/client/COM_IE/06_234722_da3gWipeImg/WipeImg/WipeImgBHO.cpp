// WipeImgBHO.cpp : Implementation of CWipeImgBHO

#include "stdafx.h"
#include "WipeImgBHO.h"


// CWipeImgBHO
/*
STDMETHODIMP CWipeImgBHO::SetSite(IUnknown* pUnkSite)
{
	if (pUnkSite != NULL)
	{
		//Get the webBrowser
		pUnkSite->QueryInterface(IID_IWebBrowser2, (void**)&m_WebBrowser);
	}
	else
	{
		// when close IE pUnkSite == NULL
		//Release Browser
		m_WebBrowser.Release();
	}
	//Base Implement
	return IObjectWithSiteImpl::SetSite(pUnkSite);
}
*/

STDMETHODIMP CWipeImgBHO::SetSite(IUnknown* pUnkSite)
{
	if (pUnkSite != NULL)
	{
		HRESULT hr = pUnkSite->QueryInterface(IID_IWebBrowser2, (void **)&m_WebBrowser);
		if(SUCCEEDED(hr))
		{
			//Regist Event from DWebBrowserEvents2
			hr = DispEventAdvise(m_WebBrowser);
			if (SUCCEEDED(hr))
			{
				m_Advised = TRUE;
			}
		}
	}
	else
	{
		if(m_Advised)
		{
			//Unregist event.
			DispEventUnadvise(m_WebBrowser);
			m_Advised = FALSE;
		}
		m_WebBrowser.Release();
	}
	return IObjectWithSiteImpl::SetSite(pUnkSite);
}

void STDMETHODCALLTYPE CWipeImgBHO::OnDocumentComplete(IDispatch *pDispatch, VARIANT *pvarURL)
{
	HWND hwnd;
	HRESULT hr = S_OK;

	//Get the IWebBrowser2.
	CComQIPtr<IWebBrowser2> tempWebBrowser = pDispatch;

	if (tempWebBrowser
		&&m_WebBrowser
		&&m_WebBrowser.IsEqualObject(tempWebBrowser))
	{
		//Get the Html Document.
		CComPtr<IDispatch> dispatchDocument;
		hr = m_WebBrowser->get_Document(&dispatchDocument);

		//Got.
		if (SUCCEEDED(hr))
		{
			//Query Html Document.
			CComQIPtr<IHTMLDocument2> htmlDocument = dispatchDocument; 
			if (htmlDocument != NULL) 
			{
				//Got the htmlDocument.
				//Remove Images.
				RemoveImages(htmlDocument);
			}
		}
	}

	/*
	HRESULT hr = m_WebBrowser->get_HWND((LONG_PTR*)&hwnd);
	if (SUCCEEDED(hr))
	{ 
		//Test Message Box.
		MessageBox(hwnd, L"I am Here!", L"Just For Test.", MB_OK); 
	}
	*/
}

void CWipeImgBHO::RemoveImages(IHTMLDocument2* pDocument)
{
	//Get the image DOM.
	CComPtr<IHTMLElementCollection> images;
	HRESULT hr = pDocument->get_images(&images); 

	if (hr == S_OK && images != NULL) 
	{
		long imageNumber = 0; 
		hr = images->get_length(&imageNumber);

		if(hr == S_OK && imageNumber > 0)
		{
			for (int i = 0; i < imageNumber; i++)
			{
				CComVariant variantItemIndex(i);
				CComVariant variantEmpty;

				CComPtr<IDispatch> dispatchImage;

				hr = images->item(variantItemIndex, variantEmpty, &dispatchImage);
				if (hr == S_OK && dispatchImage != NULL)
				{
					//Get the html element
					CComQIPtr<IHTMLElement> element = dispatchImage; 
					if (element)
					{
						//Get the style.
						CComPtr<IHTMLStyle> htmlStyle;
						hr = element->get_style(&htmlStyle);

						//Wipe image.
						if (hr == S_OK && htmlStyle != NULL)
						{
							static const CComBSTR sbstrNone(L"none"); 
							htmlStyle->put_display(sbstrNone);
						}
					} 
				}
			}
		}
	}
}