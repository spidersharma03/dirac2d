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

class PhysicalBody;

enum CONSTRAINT_TYPE { ECT_DISTANCE, ECT_HINGE, ECT_WELD, ECT_CATENARY, ECT_CATENARY_FIXED_ROTATION, ECT_LINE, ECT_PRISMATIC, ECT_WHEEL, ECT_MOTOR };

class Constraint
{
public:
	Constraint()
	{
		m_PhysicalBody1 = 0;
		m_PhysicalBody2 = 0;
		m_Erp = 0.5f;
		m_Cfm = 0.1f;
		m_Next = m_Prev = 0;
	};
	
	virtual void buildJacobian() = 0;
	
	virtual void correctVelocities() = 0;
	
	virtual void initialize() = 0;

	// References of the Physical bodies attached to this Constraint
	PhysicalBody* m_PhysicalBody1;
	PhysicalBody* m_PhysicalBody2;
	
	Constraint* m_Next;
	Constraint* m_Prev;
	// Error reduction parameter. this is used to make sure that the constarint dosent drift over time. should be less than 1.0f/timeStep.
	dfloat m_Erp;
	// Constraint force mixing. this is used to soften the constraint and can be used as a softness parameter.
	dfloat m_Cfm;
	
	CONSTRAINT_TYPE m_Type;
};

END_NAMESPACE_DIRAC2D

#endif
