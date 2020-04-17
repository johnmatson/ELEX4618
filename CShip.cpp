#include "stdafx.h"

#include "CShip.h"


CShip::CShip()
{
	_radius = 20;
	_lives = 10;
	_colour = cv::Scalar(255, 255, 255);
}


CShip::~CShip()
{
}
