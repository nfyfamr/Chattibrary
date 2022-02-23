
// ChitterDlg.h : ��� ����
//

#pragma once
#include <afxsock.h>
#include <map>
#include <string>
#include "ServerSocket.h"
#include "ConnectSocket.h"
#include "LibraryManager.h"


// CChitterDlg ��ȭ ����
class CChitterDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CChitterDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	CFont mFont;
	CSliderCtrl* mCtrSlider;
	int mTransparent;
	CEdit* mTextInput;
	CEdit* mTextPanel;
	CBrush mTextPanelBrush;

	enum Command { ����, ����, �濭��, ����, ��ip, �������, ��������, �����ݳ�, ���嵵�����, �뿩������� };
	std::map<CString, Command> cstr2Enum;

	CServerSocket* mPServerSocket;
	CConnectSocket* mPClientSocket;

	CLibraryManager mLibMgr;


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHITTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSend();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void ProcessingText(CString string);
public:

	virtual void AppendTextPanel(CString str)
	{
		int nLen = mTextPanel->GetWindowTextLength();
		mTextPanel->SetSel(nLen, nLen);
		mTextPanel->ReplaceSel(str + "\r\n");
	}
	afx_msg void OnDestroy();
	virtual void DestroyServerSocket();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void CloseChat();
	virtual void OpenChat(UINT port);
	virtual void EnterChat(CString ip, UINT port);
	virtual void ShowHelp();
	afx_msg void OnVscrollTextPanel();

	boolean bPanelScrollShowed;
	boolean bInputScrollShowed;
	afx_msg void OnVscrollTextInput();
	afx_msg void OnUpdateTextInput();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
