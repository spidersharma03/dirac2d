/*
 *  DistantConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 01/05/12.
 *
 */

#ifndef _DISTANCE_CONSTRAINT_H_
#define _DISTANCE_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct DistanceConstraintInfo : public ConstraintInfo
{
    DistanceConstraintInfo()
    {
        m_Type = ECT_DISTANCE;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
	}
    // the Anchor points are always assumed in the local coordinate frames of the PhysicalBodies.
    Vector2f m_Anchor1, m_Anchor2;
};

class DistanceConstraint : public Constraint
{
public:
    
	virtual void buildJacobian(dfloat dt);
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:	
    DistanceConstraint( const DistanceConstraintInfo& constraintInfo );
public:
	// the Anchor points are always assumed in the local coordinate frames of the PhysicalBodies.
	Vector2f m_Anchor1, m_Anchor2;
	Vector2f m_ImpulseDirection;
	Vector2f m_r1, m_r2; // Local Anchor Points on the Bodies.
	dfloat m_Distance;
	dfloat m_EffectiveMass;
	dfloat m_ImpulseMagnitude;
	dfloat m_PositionError;
};

END_NAMESPACE_DIRAC2D

#endif