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

BEGIN_NAMESPACE_DIRAC2D

END_NAMESPACE_DIRAC2D

class GunRayCastClosestCallBack;

class SimpleGun : public Gun
{
public:
	SimpleGun(FirstGame* pGame);
	
	virtual void shoot();
	
	virtual void update(float dt);
	
	virtual void render();
	
protected:
	GunRayCastClosestCallBack* m_pGunRayCastClosestCallBack;

	Vector2f m_ShootingOrigin, m_ShootingTarget;
	float m_ShootingRadius;
	float m_ShootingImpactRadius;
	float m_ShootingImpact;
};

#endif