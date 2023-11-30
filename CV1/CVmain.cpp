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
    //try
    //{
        
        // Local iconic variables
        HObject  ho_OriginImg, ho_GrayImage, ho_binImg;
        HObject  ho_Contours, ho_ContEllipse;

        ReadImage(&ho_OriginImg, my::wGetFileName("Ñ¡ÔñÍ¼Ïñ", L"D:\\TestSet\\"));
        my::imshow_ha("tuxiang", ho_OriginImg);

        Rgb1ToGray(ho_OriginImg, &ho_GrayImage);

        Threshold(ho_GrayImage, &ho_binImg, 128, 255);

        GenContourRegionXld(ho_binImg, &ho_Contours, "border");

        HTuple width, height;
        HalconCpp::GetImageSize(ho_OriginImg, &width, &height);  

        //my::hwindow xldw{ ho_OriginImg ,"xld region" };
        //xldw.show(ho_Contours);
        //xldw.click();

        cv::Mat srcImage = cv::imread("D:\\TestSet\\zh\\3wbm1n1.png");
        cv::imshow("src", srcImage);
        cv::waitKey();

        std::vector<std::vector<cv::Point2f>> xldContours;
        HTuple numContours;
        CountObj(ho_Contours, &numContours);
        for (int i = 1; i <= numContours.I(); i++)
        {
            HTuple  hv_Row, hv_Column;
            HTuple  hv_Phi, hv_Radius1, hv_Radius2, hv_StartPhi, hv_EndPhi, hv_PointOrder;
            HObject singleContour;
            SelectObj(ho_Contours, &singleContour, i);     
            xldContours.push_back(my::Hxld2CVpt<cv::Point2f>(singleContour));
            /*std::cout << xldContours[i] << std::endl;*/

            //my::hwindow xldw1{ ho_OriginImg ,"xld region_current" };
            //xldw1.show(singleContour);
            //xldw1.click();
            
            HTuple row, col;
            GetContourXld(singleContour, &row, &col);
            if (col.Length() < 30)
                continue;

            auto t1_st = std::chrono::high_resolution_clock::now();
            cv::RotatedRect ellipse1 = cv::fitEllipse(xldContours[i-1]);
            auto t1_ed = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = t1_ed - t1_st;
            std::cout << "\ncall from cv:" << duration << std::endl;
            cv::ellipse(srcImage, ellipse1, cv::Scalar(0, 255, 0), 1, cv::LineTypes::LINE_AA);
            //cv::imshow("cv_rslt", srcImage);
            //cv::waitKey();

            auto t1_st2 = std::chrono::high_resolution_clock::now();
            FitEllipseContourXld(singleContour, "fitzgibbon", -1, 2, 0, 200, 4, 2, &hv_Row, &hv_Column,
                &hv_Phi, &hv_Radius1, &hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
            auto t1_ed2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2 = t1_ed2 - t1_st2;
            std::cout << "call from halcon:" << duration2 << std::endl;
            GenEllipseContourXld(&ho_ContEllipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2,
                hv_StartPhi, hv_EndPhi, hv_PointOrder, 0.7);
            //my::hwindow ellipsew{ ho_OriginImg ,"hal_rslt" };
            //ellipsew.show(ho_ContEllipse);
            //ellipsew.click();

            for (const auto& iter : my::Hxld2CVpt<cv::Point2f>(ho_ContEllipse))
            {
                srcImage.at<cv::Vec3b>(iter) = cv::Vec3b{ 0,0,255 };
            }
            cv::imshow("cv_hal_rslt", srcImage);
            //cv::waitKey();
        }

        // Find the largest contour
        //auto largestContour = xldContours[0];
        //for (const auto& iter: xldContours)
        //{
        //    if (iter.size()>largestContour.size())
        //        largestContour = iter;
        //}

        std::cout << xldContours.size() << std::endl;
        //unsigned char B = 255, G = 0, R = 255;
        //for(const auto& iter:xldContours)
        //{
        //    auto t1_st = std::chrono::high_resolution_clock::now();
        //    cv::RotatedRect ellipse = cv::fitEllipse(iter);
        //    auto t1_ed = std::chrono::high_resolution_clock::now();
        //    std::chrono::duration<double> duration = t1_ed - t1_st;
        //    //std::cout << "\ncall from cv:" << duration << std::endl;
        //    cv::ellipse(srcImage, ellipse, cv::Scalar(B,G,R), 2, cv::LineTypes::LINE_AA);
        //    cv::imshow("CV rslt", srcImage);
        //    cv::waitKey();
        //    G += 40, R -= 30, B -= 30;
        //}

        //auto t1_st = std::chrono::high_resolution_clock::now();
        //cv::RotatedRect ellipse1 = cv::fitEllipse(xldContours[0]);
        //auto t1_ed = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<double> duration = t1_ed - t1_st;
        //std::cout << "\ncall from cv:" << duration << std::endl;
        //cv::ellipse(srcImage, ellipse1, cv::Scalar(0, 255, 0), 2, cv::LineTypes::LINE_AA);
        //cv::imshow("cv_rslt", srcImage);
        //cv::waitKey();

        //auto t1_st2 = std::chrono::high_resolution_clock::now();
        //FitEllipseContourXld(ho_Contours[1], "fitzgibbon", -1, 2, 0, 200, 4, 2, &hv_Row, &hv_Column,
        //    &hv_Phi, &hv_Radius1, &hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
        //auto t1_ed2 = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<double> duration2 = t1_ed2 - t1_st2;
        //std::cout << "\ncall from halcon:" << duration2 << std::endl;
        //GenEllipseContourXld(&ho_ContEllipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2,
        //    hv_StartPhi, hv_EndPhi, hv_PointOrder, 0.7);
        //my::hwindow ellipsew{ ho_OriginImg ,"hal_rslt" };
        //ellipsew.show(ho_ContEllipse);
        //ellipsew.click();

        //cv::Mat frmH(srcImage.rows, srcImage.cols, CV_8UC3, cv::Scalar(0, 0, 0));
        //std::vector<cv::Point2f> xldEllipsecont = my::Hxld2CVpt<cv::Point2f>(ho_ContEllipse);
        //for (const auto& iter : my::Hxld2CVpt<cv::Point2f>(ho_ContEllipse))
        //{
        //    srcImage.at<cv::Vec3b>(iter) = cv::Vec3b{ 0,0,255 };
        //}
        cv::destroyAllWindows();
        cv::imshow("cv_hal_rslt", srcImage);
        cv::waitKey();
        
        cv::imwrite("D:\\TestSet\\SPCrslt\\Out_cv+hal_resualt_2.png", srcImage, my::CVPNG_NO_COMPRESSION);


        //imshow_ha("jieguo", ho_ContEllipse);
   // }
    //catch (HException& HDevExpDefaultException)
    //{
    //    // Error handling
    //    printf("%s\n", HDevExpDefaultException.ErrorMessage().Text());
    //}
    
}
