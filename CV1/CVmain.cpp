#include <iostream>
#include <string>
#include <opencv.hpp>
#include "general.h"
#include "cvalgrm.h"
#include <HalconCpp.h>
#include <codecvt>
#include <tchar.h>
#include <chrono>
#include "imgalgm.h"

#include <windows.h>

using namespace HalconCpp;

using Himg = HImage;

wchar_t* wGetFileName(const char* title, const wchar_t* defualtPath)
{
    OPENFILENAME ofn;
    wchar_t* fileName = new wchar_t[200];
    wchar_t* wtitle = new TCHAR[200](0);
    my::CharToTchar(title, wtitle);
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lpstrTitle = L"ѡ���ļ�";
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = L"ͼ���ļ�\0*.bmp;*.jpg;*.png;*.tif;*.gif;*.jpeg;*.jpe;*.jfif\0All Files\0*.*\0";
    ofn.lpstrInitialDir = defualtPath;//Ĭ�ϵ��ļ�·�� 
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

cv::Mat HImageToMat(const HalconCpp::HImage& hImg) {
    cv::Mat mat;
    int channels = hImg.CountChannels()[0].I();
    HalconCpp::HImage hImage = hImg.ConvertImageType("byte");

    Hlong hW = 0, hH = 0; HString cType;

    if (channels == 1) {
        void* r = hImage.GetImagePointer1(&cType, &hW, &hH);
        mat.create(int(hH), int(hW), CV_8UC1);
        memcpy(mat.data, static_cast<unsigned char*>(r), int(hW * hH));
    }
    else if (channels == 3) { 
        void* r = NULL, * g = NULL, * b = NULL;

        hImage.GetImagePointer3(&r, &g, &b, &cType, &hW, &hH);
        mat.create(int(hH), int(hW), CV_8UC3);

        std::vector<cv::Mat> vec(3);
        vec[0].create(int(hH), int(hW), CV_8UC1);
        vec[1].create(int(hH), int(hW), CV_8UC1);
        vec[2].create(int(hH), int(hW), CV_8UC1);

        memcpy(vec[2].data, static_cast<unsigned char*>(r), int(hW * hH));
        memcpy(vec[1].data, static_cast<unsigned char*>(g), int(hW * hH));
        memcpy(vec[0].data, static_cast<unsigned char*>(b), int(hW * hH));
        cv::merge(vec, mat);
    }
    return mat;
}

cv::Mat HObjectToMat(const HalconCpp::HObject& hObj) {
    HalconCpp::HImage hImg(hObj);
    return HImageToMat(hImg);
}

cv::Scalar randomColor(void)
{
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    cv::RNG rng(sys.wMilliseconds);
    int icolor = (unsigned)rng;
    return cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
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
        HImage img;

        // Local control variables
        HTuple  hv_SlctdImgPath, hv_t1, hv_Row, hv_Column;
        HTuple  hv_Phi, hv_Radius1, hv_Radius2, hv_StartPhi, hv_EndPhi;
        HTuple  hv_PointOrder, hv_t2, hv_elapsed;

        // dev_open_file_dialog(...); only in hdevelop
        ReadImage(&ho_OriginImg, wGetFileName("ѡ��ͼ��", L"D:\\TestSet\\"));
        my::imshow_ha("tuxiang", ho_OriginImg);

        Rgb1ToGray(ho_OriginImg, &ho_GrayImage);

        Threshold(ho_GrayImage, &ho_binImg, 128, 255);

        GenContourRegionXld(ho_binImg, &ho_Contours, "border");

        HTuple width, height;
        HalconCpp::GetImageSize(ho_OriginImg, &width, &height);  

        my::hwindow xldw{ ho_OriginImg ,"xld region" };
        xldw.show(ho_Contours);
        xldw.click();

        my::hwindow xldw1{ ho_OriginImg ,"xld region_1" };
        xldw1.show(ho_Contours[1]);
        xldw1.click();

        std::vector<std::vector<cv::Point2f>> xldContours;
        HTuple numContours;
        CountObj(ho_Contours, &numContours);
        for (int i = 1; i < numContours.I(); i++)
        {
            std::vector<cv::Point2f> Contour;
            HObject singleContour;
            SelectObj(ho_Contours, &singleContour, i);
            HTuple row, column;
            GetContourXld(singleContour, &row, &column);
            int num_points = row.Length();
            Contour.reserve(num_points);

            // ��Halcon�ĵ�����ת��Ϊcv::Point2f
            for (int j = 0; j < num_points; j++) {
                Contour.push_back(cv::Point2f(column[j].D(), row[j].D()));
            }
            xldContours.push_back(Contour);
            /*std::cout << Contour << std::endl;*/
        }

        // Find the largest contour
        //auto largestContour = xldContours[0];
        //for (const auto& iter: xldContours)
        //{
        //    if (iter.size()>largestContour.size())
        //        largestContour = iter;
        //}

        cv::Mat srcImage = cv::imread("D:\\TestSet\\zh\\3wbm1n1.png");
        cv::imshow("src", srcImage);
        cv::waitKey();

        std::cout << xldContours.size() << std::endl;
        unsigned char B = 255, G = 0, R = 255;
        for(const auto& iter:xldContours)
        {
            auto t1_st = std::chrono::high_resolution_clock::now();
            cv::RotatedRect ellipse = cv::fitEllipse(iter);
            auto t1_ed = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = t1_ed - t1_st;
            //std::cout << "\ncall from cv:" << duration << std::endl;
            cv::ellipse(srcImage, ellipse, cv::Scalar(B,G,R), 2, cv::LineTypes::LINE_AA);
            cv::imshow("CV rslt", srcImage);
            //cv::waitKey();
            G += 40, R -= 30, B -= 30;
        }


        cv::imwrite("D:\\TestSet\\SPCrslt\\Out_xld2.png", srcImage, my::CVPNG_NO_COMPRESSION);

        auto t1_st2 = std::chrono::high_resolution_clock::now();
        FitEllipseContourXld(ho_Contours[1], "fitzgibbon", -1, 2, 0, 200, 4, 2, &hv_Row, &hv_Column,
            &hv_Phi, &hv_Radius1, &hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
        auto t1_ed2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration2 = t1_ed2 - t1_st2;
        std::cout << "\ncall from halcon:" << duration2 << std::endl;


        GenEllipseContourXld(&ho_ContEllipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2,
            hv_StartPhi, hv_EndPhi, hv_PointOrder, 1.5);

        my::hwindow ellipsew{ ho_OriginImg ,"hal_rslt" };
        ellipsew.show(ho_ContEllipse);
        ellipsew.click();

        //imshow_ha("jieguo", ho_ContEllipse);

       /* if (HDevWindowStack::IsOpen())
            SetColor(HDevWindowStack::GetActive(), "green");*/

        //// Step 4: ������ϵ���Բ
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
