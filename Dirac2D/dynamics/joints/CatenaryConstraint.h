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

class CatenaryConstraint : public Constraint
{
public:
	CatenaryConstraint();
	
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
	dfloat m_Length;
	dfloat m_EffectiveMass;
	dfloat m_ImpulseMagnitude;
	dfloat m_PositionError;
};

END_NAMESPACE_DIRAC2D

#endif