#include "stdafx.h"

#include "CAsteroidGame.h"
#include "CShip.h"
#include "CAsteroid.h"
#include "CMissile.h"
#include "CGameObject.h"

#ifdef PI4618
#include <opencv2/opencv.hpp>
#endif

CAsteroidGame::CAsteroidGame(cv::Size sketchSize, int portNum)
{
	//initialize serial communication
	elex4618control.init_com(portNum);

	//set canvas to all black
	_canvas = cv::Mat::zeros(sketchSize, CV_8UC3);

	//get size of canvas
	cheight = _canvas.rows;
	cwidth = _canvas.cols;

	//set score to 0;
	score = 0;
}

CAsteroidGame::~CAsteroidGame()
{
	cvDestroyAllWindows();
	cv::waitKey(1);
}

void CAsteroidGame::run()
{
	do
	{
		//spawn new asteroid every 0.5 seconds
		elapsed_time = ((cv::getTickCount() - start_tic) / cv::getTickFrequency());
		if (elapsed_time > 1.5)
		{
			CAsteroid astro(cwidth);
			asteroidlist.push_back(astro);
			start_tic = cv::getTickCount();
		}

		CAsteroidGame::update();

		CAsteroidGame::draw();
		exit = cv::waitKey(1);
	} while (exit != 'q');
}

void CAsteroidGame::update()
{
	//retrieve joystick values, print to screen, and set ship position
	float channel1, channel2;
	elex4618control.get_analog(channel1, channel2);
	cv::Point2f joystick = cv::Point2f(channel1, channel2);
	std::cout << std::endl << joystick;
	cv::Point2f shippos = cv::Point(channel1 * cwidth, (1 - channel2) * cheight);
	ship.set_pos(shippos);

	//retrieve pushbutton values and print fire/reset if buttons are active
	int fire, reset;
	//elex4618control.get_button(1, fire);
	elex4618control.get_data(0, 1, fire);
	//elex4618control.get_button(2, reset);
	elex4618control.get_data(0, 2, reset);
	std::cout << "\t" << fire << "\t" << reset;
	if (fire == 0)
	{
		std::cout << "\tfire!";
		CMissile mis(cwidth);
		mis.set_pos(shippos);
		missilelist.push_back(mis);
	}
	if (reset == 0)
	{
		std::cout << "\treset!";
		resetgame();
	}

	if (asteroidlist.size() != 0)
	{
		for (int count = (asteroidlist.size() - 1); count >= 0; count--)
		{
			int radius = asteroidlist[count].get_radius();
			int x = asteroidlist[count].get_pos().x;
			int y = asteroidlist[count].get_pos().y;

			if ((abs(x - ship.get_pos().x) < radius + ship.get_radius()) && (abs(y - ship.get_pos().y) < radius + ship.get_radius()))
			{
				asteroidlist[count].decrement_lives();
				ship.decrement_lives();
			}

			if ((x < (0 - radius)) || (x > (cwidth + radius)) || (y > (cheight + radius)))
				asteroidlist[count].decrement_lives();

			for (int mcount = (missilelist.size() - 1); mcount >= 0; mcount--)
			{
				int mradius = missilelist[mcount].get_radius();
				int mx = missilelist[mcount].get_pos().x;
				int my = missilelist[mcount].get_pos().y;

				if ((abs(x - mx) < radius + mradius) && (abs(y - my) < radius + mradius))
				{
					asteroidlist[count].decrement_lives();
					missilelist[mcount].decrement_lives();
					score += 10;
				}
			}
		}
	}

	if (missilelist.size() != 0)
	{
		for (int count = (missilelist.size() - 1); count >= 0; count--)
		{
			int radius = missilelist[count].get_radius();
			int y = missilelist[count].get_pos().y;

			if (y < (0 - radius))
				missilelist[count].decrement_lives();
		}
	}
}

void CAsteroidGame::draw()
{
	//reset canvas to black
	_canvas = cv::Mat::zeros(_canvas.size(), CV_8UC3);

	//draw ship
	ship.draw(_canvas);

	//loop through move and draw for all asteroids
	if (asteroidlist.size() != 0)
	{
		for (int count = (asteroidlist.size() - 1); count >= 0; count--)
		{
			if (asteroidlist[count].get_lives() > 0)
			{
				asteroidlist[count].move();
				asteroidlist[count].draw(_canvas);
			}
			else
			{
				asteroidlist.erase(asteroidlist.begin() + count);
			}
		}
	}

	//loop through move and draw for all missiles
	if (missilelist.size() != 0)
	{
		for (int count = (missilelist.size() - 1); count >= 0; count--)
		{
			if (missilelist[count].get_lives() > 0)
			{
				missilelist[count].move();
				missilelist[count].draw(_canvas);
			}
			else
			{
				missilelist.erase(missilelist.begin() + count);
			}
		}
	}

	if (ship.get_lives() <= 0)
		gameover();

	cv::Point livespos = cv::Point(20, 30);
	cv::putText(_canvas, "SCORE: " + std::to_string(score), livespos, 1, 1, white, 1, 8, false);

	cv::Point scorepos = cv::Point((cwidth - 100), 30);
	cv::putText(_canvas, "LIVES: " + std::to_string(ship.get_lives()), scorepos, 1, 1, white, 1, 8, false);

	//send diplay canvas matrix on screen
	cv::imshow("canvas", _canvas);
}

void CAsteroidGame::gameover()
{
	start_tic = cv::getTickCount();
	cv::Point center = cv::Point((cwidth / 2), (cheight / 2));
	_canvas = cv::Mat::zeros(_canvas.size(), CV_8UC3);
	cv::putText(_canvas, "GAME OVER", (center - cv::Point(150, 0)), 1, 3, white, 5, 8, false);
	cv::imshow("canvas", _canvas);
	cv::waitKey(1);
	while (elapsed_time < 5)
		elapsed_time = ((cv::getTickCount() - start_tic) / cv::getTickFrequency());
	resetgame();
}

void CAsteroidGame::resetgame()
{
	asteroidlist.clear();
	missilelist.clear();
	ship.set_lives(10);
	score = 0;
}
