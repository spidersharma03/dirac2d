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

class Vehicle : public IRenderable, public GameObject
{
public:
    Vehicle()
    {
        
    }
    
    virtual void init() = 0;
    
    virtual void initFromFile(const char* fileName) = 0;
    
    virtual void accelerate()
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
    
public:
    virtual void update(float dt){};
    
    virtual void render(){};

protected:
    MotorConstraint* m_pMotor;
protected:
    Vector2f m_Position; // Centre of Mass of the Vehicle
    
    float m_MaxTorque; // Max Motor Torque
    
};

#endif
