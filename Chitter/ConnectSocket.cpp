// ConnectSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Chitter.h"
#include "ConnectSocket.h"
#include "ChitterDlg.h"

#define BUFFER_SIZE 512


// CConnectSocket

CConnectSocket::CConnectSocket()
{
}

CConnectSocket::~CConnectSocket()
{
}


// CConnectSocket ��� �Լ�


void CConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	char buf[BUFFER_SIZE];
	::ZeroMemory(buf, BUFFER_SIZE);
	if (Receive(buf, BUFFER_SIZE))
	{
		CString tmp;
		tmp.Format("%s", buf);
		((CChitterDlg*)parent)->AppendTextPanel(tmp);
	}

	CSocket::OnReceive(nErrorCode);
}


void CConnectSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox("������ ������ ����Ǿ����ϴ�.");
	CSocket::OnClose(nErrorCode);
}
