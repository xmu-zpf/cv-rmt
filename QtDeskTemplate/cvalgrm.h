#pragma once

#include <opencv.hpp>

using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

namespace my {

	cv::RotatedRect fitEllipseHalcon(cv::Mat& grayImage);
	cv::Mat drawEllipseOnImage(cv::Mat& image, cv::RotatedRect& ellipse);

	template<typename T = u32>
	class Point {
	public:
		Point() = default;
		Point(T X) {};
		T x{ 0 };
		T y{ 0 };
	};
	
}