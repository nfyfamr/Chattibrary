// ClientSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Chitter.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "ChitterDlg.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket ��� �Լ�


void CClientSocket::SetListenSocket(CAsyncSocket* mPListenSocket)
{
	this->mPListenSocket = mPListenSocket;
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CSocket::OnClose(nErrorCode);

	CString msg;
	CString strIPAddress = "";
	UINT port = 0;
	GetPeerName(strIPAddress, port);
	CServerSocket* pServerSocket = (CServerSocket*)mPListenSocket;
	pServerSocket->CloseClientSocket(this);
	msg.Format("%s ���� �����Ͽ����ϴ�", strIPAddress);
	pServerSocket->SendDataToAll(LPSTR(LPCTSTR(msg)));
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CString msg = "";
	CString strIPAddress = "";
	UINT port = 0;
	char buf[BUFFER_SIZE];
	GetPeerName(strIPAddress, port);
	::ZeroMemory(buf, BUFFER_SIZE);
	if (Receive(buf, BUFFER_SIZE) > 0) {
		msg.Format("[%s:%d] : %s", strIPAddress, port, buf);
		CServerSocket* pServerSocket = (CServerSocket*)mPListenSocket;
		pServerSocket->SendDataToAll(LPSTR(LPCTSTR(msg)));
	}

	CSocket::OnReceive(nErrorCode);
}
