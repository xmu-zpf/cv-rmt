#pragma once

#include <iostream>
#include <HalconCpp.h>
#include <opencv.hpp>

using namespace HalconCpp;

namespace my {

	template<typename T>
	concept PointCVpt2f = std::is_same_v<T, cv::Point2f>;
	template<typename T>
	concept PointCVpt2i = std::is_same_v<T, cv::Point2i>;

	const std::vector<int> CVPNG_NO_COMPRESSION{ cv::IMWRITE_PNG_COMPRESSION ,0 };

	void imshow_ha(std::string winname, const HalconCpp::HObject& image);
	void imshow_ha(std::string winname, const HalconCpp::HImage& image);
	wchar_t* wGetFileName(const char* title, const wchar_t* defualtPath);
	template<typename T> requires PointCVpt2f<T>|| PointCVpt2i<T>
	auto Hxld2CVpt(const HObject & Hxld) -> std::vector<T>;

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

	template<typename T> requires PointCVpt2f<T> || PointCVpt2i<T>
	auto Hxld2CVpt(const HObject& Hxld) ->std::vector<T>
	{
		std::vector<T> cvContour;
		HTuple HxldROW, HxldCOL;
		GetContourXld(Hxld, &HxldROW, &HxldCOL);
		int num_points = HxldROW.Length();
		cvContour.reserve(num_points);
		for (int j = 0; j < num_points; j++)
		{
			cvContour.push_back(cv::Point2f(HxldCOL[j].D(), HxldROW[j].D()));
		}
		return cvContour;
	}

}


