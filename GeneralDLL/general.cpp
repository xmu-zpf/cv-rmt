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
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
		char* psText = new char[dwNum]; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
		WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
		std::string szDst = psText;// std::string��ֵ
		delete[] psText;// psText�����
		
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
		filepath.lpstrFilter = _T("jpg�ļ�(*.jpg)\0*.jpg\0png�ļ�(*.png)\0*.png\0�����ļ�(*.*)\0*.*\0");
		filepath.lpstrInitialDir = CharToLPCWSTR(defualtPath);//Ĭ�ϵ��ļ�·�� 
		filepath.lpstrFile = szBuffer;//����ļ��Ļ����� 
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
