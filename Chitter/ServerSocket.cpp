// ServerSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Chitter.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "ChitterDlg.h"


// CServerSocket

CServerSocket::CServerSocket()
{
	myIP = GetMyIP();
}

CServerSocket::~CServerSocket()
{
}


// CServerSocket ��� �Լ�


void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CString msg, ipaddr;
	UINT port;
	CClientSocket* pClient = new CClientSocket;
	if (Accept(*pClient)) {
		pClient->SetListenSocket(this);
		mPtrClientSocketList.AddTail(pClient);
	} 
	else 
	{
		delete pClient;
		AfxMessageBox("Accept ����");
	}
	pClient->GetPeerName(ipaddr, port);
	msg.Format("%s ���� �����ϼ̽��ϴ�", ipaddr);
	SendDataToAll(LPSTR(LPCTSTR(msg)));

	CAsyncSocket::OnAccept(nErrorCode);
}


void CServerSocket::CloseClientSocket(CClientSocket* pClient)
{
	POSITION pos;
	pos = mPtrClientSocketList.Find(pClient);
	if (pos != NULL) {
		if (pClient != NULL) {
			pClient->ShutDown();
			pClient->Close();
		}
		mPtrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}


void CServerSocket::SendDataToAll(char* str)
{
	POSITION pos;
	pos = mPtrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;
	while (pos != NULL) {
		pClient = (CClientSocket*)mPtrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->Send(str, strlen(str));
		}
	}
	((CChitterDlg*)parent)->AppendTextPanel(str);
}


CString CServerSocket::GetMyIP()
{
	CString ip = "";
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
		WSACleanup();
	}
	return ip;
}
