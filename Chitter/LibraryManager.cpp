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
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (left.name.GetString() != right || left.author.GetString() != right) {
		return FALSE;
	}
	return TRUE;
}

const bool operator==(const std::string& left, const Book& right)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
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
		((CChitterDlg*)parent)->AppendTextPanel("잘못된 입력 형식입니다. 도움말을 확인해주세요");
		return;
	}

	auto it = mBookList.find(book.GetString());
	if (it != mBookList.end() && it->second.name == book && it->second.author == author) {
		((CChitterDlg*)parent)->AppendTextPanel("이미 등록되어있는 책입니다");
		return;
	}
	num = num ? num : 1;
	mBookList.insert({ book.GetString(), Book(book, author, num) });
	((CChitterDlg*)parent)->AppendTextPanel("등록되었습니다");
}


void CLibraryManager::RentalBook(CString who, CString bookName)
{
	CString msg;
	auto it1 = mRentaledList.find(who.GetString());
	if ( it1 != mRentaledList.end()) {
		msg.Format("%s 님은 이미 %s을 대여중이십니다", who, it1->second.name);
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}

	auto it2 = mBookList.find(bookName.GetString());
	if (it2 == mBookList.end()) {
		msg.Format("%s 은 등록되지 않은 도서입니다", bookName);
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	} else if (it2->second.num < 1 ) {
		((CChitterDlg*)parent)->AppendTextPanel("전권 대출중입니다");
		return;
	}

	--it2->second.num;
	mRentaledList.insert({ who.GetString(), it2->second });
	msg.Format("%s 께서 %s 을 대출하였습니다", who, bookName);
	((CChitterDlg*)parent)->AppendTextPanel(msg);
	return;
}


void CLibraryManager::ReturnBook(CString who, CString bookName)
{
	CString msg;
	auto it1 = mBookList.find(bookName.GetString());
	if (it1 == mBookList.end()) {
		msg = bookName + " 은 소장하지 않는 도서입니다";
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}

	auto it2 = mRentaledList.find(who.GetString());
	if (it2 == mRentaledList.end()) {
		msg = who + " 님은 도서를 대출하신 기록이 없습니다";
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}
	else if (it2->second.name != bookName) {
		msg = bookName + " 은 대출하신 책이 아닙니다";
		((CChitterDlg*)parent)->AppendTextPanel(msg);
		return;
	}

	++it1->second.num;
	mRentaledList.erase(who.GetString());
	((CChitterDlg*)parent)->AppendTextPanel("반납이 완료되었습니다");
}


void CLibraryManager::ShowRentarledList()
{
	CString msg, tmp;
	msg = "이름\t\t도서명 :\r\n";
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
	msg = "도서명\t\t저자\t\t대여가능권수 :\r\n";
	msg += "=======================================\r\n";
	for (auto it = mBookList.begin(); it != mBookList.end(); ++it) {
		tmp.Format("%s\t\t%s\t\t%d\r\n", it->second.name, it->second.author, it->second.num);
		msg += tmp;
	}
	msg += "=======================================\r\n";
	((CChitterDlg*)parent)->AppendTextPanel(msg);
}
