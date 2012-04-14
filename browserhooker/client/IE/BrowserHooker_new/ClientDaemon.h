// ClientDaemon.h: interface for the CClientDaemon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTDAEMON_H__248D3979_5FE6_4C94_AD7A_C73F8BFFE9B5__INCLUDED_)
#define AFX_CLIENTDAEMON_H__248D3979_5FE6_4C94_AD7A_C73F8BFFE9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Socket.h"

class CClientDaemon
{
private:
	// member variables.
	CSocket *m_serverSocket;   // CSocket member for connecting to server.
	string m_serverAddress;    // server address assume to be IPv4
	UINT   m_serverPort;       // server port.


public:
	CClientDaemon(string servAddr, const UINT servPort);
	virtual ~CClientDaemon();

#define MAX_DATA_LEN 1024*16

	map<string, string> GetUrlMapFromServer();
	
};

#endif // !defined(AFX_CLIENTDAEMON_H__248D3979_5FE6_4C94_AD7A_C73F8BFFE9B5__INCLUDED_)
