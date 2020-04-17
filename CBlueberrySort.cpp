#include "CBlueberrySort.h"
#include "unistd.h"

#ifdef PI4618
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#endif

CBlueberrySort::CBlueberrySort()
{

}

CBlueberrySort::~CBlueberrySort()
{

}

void CBlueberrySort::run()
{
    vid.open(0);

    if (vid.isOpened() == 1)
    {
        vid >> frame;
        exit = camera.initcam(frame, cropbox);
        if (exit == 1)
            std::cout << std::endl << "init failed";
    }

    while (exit != 1)
    {
        update();
        draw();
	}
}

void CBlueberrySort::update()
{
    vid >> frame;
    vid >> frame;
    vid >> frame;
    cropframe = frame(cropbox);

    int b1, b2, b3;
    elex4618control.get_data(0, 1, b1);
    elex4618control.get_data(0, 2, b2);
    elex4618control.get_data(0, 3, b3);

    char input = cv::waitKey(1);
    if (((input == 's') || (b3 == 0)) && (on == 0))
    {
        on = 1;
        elex4618control.set_data(0, 39, 1);
    }
    else if (((input == 's') || (b3 == 0)) && (on == 1))
    {
        on = 0;
        elex4618control.set_data(0, 39, 0);
        //passind = 0;
        //rejectind = 0;
    }
    if ((input == 'p') || (b1 == 0))
    {
        pass = 1;
        passind = 1;
        rejectind = 0;
    }
    if ((input == 'r') || (b2 == 0))
    {
        reject = 1;
        rejectind = 1;
        passind = 0;
    }
    if (input == 'c')
        berrycount = 0;
    if (input == 'q')
        exit = 1;
}

void CBlueberrySort::draw()
{
    cv::Point countpos = cv::Point(10, 20);
    cv::putText(cropframe, "Count: " + std::to_string(berrycount), countpos, 1, 1, cv::Scalar(255, 0, 0), 1, 8, false);

    std::string status1;
    if (on == 1)
        status1 = "AUTO";
    else
        status1 = "MANUAL";
    cv::Point statuspos1 = cv::Point(10, 40);
    cv::putText(cropframe, status1, statuspos1, 1, 1, cv::Scalar(0, 0, 255), 1, 8, false);

    std::string status2;
    if (passind == 1)
    {
        status2 = "PASS";
        elex4618control.set_data(0, 23, 1);
        elex4618control.set_data(0, 24, 0);
    }
    else if (rejectind == 1)
    {
        status2 = "REJECT";
        elex4618control.set_data(0, 24, 1);
         elex4618control.set_data(0, 23, 0);
    }
    else
    {
        status2 = "";
        elex4618control.set_data(0, 23, 0);
        elex4618control.set_data(0, 24, 0);
    }
    cv::Point statuspos2 = cv::Point(10, 60);
    cv::putText(cropframe, status2, statuspos2, 1, 1, cv::Scalar(0, 255, 0), 1, 8, false);

    cv::imshow("crop", cropframe);

    if (on == 1)
    {
        camera.checkball(checkflag, cropframe);
        if (checkflag == 1)
        {
            int passcount = 0;
            int rejectcount = 0;
            /*for (int count = 0; count < 7; count++)
            {
                int flag = 0;
                camera.passball(flag, cropframe);
                if (flag == 1)
                    passcount++;
                else
                    rejectcount++;
                if (passcount >= 4)
                {
                    passflag = 1;
                    count = 7;
                }
                else if (rejectcount >= 4)
                {
                    passflag = 0;
                    count = 7;
                }
            }*/ camera.passball(passflag, cropframe);

            if(passflag == 1)
            {
                std::cout << std::endl << "pass";
                //camera.clearcheckvector();
                hardware.passblueberry();
                //usleep(700000);
                berrycount++;
            }
            else
            {
                std::cout << std::endl << "reject";
                //camera.clearpassvector();
                hardware.rejectblueberry();
                //usleep(700000);
            }
        }
    }
    else
    {
        if(pass == 1)
        {
            hardware.passblueberry();
            std::cout << std::endl << "pass";
            berrycount++;
            pass = 0;
        }
        else if (reject == 1)
        {
            hardware.rejectblueberry();
            std::cout << std::endl << "reject";
            reject = 0;
        }
    }
}
