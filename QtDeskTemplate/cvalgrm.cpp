#include "cvalgrm.h"

namespace my {
    cv::RotatedRect fitEllipseHalcon(cv::Mat& grayImage) 
    {
        // Thresholding the gray image
        cv::Mat binaryImage;
        cv::threshold(grayImage, binaryImage, 0, 255, cv::THRESH_BINARY);
        // Find contours in the binary image
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        // Find the largest contour
        //size_t largestContourIndex = 0;
        //double largestContourArea = 0.0;
        //for (size_t i = 0; i < contours.size(); i++)
        //{
        //    double contourArea = cv::contourArea(contours[i]);
        //    if (contourArea > largestContourArea)
        //    {
        //        largestContourArea = contourArea;
        //        largestContourIndex = i;
        //    }
        //}

        std::vector<cv::Point> PTST;
        for (auto& iter : contours)
        {
            PTST.insert(PTST.end(), iter.begin(), iter.end());
        }

        // Fit ellipse using the largest contour
        cv::RotatedRect ellipse = cv::fitEllipse(PTST);

        return ellipse;
    }

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

