/*
 *  PulleyConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 14/07/11.
 *
 */

#ifndef _PULLEY_CONSTRAINT_H_
#define _PULLEY_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct PulleyConstraintInfo : public ConstraintInfo
{
    PulleyConstraintInfo()
    {
        m_Type = ECT_PULLEY;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
        m_PulleyRatio = 1.0f;
    }
    // the Anchor point is always assumed in the local coordinate frames of the PhysicalBody.
	Vector2f m_Anchor1, m_Anchor2;
	Vector2f m_FixedPoint1, m_FixedPoint2;
    dfloat m_PulleyRatio;
};

class PulleyConstraint : public Constraint
{
public:
	
	virtual void buildJacobian(dfloat dt);
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:
    
    PulleyConstraint( const PulleyConstraintInfo& cInfo);

public:
	// the Anchor point is always assumed in the local coordinate frames of the PhysicalBody.
	Vector2f m_Anchor1, m_Anchor2;
	Vector2f m_FixedPoint1, m_FixedPoint2;
	Vector2f m_ImpulseDirection1, m_ImpulseDirection2;
	Vector2f m_r1, m_r2; // Local Anchor Points on the Body.
	dfloat m_Distance;
	dfloat m_Length1, m_Length2;
	dfloat m_TotalLength, m_FixedLength;
	dfloat m_EffectiveMass;
	dfloat m_ImpulseMagnitude;
	dfloat m_PositionError;
	dfloat m_PulleyRatio;
};

END_NAMESPACE_DIRAC2D

#endif