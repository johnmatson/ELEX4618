#pragma once
#include "CGameObject.h"

/**
*
* @brief Asteroid class for use by CAsteroidGame
*
* @author John Matson
*
*/

class CAsteroid :
	public CGameObject
{
public:
	CAsteroid(float width);
	~CAsteroid();
};

