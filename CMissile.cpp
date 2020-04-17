#include "stdafx.h"

#include "CMissile.h"


CMissile::CMissile(float width)
{
	_radius = 3;
	_velocity = cv::Point(0, -120);
	_lives = 1;
	_colour = cv::Scalar(0, 0, 255);
}


CMissile::~CMissile()
{
}
