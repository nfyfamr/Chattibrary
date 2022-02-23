#pragma once
#include <map>

class Book;

class CLibraryManager
{
public:
	CLibraryManager(CDialog* parent);
	~CLibraryManager();

	CDialog* parent;
	std::multimap<std::string, Book> mBookList;
	std::multimap<std::string, Book> mRentaledList;

	virtual void RentalBook(CString who, CString bookName);
	virtual void ReturnBook(CString who, CString bookName);
	virtual void RegisterBook(CString book, CString author, UINT num);
	virtual void ShowRentarledList();
	virtual void ShowBookList();
};

