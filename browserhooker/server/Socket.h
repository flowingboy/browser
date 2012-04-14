// Socket.h: interface for the CSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKET_H__331B1715_160A_439F_9D37_98186FE7A2F6__INCLUDED_)
#define AFX_SOCKET_H__331B1715_160A_439F_9D37_98186FE7A2F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <map>

using namespace std;

class CSocket  
{
private:
	SOCKET m_connectSocket;       // socket to connect to server.
	string m_serverAddress;      // server address.
	UINT m_serverPort;           // server port.

public:
	CSocket(string, const UINT);
	virtual ~CSocket();

	BOOL ConnectToServer(const char* addr, const UINT port);
	BOOL Listen();
	BOOL SendData(const char*);
	BOOL RecvData(char *, UINT);
	void DestroySocket(SOCKET sock);
};

#endif // !defined(AFX_SOCKET_H__331B1715_160A_439F_9D37_98186FE7A2F6__INCLUDED_)
