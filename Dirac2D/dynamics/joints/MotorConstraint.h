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

class MotorConstraint : public Constraint
{
public:
	MotorConstraint();
	
	virtual void buildJacobian();
	
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
public:
	dfloat m_EffectiveMass;
	dfloat m_Impulse;
	dfloat m_MaxTorque;
	dfloat m_Speed;
};

END_NAMESPACE_DIRAC2D

#endif