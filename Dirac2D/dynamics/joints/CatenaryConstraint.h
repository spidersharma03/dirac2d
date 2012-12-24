/*
 *  CatenaryConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 03/07/12.
 *
 */

#ifndef _CATENARY_CONSTRAINT_H_
#define _CATENARY_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct CatenaryConstraintInfo : public ConstraintInfo
{
    CatenaryConstraintInfo()
    {
        m_Type = ECT_CATENARY;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
        m_FixedLength = -1.0f;
    }
    // Anchor Point.
    Vector2f m_Anchor;
    Vector2f m_FixedPoint1, m_FixedPoint2;
    dfloat m_FixedLength;
};

class CatenaryConstraint : public Constraint
{
public:	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:
    
    CatenaryConstraint( const CatenaryConstraintInfo& cInfo);

public:
	// the Anchor point is always assumed in the local coordinate frames of the PhysicalBody.
	Vector2f m_Anchor;
	Vector2f m_FixedPoint1, m_FixedPoint2;
	Vector2f m_ImpulseDirection;
	Vector2f m_r; // Local Anchor Point on the Body.
	dfloat m_Distance;
	dfloat m_TotalLength;
	dfloat m_FixedLength;
	dfloat m_EffectiveMass;
	dfloat m_ImpulseMagnitude;
	dfloat m_PositionError;
};

END_NAMESPACE_DIRAC2D

#endif