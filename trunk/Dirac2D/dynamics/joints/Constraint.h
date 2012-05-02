/*
 *  Joint.h
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "../../definitions.h"
#include "../../Settings.h"
#include "../../maths/MathUtil.h"

#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalShape;

class Constraint
{
public:
	Constraint();
	
	Constraint(const Constraint& other);
	
	virtual void buildJacobian() = 0;
	
	virtual void correctVelocities() = 0;
	
	// References of the Physical bodies attached to this joint
	PhysicalShape* m_PhysicalShape1;
	PhysicalShape* m_PhysicalShape2;

	// Error reduction parameter. this is used to make sure that the constarint dosent drift over time. should be less than 1.0f/timeStep.
	dfloat erp;
	// Constraint force mixing. this is used to soften the constraint and can be used as a softness parameter.
	dfloat cfm;
};

END_NAMESPACE_DIRAC2D

#endif
