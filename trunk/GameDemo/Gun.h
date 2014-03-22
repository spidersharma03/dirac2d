/*
 *  Gun.h
 *  Dirac2D
 *
 *  Created by Prashant on 12/12/12.
 *
 */

#ifndef _GUN_H_
#define _GUN_H_

#include "Dirac2D.h"
#include "GameObject.h"
#include "IRenderable.h"

USE_NAMESPACE_DIRAC2D

class FirstGame;

class Gun : public GameObject, public IRenderable
{
public:
	Gun(FirstGame* pGame)
	{
		m_pGame = pGame;
	}
	
	virtual void shoot(const Vector2f& direction) = 0;
	
	virtual void update(float dt){};
	
	virtual void render(){};
	
protected:
	FirstGame *m_pGame;

	float m_ShootingInterval;
	float m_ShootingImpact;
	Vector2f m_ShootingOrigin, m_ShootingTarget;
	float m_ShootingRadius;
	float m_ShootingImpactRadius;
	
    DTimer m_Timer;
};

#endif