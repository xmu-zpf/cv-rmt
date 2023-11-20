#pragma once

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <cwchar>



#ifdef _ASDLL
#ifdef GENERALDLL_EXPORTS

#define DLLAPI _declspec(dllexport)
#else 
#define DLLAPI _declspec(dllimport)
#endif
#else
#define DLLAPI NOTDLL
#define NOTDLL
#endif // _ASDLL


namespace my {
	void DLLAPI TcharToChar(const TCHAR* tchar, char* _char);
	void DLLAPI CharToTchar(const char* _char, TCHAR* tchar);
	std::string DLLAPI Wchar_tToString(wchar_t* wchar);
	LPCWSTR DLLAPI CharToLPCWSTR(const char* charStr);

	std::string DLLAPI GetFileName(const char* title, const wchar_t* defualtPath = L"C:\\");
	wchar_t* wGetFileName(const char* title, const wchar_t* defualtPath = L"D:\\");
}

