#include "stdafx.h"

#include "CBlueberryHardware.h"
#include "unistd.h"

#ifdef PI4618
#include <pigpio.h>
#endif

#define SERVO1 5
#define SERVO2 6

CBlueberryHardware::CBlueberryHardware()
{

}

CBlueberryHardware::~CBlueberryHardware()
{

}

void CBlueberryHardware::passblueberry()
{
    gpioServo(SERVO2, 1100);
    gpioServo(SERVO1, 1500);
    usleep(180000);
    gpioServo(SERVO1, 1000);
    //usleep(300000);
}

void CBlueberryHardware::rejectblueberry()
{
    gpioServo(SERVO2, 1800);
    gpioServo(SERVO1, 1500);
    usleep(180000);
    gpioServo(SERVO1, 1000);
    //usleep(300000);
}
