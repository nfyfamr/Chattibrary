// ConnectSocket.cpp : 구현 파일입니다.
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


// CConnectSocket 멤버 함수


void CConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox("서버와 연결이 종료되었습니다.");
	CSocket::OnClose(nErrorCode);
}
