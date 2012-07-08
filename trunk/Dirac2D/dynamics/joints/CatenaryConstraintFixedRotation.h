/*
 *  CatenaryConstraintFixedRotation.h
 *  Dirac2D
 *
 *  Created by Prashant on 06/07/12.
 *
 */


#ifndef _CATENARY_CONSTRAINT_FIXED_ROTATION_H_
#define _CATENARY_CONSTRAINT_FIXED_ROTATION_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

class CatenaryConstraintFixedRotation : public Constraint
{
	enum LENGTH_LIMIT_STATE { ELLS_LOWER, ELLS_NONE };
public:
	CatenaryConstraintFixedRotation();
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
public:
	// the Anchor point is always assumed in the local coordinate frames of the PhysicalBody.
	Vector2f m_Anchor;
	Vector2f m_FixedPoint1, m_FixedPoint2;
	Vector2f m_ImpulseDirection;
	Vector2f m_r; // Local Anchor Point on the Body.
	dfloat m_Distance;
	dfloat m_TotalLength;
	dfloat m_FixedLength;
	Matrix2f m_EffectiveMassMatrix;
	Vector2f m_Impulse;
	dfloat m_PositionError;
	dfloat m_AngularError;
	dfloat m_ReferenceAngle;
	LENGTH_LIMIT_STATE m_LengthLimitState;
};

END_NAMESPACE_DIRAC2D

#endif