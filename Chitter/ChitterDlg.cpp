
// ChitterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Chitter.h"
#include "ChitterDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CChitterDlg ��ȭ ����

#define ADDITEM(x) cstr2Enum.insert(std::make_pair(#x, Command::x));

CChitterDlg::CChitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHITTER_DIALOG, pParent), mLibMgr(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ADDITEM(����);
	ADDITEM(����);
	ADDITEM(�濭��);
	ADDITEM(����);
	ADDITEM(��ip);
	ADDITEM(�������);
	ADDITEM(��������);
	ADDITEM(�����ݳ�);
	ADDITEM(���嵵�����);
	ADDITEM(�뿩�������);
	
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


// CChitterDlg �޽��� ó����

BOOL CChitterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// ������ ���̾�� �Ӽ� �߰�
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
	
	// ��Ʈ ����
	mFont.CreatePointFont(180, "����");
	GetDlgItem(IDC_TITLE_CHATTIBRARY)->SetFont(&mFont);

	// mCtrSlider�� mTransparent �ʱ�ȭ
	mCtrSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_TRANSPARENT);
	mCtrSlider->SetPos(mCtrSlider->GetRangeMax());
	mTransparent = 255;

	// mTextInput�� mTextPanel �ʱ�ȭ
	mTextInput = (CEdit*)GetDlgItem(IDC_TEXT_INPUT);
	mTextPanel = (CEdit*)GetDlgItem(IDC_TEXT_PANEL);
	mTextPanelBrush.CreateSolidBrush(RGB(245, 241, 222));

	// ��ũ�ѻ��� ���� �ʱ�ȭ
	bPanelScrollShowed = FALSE;
	bInputScrollShowed = FALSE;

	// ���۽� ���� �����ֱ�
	AppendTextPanel("Chattibrary�� �����մϴ�!");
	ShowHelp();


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CChitterDlg::OnPaint()
{
	// ���̾�α� ��ü�� ���İ� ����
	if (!SetLayeredWindowAttributes(0, mTransparent, LWA_ALPHA)) {
		AfxMessageBox("Fail to SetLayredWindowAttributes...");
		return ;
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CChitterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CChitterDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(245, 241, 222));

	return TRUE;
}



void CChitterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (pScrollBar == (CScrollBar*)mCtrSlider) {
		mTransparent = mCtrSlider->GetPos() * 2 + 55;
		Invalidate();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CChitterDlg::OnBnClickedSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int i = 0;
	keybd_event(VK_RETURN, 0, 0, 0);
	GotoDlgCtrl(GetDlgItem(IDC_TEXT_INPUT));
}


BOOL CChitterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// Shift+Enter�� ����, Enter�� ����
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
	// Ŀ�ǵ� ó��
	if (string.GetAt(0) == '!') {
		UINT port;
		CString cmd, param1, param2, param3;
		AfxExtractSubString(cmd, string, 0, ' ');
		cmd.Delete(0);
		if (cstr2Enum.find(cmd) == cstr2Enum.end()) {
			AppendTextPanel("�߸��� ��ɾ��Դϴ�");
			ShowHelp();
			return;
		}

		switch (cstr2Enum[cmd]) {
		case Command::����:
			ShowHelp();
			return;
		case Command::����:
			AfxExtractSubString(param1, string, 2, ' ');
			port = _ttoi(param1);
			AfxExtractSubString(param1, string, 1, ' ');
			AppendTextPanel("�濡 �����մϴ�...");
			EnterChat(param1, port);
			return;
		case Command::�濭��:
			AfxExtractSubString(param1, string, 1, ' ');
			port = _ttoi(param1);
			AppendTextPanel("���� ���ϴ�...");
			OpenChat(port);
			return;
		case Command::����:
			CloseChat();
			return;
		case Command::��ip:
			AppendTextPanel("�� IP�Դϴ�: " + CServerSocket::GetMyIP());
			return;
		case Command::�������:
			AfxExtractSubString(param1, string, 1, ' ');
			AfxExtractSubString(param2, string, 2, ' ');
			AfxExtractSubString(param3, string, 3, ' ');
			port = _ttoi(param3);
			mLibMgr.RegisterBook(param1, param2, port);
			return;
		case Command::��������:
			AfxExtractSubString(param1, string, 1, ' ');
			AfxExtractSubString(param2, string, 2, ' ');
			mLibMgr.RentalBook(param1, param2);
			return;
		case Command::�����ݳ�:
			AfxExtractSubString(param1, string, 1, ' ');
			AfxExtractSubString(param2, string, 2, ' ');
			mLibMgr.ReturnBook(param1, param2);
			return;
		case Command::���嵵�����:
			mLibMgr.ShowBookList();
			return;
		case Command::�뿩�������:
			mLibMgr.ShowRentarledList();
			return;
		}
		
	}
	else {
		if (mPServerSocket != NULL) {
			CString msg;
			msg.Format("[����] : %s", string);
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CChitterDlg::CloseChat()
{
	if (mPServerSocket != NULL) {
		AppendTextPanel("������� ���� �ݽ��ϴ�");
		DestroyServerSocket();
	} else if (mPClientSocket != NULL) {
		AppendTextPanel("���� �濡�� �����մϴ�");
		delete mPClientSocket;
		mPClientSocket = nullptr;
	} else {
		AppendTextPanel("���� ���� ���� �����ϴ�");
	}
	return;
}


void CChitterDlg::OpenChat(UINT port)
{
	CString msg;
	if (port == 0 || port > 65535) {
		AppendTextPanel("��Ʈ��ȣ�� Ȯ�����ּ���: " + port);
		return;
	}
	if (mPServerSocket != NULL) {
		AppendTextPanel("�̹� ������� ���� �ֽ��ϴ�!! ");
		return;
	}
	mPServerSocket = new CServerSocket;
	if (mPServerSocket->Create(port)) {
		if (!mPServerSocket->Listen()) {
			msg.Format("%d �� ���� ����!!!", port);
			AppendTextPanel(msg);
			DestroyServerSocket();
			return;
		}
		mPServerSocket->parent = this;
		msg.Format("���� �������ϴ�: %s:%d", mPServerSocket->myIP, port);
		AppendTextPanel(msg);
		return;
	}
	else {
		AppendTextPanel("���� ���� ����");
		DestroyServerSocket();
		return;
	}
}


void CChitterDlg::EnterChat(CString ip, UINT port)
{
	CString msg;
	if (port > 65535) {
		AppendTextPanel("��Ʈ��ȣ�� Ȯ�����ּ���: " + port);
		return;
	}
	if (mPClientSocket != NULL) {
		AppendTextPanel("�̹� ������ ���� �ֽ��ϴ� ");
		return;
	}
	mPClientSocket = new CConnectSocket;
	mPClientSocket->Create();
	if (mPClientSocket->Connect(ip, port) == FALSE)
	{
		AppendTextPanel("�������� ����: ");
		delete mPClientSocket;
		mPClientSocket = nullptr;
		return;
	}
	mPClientSocket->parent = this;
	msg.Format("%s:%d �� �����ϴ�...", ip, port);
	AppendTextPanel(msg);
}


void CChitterDlg::ShowHelp()
{
	AppendTextPanel("================= �� �� �� =================");
	AppendTextPanel("!���� : ������ ���ϴ�");
	AppendTextPanel("!���� <ip> <port> : ä�ù濡 ���ϴ�");
	AppendTextPanel("!�濭�� <port> : ä�ù��� ���ϴ�");
	AppendTextPanel("!���� : �������� ä�ù��� �ݽ��ϴ�");
	AppendTextPanel("!��ip : �ڽ��� ip�� Ȯ���մϴ�");
	AppendTextPanel("!������� <book> <author> [num] : ���Ⱑ���� å�� ����մϴ�");
	AppendTextPanel("!�������� <name> <book> : å�� �����մϴ�");
	AppendTextPanel("!�����ݳ� <name> <book> : å�� �ݳ��մϴ�");
	AppendTextPanel("!���嵵����� : ���Ⱑ���� å���� �����ݴϴ�");
	AppendTextPanel("!�뿩������� : �뿩���� å���� �����ݴϴ�");
	AppendTextPanel("=======================================");
	AppendTextPanel("");
}


void CChitterDlg::OnVscrollTextPanel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (bPanelScrollShowed == FALSE) {
		bPanelScrollShowed = TRUE;
		mTextPanel->ShowScrollBar(SB_VERT, TRUE);
	}
}


void CChitterDlg::OnVscrollTextInput()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (bInputScrollShowed == FALSE) {
		bInputScrollShowed = TRUE;
		mTextInput->ShowScrollBar(SB_VERT, TRUE);
	}
}


void CChitterDlg::OnUpdateTextInput()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�Ͽ�, IParam ����ũ�� OR �����Ͽ� ������ ENM_SCROLL �÷��׸� �����Ͽ� ��Ʈ�ѿ� EM_SETEVENTMASK �޽����� ������ ������
	// ���� ��Ʈ���� �ٲ� �ؽ�Ʈ�� ǥ���Ϸ��� ���� ��Ÿ���ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (bInputScrollShowed == FALSE) {
		mTextInput->ShowScrollBar(SB_VERT, FALSE);
	}
}


HBRUSH CChitterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	if (pWnd->GetDlgCtrlID() == IDC_TEXT_PANEL) {
		pDC->SetBkColor(RGB(245, 241, 222));
		hbr = mTextPanelBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TITLE_BACKGROUND) {
		pDC->SetBkColor(RGB(241, 221, 207));

	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
