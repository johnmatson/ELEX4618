#pragma once

#include "CBase4618.h"

#ifdef WIN4618
#include "opencv.hpp"
#endif
#ifdef PI4618
#include <opencv2/opencv.hpp>
#endif

/**
*
* @brief Contains variables and functions to be which can be used through inheritance by various classes
*
* @author John Matson
*
*/

class CGameObject : public CBase4618
{
protected:
	cv::Point2f _position; ///< Open CV position vector point
	cv::Point2f _velocity; ///< Open CV movement vector point
	int _radius; ///< radius value
	int _lives; ///< life counter value
	cv::Scalar _colour; ///< Open CV colour value scaler
	double vstart_tic = cv::getTickCount(); ///< start time counter
	double velapsed_time; ///< elapsed time counter
public:
	CGameObject();

	~CGameObject();

	/**
	* @brief calculates position based on elasped time, velocity, and former position
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void move();

	/**
	* @brief returns radius of object
	*
	* @param no input parameters
	* @return int radius of object
	*/
	int get_radius() { return _radius; }

	/**
	* @brief returns number of lives of object
	*
	* @param no input parameters
	* @return int number of object lives
	*/
	int get_lives() { return _lives; }

	/**
	* @brief set number of lives for object
	*
	* @param int number of lives
	* @return no output parameters
	*/
	void set_lives(int lives) { _lives = lives; }

	/**
	* @brief decreased number of lives for object by one
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void decrement_lives() { _lives--; }

	/**
	* @brief sets position of object
	*
	* @param Open CV point position of object
	* @return no output parameters
	*/
	void set_pos(cv::Point2f pos) { _position = pos; }

	/**
	* @brief returns position of object
	*
	* @param no input parameters
	* @return Open CV point position of object
	*/
	cv::Point2f get_pos() { return _position; }

	/**
	* @brief draws object on passed Open CV matrix
	*
	* @param Open CV matrix to draw on
	* @return no output parameters
	*/
	void draw(cv::Mat &im);
};
