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
	Vector2f m_Anchor;    // Anchor Point. this is always deffined locally wrt the first body.
	Vector2f m_Impulse;
	Vector2f m_r1, m_r2; // Radius Vectors wrt the Bodies centres.
	Matrix2f m_EffectiveMass;
	Vector2f m_PositionError;
};

END_NAMESPACE_DIRAC2D

#endif