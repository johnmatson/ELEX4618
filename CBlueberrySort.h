#pragma once

#include "stdafx.h"

#include "CBase4618.h"
#include "CBlueberryHardware.h"
#include "CBlueberryCamera.h"

#ifdef PI4618
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#endif

class CBlueberrySort : public CBase4618
{
    public:
        CBlueberrySort();
        ~CBlueberrySort();

        void run();
        void draw();
        void update();

    protected:
        CBlueberryHardware hardware;
        CBlueberryCamera camera;
        int checkflag = 0;
        int passflag = 0;
        int pass = 0;
        int reject = 0;
        int on = 0;
        int berrycount = 0;
        int passind = 0;
        int rejectind = 0;

        cv::VideoCapture vid;
        cv::Mat frame;
        cv::Mat cropframe;
        cv::Rect cropbox;

    private:
};


