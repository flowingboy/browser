// ClientDaemon.cpp: implementation of the CClientDaemon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientDaemon.h"

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CClientDaemon::CClientDaemon(string servAddr, const UINT servPort){
	m_serverAddress = servAddr;
	m_serverPort = servPort;

	m_serverSocket = new CSocket(m_serverAddress, m_serverPort);
}

CClientDaemon::~CClientDaemon(){

}

map<string, string> CClientDaemon::GetUrlMapFromServer(){
	map<string, string> urlMap;
	char *data = (char *)malloc(MAX_DATA_LEN);
	if(!data){
		urlMap.clear();
		return urlMap;
	}

	string s1, s2, s;
	size_t pos1, pos2, pos0 = 0;

	memset(data, 0, MAX_DATA_LEN);
	
	if(m_serverSocket->ConnectToServer(m_serverAddress.c_str(), m_serverPort)
	&& m_serverSocket->SendData("GET /urlmap HTTP1.1\r\n")
	&& m_serverSocket->RecvData(data, MAX_DATA_LEN)
		){

		s = data;
		pos1 = s.find("|");

		while(pos1 != string::npos){
			s1 = s.substr(pos0, pos1 - pos0);


			pos2 = s1.find("-");
			urlMap.insert(make_pair(s1.substr(pos0, pos2 - pos0), s1.substr(pos2 + 1, pos1 - pos2 - 1)));
			
			pos0 = pos1 + 1;
			pos1 = s.find("|");
		}
	}

	return urlMap;
}