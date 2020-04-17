////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated March 23, 2017
////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>
#include <iomanip>

#include "Client.h"
#include "server.h"
#include "CAsteroidGame.h"
#include "CPong.h"
#include "CSketch.h"
#include "CBase4618.h"

#ifdef WIN4618
#include "CControl.h"
#include "Serial.h"
#include "opencv.hpp"
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")
#endif
#ifdef PI4618
#include "CControlPi.h"
#include "CBlueberrySort.h"
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#include <pigpio.h>
#include "unistd.h"
#include <sys/ioctl.h>
#include <termios.h>
#endif

#define SERVO1 5
#define SERVO2 6

#ifdef PI4618
bool kbhit();
#endif

int main(int argc, char* argv[])
{

	char mm_input;
	int exit = 0;

	void lab4();
	void lab5();
	void lab6();
	#ifdef PI4618
	void lab7();
	void lab8();
	#endif
	void lab9();


	do
	{
		std::cout << std::endl << "Enter the lab you would like to run or enter 'q' to exit" << std::endl << "Lab: ";
		std::cin >> mm_input;

		switch (mm_input)
		{
		case '1':
			break;

		case '2':
			break;

		case '3':
			break;

		case '4':
			lab4();
			break;

		case '5':
			lab5();
			break;

		case '6':
			lab6();
			break;

		case '7':
			#ifdef PI4618
			lab7();
			#endif
			break;

		case '8':
			#ifdef PI4618
			lab8();
			#endif
			break;

		case '9':
			lab9();
			break;

		case 'q':
			exit = 1;
			break;

		default:
			std::cout << std::endl << "Error, please try again" << std::endl << "Lab: ";
		}
	} while (exit == 0);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
	cv::Mat im;

	im = cv::imread("BCIT.jpg");

	srand(time(0));

	for (int i = 0; i < 500; i++)
	{
		float radius = 50 * rand() / RAND_MAX;
		cv::Point center = cv::Point(im.size().width*rand() / RAND_MAX, im.size().height*rand() / RAND_MAX);

		cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, 8);

		im.at<char>(i, i) = 255;

		cv::imshow("Image", im);
		cv::waitKey(1);
	}
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
	cv::VideoCapture vid;

	vid.open(0);

	if (vid.isOpened() == 1)
	{
		do
		{
			cv::Mat frame, edges;
			vid >> frame;
			if (frame.empty() == 0)
			{
				cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
				cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
				cv::Canny(edges, edges, 0, 30, 3);
				cv::imshow("edges", edges);
			}
		} while (cv::waitKey(10) != ' ');
	}
}

////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
Server serv;

// Start TCP server
void serverfunc()
{
	serv.start(4618);
}

// Send image to TCP server
void serverimagefunc()
{
	cv::VideoCapture vid;

	vid.open(0);

	if (vid.isOpened() == TRUE)
	{
		do
		{
			cv::Mat frame;
			vid >> frame;
			serv.set_txim(frame);
		} while (cv::waitKey(10) != ' ');
	}
}

void clientserver()
{
	std::string str;
	cv::Mat im;

	// Start server
	std::thread t1(&serverfunc);
	t1.detach();

	// Start image send to server
	std::thread t2(&serverimagefunc);
	t2.detach();

	// Wait until server starts up (webcam is slow)
	Sleep(2000);

	// connect
	Client client(4618, "127.0.0.1");
	//Client client(4618, "192.168.1.80");

	// Wait until server starts up (webcam is slow)
	Sleep(500);

	client.tx_str("cmd");
	do
	{
		client.rx_str(str);
		if (str.length() > 0)
		{
			std::cout << "\nClient Rx: " << str;
		}
	} while (str.length() == 0);

	int count = 0;
	while (1)
	{
		client.tx_str("im");

		if (client.rx_im(im) == TRUE)
		{
			if (im.empty() == false)
			{
				count++;
				std::cout << "\nImage received: " << count;
				cv::imshow("rx", im);
				cv::waitKey(100);
			}
		}
	}
}

void print_menu(void)
{
	std::cout << std::setfill('/') << std::setw(81) << "\n\n";
	std::cout << "ELEX4618 COM Port Test Menu, by John Matson\n\n";
	std::cout << std::setfill('/') << std::setw(81) << "\n\n";
	std::cout << "(A)nalog test" << std::endl;
	std::cout << "(D)igital test" << std::endl;
	std::cout << "(B)utton test" << std::endl;
	std::cout << "(S)ervo test" << std::endl;
	std::cout << "(Q)uit\n\n";
	std::cout << "CMD> ";
}

void lab4()
{
	int portnum = 4;
	#ifdef WIN4618
	std::cout << std::endl << "Enter the COM port number you'd like to use (port 4 recommended)" << std::endl << "Port: ";
	std::cin >> portnum;
	#endif
	cv::Size defaultSize(500, 500);
	CSketch _lab4(defaultSize, portnum);
	_lab4.run();
}

void lab5()
{
	int portnum = 4;
	#ifdef WIN4618
	std::cout << std::endl << "Enter the COM port number you'd like to use (port 4 recommended)" << std::endl << "Port: ";
	std::cin >> portnum;
	#endif
	cv::Size defaultSize(1000, 800);
	CPong _lab5(defaultSize, portnum);
	_lab5.run();
}

void lab6()
{
	int portnum = 4;
	#ifdef WIN4618
	std::cout << std::endl << "Enter the COM port number you'd like to use (port 4 recommended)" << std::endl << "Port: ";
	std::cin >> portnum;
	#endif
	cv::Size defaultSize(800, 800);
	CAsteroidGame _lab6(defaultSize, portnum);
	_lab6.run();
}

#ifdef PI4618
void lab7()
{
    char sm_input;
	std::cout << std::endl << "(a)nalog test" << std::endl;
	std::cout << "(d)igital test" << std::endl;
	std::cout << "(b)utton test" << std::endl;
	std::cout << "(s)ervo test" << std::endl;
	std::cout << "(q)uit\n\n";
	std::cout << "CMD> ";
	std::cin >> sm_input;

	CControlPi test;
	int exit = 0;
	int test_exit = 0;

	do
	{
        switch (sm_input)
        {

        case 'a':
        {
            do
            {
                int j1, j2;
                test.get_data(1, 15, j1);
                test.get_data(1, 9, j2);

                std::cout << std::endl << "joystick x: " << j1 << "\tjoystick y: " << j2;
                if (kbhit())
                    test_exit = 1;
            } while (test_exit != 1);
            break;
        }

        case 'd':
        {
            do
            {
                int b1, b2;
                test.get_data(0, 1, b1);
                test.get_data(0, 2, b2);

                test.set_data(0, 39, !b1);

                std::cout << std::endl << "button 1: " << b1 << "\tbutton 2: " << b2;
                if (kbhit())
                    test_exit = 1;
            } while (test_exit != 1);
            break;
        }

        case 'b':
        {
            int _count = 0;
            double start_tic = cv::getTickCount();

            do
            {
                int b1;
                test.get_data(0, 1, b1);

                if ((b1 == 0) && (((cv::getTickCount() - start_tic) / cv::getTickFrequency()) > 0.5))
                {
                    start_tic = cv::getTickCount();
                    _count++;
                }

                std::cout << std::endl << "count: " << _count;
                if (kbhit())
                    test_exit = 1;
            } while (test_exit != 1);
            break;
        }

        case 's':
        {
            do
            {
                gpioServo(SERVO1, 500); sleep(1); gpioServo(SERVO1, 2500); sleep (1);
                if (kbhit())
                    test_exit = 1;
            } while (test_exit != 1);
            break;
        }

        case 'q':
            exit = 1;
            break;

        default:
            std::cout << std::endl << "Error, please try again" << std::endl << "CMD> ";
        }
    } while (exit != 1);
}
#endif

#ifdef PI4618
void lab8()
{
    CBlueberrySort _lab8;
    _lab8.run();

    /*CControlPi test;
    gpioServo(SERVO1, 1400); // 1000->1400
    gpioServo(SERVO2, 1800); // 1000<->1800
    sleep(1);*/

	/*int exit = 0;
	do
	{
		cv::VideoCapture vid;

		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point>> contours;

		cv::Mat frame, hsv, bluerange, pinkrange, greenrange, whiterange, contourframe;
		cv::Scalar lowblue = cv::Scalar(80, 70, 160), highblue = cv::Scalar(120, 255, 255);
		int blueballs = 0, pinkballs = 0, greenballs = 0, whiteballs = 0;

		vid.open(0);

		if (vid.isOpened() == 1)
		{
			do
			{
				int blueballs = 0;
				vid >> frame;
				if (frame.empty() == 0)
				{
					cv::imshow("frame", frame);
					cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
					//cv::imshow("hsv", hsv);

					cv::inRange(hsv, lowblue, highblue, bluerange);
					cv::imshow("bluerange", bluerange);
					cv::erode(bluerange, bluerange, cv::Mat(), cv::Point(-1, -1), 2);
					cv::dilate(bluerange, bluerange, cv::Mat(), cv::Point(-1, -1), 2);
					cv::imshow("dilate/erode", bluerange);

					cv::findContours(bluerange, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
					for (unsigned int i = 0; i < contours.size(); i++)
					{
						if ((contours[i].size() > 100) && (contours[i].size() < 250))
						{
							cv::Rect r = cv::boundingRect(contours.at(i));
							cv::drawContours(frame, contours, i, cv::Scalar(255, 0, 0), 4, 8, hierarchy);
							blueballs++;
						}
					}

					cv::imshow("contours", frame);
					std::cout << std::endl << "blue: " << blueballs << "\t\tpink: " << pinkballs << "\t\tgreen: " << greenballs << "\twhite: " << whiteballs;
				}
			} while (cv::waitKey(10) != ' ');
		}
	} while (exit != 1);*/
}
#endif

#ifdef PI4618
bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}
#endif

void lab9()
{
	std::string str;
	cv::Mat im;
	/*
	// Start server
	std::thread t1(&serverfunc);
	t1.detach();

	// Start image send to server
	std::thread t2(&serverimagefunc);
	t2.detach();

	// Wait until server starts up (webcam is slow)
	Sleep(2000);
	*/
	// connect
	//Client client(4618, "127.0.0.1");
	//Client client(4618, "192.168.0.27");
	Client client(4618, "10.0.1.2");

	// Wait until server starts up (webcam is slow)
	Sleep(500);
	
	client.tx_str("cmd");
	do
	{
		client.rx_str(str);
		if (str.length() > 0)
		{
			std::cout << "\nClient Rx: " << str;
		}
	} while (str.length() == 0);

	int count = 0;
	int keycmd;
	while (1)
	{
		client.tx_str("im");

		if (client.rx_im(im) == TRUE)
		{
			if (im.empty() == false)
			{
				count++;
				std::cout << "\nImage received: " << count;
				cv::imshow("rx", im);
				cv::waitKey(100);
			}
		}
		else
			std::cout << "no image\n";

		keycmd = cv::waitKey(100);
		switch (keycmd)
		{
		case 's':
			client.tx_str("S 0 1\n");
			std::cout << keycmd;
			break;

		case 'p':
			client.tx_str("S 1 1\n");
			std::cout << keycmd;
			break;

		case 'r':
			client.tx_str("S 1 0\n");
			std::cout << keycmd;
			break;

		case 'c':
			client.tx_str("S 0 0\n");
			std::cout << keycmd;
			break;

		default:
			break;
		}
	}
}
