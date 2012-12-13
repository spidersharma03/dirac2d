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

class SimpleVehicle : public Vehicle
{
public:
    SimpleVehicle(FirstGame* pGame);
    
	virtual void applyForce( const Vector2f& force );
	
	virtual void applyImpulse( const Vector2f& impulse);

	virtual void applyTorqueImpulse( const float torqueImpulse);

    virtual void init();
    
    virtual void initFromFile(const char* fileName);
    
    virtual void update(float dt);
    
    virtual void render();
    
	virtual void shoot();
	
private:
    FirstGame* m_pGame;
    
    PhysicalBody* m_pVehicleBody;
    
};

#endif
