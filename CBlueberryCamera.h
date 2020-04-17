#pragma once

#include "stdafx.h"

#include "CBase4618.h"

#ifdef PI4618
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#endif

class CBlueberryCamera : public CBase4618
{
    public:
        CBlueberryCamera();
        ~CBlueberryCamera();

        bool initcam(cv::Mat &frame, cv::Rect &_cropbox);
        void checkball(int &check, cv::Mat &cropframe);
        void passball(int &pass, cv::Mat &cropframe);
        void clearcheckvector();
        void clearpassvector();

    protected:
        cv::Rect cropbox;
        std::vector<std::vector<cv::Point>> checkcontours;
        std::vector<std::vector<cv::Point>> passcontours;
        int crop = 0;

        int iLowH = 0;
        int iHighH = 179;

        int iLowS = 0;
        int iHighS = 255;

        int iLowV = 0;
        int iHighV = 255;

        int bLowH = 0;
        int bHighH = 255;

        int bLowS = 0;
        int bHighS = 255;

        int bLowV = 0;
        int bHighV = 255;

    private:
};


