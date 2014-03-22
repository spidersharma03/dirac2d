/*
 *  MouseConstraint.h
 *  Dirac2D
 *
 *  Created by Prashant on 01/05/12.
 *
 */

#ifndef _MOUSE_CONSTRAINT_H_
#define _MOUSE_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct MouseConstraintInfo : public ConstraintInfo
{
    MouseConstraintInfo()
    {
        m_Type = ECT_MOUSE;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
	}
};

class MouseConstraint : public Constraint
{
public:
    
	virtual void buildJacobian(dfloat dt);
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
	Vector2f getMousePosition()
	{
		return m_MousePoint;
	}
	
	Vector2f getTargetPickPosition();
	
	void setMousePosition(const Vector2f& position);

	void setTargetBody( PhysicalBody* pBody, const Vector2f position = Vector2f() );

    friend class PhysicalWorld;
    
protected:	
    MouseConstraint( const MouseConstraintInfo& constraintInfo );

private:
	// the Anchor point is always assumed in the local coordinate frames of the PhysicalBodies.
	Vector2f m_Impulse;
	Vector2f m_r; // Local Anchor Point on the Body.
	
	Vector2f m_MousePoint; // Current Mouse Location in world space
	Vector2f m_BodyLocalPickPosition; // position where a body is picked with the mouse  in local space.
	
	Vector2f m_PositionError;
	Matrix2f m_EffectiveMassMatrix;
	
	dfloat m_MaxImpulse;
};

END_NAMESPACE_DIRAC2D

#endif