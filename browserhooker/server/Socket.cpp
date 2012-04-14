// Socket.cpp: implementation of the CSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Socket.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// for client socket operations, see : 
// http://msdn.microsoft.com/en-us/library/bb530750(VS.85).aspx
CSocket::CSocket(string Addr, const UINT Port)
{
	m_connectSocket = -1;
	m_serverAddress = Addr;
	m_serverPort = (UINT)Port;
}

CSocket::~CSocket()
{
}

/*
 * addr => server address
 * port => server port
 *
 */
BOOL CSocket::ConnectToServer(const char* addr, const UINT port){

	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR){
		WSACleanup();
		return FALSE;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService; 
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(addr);
	clientService.sin_port = htons(port);


	// Create a SOCKET for connecting to server
	m_connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_connectSocket == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}

	// Connect to server.
	if(connect(m_connectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

BOOL CSocket::SendData(const char* data){
	
	int iResult = 0;

	iResult = send(m_connectSocket, (char *)data, (int)strlen(data), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send() failed with error: %d\n", WSAGetLastError());
        closesocket(m_connectSocket);
        WSACleanup();
        return FALSE;
    }

	// shutdown the socket since no more data will be sent to server.
	iResult = shutdown(m_connectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_connectSocket);
        WSACleanup();
        return FALSE;
    }

	return TRUE;
}

/* 每次接收的包的大小是否有限制? */
#define DEFAULT_BUFLEN 4096

BOOL CSocket::RecvData(char* recvBuffer, UINT recvBufLen){
	int iResult = 0;
	int recvLen = 0;
	char recvBuf[DEFAULT_BUFLEN] = {0};

	BOOL bSuccess = TRUE;

	string msg;
    // Receive until the peer closes the connection
    do {

        iResult = recv(m_connectSocket, recvBuf, DEFAULT_BUFLEN, 0);
        if ( iResult > 0 ){
			if(iResult + recvLen > 0 && iResult + recvLen < recvBufLen){
				strncat(recvBuffer, recvBuf, iResult);
				recvLen += iResult;
			}else{
				//AfxMessageBox(_T("Buffer is overruned, get more buffer for it"), MB_YESNO, 0);
			}
		}else if ( iResult == 0 ){
            //AfxMessageBox(_T("Connection closed!"), MB_YESNO, 0);
			bSuccess = TRUE;
		}else if (iResult == SOCKET_ERROR){
			bSuccess = FALSE;
		}else{
			msg = "recv failed with error: " + WSAGetLastError();
            bSuccess = FALSE;
		}

    } while(iResult > 0);


    // cleanup
    closesocket(m_connectSocket);
    WSACleanup();
	return bSuccess;
}