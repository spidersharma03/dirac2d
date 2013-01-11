/*
 *  MotorConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 08/07/11.
 *
 */

#ifndef _MOTOR_CONSTRAINT_H_
#define _MOTOR_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct MotorConstraintInfo : public ConstraintInfo
{
    MotorConstraintInfo()
    {
        m_Type = ECT_MOTOR;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
        m_MaxTorque = 1.0f;
        m_Speed = 1.0f;
    }
    dfloat m_MaxTorque, m_Speed;
};

class MotorConstraint : public Constraint
{
public:

	virtual void buildJacobian(dfloat dt);
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
	void setMaxTorque( const dfloat maxTorque )
	{
		m_MaxTorque = maxTorque;
	}
	
	dfloat getMaxTorque()
	{
		return m_MaxTorque;
	}
    
    friend class PhysicalWorld;
    
protected:
    	
    MotorConstraint(const MotorConstraintInfo& cInfo);

protected:
	dfloat m_EffectiveMass;
	dfloat m_Impulse;
	dfloat m_MaxTorque;
	dfloat m_Speed;
};

END_NAMESPACE_DIRAC2D

#endif