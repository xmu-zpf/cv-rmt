#include <iostream>
#include <string>
#include <opencv.hpp>
//#include <general.h>
#include "cvalgrm.h"
#include <HalconCpp.h>
#include <codecvt>
#include <tchar.h>

#include <HalconCpp/HalconCpp.h>
#include <HalconCpp/HDevThread.h>
#include <HalconCpp/HFramegrabber.h>

using namespace HalconCpp;

using himg = HImage;

std::string WStringToString(std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str_to(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str_to[0], size_needed, NULL, NULL);
    return str_to;
}

std::wstring StringToWString(std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr_to(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr_to[0], size_needed);
    return wstr_to;
}

std::string OpenFileDialog() {
    OPENFILENAME ofn;
    wchar_t* fileName = new wchar_t[200];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lpstrTitle = L"选择文件";
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = L"图像文件\0*.bmp;*.jpg;*.png;*.tif;*.gif;*.jpeg;*.jpe;*.jfif\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    if (GetOpenFileName(&ofn))
    {
        std::wstring wstr = fileName;
        std::string str = WStringToString(wstr);
        delete[] fileName;
        return str;
    }
    return "";
}

wchar_t* wOpenFileDialog() {
    OPENFILENAME ofn;
    wchar_t* fileName = new wchar_t[200];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lpstrTitle = L"选择文件";
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = L"图像文件\0*.bmp;*.jpg;*.png;*.tif;*.gif;*.jpeg;*.jpe;*.jfif\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    if (GetOpenFileName(&ofn))
    {
        return fileName;
    }
    return nullptr;
}

int main()
{
	//cv::Mat img = cv::imread(OpenFileDialog(), cv::IMREAD_GRAYSCALE);
	//cv::imshow("1", img);
	//cv::waitKey();

    
	//auto rslt = my::fitEllipseHalcon(img);
	//cv::Mat ellipsimg = my::drawEllipseOnImage(img, rslt);
	//cv::imshow("rslt", ellipsimg);
	//cv::waitKey();
	/*himg himg("bytr", img.cols, img.rows, img.data);
	himg.DispImage(HWindow());*/
    try
    {
        // Step 1: 读取图像
        HObject  ho_Image;
        ReadImage(&ho_Image, wOpenFileDialog());

        // Step 2: 转为灰度图像，然后进行边缘检测
        HObject ho_ImageReduced, ho_Edges, ho_Region, ho_ConnectedRegions;
        Rgb1ToGray(ho_Image, &ho_ImageReduced);
        EdgesSubPix(ho_ImageReduced, &ho_Edges, "canny", 1, 20, 40);
        GenRegionContourXld(ho_Edges, &ho_Region, "filled");

        // Step 2.5: 将边缘转为区域
        GenRegionContourXld(ho_Edges, &ho_Region, "filled");
        Connection(ho_Region, &ho_ConnectedRegions);

        // Step 3: 拟合椭圆
        HTuple hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2, hv_StartPhi, hv_EndPhi, hv_PointOrder;
        FitEllipseContourXld(ho_Edges, "algebraic", -1, 2, 0, 3, 3, 1.0, &hv_Row, &hv_Column,
            &hv_Phi, &hv_Radius1, &hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

        // Step 4: 绘制拟合的椭圆
        HObject ho_Ellipse;
        GenEllipseContourXld(&ho_Ellipse, hv_Row, hv_Column, hv_Phi, hv_Radius1, hv_Radius2, hv_StartPhi, hv_EndPhi, hv_PointOrder, 3);
        DispObj(ho_Ellipse, HDevWindowStack::GetActive());
    }
    catch (HException& HDevExpDefaultException)
    {
        // Error handling
        printf("%s\n", HDevExpDefaultException.ErrorMessage().Text());
    }
    
}
