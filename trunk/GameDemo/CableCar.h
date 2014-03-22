/*
 *  CableCar.h
 *  Dirac2D
 *
 *  Created by Prashant on 14/02/13.
 *
 */

#ifndef _CABLE_CAR_H_
#define _CABLE_CAR_H_

#include "GameObject.h"
#include "Dirac2D.h"


USE_NAMESPACE_DIRAC2D

class FirstGame;


struct CableCarInfo : public GameObjectInfo
{
    CableCarInfo()
    {
        m_ObjectType = EOT_CABLE_CAR;
    }
	
	Vector2f m_FixedPoint1, m_FixedPoint2;
	float m_FixedLength;
};

class CableCar : public GameObject
{
	
public:
	
	CableCar( CableCarInfo cInfo, FirstGame* pGame );

	virtual void update(float dt);
	
	// COLLISION CALL BACKS
    
    virtual void onCollisionEnter( GameObject* pObject, ContactManifold& manifold );
    
    virtual void onCollisionExit( GameObject* pObject, ContactManifold& manifold );

	virtual ~CableCar();
	
protected:
	
	void init();
	
protected:
	
	FirstGame* m_pGame;
	
	PhysicalBody* m_pCableCarBody;
	
	CatenaryConstraint* m_pCableCarConstraint;
	
	Vector2f m_FixedPoint1, m_FixedPoint2;
	float m_FixedLength;
};

#endif

