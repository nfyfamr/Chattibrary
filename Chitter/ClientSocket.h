#pragma once

// CClientSocket ��� ����Դϴ�.

#define BUFFER_SIZE 512


class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	CAsyncSocket* mPListenSocket;
	virtual void SetListenSocket(CAsyncSocket* mPListenSocket);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	CString caption;
	CDialog* parent;

};


