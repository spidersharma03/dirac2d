/*
 *  SimpleGun.h
 *  Dirac2D
 *
 *  Created by Prashant on 12/12/12.
 *
 */

#ifndef _SIMPLE_GUN_H_
#define _SIMPLE_GUN_H_

#include "Gun.h"

class GunRayCastClosestCallBack;

class SimpleGun : public Gun
{
public:
	SimpleGun(FirstGame* pGame);
	
	virtual void shoot(const Vector2f& direction);
	
	virtual void update(float dt);
	
	virtual void render();
	
protected:
	GunRayCastClosestCallBack* m_pGunRayCastClosestCallBack;
};

#endif