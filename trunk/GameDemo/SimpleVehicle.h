//
//  SimpleVehicle.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Vehicle.h"

#ifndef _SIMPLE_VEHICLE_H_
#define _SIMPLE_VEHICLE_H_

class FirstGame;
class RayCastTerrainCallBack;


class SimpleVehicle : public Vehicle
{
public:
    SimpleVehicle(FirstGame* pGame);
    
	virtual void accelerate();
	
	virtual void applyBrake();
    
	virtual void applyForce( const Vector2f& force );
	
	virtual void applyImpulse( const Vector2f& impulse);

	virtual void applyTorqueImpulse( const float torqueImpulse);

    virtual void init();
    
    virtual void initFromFile(const char* fileName);
    
    virtual void update(float dt);
    
    virtual void render();
    
	virtual void shoot(const Vector2f& direction);
	
	float getDistanceFromTerrain()
	{
		return m_DistanceFromTerrain;
	}
	// COLLISION CALL BACKS
    
    virtual void onCollisionEnter( GameObject* pObject, ContactManifold& manifold );
    
    virtual void onCollisionExit( GameObject* pObject, ContactManifold& manifold );
	
private:
    FirstGame* m_pGame;
    RayCastTerrainCallBack* m_pRayCastTerrainCallBack;
	
	
	float m_DistanceFromTerrain;
};

#endif
