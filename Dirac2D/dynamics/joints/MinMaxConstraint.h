/*
 *  MinMaxConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 10/08/12.
 *
 */

#ifndef _MIN_MAX_CONSTRAINT_H_
#define _MIN_MAX_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct MinMaxConstraintInfo : public ConstraintInfo
{
    MinMaxConstraintInfo()
    {
        m_Type = ECT_MIN_MAX;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
        m_UpperLimit = 100000.0f;
        m_LowerLimit = -100000.0f;
    }
    // the Anchor point is always assumed in the local coordinate frames of the PhysicalBody.
	Vector2f m_Anchor1, m_Anchor2;
	dfloat m_UpperLimit;
	dfloat m_LowerLimit;
};

class MinMaxConstraint : public Constraint
{
public:
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:
    
    MinMaxConstraint( const MinMaxConstraintInfo& cInfo);

public:
	// the Anchor points are always assumed in the local coordinate frames of the PhysicalBodies.
	Vector2f m_Anchor1, m_Anchor2;
	Vector2f m_ImpulseDirection;
	Vector2f m_r1, m_r2; // Local Anchor Points on the Bodies.
	dfloat m_Distance;
	dfloat m_EffectiveMass;
	dfloat m_ImpulseMagnitude;
	dfloat m_PositionError;
	
	dfloat m_UpperLimit;
	dfloat m_LowerLimit;
	
	CONSTRAINT_LIMIT_STATE m_LimitState;
};

END_NAMESPACE_DIRAC2D

#endif