#pragma once

#include <iostream>
#include <HalconCpp.h>
#include <opencv.hpp>

namespace my {
	class hwindow {
	public:
		hwindow(const HTuple width, const HTuple height, const char* title = "window title");
		hwindow();
		hwindow(int width, int height, const char* title = "window title");
		hwindow(const HalconCpp::HObject& image, const char* title = "window title");

		void setwindowparam(const char* name, const char* param);
		template<>
		void show();
		void click();


		HTuple windowHight;
		HTuple windowWidth;
		HalconCpp::HWindow *w;

		~hwindow();
	};


	
}


