#pragma once
#include "CBase4618.h"
#include "CShip.h"
#include "CAsteroid.h"
#include "CMissile.h"

/**
*
* @brief Renders Asteroids game on Open CV canvas using the MSP432 joystick and push buttons.
*
* This class inherits from the CBase4618 class which uses an
* object of CControl to read from the MSP432.
*
* @author John Matson
*
*/

class CAsteroidGame :
	public CBase4618
{
public:
	CAsteroidGame(cv::Size sketchSize, int portNum);
	~CAsteroidGame();

	/**
	* @brief calls update and draw objects while checking for user input to end function
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void run();

	/**
	* @brief reads values from the MSP432 using class CControl and updates CAsteroidGame variables
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

protected:
	CShip ship; ///< object of CShip
	std::vector<CAsteroid> asteroidlist; ///< vector of CAsteroid objects
	std::vector<CMissile> missilelist; ///< vectore of CMissile objects
	int score; ///< score counter

	/**
	* @brief runs end of game animation
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void gameover();

	/**
	* @brief resets all parameters of game
	*
	* @param no input parameters
	* @return no output parameters
	*/
	void resetgame();
};
