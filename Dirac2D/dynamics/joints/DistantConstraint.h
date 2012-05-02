/*
 *  DistantConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 01/05/12.
 *
 */

#ifndef _DISTANCE_CONSTRAINT_H_
#define _DISTANCE_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

class DistanceConstraint : public Constraint
{
public:
	DistanceConstraint();
	
	DistanceConstraint(const Constraint& other);
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
public:
	Vector2f m_Anchor1, m_Anchor2;
};

END_NAMESPACE_DIRAC2D

#endif