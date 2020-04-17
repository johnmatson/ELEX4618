#include "stdafx.h"

#include "CSketch.h"
#ifdef WIN4618
#include "CControl.h"
#endif
#ifdef PI4618
#include "CControlPi.h"
#endif

CSketch::CSketch(cv::Size sketchSize, int portNum)
{
	elex4618control.init_com(portNum);
	_canvas = cv::Mat::zeros(sketchSize, CV_8UC3);
}

CSketch::~CSketch()
{
	cvDestroyAllWindows();
}

void CSketch::run()
{
	char exit;
	do
	{
		CSketch::update();
		CSketch::draw();
		exit = cv::waitKey(1);
	} while (exit != 'q');
}

void CSketch::update()
{
	int x, y;
	float fx, fy;
	static cv::Point point1;
	static cv::Point point2;

	static int runonce;
	if (!runonce)
	{
		elex4618control.get_data(1, 15, x);
		elex4618control.get_data(1, 9, y);

		fx = x;
		fy = y;

		fx = (((fx / 1024) * 1000) - 400);
		fy = ((((1024 - fy) / 1024) * 800) - 150);

		if (fx < 0)
			fx = 0;
		if (fx > 500)
			fx = 500;

		if (fy < 0)
			fy = 0;
		if (fy > 500)
			fy = 500;

		x = (int)fx;
		y = (int)fy;

		point2 = cv::Point(x, y);

		runonce = 1;
	}

	point1 = point2;

	elex4618control.get_data(1, 15, x);
	elex4618control.get_data(1, 9, y);

	fx = x;
	fy = y;

	fx = (((fx / 1024) * 1000) - 400);
	fy = ((((1024 - fy) / 1024) * 800) - 150);

	if (fx < 0)
		fx = 0;
	if (fx > 500)
		fx = 500;

	if (fy < 0)
		fy = 0;
	if (fy > 500)
		fy = 500;

	x = (int)fx;
	y = (int)fy;

	point2 = cv::Point(x, y);
	std::cout << point2 << "	";

	int reset;
	cv::Size defaultSize(500, 500);
	elex4618control.get_button(1, reset);
	std::cout << reset << "	";

	if (reset == 0)
		_canvas = cv::Mat::zeros(defaultSize, CV_8UC3);

	cv::Scalar colourscale;
	enum bgr {blue, green, red};
	static int colour;
	static int colourset = blue;
	elex4618control.get_button(2, colour);
	if (colour == 0)
		colourset++;
	if (colourset > red)
		colourset = blue;

	switch (colourset)
	{
	case blue:
		colourscale = cv::Scalar(255, 0, 0);
		elex4618control.set_data(0, 37, 1);
		elex4618control.set_data(0, 38, 0);
		elex4618control.set_data(0, 39, 0);
		break;

	case green:
		colourscale = cv::Scalar(0, 255, 0);
		elex4618control.set_data(0, 37, 0);
		elex4618control.set_data(0, 38, 1);
		elex4618control.set_data(0, 39, 0);
		break;

	case red:
		colourscale = cv::Scalar(0, 0, 255);
		elex4618control.set_data(0, 37, 0);
		elex4618control.set_data(0, 38, 0);
		elex4618control.set_data(0, 39, 1);
		break;
	}

	std::cout << colour << std::endl;

	cv::line(_canvas, point1, point2, colourscale, 5, 8, 0);
}

void CSketch::draw()
{
	cv::imshow("canvas", _canvas);
}
