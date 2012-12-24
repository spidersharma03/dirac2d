/*
 *  WeldConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 26/06/12.
 *
 */

#ifndef _WELD_CONSTRAINT_H_
#define _WELD_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct WeldConstraintInfo : public ConstraintInfo
{
    WeldConstraintInfo()
    {
        m_Type = ECT_WELD;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
    }
    // Anchor Point.
    Vector2f m_Anchor;
};

class WeldConstraint : public Constraint
{
public:
	
    virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:
    	
    WeldConstraint(const WeldConstraintInfo& cInfo);
    
public:
	Vector2f m_Anchor;    // Anchor Point. this is always defined locally wrt the first body.
	Vector3f m_Impulse;
	Vector2f m_Anchor1, m_Anchor2; // Local Anchor Points
	Vector2f m_r1, m_r2; // Radius Vectors wrt the Bodies centres.
	Matrix3f m_EffectiveMass;
	Vector2f m_PositionError;
	dfloat   m_AngleError;
	dfloat   m_InitialAngle;
};

END_NAMESPACE_DIRAC2D

#endif