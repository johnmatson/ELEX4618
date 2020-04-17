#include "CBlueberryCamera.h"
#include "unistd.h"

#ifdef PI4618
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#endif

//#define CONFIG
//#define CONFIG1
//#define CONFIG2
#define RANGE 50
#define BLK 25
#define HB_LOW 90
#define SB_LOW 90
#define VB_LOW 110
#define HB_HIGH 135
#define SB_HIGH 255
#define VB_HIGH 255
#define HO_LOW 0
#define SO_LOW 0
#define VO_LOW 90
#define HO_HIGH 179
#define SO_HIGH 245
#define VO_HIGH 255

CBlueberryCamera::CBlueberryCamera()
{
    #ifdef CONFIG
    cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    cvCreateTrackbar("BlueLowH", "Control", &bLowH, 255); //Hue (0 - 255)
    cvCreateTrackbar("BlueHighH", "Control", &bHighH, 255);

    cvCreateTrackbar("BlueLowS", "Control", &bLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("BlueHighS", "Control", &bHighS, 255);

    cvCreateTrackbar("BlueLowV", "Control", &bLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("BlueHighV", "Control", &bHighV, 255);
    #endif
}

CBlueberryCamera::~CBlueberryCamera()
{

}

bool CBlueberryCamera::initcam(cv::Mat &frame, cv::Rect &_cropbox)
{
    if (frame.empty() == 0)
    {
        cv::Mat gray, thresh;
        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point>> contours;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, thresh, 230, 255, cv::THRESH_BINARY);
        cv::erode(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 5);
        //cv::imshow("first thresh", thresh);
        //cv::waitKey(1);
        cv::findContours(thresh, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (int iterate = 0; iterate < 3; iterate++)
        {
            for (unsigned int i = 0; i < contours.size(); i++)
            {
                if (contours[i].size() > 120)
                {
                    cropbox = cv::boundingRect(contours.at(i));
                    _cropbox = cropbox;
                    //std::cout << "\ncrop";
                    crop = 1;
                    iterate = 3;
                    i = contours.size();

                    //cv::Mat crop = frame(cropbox);
                    //cv::imshow("crop", crop);
                }
            }
        }
    }
    if (crop == 1)
        return 0;
    else
        return 1;
}

void CBlueberryCamera::checkball(int &check, cv::Mat &cropframe)
{
	std::vector<cv::Vec4i> hierarchy;

	cv::Mat gray, thresh, range, contourframe, hsv;
	cv::Scalar lowbox = cv::Scalar(0, 30, 60), highbox = cv::Scalar(255, 255, 250);
	int balls = 0;

    if (cropframe.empty() == 0)
    {
        cv::cvtColor(cropframe, hsv, cv::COLOR_BGR2HSV);
        cv::waitKey(1);

        #ifdef CONFIG1
        cv::inRange(hsv, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), range);
        #else
        cv::inRange(hsv, lowbox, highbox, range);
        #endif

        //cv::imshow("range", range);
        cv::waitKey(1);
        cv::erode(range, range, cv::Mat(), cv::Point(-1, -1), 1);
        cv::dilate(range, range, cv::Mat(), cv::Point(-1, -1), 1);
        cv::imshow("checkball", range);

        cv::findContours(range, checkcontours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (unsigned int i = 0; i < checkcontours.size(); i++)
        {
            if ((checkcontours[i].size() > 110) && (checkcontours[i].size() < 200))
            {
                //cv::Rect r = cv::boundingRect(checkcontours.at(i));
                //cv::drawContours(frame, checkcontours, i, cv::Scalar(255, 0, 0), 4, 8, hierarchy);
                balls = 1;
                i = checkcontours.size();
            }
        }
    }

    if (balls == 1)
        check = 1;
    else
        check = 0;
}

void CBlueberryCamera::passball(int &pass, cv::Mat &cropframe)
{
		std::vector<cv::Vec4i> hierarchy;

		cv::Mat frame, hsv, bluerange, contourframe;
		cv::Scalar lowblue = cv::Scalar(80, 70, 30), highblue = cv::Scalar(120, 255, 255);
		int blueballs = 0;

            if (cropframe.empty() == 0)
            {
                //cv::imshow("frame", frame);
                cv::cvtColor(cropframe, hsv, cv::COLOR_BGR2HSV);
                //cv::imshow("hsv", hsv);

                #ifdef CONFIG2
                cv::inRange(hsv, cv::Scalar(bLowH, bLowS, bLowV), cv::Scalar(bHighH, bHighS, bHighV), bluerange);
                #else
                cv::inRange(hsv, lowblue, highblue, bluerange);
                #endif

                //cv::imshow("bluerange", bluerange);
                cv::erode(bluerange, bluerange, cv::Mat(), cv::Point(-1, -1), 2);
                cv::dilate(bluerange, bluerange, cv::Mat(), cv::Point(-1, -1), 6);
                cv::imshow("passball", bluerange);

                cv::findContours(bluerange, passcontours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
                for (unsigned int i = 0; i < passcontours.size(); i++)
                {
                    if ((passcontours[i].size() > 70) /*&& (passcontours[i].size() < 250)*/)
                    {
                        //cv::Rect r = cv::boundingRect(passcontours.at(i));
                        //cv::drawContours(frame, passcontours, i, cv::Scalar(255, 0, 0), 4, 8, hierarchy);
                        blueballs = 1;
                    }
                }
            }

        if (blueballs == 1)
            pass = 1;
        else
            pass = 0;
}

void CBlueberryCamera::clearcheckvector()
{
    checkcontours.clear();
}

void CBlueberryCamera::clearpassvector()
{
    passcontours.clear();
}

