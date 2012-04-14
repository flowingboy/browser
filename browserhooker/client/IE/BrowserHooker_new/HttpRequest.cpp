// HttpRequest.cpp: implementation of the CHttpRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpRequest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpRequest::CHttpRequest(){
	m_pHttpSocket = NULL;
	m_host = "";
	m_serverIP = "";
	m_serverPort = 0;
	m_uri = "";
	m_proto = "";
	m_userAgent = "Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 5.1)";
	m_cookie = "";

	m_httpHeader = "";
}

CHttpRequest::~CHttpRequest()
{
	if(m_pHttpSocket)
		delete m_pHttpSocket;

	m_pHttpSocket = NULL;
}

BOOL CHttpRequest::getIPAddress(string domain){

	struct hostent *ServerHost;
	struct in_addr addr;
	BOOL bSuccess = TRUE;

	ServerHost = gethostbyname(domain.c_str());
	if(ServerHost){
		switch (ServerHost->h_addrtype){
			case AF_INET:
				addr.S_un.S_addr = *(u_long *)ServerHost->h_addr_list[0];
				m_serverIP = inet_ntoa(addr);
				break;

			case AF_INET6:
			case AF_NETBIOS:
			default:
				bSuccess = FALSE;
				break;
		}
	}else{
		bSuccess = FALSE;
	}

	return bSuccess;
}

BOOL CHttpRequest::parseUrl(string url){
	if(stricmp(url.substr(0, 7).c_str(), "http://")){
		return FALSE;
	}

	size_t pos1 = string::npos;
	size_t pos2 = string::npos;
	size_t pos3 = string::npos;
	string port;

	pos1 = strlen("http://");
	pos2 = url.find("/", pos1);

	m_proto = url.substr(0, 4);
	m_host = url.substr(pos1, pos2 - pos1);
	m_uri  = pos2 == string::npos ? "/" : url.substr(pos2, url.size() - pos2 + 1);

	pos3 = m_host.find(":", 0);
	if(pos3 == string::npos){
		port = "80";

	}else{
		port = m_host.substr(pos3 + 1, m_host.size() - pos3);
		m_host = m_host.substr(0, pos3 - 1);
	}

	// get server ip and assigned to m_serverIP.
	if(!getIPAddress(m_host))
		return FALSE;

	m_serverPort = (UINT)atoi(port.c_str());

	return TRUE;
}

// http请求最后一行要有"\r\n"，以表明请求的结束
void CHttpRequest::makeHttpRequestHeader(BOOL isGet = TRUE){
	if(isGet){
		m_httpHeader  = "GET " + m_uri + " HTTP/1.1\r\n";
		m_httpHeader += "Host:" + m_host + "\r\n";
		m_httpHeader += "User-Agent:" + m_userAgent + "\r\n";
		m_httpHeader += "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
		m_httpHeader += "Accept-Language:en-us,en;q=0.5\r\n";
		m_httpHeader += "Accept-Encoding:gzip,deflate\r\n";
		m_httpHeader += "Accept-Charset:ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
		m_httpHeader += "Keep-Alive:300\r\n";
		m_httpHeader += "Connection:keep-alive\r\n";
		m_httpHeader += "\r\n";

	}else{
		m_httpHeader  = "POST " + m_uri + " HTTP/1.1\r\n";
		m_httpHeader += "Host:" + m_host + "\r\n";
		m_httpHeader += "User-Agent:" + m_userAgent + "\r\n";
		m_httpHeader += "\r\n";
	}
}

string CHttpRequest::HttpGet(string url){
	string page = "";

	//这里不能用数组分配buffer，因为堆栈空间的大小是有限制的
	char *pageBuffer = (char *)malloc(MAX_PAGE_SIZE);
	if(!pageBuffer)
		return page;

	memset(pageBuffer, 0, MAX_PAGE_SIZE);

	if(parseUrl(url)){
		makeHttpRequestHeader(TRUE);
		m_pHttpSocket = new CSocket(m_serverIP, m_serverPort);

		//连接服务器，发送http请求，获取html页面数据
		if(m_pHttpSocket->ConnectToServer(m_serverIP.c_str(), m_serverPort)
		&& m_pHttpSocket->SendData(m_httpHeader.c_str())
		&& m_pHttpSocket->RecvData(pageBuffer, MAX_PAGE_SIZE)){
			page = pageBuffer;
		}
	}

	return page;
}

string CHttpRequest::HttpPost(string url, string postData){
	string page;
	return page;
}