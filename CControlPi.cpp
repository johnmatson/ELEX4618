#include "stdafx.h"

#include "CControlPi.h"
#include <pigpio.h>
#include <opencv2/opencv.hpp>

/*
// red LED - gpio 2
// external green LED - gpio 23
// external red LED - gpio 24
// push button 1 - gpio 3
// push button 2 - gpio 4
// push button 3 - gpio 18
// servo 1 - gpio 5
// servo 2 - gpio
*/

#define CHANNEL1 3
#define CHANNEL2 4
#define CHANNEL3 18
#define CHANNEL9
#define CHANNEL15
#define CHANNEL37
#define CHANNEL38
#define CHANNEL39 2
#define SERVO1 5
#define SERVO2 6

CControlPi::CControlPi()
{
    gpioInitialise();
    gpioSetMode(CHANNEL1, PI_INPUT);
    gpioSetMode(CHANNEL2, PI_INPUT);
    gpioSetMode(CHANNEL39, PI_OUTPUT);
    gpioSetMode(SERVO1, PI_OUTPUT);
    gpioSetMode(SERVO2, PI_OUTPUT);
}


CControlPi::~CControlPi()
{
    gpioTerminate();
}


void CControlPi::init_com(int comport)
{

}


bool CControlPi::get_data(int type, int channel, int &result)
{
    switch (type)
    {
    case 0:
    {
        if (channel == 1)
            result = gpioRead(CHANNEL1);
        else if (channel == 2)
            result = gpioRead(CHANNEL2);
        else if (channel == 3)
            result = gpioRead(CHANNEL3);
        return 1;
        break;
    }

    case 1:
    {
        unsigned char inBuf[3];

        if (channel == 15)
        {
            char cmd[] = { 1, 128, 0 };
            int handle = spiOpen(0, 200000, 3);
            spiXfer(handle, cmd, (char*)inBuf, 3);
            result = ((inBuf[1] & 3) << 8) | inBuf[2];
            spiClose(handle);
        }

        else if (channel == 9)
        {
            char cmd[] = { 1, 144, 0 };
            int handle = spiOpen(0, 200000, 3);
            spiXfer(handle, cmd, (char*)inBuf, 3);
            result = ((inBuf[1] & 3) << 8) | inBuf[2];
            spiClose(handle);
        }

        return 1;
        break;
    }

    case 2:
    {
        return 1;
        break;
    }

    default:
        return 0;
    }
}


bool CControlPi::set_data(int type, int channel, int val)
{
    switch (type)
    {
    case 0:
        if (channel == 39)
            gpioWrite(CHANNEL39, val);
        if (channel == 23)
            gpioWrite(23, val);
        if (channel == 24)
            gpioWrite(24, val);
        return 1;
        break;

    case 1:
        return 1;
        break;

    case 2:
        return 1;
        break;

    default:
        return 0;
    }
}


void CControlPi::get_analog(float &outputA, float &outputB)
{
	int temp1, temp2;

	get_data(1, 9, temp1); //get y data
	outputB = temp1;
	outputB = (outputB / 1024); //give a percentage

	get_data(1, 15, temp2); //get x data
	outputA = temp2;
	outputA = (outputA / 1024); //give a percentage
}


void CControlPi::scale_analog(float &outputA, float &outputB)
{

}

//FIX THIS
void CControlPi::get_button(int button, int &_result)
{
    int button_first = 1;
    int result, tracker;

    get_data(0, button, result);

    if (button_first == result)
    {
        tracker = 1;
    }
    else
    {
        tracker =0;
        button_first = result;
    }
    if ((button_first == 0) && (tracker == 0))
    {
        _result = 0;
        cv::waitKey(70);
        tracker = 1;
    }
}
