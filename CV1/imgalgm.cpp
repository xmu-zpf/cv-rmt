#include "imgalgm.h"

namespace my {

/************************************
Í¼ÏñÏÔÊ¾£ºtcy shanghai 2021/3/17  V1.0
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