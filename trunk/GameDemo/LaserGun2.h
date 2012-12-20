/*
 *  LaserGun2.h
 *  Dirac2D
 *
 *  Created by Prashant on 17/12/12.
 *
 */

#ifndef _LASER_GUN2_H_
#define _LASER_GUN2_H_

#include "Gun.h"

class GunRayCastManyCallBack;

class LaserGun2 : public Gun
{
public:
	LaserGun2(FirstGame* pGame);
	
	virtual void shoot(const Vector2f& direction);
	
	virtual void update(float dt);
	
	virtual void render();
	
protected:
	GunRayCastManyCallBack* m_pGunRayCastManyCallBack;
};

#endif

