// ClientSocket.cpp : 구현 파일입니다.
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


// CClientSocket 멤버 함수


void CClientSocket::SetListenSocket(CAsyncSocket* mPListenSocket)
{
	this->mPListenSocket = mPListenSocket;
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CSocket::OnClose(nErrorCode);

	CString msg;
	CString strIPAddress = "";
	UINT port = 0;
	GetPeerName(strIPAddress, port);
	CServerSocket* pServerSocket = (CServerSocket*)mPListenSocket;
	pServerSocket->CloseClientSocket(this);
	msg.Format("%s 께서 퇴장하였습니다", strIPAddress);
	pServerSocket->SendDataToAll(LPSTR(LPCTSTR(msg)));
}


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
