/*
 *  HingeConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 12/06/12.
 *
 */

#ifndef _HINGE_CONSTRAINT_H_
#define _HINGE_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct HingeConstraintInfo : public ConstraintInfo
{
    HingeConstraintInfo()
    {
        m_Type = ECT_HINGE;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
        m_LowerAngle = 0.0f;
        m_UpperAngle = PI;
    }
    // Anchor Point.
    Vector2f m_Anchor;
    // Angular Limits
    dfloat m_LowerAngle, m_UpperAngle;
};

class HingeConstraint : public Constraint
{
public:	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:
    
    HingeConstraint(const HingeConstraintInfo& cInfo);

public:
	Vector2f m_Anchor;    // Anchor Point. this is always defined locally wrt the first body.
	Vector3f m_Impulse;
	Vector2f m_Anchor1, m_Anchor2; // Local Anchor Points
	Vector2f m_r1, m_r2; // Radius Vectors wrt the Bodies centres.
	Matrix3f m_EffectiveMass;
	Vector2f m_PositionError;
	dfloat m_AngularError;
	dfloat m_LowerAngle, m_UpperAngle;
	dfloat m_ReferenceAngle;
	CONSTRAINT_LIMIT_STATE m_AngleLimitState;
};

END_NAMESPACE_DIRAC2D

#endif