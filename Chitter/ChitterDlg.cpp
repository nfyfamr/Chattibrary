
// ChitterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Chitter.h"
#include "ChitterDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChitterDlg 대화 상자

#define ADDITEM(x) cstr2Enum.insert(std::make_pair(#x, Command::x));

CChitterDlg::CChitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHITTER_DIALOG, pParent), mLibMgr(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ADDITEM(도움말);
	ADDITEM(입장);
	ADDITEM(방열기);
	ADDITEM(퇴장);
	ADDITEM(내ip);
	ADDITEM(도서등록);
	ADDITEM(도서대출);
	ADDITEM(도서반납);
	ADDITEM(소장도서목록);
	ADDITEM(대여도서목록);
	
//	AfxMessageBox(cstr2Enum.size());
}

void CChitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChitterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
ON_WM_HSCROLL()
ON_BN_CLICKED(ID_SEND, &CChitterDlg::OnBnClickedSend)
ON_WM_DESTROY()
ON_WM_LBUTTONDOWN()
ON_EN_VSCROLL(IDC_TEXT_PANEL, &CChitterDlg::OnVscrollTextPanel)
ON_EN_VSCROLL(IDC_TEXT_INPUT, &CChitterDlg::OnVscrollTextInput)
ON_EN_UPDATE(IDC_TEXT_INPUT, &CChitterDlg::OnUpdateTextInput)
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChitterDlg 메시지 처리기

BOOL CChitterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 윈도우 레이어드 속성 추가
	LONG lResult = SetWindowLong(this->GetSafeHwnd()
		, GWL_EXSTYLE
		, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);

	if (!lResult) {
		AfxMessageBox("Fail to SetWindowLong...");
		return FALSE;
	}

	if (!SetLayeredWindowAttributes(0, mTransparent, LWA_ALPHA)) {
		AfxMessageBox("Fail to SetLayredWindowAttributes...");
		return FALSE;
	}
	
	// 폰트 설정
	mFont.CreatePointFont(180, "굴림");
	GetDlgItem(IDC_TITLE_CHATTIBRARY)->SetFont(&mFont);

	// mCtrSlider와 mTransparent 초기화
	mCtrSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_TRANSPARENT);
	mCtrSlider->SetPos(mCtrSlider->GetRangeMax());
	mTransparent = 255;

	// mTextInput과 mTextPanel 초기화
	mTextInput = (CEdit*)GetDlgItem(IDC_TEXT_INPUT);
	mTextPanel = (CEdit*)GetDlgItem(IDC_TEXT_PANEL);
	mTextPanelBrush.CreateSolidBrush(RGB(245, 241, 222));

	// 스크롤상태 변수 초기화
	bPanelScrollShowed = FALSE;
	bInputScrollShowed = FALSE;

	// 시작시 도움말 보여주기
	AppendTextPanel("Chattibrary를 시작합니다!");
	ShowHelp();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChitterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChitterDlg::OnPaint()
{
	// 다이얼로그 전체의 알파값 변경
	if (!SetLayeredWindowAttributes(0, mTransparent, LWA_ALPHA)) {
		AfxMessageBox("Fail to SetLayredWindowAttributes...");
		return ;
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChitterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CChitterDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(245, 241, 222));

	return TRUE;
}



void CChitterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar == (CScrollBar*)mCtrSlider) {
		mTransparent = mCtrSlider->GetPos() * 2 + 55;
		Invalidate();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CChitterDlg::OnBnClickedSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i = 0;
	keybd_event(VK_RETURN, 0, 0, 0);
	GotoDlgCtrl(GetDlgItem(IDC_TEXT_INPUT));
}


BOOL CChitterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// Shift+Enter면 개행, Enter면 전송
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetDlgItem(IDC_TEXT_INPUT) == GetFocus()) {
		if (::GetKeyState(VK_LSHIFT) & 0x8000 || ::GetKeyState(VK_RSHIFT) & 0x8000) {
			int nLen = mTextInput->GetWindowTextLength();
			mTextInput->SetSel(nLen, nLen);
			mTextInput->ReplaceSel("\r\n");
		}
		else {
			CString str;
			mTextInput->GetWindowText(str);
			if (!str.Trim().IsEmpty()) {
				ProcessingText(str);
				mTextInput->SetSel(0, -1);
				mTextInput->Clear();
			}
			bInputScrollShowed = FALSE;
		}
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CChitterDlg::ProcessingText(CString string)
{
	// 커맨드 처리
	if (string.GetAt(0) == '!') {
		UINT port;
		CString cmd, param1, param2, param3;
		AfxExtractSubString(cmd, string, 0, ' ');
		cmd.Delete(0);
		if (cstr2Enum.find(cmd) == cstr2Enum.end()) {
			AppendTextPanel("잘못된 명령어입니다");
			ShowHelp();
			return;
		}

		switch (cstr2Enum[cmd]) {
		case Command::도움말:
			ShowHelp();
			return;
		case Command::입장:
			AfxExtractSubString(param1, string, 2, ' ');
			port = _ttoi(param1);
			AfxExtractSubString(param1, string, 1, ' ');
			AppendTextPanel("방에 입장합니다...");
			EnterChat(param1, port);
			return;
		case Command::방열기:
			AfxExtractSubString(param1, string, 1, ' ');
			port = _ttoi(param1);
			AppendTextPanel("방을 엽니다...");
			OpenChat(port);
			return;
		case Command::퇴장:
			CloseChat();
			return;
		case Command::내ip:
			AppendTextPanel("내 IP입니다: " + CServerSocket::GetMyIP());
			return;
		case Command::도서등록:
			AfxExtractSubString(param1, string, 1, ' ');
			AfxExtractSubString(param2, string, 2, ' ');
			AfxExtractSubString(param3, string, 3, ' ');
			port = _ttoi(param3);
			mLibMgr.RegisterBook(param1, param2, port);
			return;
		case Command::도서대출:
			AfxExtractSubString(param1, string, 1, ' ');
			AfxExtractSubString(param2, string, 2, ' ');
			mLibMgr.RentalBook(param1, param2);
			return;
		case Command::도서반납:
			AfxExtractSubString(param1, string, 1, ' ');
			AfxExtractSubString(param2, string, 2, ' ');
			mLibMgr.ReturnBook(param1, param2);
			return;
		case Command::소장도서목록:
			mLibMgr.ShowBookList();
			return;
		case Command::대여도서목록:
			mLibMgr.ShowRentarledList();
			return;
		}
		
	}
	else {
		if (mPServerSocket != NULL) {
			CString msg;
			msg.Format("[방장] : %s", string);
			mPServerSocket->SendDataToAll(LPSTR(LPCTSTR(msg)));
		}
		else if (mPClientSocket != NULL) {
			mPClientSocket->Send(string, string.GetLength());
		}
	}
}

void CChitterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DestroyServerSocket();
	mTextPanelBrush.DeleteObject();
}


void CChitterDlg::DestroyServerSocket()
{
	if (mPServerSocket != NULL)
	{
		POSITION pos;
		pos = mPServerSocket->mPtrClientSocketList.GetHeadPosition();
		CClientSocket* pClient = NULL;
		while (pos != NULL)
		{
			pClient = (CClientSocket*)mPServerSocket->mPtrClientSocketList.GetNext(pos);
			if (pClient != NULL)
			{
				pClient->ShutDown();
				pClient->Close();
				delete pClient;
			}
		}
		mPServerSocket->ShutDown();
		mPServerSocket->Close();
		delete mPServerSocket;
		mPServerSocket = nullptr;
	}
}


void CChitterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CChitterDlg::CloseChat()
{
	if (mPServerSocket != NULL) {
		AppendTextPanel("열어놓은 방을 닫습니다");
		DestroyServerSocket();
	} else if (mPClientSocket != NULL) {
		AppendTextPanel("현재 방에서 퇴장합니다");
		delete mPClientSocket;
		mPClientSocket = nullptr;
	} else {
		AppendTextPanel("열어 놓은 방이 없습니다");
	}
	return;
}


void CChitterDlg::OpenChat(UINT port)
{
	CString msg;
	if (port == 0 || port > 65535) {
		AppendTextPanel("포트번호를 확인해주세요: " + port);
		return;
	}
	if (mPServerSocket != NULL) {
		AppendTextPanel("이미 열어놓은 방이 있습니다!! ");
		return;
	}
	mPServerSocket = new CServerSocket;
	if (mPServerSocket->Create(port)) {
		if (!mPServerSocket->Listen()) {
			msg.Format("%d 에 열기 실패!!!", port);
			AppendTextPanel(msg);
			DestroyServerSocket();
			return;
		}
		mPServerSocket->parent = this;
		msg.Format("방을 열었습니다: %s:%d", mPServerSocket->myIP, port);
		AppendTextPanel(msg);
		return;
	}
	else {
		AppendTextPanel("소켓 생성 실패");
		DestroyServerSocket();
		return;
	}
}


void CChitterDlg::EnterChat(CString ip, UINT port)
{
	CString msg;
	if (port > 65535) {
		AppendTextPanel("포트번호를 확인해주세요: " + port);
		return;
	}
	if (mPClientSocket != NULL) {
		AppendTextPanel("이미 입장한 방이 있습니다 ");
		return;
	}
	mPClientSocket = new CConnectSocket;
	mPClientSocket->Create();
	if (mPClientSocket->Connect(ip, port) == FALSE)
	{
		AppendTextPanel("서버연결 실패: ");
		delete mPClientSocket;
		mPClientSocket = nullptr;
		return;
	}
	mPClientSocket->parent = this;
	msg.Format("%s:%d 에 들어갔습니다...", ip, port);
	AppendTextPanel(msg);
}


void CChitterDlg::ShowHelp()
{
	AppendTextPanel("================= 도 움 말 =================");
	AppendTextPanel("!도움말 : 도움말을 봅니다");
	AppendTextPanel("!입장 <ip> <port> : 채팅방에 들어갑니다");
	AppendTextPanel("!방열기 <port> : 채팅방을 엽니다");
	AppendTextPanel("!퇴장 : 진행중인 채팅방을 닫습니다");
	AppendTextPanel("!내ip : 자신의 ip를 확인합니다");
	AppendTextPanel("!도서등록 <book> <author> [num] : 대출가능한 책을 등록합니다");
	AppendTextPanel("!도서대출 <name> <book> : 책을 대출합니다");
	AppendTextPanel("!도서반납 <name> <book> : 책을 반납합니다");
	AppendTextPanel("!소장도서목록 : 대출가능한 책들을 보여줍니다");
	AppendTextPanel("!대여도서목록 : 대여중인 책들을 보여줍니다");
	AppendTextPanel("=======================================");
	AppendTextPanel("");
}


void CChitterDlg::OnVscrollTextPanel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (bPanelScrollShowed == FALSE) {
		bPanelScrollShowed = TRUE;
		mTextPanel->ShowScrollBar(SB_VERT, TRUE);
	}
}


void CChitterDlg::OnVscrollTextInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (bInputScrollShowed == FALSE) {
		bInputScrollShowed = TRUE;
		mTextInput->ShowScrollBar(SB_VERT, TRUE);
	}
}


void CChitterDlg::OnUpdateTextInput()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (bInputScrollShowed == FALSE) {
		mTextInput->ShowScrollBar(SB_VERT, FALSE);
	}
}


HBRUSH CChitterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (pWnd->GetDlgCtrlID() == IDC_TEXT_PANEL) {
		pDC->SetBkColor(RGB(245, 241, 222));
		hbr = mTextPanelBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TITLE_BACKGROUND) {
		pDC->SetBkColor(RGB(241, 221, 207));

	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
