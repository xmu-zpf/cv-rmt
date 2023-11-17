#include <general.h>

namespace my {
	void TcharToChar(const TCHAR* tchar, char* _char)
	{
		int iLength;

		iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
	}

	void CharToTchar(const char* _char, TCHAR* tchar)
	{
		int iLength;

		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
	}

	std::string Wchar_tToString(wchar_t* wchar)
	{
		wchar_t* wText = wchar;
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
		char* psText = new char[dwNum]; // psText为char*的临时数组，作为赋值给std::string的中间变量
		WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
		std::string szDst = psText;// std::string赋值
		delete[] psText;// psText的清除
		
		return szDst;
	}

	LPCWSTR CharToLPCWSTR(const char* charStr)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, charStr, -1, NULL, 0);
		wchar_t* wideStr = new wchar_t[size];
		MultiByteToWideChar(CP_UTF8, 0, charStr, -1, wideStr, size);
		LPCWSTR lpcWideStr = wideStr;
		delete[] wideStr;

		return lpcWideStr;
	}

	HWND _hwndf;
	std::string GetFileName(const char* title, const char* defualtPath)
	{
		TCHAR* szBuffer = new TCHAR[200](0);
		TCHAR* wtitle = new TCHAR[200](0);
		CharToTchar(title, wtitle);
		OPENFILENAME filepath{ 0 };
		filepath.lStructSize = sizeof(filepath);
		filepath.hwndOwner = _hwndf;
		filepath.lpstrFilter = _T("jpg文件(*.jpg)\0*.jpg\0png文件(*.png)\0*.png\0所有文件(*.*)\0*.*\0");
		filepath.lpstrInitialDir = CharToLPCWSTR(defualtPath);//默认的文件路径 
		filepath.lpstrFile = szBuffer;//存放文件的缓冲区 
		filepath.nMaxFile = 200;
		filepath.nFilterIndex = 0;
		filepath.lpstrTitle = wtitle;
		filepath.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
		BOOL bSel = GetOpenFileName(&filepath);

		std::string filename = Wchar_tToString(szBuffer);
		for (auto& i : filename)
		{
			if (i == '\\')
				i = '/';
		}

		delete[] szBuffer;
		szBuffer = nullptr;
		return filename;
	}
}
