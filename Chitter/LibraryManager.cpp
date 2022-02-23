#include "stdafx.h"
#include "LibraryManager.h"
#include "ChitterDlg.h"
#include <string>

class Book {
public:
	CString name;
	CString author;
	UINT num;

	Book(CString name, CString author, UINT num = 1);
	~Book();
	friend const bool operator==(const Book& left, const std::string& right);
	friend const bool operator==(const std::string& left, const Book& right);
	friend const bool operator<(const Book& left, const Book& right);
};

Book::Book(CString name, CString author, UINT num) {
	this->name = name;
	this->author = author;
	this->num = num;
}

Book::~Book() {};

const bool operator==(const Book& left, const std::string& right)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	if (left.name.GetString() != right || left.author.GetString() != right) {
		return FALSE;
	}
	return TRUE;
}

const bool operator==(const std::string& left, const Book& right)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return right == left;
}

const bool operator<(const Book& left, const Book& right)
{
	if (left.name < right.name) {
		return TRUE;
	}
	else if (left.name == right.name) {
		if (left.author < right.author) {
			return TRUE;
		}
	}
	return FALSE;
}

CLibraryManager::CLibraryManager(CDialog* parent)
{
	this->parent = parent;
}


CLibraryManager::~CLibraryManager()
{
}


void CLibraryManager::RegisterBook(CString book, CString author, UINT num)
{
	if (book.GetLength() == 0 || author.GetLength() == 0) {
		((CChitterDlg*)parent)->AppendTextPanel("�߸��� �Է� �����Դϴ�. ������ Ȯ�����ּ���");
		return;
	}

	auto it = mBookList.find(book.GetString());
	if (it != mBookList.end() && it->second.name == book && it->second.author == author) {
		((CChitterDlg*)parent)->AppendTextPanel("�̹� ��ϵǾ��ִ� å�Դϴ�");
		return;
	}
	num = num ? num : 1;
	mBookList.insert({ book.GetString(), Book(book, author, num) });
	((CChitterDlg*)parent)->AppendTextPanel("��ϵǾ����ϴ�");
}


void CLibraryManager::RentalBook(CString who, CString bookName)
{
	CString msg;
	auto it1 = mRentaledList.find(who.GetString());
	if ( it1 != mRentaledList.end()) {
		msg.Format("%s ���� �̹� %s�� �뿩���̽ʴϴ�", who, it1->second.name);
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}

	auto it2 = mBookList.find(bookName.GetString());
	if (it2 == mBookList.end()) {
		msg.Format("%s �� ��ϵ��� ���� �����Դϴ�", bookName);
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	} else if (it2->second.num < 1 ) {
		((CChitterDlg*)parent)->AppendTextPanel("���� �������Դϴ�");
		return;
	}

	--it2->second.num;
	mRentaledList.insert({ who.GetString(), it2->second });
	msg.Format("%s ���� %s �� �����Ͽ����ϴ�", who, bookName);
	((CChitterDlg*)parent)->AppendTextPanel(msg);
	return;
}


void CLibraryManager::ReturnBook(CString who, CString bookName)
{
	CString msg;
	auto it1 = mBookList.find(bookName.GetString());
	if (it1 == mBookList.end()) {
		msg = bookName + " �� �������� �ʴ� �����Դϴ�";
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}

	auto it2 = mRentaledList.find(who.GetString());
	if (it2 == mRentaledList.end()) {
		msg = who + " ���� ������ �����Ͻ� ����� �����ϴ�";
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}
	else if (it2->second.name != bookName) {
		msg = bookName + " �� �����Ͻ� å�� �ƴմϴ�";
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}

	++it1->second.num;
	mRentaledList.erase(who.GetString());
	((CChitterDlg*)parent)->AppendTextPanel("�ݳ��� �Ϸ�Ǿ����ϴ�");
}


void CLibraryManager::ShowRentarledList()
{
	CString msg, tmp;
	msg = "�̸�\t\t������ :\r\n";
	msg += "=======================================\r\n";
	for (auto it = mRentaledList.begin(); it != mRentaledList.end(); ++it) {
		tmp.Format("%s\t\t%s\r\n", it->first.data(), it->second.name);
		msg += tmp;
	}
	msg += "=======================================\r\n";
	((CChitterDlg*)parent)->AppendTextPanel(msg);
}


void CLibraryManager::ShowBookList()
{
	CString msg, tmp;
	msg = "������\t\t����\t\t�뿩���ɱǼ� :\r\n";
	msg += "=======================================\r\n";
	for (auto it = mBookList.begin(); it != mBookList.end(); ++it) {
		tmp.Format("%s\t\t%s\t\t%d\r\n", it->second.name, it->second.author, it->second.num);
		msg += tmp;
	}
	msg += "=======================================\r\n";
	((CChitterDlg*)parent)->AppendTextPanel(msg);
}
