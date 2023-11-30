#pragma once

#include <iostream>
#include <HalconCpp.h>
#include <opencv.hpp>

using namespace HalconCpp;

namespace my {

	const std::vector<int> CVPNG_NO_COMPRESSION{ cv::IMWRITE_PNG_COMPRESSION ,0 };

	void imshow_ha(std::string winname, const HalconCpp::HObject& image);
	void imshow_ha(std::string winname, const HalconCpp::HImage& image);

	class hwindow {
	public:
		hwindow(const HTuple wwidth, const HTuple wheight, const char* title = "window title");
		hwindow();
		//hwindow(int width, int height, const char* title = "window title");
		hwindow(HalconCpp::HObject& image, const char* title = "window title");

		void setwindowparam(const char* name, const char* param);
		void show(const HalconCpp::HObject& obj) const;
		void show() const;
		void click() const;
		void clearwindow();

		HTuple windowHight;
		HTuple windowWidth;
		HalconCpp::HWindow *w;
		HObject* object{ nullptr };

		~hwindow();
	};

}


