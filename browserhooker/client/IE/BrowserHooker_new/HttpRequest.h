// HttpRequest.h: interface for the CHttpRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPREQUEST_H__6FCFC6E6_4A67_4593_9F15_766D91636BAC__INCLUDED_)
#define AFX_HTTPREQUEST_H__6FCFC6E6_4A67_4593_9F15_766D91636BAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Socket.h"

#include <string>
#include <cstddef>

using namespace std;

class CHttpRequest
{
public:
	CHttpRequest();
	virtual ~CHttpRequest();

private:
	CSocket* m_pHttpSocket;
	string m_host;
	string m_serverIP;
	UINT m_serverPort;
	string m_uri;
	string m_proto;
	string m_userAgent;
	string m_cookie;
	string m_httpHeader;

	BOOL parseUrl(string url);
	BOOL getIPAddress(string domain);
	void makeHttpRequestHeader(BOOL);

public:
	string HttpGet(string url);
	string HttpPost(string url, string postData);

#define MAX_PAGE_SIZE 1024*1024
};

#endif // !defined(AFX_HTTPREQUEST_H__6FCFC6E6_4A67_4593_9F15_766D91636BAC__INCLUDED_)
