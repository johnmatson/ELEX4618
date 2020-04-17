#pragma once

#include "stdafx.h"

#define WIN4618
//#define PI4618

#ifdef WIN4618
#include "CControl.h"
#endif
#ifdef PI4618
#include "CControlPi.h"
#endif

/**
*
* @brief This class contains common variables and methods for 4618 labs.
*
* @author John Matson
*
*/

class CBase4618
{
protected:
#ifdef WIN4618
	CControl elex4618control; ///< object of CControl
#endif
#ifdef PI4618
	CControlPi elex4618control;
#endif
	cv::Mat _canvas; ///< matrix for displaying images
	char exit;
	int cheight;
	int cwidth;
	double start_tic = cv::getTickCount();
	double elapsed_time;
	cv::Scalar white = cv::Scalar(255, 255, 255);

	CBase4618();
	void run();
	void update();
	void draw();
};
