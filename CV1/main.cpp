#include <iostream>
#include <string>
#include <opencv.hpp>
#include "general.h"
#include "cvalgrm.h"
#include <HalconCpp.h>
#include <codecvt>
#include <tchar.h>
#include <chrono>

using namespace HalconCpp;

using Himg = HImage;

wchar_t* wGetFileName(const char* title, const wchar_t* defualtPath)
{
    OPENFILENAME ofn;
    wchar_t* fileName = new wchar_t[200];
    wchar_t* wtitle = new TCHAR[200](0);
    my::CharToTchar(title, wtitle);
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

int main()
{
	cv::Mat img = cv::imread(my::GetFileName("选择一个",L"D:\\TestSet\\"), cv::IMREAD_GRAYSCALE | CV_32F);
	cv::imshow("1", img);
	cv::waitKey();

    //std::vector<std::vector<cv::Point2d>> contours;
    //cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //int maxindex = 0;
    //for (int i = 0; i < contours.size(); ++i)
    //{
    //    if (contours[i].size() > contours[maxindex].size())
    //        maxindex = i;
    //}
    //auto& mcontour = contours[maxindex];

    auto ellipseRRTC =  my::fitEllipseHalcon(img);
    cv::Mat RsltImg = my::drawEllipseOnImage(img, ellipseRRTC);

 //   auto t1_st = std::chrono::high_resolution_clock::now();
 //   auto rslt = my::fitEllipseHalcon(img);
 //   auto t1_ed = std::chrono::high_resolution_clock::now();
 //   std::chrono::duration<double> duration = t1_ed - t1_st;
 //   std::cout << duration << std::endl;
	//cv::Mat ellipsimg = my::drawEllipseOnImage(img, rslt);
	cv::imshow("rslt", RsltImg);
	cv::waitKey();
	/*himg himg("bytr", img.cols, img.rows, img.data);*/
//    try
//    {
//        Himg himg;
//        HTuple HWind;
//        himg.ReadImage(wGetFileName("选择图像", L"D:\\TestSet\\"));
//        HalconCpp::DispImage(himg, HWind);
//    }
//
//
//    catch (HException& HDevExpDefaultException)
//{
//    // Error handling
//    printf("%s\n", HDevExpDefaultException.ErrorMessage().Text());
//}
    //try
    //{
    //    HObject  ho_Image, ho_GrayImage, ho_Edges, ho_SelectedXLD;
    //    HObject  ho_ContoursSplit, ho_SelectedXLD1, ho_ContEllipse;
    //    HTuple  hv_Row, hv_Column, hv_Phi, hv_Radius1;
    //    HTuple  hv_Radius2, hv_StartPhi, hv_EndPhi, hv_PointOrder;
    //    HTuple  hv_Width, hv_Height, hv_WindowHandle, hv_WindowHandle1;

    //    ReadImage(&ho_Image, wOpenFileDialog());
    //    Rgb1ToGray(ho_Image, &ho_GrayImage);
    //    EdgesSubPix(ho_GrayImage, &ho_Edges, "canny", 1, 5, 10);
    //    //亚像素边缘
    //    SelectShapeXld(ho_Edges, &ho_SelectedXLD, "contlength", "and", 367, 369);
    //    SegmentContoursXld(ho_SelectedXLD, &ho_ContoursSplit, "lines_ellipses", 5, 4, 2);
    //    SelectShapeXld(ho_ContoursSplit, &ho_SelectedXLD1, "contlength", "and", 72, 77);

    //    FitEllipseContourXld(ho_SelectedXLD1, "fitzgibbon", -1, 0, 0, 200, 3, 2, &hv_Row,
    //        &hv_Column, &hv_Phi, &hv_Radius1, &hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

    //    // Step 4: 绘制拟合的椭圆
    //    HObject ho_Ellipse;
    //    GenEllipseContourXld(&ho_ContEllipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2,
    //        hv_StartPhi, hv_EndPhi, "positive", 1.5);
    //    GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
    //    SetWindowAttr("background_color", "black");
    //    OpenWindow(10, 10, hv_Width, hv_Height, 0, "visible", "", &hv_WindowHandle);
    //    HDevWindowStack::Push(hv_WindowHandle);
    //    if (HDevWindowStack::IsOpen())
    //        DispObj(ho_SelectedXLD1, HDevWindowStack::GetActive());

    //    SetWindowAttr("background_color", "black");
    //    OpenWindow(10, 10, hv_Width, hv_Height, 0, "visible", "", &hv_WindowHandle1);
    //    HDevWindowStack::Push(hv_WindowHandle1);
    //    if (HDevWindowStack::IsOpen())
    //        DispObj(ho_ContEllipse, HDevWindowStack::GetActive());
    //}
    //catch (HException& HDevExpDefaultException)
    //{
    //    // Error handling
    //    printf("%s\n", HDevExpDefaultException.ErrorMessage().Text());
    //}
    
}
