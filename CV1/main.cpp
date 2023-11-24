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
	w.ClearWindow();                                  \
}

void imshow_ha(std::string winname, const HalconCpp::HObject& image) {
    __imshow_ha__(winname, image);
}
void imshow_ha(std::string winname, const HalconCpp::HImage& image) {
    __imshow_ha__(winname, image);
}

int main()
{
	//cv::Mat img = cv::imread("D:\\TestSet\\zh\\2wbn1.png", cv::IMREAD_GRAYSCALE | CV_32F);
	//cv::imshow("1", img);
	//cv::waitKey();

    //std::vector<std::vector<cv::Point2d>> contours;
    //cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //int maxindex = 0;
    //for (int i = 0; i < contours.size(); ++i)
    //{
    //    if (contours[i].size() > contours[maxindex].size())
    //        maxindex = i;
    //}
    //auto& mcontour = contours[maxindex];

    //auto ellipseRRTC =  my::fitEllipseHalcon(img);
    //cv::Mat RsltImg = my::drawEllipseOnImage(img, ellipseRRTC);

 //   auto t1_st = std::chrono::high_resolution_clock::now();
 //   auto rslt = my::fitEllipseHalcon(img);
 //   auto t1_ed = std::chrono::high_resolution_clock::now();
 //   std::chrono::duration<double> duration = t1_ed - t1_st;
 //   std::cout << duration << std::endl;
	//cv::Mat ellipsimg = my::drawEllipseOnImage(img, rslt);
	//cv::imshow("rslt", RsltImg);
	//cv::waitKey();
	//Himg himg("bytr", img.cols, img.rows, img.data);
    //try
    //{
    //    Himg himg;
    //    HTuple HWind;
    //    HTuple  Width, Height;
    //    himg.ReadImage("D:\\TestSet\\zh\\2wbn1.png");
    //    OpenWindow(10, 10, Width, Height, 0, "visible", "", &HWind);
    //    HalconCpp::DispImage(himg, HWind);
    //}


//    catch (HException& HDevExpDefaultException)
//{
//    // Error handling
//    printf("%s\n", HDevExpDefaultException.ErrorMessage().Text());
//}
    try
    {
        
        // Local iconic variables
        HObject  ho_OriginImg, ho_GrayImage, ho_binImg;
        HObject  ho_Contours, ho_ContEllipse;

        // Local control variables
        HTuple  hv_SlctdImgPath, hv_t1, hv_Row, hv_Column;
        HTuple  hv_Phi, hv_Radius1, hv_Radius2, hv_StartPhi, hv_EndPhi;
        HTuple  hv_PointOrder, hv_t2, hv_elapsed;

        // dev_open_file_dialog(...); only in hdevelop
        ReadImage(&ho_OriginImg, wGetFileName("选择图像", L"D:\\TestSet\\"));
        imshow_ha("tuxiang", ho_OriginImg);

        Rgb1ToGray(ho_OriginImg, &ho_GrayImage);

        Threshold(ho_GrayImage, &ho_binImg, 128, 255);

        GenContourRegionXld(ho_binImg, &ho_Contours, "border");

        auto t1_st = std::chrono::high_resolution_clock::now();
        FitEllipseContourXld(ho_Contours, "fitzgibbon", -1, 2, 0, 200, 3, 2, &hv_Row, &hv_Column,
            &hv_Phi, &hv_Radius1, &hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
        auto t1_ed = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = t1_ed - t1_st;
        std::cout << "\ncall from algm:" << duration << std::endl;

        GenEllipseContourXld(&ho_ContEllipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2,
            hv_StartPhi, hv_EndPhi, hv_PointOrder, 1.5);

        //imshow_ha("jieguo", ho_ContEllipse);

       /* if (HDevWindowStack::IsOpen())
            SetColor(HDevWindowStack::GetActive(), "green");*/

        //// Step 4: 绘制拟合的椭圆
        //HObject ho_Ellipse;
        //GenEllipseContourXld(&ho_ContEllipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2,
        //    hv_StartPhi, hv_EndPhi, "positive", 1.5);
        //GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
        //SetWindowAttr("background_color", "black");
        //OpenWindow(10, 10, hv_Width, hv_Height, 0, "visible", "", &hv_WindowHandle);
        //HDevWindowStack::Push(hv_WindowHandle);
        //if (HDevWindowStack::IsOpen())
        //    DispObj(ho_SelectedXLD1, HDevWindowStack::GetActive());

        //SetWindowAttr("background_color", "black");
        //OpenWindow(10, 10, hv_Width, hv_Height, 0, "visible", "", &hv_WindowHandle1);
        //HDevWindowStack::Push(hv_WindowHandle1);
        //if (HDevWindowStack::IsOpen())
        //    DispObj(ho_ContEllipse, HDevWindowStack::GetActive());
    }
    catch (HException& HDevExpDefaultException)
    {
        // Error handling
        printf("%s\n", HDevExpDefaultException.ErrorMessage().Text());
    }
    
}
