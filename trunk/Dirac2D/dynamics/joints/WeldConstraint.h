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

class WeldConstraint : public Constraint
{
public:
	WeldConstraint();
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
public:
	Vector2f m_Anchor;    // Anchor Point. this is always defined locally wrt the first body.
	Vector2f m_Impulse;
	Vector2f m_Anchor1, m_Anchor2; // Local Anchor Points
	Vector2f m_r1, m_r2; // Radius Vectors wrt the Bodies centres.
	Matrix3f m_EffectiveMass;
	Vector2f m_PositionError;
	dfloat   m_AngleError;
	dfloat   m_InitialAngle;
};

END_NAMESPACE_DIRAC2D

#endif