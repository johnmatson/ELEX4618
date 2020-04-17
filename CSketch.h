#pragma once

#include "stdafx.h"

#include "CBase4618.h"

/**
*
* @brief Draws on Open CV canvas using the MSP432 joystick.
*
* This class inherits from the CBase4618 class which uses an
* object of CControl to read from the MSP432.
*
* @author John Matson
*
*/

class CSketch : public CBase4618
{
public:
	CSketch(cv::Size sketchSize, int portNum);
	~CSketch();

	/**
	* @brief calls update and draw objects while checking for user input to end function
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void run();

	/**
	* @brief reads values from the MSP432 using class CControl and updates CSketch variables
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void update();

	/**
	* @brief performs all drawing on Mat image and diplays the image on the screen
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void draw();
};
