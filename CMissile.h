#pragma once
#include "CGameObject.h"

/**
*
* @brief Missile class for use by CAsteroidGame
*
* @author John Matson
*
*/

class CMissile :
	public CGameObject
{
public:
	CMissile(float width);
	~CMissile();
};

