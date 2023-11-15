#include <iostream>
#include <opencv.hpp>
#include <general.h>
#include "cvalgrm.h"
#include <HalconCpp.h>

using namespace HalconCpp;

using himg = HImage;

int main()
{
	cv::Mat img = cv::imread(my::GetFileName(), cv::IMREAD_GRAYSCALE);
	/*cv::imshow("1", img);
	cv::waitKey();*/

	//auto rslt = my::fitEllipseHalcon(img);
	//cv::Mat ellipsimg = my::drawEllipseOnImage(img, rslt);
	//cv::imshow("rslt", ellipsimg);
	//cv::waitKey();
	himg himg("bytr", img.cols, img.rows, img.data);
	himg.DispImage(HWindow());

}
