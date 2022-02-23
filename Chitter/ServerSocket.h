#pragma once

#include "ClientSocket.h"
// CServerSocket 명령 대상입니다.

class CServerSocket : public CAsyncSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	CPtrList mPtrClientSocketList;
	CString caption;
	CString myIP;
	CDialog* parent;

	virtual void CloseClientSocket(CClientSocket* pClient);
	virtual void SendDataToAll(char* str);
	static CString GetMyIP();
};


