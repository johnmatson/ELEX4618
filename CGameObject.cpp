#include "stdafx.h"
#include "CGameObject.h"

#ifdef PI4618
#include <opencv2/opencv.hpp>
#endif

CGameObject::CGameObject()
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::move()
{
	velapsed_time = ((cv::getTickCount() - vstart_tic) / cv::getTickFrequency());
	_position = (_position + (velapsed_time * _velocity));
	vstart_tic = cv::getTickCount();
}

void CGameObject::draw(cv::Mat &im)
{
	cv::circle(im, _position, _radius, _colour, -1);
}
