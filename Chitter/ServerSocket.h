#pragma once

#include "ClientSocket.h"
// CServerSocket ��� ����Դϴ�.

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


