#include "imgalgm.h"

namespace my {

/************************************
图像显示：tcy shanghai 2021/3/17  V1.0
*************************************/
#define __imshow_ha__(winname,image)                  \
{                                                     \
	HTuple width, height;                             \
	                                                  \
	HalconCpp::GetImageSize(image, &width, &height);  \
	HWindow w(0, 0, width, height);                   \
	w.SetWindowParam("window_title", winname.c_str());\
	                                                  \
	image.DispObj(w);                                 \
	w.Click();                                        \
	/*w.ClearWindow();*/                                  \
}

	void imshow_ha(std::string winname, const HalconCpp::HObject& image)
	{
		__imshow_ha__(winname, image);
	}
	void imshow_ha(std::string winname, const HalconCpp::HImage& image) 
	{
		__imshow_ha__(winname, image);
	}

	void CharToTchar(const char* _char, TCHAR* tchar)
	{
		int iLength;

		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
	}

	wchar_t* wGetFileName(const char* title, const wchar_t* defualtPath)
	{
		OPENFILENAME ofn;
		wchar_t* fileName = new wchar_t[200];
		wchar_t* wtitle = new TCHAR[200](0);
		CharToTchar(title, wtitle);
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lpstrTitle = L"选择文件";
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFilter = L"图像文件\0*.bmp;*.jpg;*.png;*.tif;*.gif;*.jpeg;*.jpe;*.jfif\0All Files\0*.*\0";
		ofn.lpstrInitialDir = defualtPath;//默认的文件路径 
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = wtitle;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT;
		if (GetOpenFileName(&ofn))
		{
			return fileName;
		}
		return nullptr;
	}

	hwindow::hwindow(const HTuple width, const HTuple height, const char* title)
	{
		w = new HWindow{ 0,0,width,height };
		w->SetWindowParam("window_title", title);
	}
	hwindow::hwindow()
		:hwindow(0, 0)
	{
	}
	//hwindow::hwindow(int width, int height, const char* title)
	//	:hwindow(static_cast< HTuple>(width), static_cast<HTuple>(width), title)
	//{
	//}
	hwindow::hwindow(HalconCpp::HObject& image, const char* title)
	{
		HTuple width, height;
		HalconCpp::GetImageSize(image, &width, &height);
		object = &image;
		w = new HWindow{ 0, 0, width, height };
		w->SetWindowParam("window_title", title);
	}
	void hwindow::setwindowparam(const char* name, const char* param)
	{
		if (w != nullptr)
		{
			w->SetWindowParam(name, param);
		}
	}
	void hwindow::show(const HalconCpp::HObject& obj) const
	{
		w->DispObj(obj);
	}
	void hwindow::show() const
	{
		w->DispObj(*object);
	}
	void hwindow::click() const
	{
		w->Click();
	}
	void hwindow::clearwindow()
	{
		w->ClearWindow();
	}
	hwindow::~hwindow()
	{
		delete w;
	}

}