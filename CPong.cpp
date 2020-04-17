#include "stdafx.h"

#include "CPong.h"

#ifdef WIN4618
#include "CControl.h"
#endif
#ifdef PI4618
#include "CControlPi.h"
#include <opencv2/opencv.hpp>
#endif

#define BALL_RADIUS 25
#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 125
#define FRAME_RATE 30
#define VELOCITY 200

CPong::CPong(cv::Size sketchSize, int portNum)
{
	elex4618control.init_com(portNum);
	_canvas = cv::Mat::zeros(sketchSize, CV_8UC3);

	//get size of canvas
	cheight = _canvas.rows;
	cwidth = _canvas.cols;
}

CPong::~CPong()
{
	cvDestroyAllWindows();
}

void CPong::run()
{
	do
	{
		CPong::update();
		CPong::draw();
		exit = cv::waitKey(1);
	} while (exit != 'q');
}

void CPong::update()
{
	//reset canvas to black
	_canvas = cv::Mat::zeros(_canvas.size(), CV_8UC3);

	//variables for the ball and the user paddle
	float upaddle;
	int cpaddle;
	float dnull;
	int inull;
	int ball_x;
	int ball_y;
	int reset = 1;
	int button = 1;

	//define object and text points
	static cv::Point ball;
	static cv::Point center = cv::Point((cwidth / 2), (cheight / 2));
	static cv::Point frtext = cv::Point((cwidth - 150), 40);
	static cv::Point ustext = cv::Point(((cwidth / 2) - 120), 40);
	static cv::Point cstext = cv::Point(((cwidth / 2) + 20), 40);

	//define scalar for the colour white
	cv::Scalar white = cv::Scalar(255, 255, 255);

	//assign value of the pot to the user paddle
	elex4618control.get_analog(dnull, upaddle);
	upaddle = cheight - (cheight * upaddle);

	//assign value of ball position to computer paddle
	cpaddle = ball.y;

	//checks to see if user or computer paddle is outside the playable area and corrects
	if (upaddle < (0 + (PADDLE_HEIGHT / 2)))
		upaddle = (PADDLE_HEIGHT / 2);
	if (upaddle > (cheight - (PADDLE_HEIGHT / 2)))
		upaddle = (cheight - (PADDLE_HEIGHT / 2));
	if (cpaddle < (0 + (PADDLE_HEIGHT / 2)))
		cpaddle = (PADDLE_HEIGHT / 2);
	if (cpaddle > (cheight - (PADDLE_HEIGHT / 2)))
		cpaddle = (cheight - (PADDLE_HEIGHT / 2));

	//assign center point to ball point if condition is true
	static int resetball = true;
	if (resetball == true)
	{
		ball = center;
		resetball = false;
	}

	//find elapsed time between updates
	static double ustart_tic = cv::getTickCount();
	double uelapsed_time = ((cv::getTickCount() - ustart_tic) / cv::getTickFrequency());
	ustart_tic = cv::getTickCount();

	//find ball displacement
	int xdisplacement = uelapsed_time * VELOCITY;
	int ydisplacement = uelapsed_time * VELOCITY;

	//declare x and y multipliers to change ball direction on bounce
	static int xbounce = 1;
	static int ybounce = 1;

	//check for ball to hit top of playable area and bounce
	if (ball.y < (0 + BALL_RADIUS))
		ybounce = ybounce * -1;

	//check for ball to hit bottom of playable area and bounce
	if (ball.y > (cheight - BALL_RADIUS))
		ybounce = ybounce * -1;

	//check for ball to hit user paddle and bounce
	if (((ball.x - BALL_RADIUS) < (PADDLE_WIDTH)) && (ball.y > (upaddle - (PADDLE_HEIGHT / 2))) && ball.y < (upaddle + (PADDLE_HEIGHT / 2)))
		xbounce = xbounce * -1;

	//check for ball to hit computer paddle and bounce
	if ((ball.x + BALL_RADIUS) > (cwidth - PADDLE_WIDTH))
		xbounce = xbounce * -1;

	//multiply diplacement by bounce direction
	ydisplacement = ydisplacement * ybounce;
	xdisplacement = xdisplacement * xbounce;

	//update ball location
	ball = cv::Point((ball.x + xdisplacement), (ball.y + ydisplacement));

	//define the corner points of the user and computer paddles
	cv::Point upaddle1 = cv::Point(0, (upaddle - (PADDLE_HEIGHT / 2)));
	cv::Point upaddle2 = cv::Point(PADDLE_WIDTH, (upaddle + (PADDLE_HEIGHT / 2)));
	cv::Point cpaddle1 = cv::Point(cwidth, (cpaddle - (PADDLE_HEIGHT / 2)));
	cv::Point cpaddle2 = cv::Point((cwidth - PADDLE_WIDTH), (cpaddle + (PADDLE_HEIGHT / 2)));

	//declare score variables
	static int uscore = 0;
	static int cscore = 0;

	//check for computer scoring
	if (ball.x < (0 - BALL_RADIUS))
	{
		cscore++;
		resetball = true;
	}

	//check for user scoring
	if (ball.x > (cwidth + BALL_RADIUS))
	{
		uscore++;
		resetball = true;
	}

	//check for a winner
	if ((uscore >= 5) || (cscore >= 5))
	{
		_canvas = cv::Mat::zeros(_canvas.size(), CV_8UC3);
		cv::putText(_canvas, "GAME OVER", (center - cv::Point(150, 0)), 1, 3, white, 5, 8, false);
		cv::imshow("canvas", _canvas);
		cv::waitKey(1);
		while (1);
	}


	elex4618control.get_button(button, reset);
	if (reset == 0)
	{
		uscore = 0;
		cscore = 0;
		resetball = true;
	}

	//draw the ball and paddles on the canvas
	cv::circle(_canvas, ball, BALL_RADIUS, white, -1);
	cv::rectangle(_canvas, upaddle1, upaddle2, white, -1, 1);
	cv::rectangle(_canvas, cpaddle1, cpaddle2, white, -1, 1);

	//draw the text on the canvas
	cv::putText(_canvas, "FPS: " + std::to_string(frame_rate), frtext, 1, 1, white, 1, 8, false);
	cv::putText(_canvas, "PLAYER: " + std::to_string(uscore), ustext, 1, 1, white, 1, 8, false);
	cv::putText(_canvas, "COMPUTER: " + std::to_string(cscore), cstext, 1, 1, white, 1, 8, false);
}

void CPong::draw()
{
	elapsed_time = ((cv::getTickCount() - start_tic) / cv::getTickFrequency());
	//cv::waitKey(5);

	if (elapsed_time >= (1 / FRAME_RATE))
	{
		frame_rate = (1 / elapsed_time);
		cv::imshow("canvas", _canvas);
		start_tic = cv::getTickCount();
	}
	//std::cout << start_tic << "\t" << elapsed_time << "\t" << freq << "\t" << frame_rate << std::endl;
}
