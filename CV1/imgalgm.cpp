#include "imgalgm.h"

using namespace HalconCpp;

namespace my {
	hwindow::hwindow(const HTuple width, const HTuple height, const char* title)
	{
		w = new HWindow{ 0,0,width,height };
		w->SetWindowParam("window_title", title);
	}
	hwindow::hwindow()
		:hwindow(0, 0)
	{
	}
	hwindow::hwindow(int width, int height, const char* title)
		:hwindow(static_cast< HTuple>(width), static_cast<HTuple>(width), title)
	{
	}
	hwindow::hwindow(const HalconCpp::HObject& image, const char* title)
	{
		HTuple width, height;
		HalconCpp::GetImageSize(image, &width, &height);
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
	void hwindow::show()
	{

	}
	hwindow::~hwindow()
	{
		delete w;
	}

}