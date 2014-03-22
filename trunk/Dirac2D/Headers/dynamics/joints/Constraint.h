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

enum CONSTRAINT_TYPE { ECT_DISTANCE, ECT_MOUSE, ECT_HINGE, ECT_WELD, ECT_CATENARY, ECT_CATENARY_FIXED_ROTATION, ECT_LINE, ECT_PRISMATIC, ECT_WHEEL, ECT_MOTOR, ECT_PULLEY, ECT_GEAR, ECT_CUBIC_SPLINE, ECT_MIN_MAX, ECT_CIRCULAR_PATH };

enum CONSTRAINT_LIMIT_STATE { ECLS_LOWER, ECLS_UPPER, ECLS_LOWER_UPPER, ECLS_NONE };

class Constraint;

struct ConstraintEdge
{
	Constraint* m_pConstraint;
	PhysicalBody* pBody;
	ConstraintEdge* m_Next;
	ConstraintEdge* m_Prev;
};

struct ConstraintInfo
{
    ConstraintInfo()
    {
        m_Frequency = 60.0f;
        m_DampingRatio = 0.0f;
        m_PhysicalBody1 = 0;
        m_PhysicalBody2 = 0;
		m_bCollideConnected = true;
    }
    
    PhysicalBody* m_PhysicalBody1;
	PhysicalBody* m_PhysicalBody2; 
    CONSTRAINT_TYPE m_Type;
	dbool m_bCollideConnected;
	dfloat m_Frequency;
	dfloat m_DampingRatio;
};

class Constraint
{
public:
	inline dbool shouldCollideConnected() const
	{
		return m_bCollideConnected;
	}
protected:
    Constraint(const ConstraintInfo& cInfo)
	{
		m_PhysicalBody1 = cInfo.m_PhysicalBody1;
		m_PhysicalBody2 = cInfo.m_PhysicalBody2;
		m_DampingRatio = cInfo.m_DampingRatio;
		m_Next = m_Prev = 0;
		m_Frequency = cInfo.m_Frequency;
		m_DampingRatio = cInfo.m_DampingRatio; // Critically damped.
		m_Cfm = 0.0f;
        m_bCollideConnected = cInfo.m_bCollideConnected;
	};
	
protected:
	
	virtual void buildJacobian(dfloat dt) = 0;
	
	virtual void correctVelocities() = 0;
	
	virtual void initialize() = 0;

	friend class PhysicalWorld;
	
	// References of the Physical bodies attached to this Constraint
	PhysicalBody* m_PhysicalBody1;
	PhysicalBody* m_PhysicalBody2;
	
	Constraint* m_Next;
	Constraint* m_Prev;
	// Constraint force mixing. this is used to soften the constraint.
	dfloat m_Cfm;
	
	//The Natural Frequency of this Constraint( without damping ) . expressed in Hz.
	dfloat m_Frequency;
	/* The Damping ratio for this Constraint. different damping behaviors are obtained based upon the value of Damping Ratio. it is dimentionless.
	   m_DampingRatio = 1 ---> Critically Damped
	   m_DampingRatio > 1 ---> Over Damped.
	   m_DampingRatio < 1 && >= 0 ---> Under Damped. if m_DampingRatio == 0 the System will Oscillate without damping with natural frequency.
	 */
	dfloat m_DampingRatio;
	
	// decides whether the bodies connected by this constraint should collide 
	dbool m_bCollideConnected;
    
	//
    ConstraintEdge m_ConstraintEdge1;
    ConstraintEdge m_ConstraintEdge2;
    
	CONSTRAINT_TYPE m_Type;
};

END_NAMESPACE_DIRAC2D

#endif
