/*
 *  PrismaticConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 08/07/11.
 *
 */

#ifndef _PRISMATIC_CONSTRAINT_H_
#define _PRISMATIC_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

class PrismaticConstraint : public Constraint
{
	enum PRISMATIC_CONSTRAINT_LIMIT_STATE { EPCLS_LOWER, EPCLS_UPPER, EPCLS_LOWER_UPPER, EPCLS_NONE };
public:
	PrismaticConstraint();
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
public:
	Vector2f m_Anchor;    // Anchor Point. this is always defined locally wrt the first body.
	Vector2f m_LocalAxis; // Local Axis for the Line Constraint wrt Body1.
	Vector2f m_LocalPerpendicularAxis; // Perpendicular axis to the local Axis
	dfloat m_ImpulseMagnitude;
	dfloat m_ImpulseMagnitudeParallel;
	Vector2f m_ImpulseDirection;
	Vector2f m_Anchor1, m_Anchor2; // Local Anchor Points
	Vector2f m_r1, m_r2; // Radius Vectors wrt the Bodies centres.
	Matrix3f m_EffectiveMassMatrix;
	dfloat m_PositionError, m_PositionErrorParallel;
	Vector2f m_WorldPerpendicularAxis;
	Vector2f m_WorldAxis;
	Vector2f d;
	PRISMATIC_CONSTRAINT_LIMIT_STATE m_LimitState;
};

END_NAMESPACE_DIRAC2D

#endif
