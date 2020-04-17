#include "stdafx.h"

#include "CAsteroid.h"
#include <time.h>

CAsteroid::CAsteroid(float width)
{
	_radius = (7 + (10 * ((float)rand() / (float)RAND_MAX)));
	_velocity = cv::Point2f((-15 + (30 * ((float)rand() / (float)RAND_MAX))), (20 + (50 * ((float)rand() / (float)RAND_MAX))));
	_position = cv::Point2f((_radius + ((width - (2 * _radius)) * ((float)rand() / (float)RAND_MAX))), (0 - _radius));
	_lives = 1;
	_colour = cv::Scalar(100, 100, 100);
}


CAsteroid::~CAsteroid()
{
}
