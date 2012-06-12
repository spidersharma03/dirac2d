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

class HingeConstraint : public Constraint
{
public:
	HingeConstraint();
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
public:
	Vector2f m_Anchor;
	Vector2f m_ImpulseDirection;
	Vector2f m_r1, m_r2; // Local Anchor Points on the Bodies.
	Matrix2f m_EffectiveMass;
	Vector2f m_PositionError;
};

END_NAMESPACE_DIRAC2D

#endif