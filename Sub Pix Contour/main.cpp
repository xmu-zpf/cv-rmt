#include "edgeTest.h"
#include "WriteFile.h"
#include <general.h>
using namespace std;
using namespace cv;

namespace my {
	cv::Mat drawEllipseOnImage(cv::Mat& image, cv::RotatedRect& ellipse)
	{
		// Convert the image to color (3 channels)
		cv::Mat imageColor;
		cv::cvtColor(image, imageColor, cv::COLOR_GRAY2BGR);

		// Draw the ellipse on the color image
		cv::ellipse(imageColor, ellipse, cv::Scalar(0, 255, 0), 2);

		return imageColor;
	}
}


int main()
{
	Mat srcImage, grayImage, dstImage;
	//srcImage = imread(my::GetFileName("Ñ¡ÔñÎÄ¼þ", L"D:\\TestSet\\"));
	srcImage = imread("D:\\TestSet\\zh\\2wbn1.png");
	if (srcImage.empty())
	{
		cout << "load error!" << endl;
		return -1;
	}

	//parameters setting
	double * x;          /* x[n] y[n] coordinates of result contour point n */
	double * y;
	int * curve_limits;  /* limits of the curves in the x[] and y[] */
	int N, M;         /* result: N contour points, forming M curves */
	double S = 1.5; /* default sigma=0 */
	double H = 4.2; /* default th_h=0  */
	double L = 0.81; /* default th_l=0  */
	double W = 1.0; /* default W=1.3   */
	char pdf_out[]( "D:\\TestSet\\SPCrslt\\Out.pdf" );  /*pdf filename*/
	char txt_out[]{ "D:\\TestSet\\SPCrslt\\Out.txt" };

	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	dstImage = grayImage;
	const int iHeight = dstImage.rows;
	const int iWidth = dstImage.cols;
	uchar* pSrc = grayImage.data;//new uchar[iHeight*iWidth];
	uchar* pDst = dstImage.data;

	//imshow("input image", grayImage);
	ndevernay(&x, &y, &N, &curve_limits, &M, pSrc, pDst, iWidth, iHeight, S, H, L);
	if (pdf_out != NULL) write_curves_pdf(x, y, curve_limits, M, pdf_out, iWidth, iHeight, W);
	if (txt_out != NULL) write_curves_txt(x, y, curve_limits, M, txt_out);

	std::vector<std::vector<cv::Point2f >> cvContours;
	for (int k = 0; k < M; k++)
	{
		std::vector<cv::Point2f > cvContour;
		for (int i = curve_limits[k]; i < curve_limits[k + 1]; i++)
			cvContour.push_back(cv::Point2f(x[i], y[i]));
		cvContours.push_back(cvContour);
	}

#ifdef _CONSOLE
	auto t1_st = std::chrono::high_resolution_clock::now();
#endif // _CONSOLE
	cv::RotatedRect ellipse = cv::fitEllipse(cvContours[3]);
#ifdef _CONSOLE
	auto t1_ed = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = t1_ed - t1_st;
	std::cout << "\ncall from algm:" << duration << std::endl;
#endif // _CONSOLE

	cv::ellipse(srcImage, ellipse, cv::Scalar(0, 255, 0), 2, cv::LineTypes::LINE_AA);

	imshow("rslt", srcImage);
	waitKey();

	vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);  
	compression_params.push_back(0); 
	cv::imwrite("D:\\TestSet\\SPCrslt\\Out.png", srcImage, compression_params);

	return 0;
}