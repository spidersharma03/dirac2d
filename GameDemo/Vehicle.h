//
//  Vehicle.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "../Dirac2D/Dirac2D.h"
#include "IRenderable.h"
#include "GameObject.h"

BEGIN_NAMESPACE_DIRAC2D
class MotorConstraint;
END_NAMESPACE_DIRAC2D

USE_NAMESPACE_DIRAC2D

class Gun;

class Vehicle :  public GameObject, public IRenderable
{
public:
    Vehicle()
    {
		m_ObjectInfo.m_ObjectType = EOT_VEHICLE;
        m_bBoostEnable = false;
		m_BoostIncreament = 1.0f;
    }
    
    virtual void init() = 0;
    
    virtual void initFromFile(const char* fileName) = 0;
    
	virtual void shoot(const Vector2f& direction)
	{
	}
	
	virtual void applyForce( const Vector2f& force )
	{
	}
	
	virtual void applyImpulse( const Vector2f& impulse)
	{
	}
	
	void setBoostEnable(bool bEnable)
	{
		m_bBoostEnable = bEnable;
	}

    virtual void accelerate()
    {
        
    }
    
	virtual void applyTorqueImpulse( const float torqueImpulse)
	{
	}
	
    virtual void applyBrake()
    {
        
    }
    
    inline Vector2f getVelocity()
    {
        return m_Velocity;
    }
    
    inline Vector2f getPosition()
    {
        return m_Position;
    }
    
    inline void setMaxMotorTorque(float torque)
    {
        m_MaxTorque = torque; 
    }
    
    inline void setMotorSpeed( float motorSpeed )
    {
        m_pMotor->setSpeed( motorSpeed );
    }
    
    inline float getMotorSpeed()
    {
        return m_pMotor->getSpeed();
    }
public:
    virtual void update(float dt){};
    
    virtual void render(){};

	
protected:
    MotorConstraint* m_pMotor;
	
	bool m_bBoostEnable;
	float m_BoostIncreament; // Every frame this much speed will be increased

protected:
    Vector2f m_Com; // Centre of Mass of the Vehicle
    
    float m_MaxTorque; // Max Motor Torque
    
	Gun* m_pGun;
};

#endif
